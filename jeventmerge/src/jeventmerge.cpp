/* ==========================================================================
Copyright 2017 The CUTU Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
============================================================================= */

#include <glob.h>
#include <set>
#include "tclap/CmdLine.h"
#include "jeventmerge.h"
#include "JSONTraceBuffIOS.h"
#include "Trace.h"

using namespace ETrace;
using namespace TCLAP;
using namespace std;


/* I/O paths */
string ipath{};
string opath{};

/* Filtered accepted traces sufix label */
string atl{};

/* Filtered rejected traces sufix label */
string rtl{};


/**
*
*   Parses the Command Line Arguments (CLA). Valid switches for this application 
*   are the following:
*
*/
void parseCLA(int argc, char** argv) {
    try{
        CmdLine cmd("jeventmerge (JSON Event Merge) perfoms two phases: localization and evaluation. The first phase, finds and concatenates events labeled with equal trace identifier. Events are extrated concurrently from multiple JSON formatted logs. Concatenation is applied if temporal order and valid transitions are satisfied between adyacent events. The later phase, classifies traces as either complete or incomplete. A trace is complete if it contains the initial event, reaches a final event, and all event transitions are valid, otherwise it is incomplete. \n\
\n\
Modes of execution: \n\
jeventmerge -f <path to JSON logs> -a <accepted trace filename>.json -r <rejected trace filename>.json", ' ', "0.1");

        // List of value arguments
        ValueArg<string> source("s", "source", "The path to the unfitered traces. The path must be suffixed by a regular expression that expands to the target files. For example, Valid patterns include:  /home/\\*.cvs  /home/\\??2?.csv, etc. See glob(3)", true, "", "string");
        cmd.add( source );
        
        ValueArg<string> destination("d", "destination","Path to the filtered traces",true,"","string");
        cmd.add( destination );

        ValueArg<string> _accepted("a", "accepted", "Accepted traces sufix label", true, "accept.json", "string");
        cmd.add( _accepted );

        ValueArg<string> _rejected("r", "rejected", "Rejected traces sufix label", true, "reject.json", "string");
        cmd.add( _rejected );
        

        // Parse the argumnets
        cmd.parse( argc, argv );
        ipath = source.getValue();
        opath = destination.getValue();
        atl = _accepted.getValue();
        rtl = _rejected.getValue();

        // Output path preparation
        std::size_t len = opath.length();
        if( opath.substr(len-1,len) != "/" )
            opath.append("/");

        }catch(ArgException &e) {
            cerr << "Error: " << e.error() << " for argument " << e.argId() << endl;
            exit(EXIT_FAILURE);
        }
}


/**
*
*   The main program
*
*/ 
int main(int argc, char** argv)
{
    parseCLA(argc,argv);
    JEventMerge parser{ipath, opath, atl, rtl};
    parser.master();
    return 0;
} 


/**
* 
*   LoadTraceIds,
*
*   Trace events may reside at multiple logs. Traces ids are stored in a frequency table.
*   The table size can be significantly large. No optimization is currently applied to
*   reduce the memory footprint.
*
*   Since concurrent writes are not allowed, logs are loaded sequentially.
*   Only one thread is active while loading a log. 
*
*/ 
unique_ptr<MapDataStream<unsigned long long>> JEventMerge::loadTraceIDs(string log) {
    /*
        No es necesario cargar todos los estados. Es suficiente solo guardar conocimiento de los estados finales.
        Asi cada hilo de ejecucion puede crear una lista independiente con los identificadores de los estados unicos
        que encontro. La funcion puede regresar esa lista. Es posible que el problema sea en mantener todos los datos en 
        Memoria. 
    */
    unique_ptr<MapDataStream<unsigned long long>> dsd{ new MapDataStream<unsigned long long>};
    bitset<5> state; 

    JSONTraceBuffIOS reader(log);
    do{
        state.reset();
        Trace trace = reader.next();
        if( trace.size() != 0 ) {
            if(trace.evicted())
                state.set(0);
            if(trace._failed())
                state.set(1);
            if(trace.finished())
                state.set(2);
            if(trace.killed())
                state.set(3);
            if(trace.lost())
                state.set(4);
        
            if(state.any()) {
                auto tid = trace.get_tid();
                dsd->insert(tid);
            }
        }
    }while(!reader.empty());

    return dsd;
    /*
    JSONTraceBuffIOS reader(log);
    
    std::unique_lock<std::mutex> lk(fdm);
    fdcv.wait(lk,[this]{return available;});
    available = false;
    do{
        Trace trace = reader.next();
        if(trace.size() != 0) {
            auto tid = trace.get_tid();
            dsd.insert(tid);
        }
    }while(!reader.empty());
    available = true;
    lk.unlock();
    fdcv.notify_one();
    */
}


/**
*
*   buildLookupTable,
*
*   Stores duplicate values in a lookup table that enables 
*   concurrent reads.
*
*
*/
void JEventMerge::buildLookupTable(unique_ptr<MapDataStream<unsigned long long>> dsd){
    std::unique_lock<std::mutex> lk(fdm);
    fdcv.wait(lk,[this]{return available;});
    available = false;
    
    std::set<unsigned long long> dup = dsd->getDuplicates();
    for(unsigned long long val : dup)
        duplicates.insert(val, val);
    
    available = true;
    lk.unlock();
    fdcv.notify_one();

    // Free memory of local dsd 
    dsd.reset(nullptr);
}


/**
*
*
*
*/
void JEventMerge::uniqueEventFilter(string log) {
    JSONTraceBuffIOS reader(log);

    // Prepare the output file name and path
    std::size_t pos = log.rfind("/")+1;
    std::string dlog{opath + log.substr(pos,log.size())};
    
    ofstream aosw(dlog);
    aosw << "[";
    do{
        Trace trace = reader.next();
        unsigned long long tid = trace.get_tid();
        // No estoy muy reguro regrese un valor sentinela.
        // TODO: Evaluar regreso correcto de valores en la tabla.
        
        if(duplicates.at(tid)) // Verificar valores de retorno
            aosw << trace.to_json();
    }while(!reader.empty()); 
    aosw << "]"; 
}


/**
*   incompleTraceFilter, remove incomplete traces from the set of logs. It does so, by:
*   1. Loading/storing trace id's in a frequency map (loadTraceIds).
*   2. Creating a lookup table that enables concurrent reads. Built by one thread.
*   3. Filtering of single events. The lookup table is used by all threads concurrently.
*
*/
void JEventMerge::logReader(string log, int id) {
    
    auto dsd = loadTraceIDs(log);
    buildLookupTable(std::move(dsd));
    barrier.wait();
    uniqueEventFilter(log);
    

    // TODO: Evaluate if the following code is needed.
    //barrier.wait();

    // Set state to 1 to indicate that processing is done
    //std::unique_lock<std::mutex> lk(rsm);
    //    state[id] = 1;
    //lk.unlock();
    
    //if(id == 0)
    //    ccv.notify_one();
}


/**
*   The coordinator keeps track of time (global time). It uses the notion of global time, time window, and time instance 
*   of the first event in each log to determine of the log should be processed or not.  If an overlap occurs between the 
*   first event time instance and the time window, the log is processed. Otherwise, processing of the log is delayed.
*/

void JEventMerge::master() 
{
    glob_t globbuf;
    glob(ipath.c_str(), GLOB_NOSORT, NULL, &globbuf);

    barrier.reset(globbuf.gl_pathc);

    // Create as many readers as logs
    boost::dynamic_bitset<> st(globbuf.gl_pathc);
    state = std::move(st);
    
    std::vector<std::thread> pool;
    for(unsigned i=0; i<globbuf.gl_pathc; ++i)
        pool.push_back(std::thread(&JEventMerge::logReader, this, string(globbuf.gl_pathv[i]), i));
    std::for_each(pool.begin(), pool.end(), std::mem_fn(&std::thread::join));

    // At completion. Multiple logs are created. Each should contain traces
    // t i presented two or more eventrs.hatCreate a log with the IDs of complete traces. 
    
    // Create a thread safe lookup table 
    //std::unique_lock<std::mutex> lk(csm);
    //ccv.wait(lk,[this]{ return state.all();});
    //lk.unlock();

    // At completion. Each slave thread created a log with duplicate values.
    // Logs are buffered to memory. As traces become complete, they are flushed to file.
    // At completion. Incomplete traces are flushed to file. 
    // Each trace must be read, concatenated, and flushed to persistance if the trace is complete. 

    globfree(&globbuf);
}

