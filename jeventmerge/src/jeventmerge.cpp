#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <glob.h>
#include <cmath>
#include <climits>
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

//    JEventMerge parser{in_file, aos, ros};
 //   parser.process();

    return 0;
} 



void JEventMerge::process()
{
    const int num_traces = 100;
    glob_t glob_result;
    glob(infile.c_str(), GLOB_TILDE, NULL ,&glob_result);
    bitset<200> available_readers;
    
    // Load buffered JSON readers 
    available_readers.none();
    for(unsigned int i=0; i<glob_result.gl_pathc; ++i){
        available_readers.set(i);
        logs.push_back(JSONTraceBuffIOS(glob_result.gl_pathv[i],num_traces));    
    }
    
    while(available_readers.any()){
        
        // Prepare the priority queue
        for(int i=0; i<logs.size(); ++i){
            if(logs[i].empty())
                available_readers.reset(i);
            else {
                int count{0};
                while(!logs[i].empty() && count < num_traces) {
                    queue.push(logs[i].next());
                    count++;
                }
            }
        } 

        // Merge traces with existing traces.
        while(!queue.empty()) {
            Trace trace = queue.top();
            queue.pop();

            string id = trace.tid;
            map<string,Trace>::iterator it = traces.find(id);
            if(it != traces.end()) {
                if(traces[id].merge(trace))
                    if(traces[id].isComplete()){
                        completeTraces.push_back(traces[id].to_json());
                        traces.erase(it);
                    }
                else {
                    // Trace could not be merged. Start a new trace
                    incompleteTraces.push_back(traces[id].to_json());
                    traces[id] = trace;
                }
            } else //Start a new trace entry
                traces[id] = trace;
        }

        // Flush complete traces if at least X has been created.
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
}
