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

#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Trace.h"
#include "Event.h"
#include "rapidjson/schema.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace boost;
using namespace Event;
using namespace ETrace;
using namespace rapidjson;


void ETrace::AvgAllocResources::add(const TaskEvent &ev ) 
{
    if( ev.norm_req_cores > 0 ) {
        num_req_cores++;
        sum_norm_req_cores += ev.norm_req_cores;
    }
    
    if( ev.norm_req_ram > 0 ) {
        num_req_ram++;
        sum_norm_req_ram += ev.norm_req_ram;
    }

    if( ev.norm_req_disk > 0) {
        num_req_disk++;
        sum_norm_req_disk += ev.norm_req_disk;
    }
};

void ETrace::AvgAllocResources::clear() {
    num_req_cores = 0;
    num_req_ram = 0;
    num_req_disk = 0;
    sum_norm_req_cores = 0.0;
    sum_norm_req_ram = 0.0;
    sum_norm_req_disk = 0.0;
};


string ETrace::AvgAllocResources::to_json() const 
{
    float avgCores = (num_req_cores > 0)? (float)(sum_norm_req_cores/num_req_cores) : -1;
    float avgRam   = (num_req_ram > 0)? (float)(sum_norm_req_ram/num_req_ram) : -1;
    float avgDisk  = (num_req_disk > 0)? (float)(sum_norm_req_disk/num_req_disk) : -1;

     std::string json =
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\":" + std::to_string(avgCores) + ","
    "\"avg_norm_alloc_ram\":" + std::to_string(avgRam) + ","
    "\"avg_norm_alloc_disk\":" + std::to_string(avgDisk) + " }";
    
    return json;
};


ETrace::Trace::Trace(const string &json) 
{
    Document d;
    d.Parse(json.c_str());

    tid = d["trace_id"].GetString();
    jid = d["job_id"].GetUint64();
    user = d["user_name"].GetString();
    uuid = d["uuid"].GetString();
    startTime = d["timestamp"].GetUint64();

    int i = 0;

    for(auto& v : d["events"].GetArray()) {
        string str{};
        str += to_string(d["events"][i]["timestamp"].GetUint64());
        str += ",";
        str += to_string(d["events"][i]["missing_info"].GetInt());
        str += ",";
        str += to_string(d["events"][i]["job_id"].GetUint64());
        str += ",";
        str += to_string(d["events"][i]["task_index"].GetUint64());
        str += ",";
        str += to_string(d["events"][i]["machine_id"].GetUint64());
        str += ",";
        str += to_string(d["events"][i]["event_type"].GetInt());
        str += ",";
        str += d["events"][i]["user_name"].GetString();
        str += ",";
        str += to_string(d["events"][i]["scheduling_class"].GetInt());
        str += ",";
        str += to_string(d["events"][i]["priority"].GetInt());
        str += ",";
        str += to_string(d["events"][i]["norm_req_cores"].GetFloat());
        str += ",";
        str += to_string(d["events"][i]["norm_req_ram"].GetFloat());
        str += ",";
        str += to_string(d["events"][i]["norm_req_disk"].GetFloat());
        str += ",";
        str += to_string(d["events"][i]["constraints"].GetBool());

        TaskEvent event{str};
        insert(event);
        i++;
    }
};


bool ETrace::Trace::empty() {
    return (events.size() == 0)? true : false;
};


unsigned int ETrace::Trace::size() {
    return events.size();
};


void ETrace::Trace::insert(const TaskEvent &event) {
    if( empty() && tid == "") {
        jid  = event.job_id;
        tid  = event.id;
        user = event.user_name;
        // startTime is set when the trace is sent to persistence
        boost::uuids::uuid uid = uuids::random_generator()();
        uuid = boost::uuids::to_string(uid);
    }
    events.insert(event);
    resources.add(event);
};


void ETrace::Trace::clear() {
    jid = -1;
    tid.clear();
    user.clear();
    uuid.clear();
    startTime = -1;
    events.clear();
    resources.clear();
};


// The UUID of the parameter trace is lost
bool ETrace::Trace::merge(Trace &trace) {
    if( jid != trace.get_jid() ) 
        return false;
    
    events.insert(trace.events.begin(),trace.events.end());
    // Startime is set when the trace is sent to persistance
    return true;
};

// Returns an empty object if not found
TaskEvent& ETrace::Trace::last_event() {
    std::vector<Event::TaskEvent> ev(events.begin(), events.end());
    return ev[ev.size()-1];
};


bool ETrace::Trace::validateEvent(Event::EventType etype) {
    for(Event::TaskEvent tev : events) {
        if( tev.event_type == etype )
            return true;
    }
    return false;
};


bool ETrace::Trace::evicted(){
    return validateEvent(EventType::evict);
};


bool ETrace::Trace::_failed(){
    return validateEvent(EventType::fail);
};


bool ETrace::Trace::finished(){
    return validateEvent(EventType::finish);
};


bool ETrace::Trace::completed(){
    return (last_event().event_type == EventType::finish);
};


bool ETrace::Trace::killed(){
    return validateEvent(EventType::kill);
};


bool ETrace::Trace::lost() {
    return validateEvent(EventType::lost);
};

/**
*   @brief  A trace is complete if it begins with a submmit event,
*        reaches one of the following events {evicted, fail, finish, kill, or lost}
*        and all its transitions are valid.   
*  
*   @return true is the trace is complete, false otherwise 
*/
bool ETrace::Trace::isComplete() {
    std::vector<Event::TaskEvent> event(events.begin(), events.end());
    unsigned sz = event.size();
    if(event[0].event_type != EventType::submit)
        return false;
    auto search = FinalEvents.find(event[sz].event_type);
    if( search != FinalEvents.end() )
        return false;
    for(unsigned i=0; i<sz; i++)
        if(!validateEventChange(event[i].event_type,event[i+1].event_type))
            return false;
    return true;
}


/**
*   @brief A trace is resubmutted if at least one of the following transitions exists
*           evict to submit
*           fail to submit
*           finish to submit
*           kill to submit
*           lost to submit
*   The method does not analyse the number ot times the job was resubmitted.
*/
bool ETrace::Trace::isResubmitted() {
    std::vector<Event::TaskEvent> event(events.begin(),events.end());
    std::vector<unsigned> index;
    unsigned sz = events.size();

    // Find all task finalization events
    for(unsigned i=0; i<sz; i++) {
        auto search = FinalEvents.find(event[i].event_type);
        if(search != FinalEvents.end())
            index.push_back(i);
    }
    
    for(unsigned i : index)
        if( i+1 < sz )
            if(event[i+1].event_type == EventType::submit)
                return true;
    
    return false;
}


string ETrace::Trace::to_json() const 
{
    unsigned long long minStartTime = ULLONG_MAX;
    string user_name;
  
    string ev;
    for(set<TaskEvent>::iterator it = events.begin(); it != events.end(); it++) {
        unsigned long long ts = it->timeStamp();
        if( ts < minStartTime ) {
            minStartTime = ts;
            user_name = it->user_name;
        }
        ev += it->to_json() + ",";
    }
    ev.pop_back(); 
    
    string json =
    "{"
    "\"trace_id\":\"" +   tid + "\","
    "\"job_id\":" +     std::to_string(jid) + ","
    "\"user_name\":\"" +  user + "\","
    "\"uuid\":\"" + uuid + "\","
    "\"timestamp\":" + std::to_string(minStartTime) + ","
    "\"events\": [" +   ev +
    "]," +
    resources.to_json() +
    "}";

    return json;
}
