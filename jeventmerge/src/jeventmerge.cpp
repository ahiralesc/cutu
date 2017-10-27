#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <glob.h>
#include <cmath>
#include <climits>
#include <thread>
#include <mutex>
#include <set>
#include "tclap/CmdLine.h"
#include "JEventMerge.h"

using namespace ETrace;
using namespace Event;
using namespace TCLAP;
using namespace std;


/* the input file name */
string in_file{};


/* Accepted traces output file name */
string aos{};


/* Rejected traces output file name */
string ros{};


/**
*  Parses the Command Line Arguments (CLA). Valid switches for this application are the following:
*  - r, specifies the number of rows that will be loaded in memory.
*
*/
void parseCLA(int argc, char** argv) {
    try{
        CmdLine cmd("jeventmerge (JSON Event Merge) perfoms two phases: localization and evaluation. The first phase, finds and concatenates events labeled with equal trace identifier. Events are extrated concurrently from multiple JSON formatted logs. Concatenation is applied if temporal order and valid transitions are satisfied between adyacent events. The later phase, classifies traces as either complete or incomplete. A trace is complete if it contains the initial event, reaches a final event, and all event transitions are valid, otherwise it is incomplete. \n\
\n\
Modes of execution: \n\
jeventmerge -f <path to JSON logs> -a <accepted trace filename>.json -r <rejected trace filename>.json", ' ', "0.1");

        // List of value arguments
        ValueArg<string> in_file_name("f", "in_file", "The path to the JSON traces", true, "", "string");
        cmd.add( in_file_name );

        ValueArg<string> accepted_os("a", "accepted", "The file name where the accepted traces will be stored", true, "accept.json", "string");
        cmd.add( accepted_os );

        ValueArg<string> rejected_os("r", "rejected", "The file name where the rejected traces will be stored", true, "reject.json", "string");
        cmd.add( rejected_os );
        

  // Parse the argumnets
        cmd.parse( argc, argv );
        in_file = in_file_name.getValue();
        aos = accepted_os.getValue();
        ros = rejected_os.getValue();

        }catch(ArgException &e) {
            cerr << "Error: " << e.error() << " for argument " << e.argId() << endl;
            exit(EXIT_FAILURE);
        }
}


/**
*   The main program
*/ 
int main(int argc, char** argv)
{
    parseCLA(argc,argv);
    JEventMerge parser{in_file, aos, ros};
    parser.coordinator();
    return 0;
} 


/**
* 1. Load log, purge, and store onto a vector 
*
*/
void JEventMerge::logReader(string log, int i) {
    JSONTraceBuffIOS reader(log);
    std::vector<tuple> l;

    while(!reader.empty()){
        Trace trace = reader.next();
        TT tuple{trace,i};
        l.push_back(tuple);
    }

/*
    if(!reader.empty()){
        RCB rcb;
        rcb.id = std::this_thread::get_id();
        rcb.breader = &reader;
        unsigned long long ltime = reader.peek().time_stamp();
        stm.lock();
        if(ltime < globalTime)
            globalTime = ltime;        
        readers.insert(std::pair<int,RCB>(i,rcb));
        state[i] = 1;
        stm.unlock();
    }
   
    barrier.wait();
    if( i == 0 ) 
        trec.notify_one();
*/
  /*  do{
        std::unique_lock<std::mutex> lk(revet);
        rcv.wait(lk, [this] {return turn == i;});
        lk.unlock();

    }while(1);*/
}


/**
*   The coordinator keeps track of time (global time). It uses the notion of global time, time window, and time instance 
*   of the first event in each log to determine of the log should be processed or not.  If an overlap occurs between the 
*   first event time instance and the time window, the log is processed. Otherwise, processing of the log is delayed.
*/
void JEventMerge::coordinator( ) 
{
    glob_t globbuf;
    glob(infile.c_str(), GLOB_NOSORT, NULL, &globbuf);

    std::cout << "Bitacoras: " << globbuf.gl_pathc << std::endl;
    barrier.reset(globbuf.gl_pathc);

    // Set the size of the readers state
    boost::dynamic_bitset<> st(globbuf.gl_pathc);
    state = std::move(st);
    
    std::vector<std::thread> pool;
    for(unsigned i=0; i<globbuf.gl_pathc; ++i)
        pool.push_back(std::thread(&JEventMerge::logReader, this, string(globbuf.gl_pathv[i]), i));
    std::for_each(pool.begin(), pool.end(), std::mem_fn(&std::thread::join));
    
    std::unique_lock<std::mutex> lk(gtm);    
    trec.wait(lk, [this]{return state.all();}); 
    std::cout << "El tiempo global es : " << globalTime << std::endl;
    lk.unlock();

    globfree(&globbuf);
}


/*
void JEventMerge::process()
{
    
    // A coordinator thread evaluates wich threads can be woken.
    std::thread poolManager(&coordinator);

    
    
    // Load buffered JSON readers 
    readers.none();
    for(unsigned int i=0; i<globbuf.gl_pathc; ++i){
        readers.set(i);
        logs.push_back(JSONTraceBuffIOS(globbuf.gl_pathv[i],num_traces));    
        //std::cout << "Bitacora cargada : " << glob_result.gl_pathv[i] << std::endl;
    }
    
    while(available_readers.any()){
        
        // Fill the priority queue
        for(int i=0; i<logs.size(); ++i){
            if(logs[i].empty()) {
                readers.reset(i);
            }else {
                int count{0};
                while(!logs[i].empty()) {
                    Trace trace = logs[i].next();
                    queue.push(trace);
                }
            }
        } 

        // Merge traces with existing traces.
        while(!queue.empty()) {
            Trace trace = queue.top();
            queue.pop();

            string id = trace.tid;
            cout << "Trace id is " << id << std::endl;
            map<string,Trace>::iterator it = traces.find(id);
            if(it != traces.end()) {
                traces[id].merge(trace);
                if(traces[id].isComplete()){
                    completeTraces.push_back(traces[id].to_json());
                    traces.erase(it);
                }
            }else
                traces[id] = trace;
        }
    }


    // Cleanup. Add rejected traces to incomplete list.
    for ( const auto &k : traces )
        incompleteTraces.push_back(k.second.to_json());
    

    // Flush accepted traces
    ofstream aosw(aos);
    string out = "[";
    for(int i=0; i<completeTraces.size(); ++i) 
        out += completeTraces[i] + ",";
    out.pop_back();
    out += "]";
    aosw << out;
    aosw.close();
    
    // Flush rejected traces
    ofstream rosw(ros);
    out = "[";
    for (int i=0; i<incompleteTraces.size(); ++i) 
        out += incompleteTraces[i] + ",";
    out.pop_back();
    out += "]";
    rosw << out;
    rosw.close();
} */
