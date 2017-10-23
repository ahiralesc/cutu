#ifndef _JEVENT_MERGE_
#define _JEVENT_MERGE_

#include <map>
#include <climits>
#include <thread>
#include <condition_variable>
#include <string>
#include <queue>
#include <boost/dynamic_bitset.hpp>
#include "Trace.h"
#include "JSONTraceBuffIOS.h"
#include "Barrier.h"

class JEventMerge{
    private:

    // Reader control block
    struct RCB { 
        std::thread::id id;
        JSONTraceBuffIOS *breader;
    };

    // Command line arguments
    std::string infile;
    std::string aos;
    std::string ros;

    // Mutex for concurrent parsing of logs
    std::mutex gtm;                 // Global time mutex
    std::mutex stm;                 // Reader state mutex
    std::condition_variable trec;   // Time recorded completion event
    Barrier barrier{0};             // log reader barrier

    // Log and log reader state
    boost::dynamic_bitset<> state;
    std::map<int,RCB> readers;

    // Global time
    unsigned long long globalTime{ULLONG_MAX};
public:

    void logReader(std::string log, int i);
    void coordinator();

    /*
    std::priority_queue<ETrace::Trace, std::vector<ETrace::Trace>,
    ETrace::StartTimeComparator> queue;
    std::map<std::string,ETrace::Trace> traces;
    std::vector<JSONTraceBuffIOS> logs;
    std::vector<std::string> completeTraces;
    std::vector<std::string> incompleteTraces;
    */

    
    JEventMerge(std::string in_file, std::string _aos, std::string _ros) :
    infile{in_file}, aos{_aos}, ros{_ros} { };
    //void process();
};

#endif
