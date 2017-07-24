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
// #include <typeinfo> for type(variable).name()
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


ETrace::AvgAllocResources::AvgAllocResources() 
{
    num_req_cores = 0;
    num_req_ram   = 0;
    num_req_disk  = 0;
    sum_norm_req_cores = 0;
    sum_norm_req_ram   = 0;
    sum_norm_req_disk  = 0; 
};


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


ETrace::Trace::Trace() 
{
    _empty = true;
};


ETrace::Trace::Trace(const TaskEvent &event) 
{
    lre = event;
    events.insert(event);
    resources.add(event);
    tid  = event.id;
    jid  = event.job_id;
    event_bs.none();
    event_bs.set(static_cast<int>(EventType::submit));

    // Assign a UUID (Boost)
    boost::uuids::uuid uid = uuids::random_generator()();
    uuid = boost::uuids::to_string(uid);

    // Empty
    _empty = false;
};


ETrace::Trace::Trace(const string &json) 
{
    Document d;
    d.Parse(json.c_str());
    event_bs.none();

    tid = d["trace_id"].GetString();
    jid = d["job_id"].GetUint64();
    user = d["user_name"].GetString();
    uuid = d["uuid"].GetString();
    startTime = d["events"][0]["timestamp"].GetUint64();

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
        addEvent(event);
        i++;
    }
    // Empty
    _empty = false;
}


long ETrace::Trace::distance(const TaskEvent &ne) 
{
    return abs( lre.timeStamp() - ne.timeStamp() );
}


TaskEvent& ETrace::Trace::last_event() 
{
    return lre;
}


void ETrace::Trace::addEvent(const TaskEvent &event)
{
    lre = event;
    events.insert(event);
    resources.add(event);
    event_bs.set(reindex(event.event_type));
}


string ETrace::Trace::to_json() const 
{
    string ev;
    for(set<TaskEvent>::iterator it = events.begin(); it != events.end(); it++)
        ev += it->to_json() + ",";
    ev.pop_back(); 
    
    string json =
    "{"
    "\"trace_id\":\"" +   tid + "\","
    "\"job_id\":" +     std::to_string(jid) + ","
    "\"user_name\":\"" +  user + "\","
    "\"uuid\":\"" + uuid + "\","
    "\"events\": [" +   ev +
    "]," +
    resources.to_json() +
    "}";

    return json;
}


string ETrace::Trace::id() const 
{
    return tid;
}


bool ETrace::Trace::merge(Trace &other) 
{
    TaskEvent e1 = *events.rbegin();
    TaskEvent e2 = *other.events.begin();
    
    if( validateStateChange( e1.event_type, e2.event_type) && e1.timeStamp() < e2.timeStamp() ) {
        event_bs = event_bs |= other.event_bs;
        events.insert(other.events.begin(), other.events.end());
        return true;
    }
    
    return false;
}


bool ETrace::Trace::isComplete() 
{
    int val = (int) event_bs.to_ulong();
    auto search = valid_state.find(val);
    if( search != valid_state.end())
        return true;
    return false;
}

bool ETrace::Trace::empty() 
{
    return _empty;
}
