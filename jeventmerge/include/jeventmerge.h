#ifndef _JEVENT_MERGE_
#define _JEVENT_MERGE_

//#include <map>
#include <thread>
#include <condition_variable>
#include <string>
//#include <queue>
#include <boost/dynamic_bitset.hpp>
//#include "Trace.h"
//#include "JSONTraceBuffIOS.h"
#include "barrier.h"
#include "onlinedatastrmdup.h"
#include "threadsafe_lookup_table.h"

class JEventMerge{
private:
    // Command line arguments
    std::string infile;
    std::string aos;
    std::string ros;

    // Find duplicate mutex
    std::mutex fdm;                 // Find duplicate mutex
    std::condition_variable fdcv;   // Find duplicate condition variable
    bool available;

    std::mutex rsm;                 // Reader state mutex
    std::mutex csm;                 // Coordinator syncronization mutex
    std::condition_variable ccv;    // Coordinator contion variable
    Barrier barrier{0};             // log reader barrier


    // Data stream duplicates
    MapDataStream<unsigned long long> dsd; 
    threadsafe_lookup_table<unsigned long long, unsigned long long,
    std::hash<unsigned long long>> duplicates;
    

    // Log and log reader state
    boost::dynamic_bitset<> state;

    void findDuplicates(std::string log, int chunck);
    void createLookupTable();

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
    infile{in_file}, aos{_aos}, ros{_ros} { available = false; };
};

#endif
