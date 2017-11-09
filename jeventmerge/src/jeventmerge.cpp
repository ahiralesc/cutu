//#include <iostream>
//#include <fstream>
//#include <rapidjson/document.h>
#include <glob.h>
#include <set>
//#include <cmath>
//#include <climits>
//#include <thread>
//#include <mutex>
//#include <set>
#include "tclap/CmdLine.h"
#include "JEventMerge.h"
#include "JSONTraceBuffIOS.h"
#include "Trace.h"

using namespace ETrace;
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
//    parser.coordinator();
    return 0;
} 


/**
* Task labeling is done by storing a single instance of duplicate task ids in shared data structure.
* To reduce access contention to the shared data structure, log readers are allowed to have access to
* itLabel tasks as either complete or incomplete
*
*/
void JEventMerge::findDuplicates(string log, int chunk) {
    JSONTraceBuffIOS reader(log);
    do{
        std::unique_lock<std::mutex> lk(fdm);
        if(!available){
            available= true;
            auto n = chunk;
            while(--n!=0) {
                Trace trace = reader.next();
                if(trace.size()!=0) {
                    auto val = trace.get_tid();
                    dsd.insert(val);
                }
            }
            n = chunk;
            available = false; 
            fdcv.notify_one();
            std::this_thread::yield();
        } else
            fdcv.wait(lk,[this]{ return !available;});
    }while(!reader.empty());
}


void JEventMerge::createLookupTable(){
    std::set<unsigned long long> dup = dsd.getDuplicates();
//    for(unsigned long long val : dup)
//        duplicates.add_or_update_mapping(val, val); 
}



void JEventMerge::logReader(string log, int i) {
    findDuplicates(log, 1000);
    rsm.lock();
        state[i] = 1;
    rsm.unlock();
    barrier.wait();
    if( i == 0)
        ccv.notify_one();
    
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

    // Create as many readers as logs found
    boost::dynamic_bitset<> st(globbuf.gl_pathc);
    state = std::move(st);
    
    std::vector<std::thread> pool;
    for(unsigned i=0; i<globbuf.gl_pathc; ++i)
        pool.push_back(std::thread(&JEventMerge::logReader, this, string(globbuf.gl_pathv[i]), i));
    std::for_each(pool.begin(), pool.end(), std::mem_fn(&std::thread::join));

    // Create a thread safe lookup table 
    std::unique_lock<std::mutex> lk(csm);
    ccv.wait(lk,[this]{ return state.all();});
    createLookupTable();
    lk.unlock();

    globfree(&globbuf);
}

