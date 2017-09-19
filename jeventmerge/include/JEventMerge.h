#ifndef _JEVENT_MERGE_
#define _JEVENT_MERGE_

#include <vector>
#include <string>
#include <queue>
#include "Trace.h"
#include "JSONTraceBuffIOS.h"

class JEventMerge{
    private:

    // For parsing command line arguments
    std::string infile;
    std::string aos;
    std::string ros;

    std::priority_queue<ETrace::Trace, std::vector<ETrace::Trace>,
    ETrace::StartTimeComparator> queue;
    std::map<std::string,ETrace::Trace> traces;
    std::vector<JSONTraceBuffIOS> logs;
    std::vector<std::string> completeTraces;
    std::vector<std::string> incompleteTraces;

    void writeTraces();

    public:
    
    JEventMerge(std::string in_file, std::string _aos, std::string _ros) :
    infile{in_file}, aos{_aos}, ros{_ros} { };
    void process();
};

#endif
