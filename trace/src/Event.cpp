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

#include <boost/algorithm/string.hpp>
#include <string>
#include <cstdlib>
#include "Event.h"
#include <iostream>

using namespace boost;
using namespace std;


Event::Event::Event() { };


Event::Event::Event(unsigned long long time_stamp) : 
    timestamp {time_stamp} { };


unsigned long long Event::Event::timeStamp() const{
    return timestamp;
};


Event::TaskEvent::TaskEvent() : Event() { };


Event::TaskEvent::TaskEvent(const std::string& line) : Event() {
    std::vector<std::string> values;

    /*
    1 - timestamp        values[0]
    2 - missing_info     values[1]
    3 - job_id           values[2]
    4 - task_index       values[3]
    5 - machine_id       values[4]
    6 - event_type       values[5]
    7 - user_name        values[6]
    8 - scheduling_class values[7]
    9 - priority         values[8]
    10 - norm_req_cores  values[9]
    11 - norm_req_ram    values[10]
    12 - norm_req_disk   values[11]
    13 - constraints     values[12]

    Validation: Empty string inputs are labeled -1 */

    split(values, line, is_any_of(","));

    timestamp       = (unsigned long long) atoll(values[0].c_str());
    missing_info    = (values[1].empty())? -1 : (unsigned int) atoi(values[1].c_str());
    job_id          = (values[2].empty())? -1 : (unsigned long long) atoll(values[2].c_str());
    task_index      = (values[3].empty())? -1 : (unsigned long long) atoll(values[3].c_str());
    machine_id      = (values[4].empty())? -1 : (unsigned long long) atoll(values[4].c_str());
    event_type      = static_cast<EventType>((unsigned int) atoi(values[5].c_str()));
    user_name       = (values[6].empty())? "-1" : values[6];
    scheduling_class= (values[7].empty())? -1 : (unsigned int) atoi(values[7].c_str());
    priority        = (values[8].empty())? -1 : (unsigned int) atoi(values[8].c_str());
    norm_req_cores  = (values[9].empty())? -1 : stof(values[9].c_str());
    norm_req_ram    = (values[10].empty())? -1 : stof(values[10].c_str());
    norm_req_disk   = (values[11].empty())? -1 : stof(values[11].c_str());
    constraints     = (values[12].empty())? false : values[12]=="1";
    job_type        = string{"Rigid"};
    id              = string{ values[2] + values[3] };

};


string Event::TaskEvent::to_json() const {
    
    string json =
    "{"
    "\"timestamp\":" +      std::to_string(timestamp) + ","
    "\"missing_info\":" +   std::to_string(missing_info) + ","
    "\"job_id\":" +         std::to_string(job_id) + ","
    "\"task_index\":" +     std::to_string(task_index) + ","
    "\"machine_id\":" +     std::to_string(machine_id) + ","
    "\"event_type\":" +     std::to_string(static_cast<int>(event_type)) + ","
    "\"user_name\":\"" +      user_name + "\","
    "\"scheduling_class\":"+std::to_string(scheduling_class) + ","
    "\"priority\":" +       std::to_string(priority) + ","
    "\"norm_req_cores\":" + std::to_string(norm_req_cores) + ","
    "\"norm_req_ram\":" +   std::to_string(norm_req_ram) + ","
    "\"norm_req_disk\":" +  std::to_string(norm_req_disk) + ","
    "\"constraints\":" +    std::to_string(constraints) +
    "}";

    return json;
}


Event::JobEvent::JobEvent(const std::string& line) : Event() {
    std::vector<std::string> values;

    /* 
    1 - timestamp        values[0]
    2 - missing_info     values[1]
    3 - job_id           values[2]
    4 - event_type       values[3]
    5 - user_name        values[4]
    6 - scheduling_class values[5]
    7 - job_name         values[6]
    8 - logical_job_name values[7]
    9 - priority         -1
    10- job_type         "Composite"

    Validation: Empty string inputs are labeled -1 */

    split( values, line, is_any_of(","));
    
    timestamp       = (unsigned long long) atoll(values[0].c_str());
    missing_info    = (values[1].empty())? -1 : (unsigned int) atoi(values[1].c_str());
    job_id          = (values[2].empty())? -1 : (unsigned long long) atoll(values[2].c_str());
    event_type      = static_cast<EventType>((unsigned int) atoi(values[3].c_str()));
    user_name       = (values[4].empty())? "-1" : values[4];
    scheduling_class= (values[5].empty())? -1 : (unsigned int) atoi(values[5].c_str());
    job_name        = (values[6].empty())? "-1" : values[6];
    logical_job_name= (values[7].empty())? "-1" : values[7];
    priority        = -1;
    job_type        = string{"Composite"};
};


string Event::JobEvent::to_json() const {

    string json =
    "{"
    "\"timestamp\":" +      std::to_string(timestamp) + ","
    "\"missing_info\":" +   std::to_string(missing_info) + ","
    "\"job_id\":" +         std::to_string(job_id) + ","
    "\"event_type\":" +     std::to_string(static_cast<int>(event_type)) + ","
    "\"user_name\":" +      user_name + ","
    "\"scheduling_class\":"+std::to_string(scheduling_class) + ","
    "\"job_name\":" +       job_name + ","
    "\"logical_job_name\":"+logical_job_name + ","
    "\"priority\":" +       std::to_string(priority) + ","
    "\"job_type\":" +       job_type +
    "}";

    return json;
}



Event::ResourceEvent::ResourceEvent(const std::string& line) : Event(){
    std::vector<std::string> values;

    /* 
    1 - timestamp        values[0]
    2 - resource_id      values[1]
    3 - event_type       values[2]
    4 - platform_id      values[3]
    5 - cpu_capacity     values[4]
    6 - ram_capacity     values[5]
    7 - disk_capacity    NA
    8 - resource_class   1 = machine
    
    Validation: Empty string inputs are labeled -1 */

    split( values, line, is_any_of(","));

    timestamp       = (long long unsigned) atoll(values[0].c_str());
    resource_id     = (values[1].empty())? -1 :(unsigned long long) atoll(values[1].c_str());
    event_type      = static_cast<ResourceEventType>((unsigned int) atoi(values[2].c_str()));
    platform_id     = (values[3].empty())? "-1" : values[3];
    cpu_capacity    = (values[4].empty())? -1 : (unsigned int) atoi(values[4].c_str());
    ram_capacity    = (values[5].empty())? -1 : (unsigned int) atoi(values[5].c_str());
    disk_capacity   = -1;     
    resource_class  = string{"Machine"};
};


string Event::ResourceEvent::to_json() const {

    string json =
    "{"
    "\"timestamp\":" +      std::to_string(timestamp) + ","
    "\"resource_id\":" +    std::to_string(resource_id) + ","
    "\"event_type\":" +     std::to_string(static_cast<int>(event_type)) + ","
    "\"platform_id\":" +    platform_id + ","
    "\"cpu_capacity\":" +   std::to_string(cpu_capacity) + ","
    "\"ram_capacity\":" +   std::to_string(ram_capacity) + ","
    "\"disk_capacity\":" +  std::to_string(disk_capacity) + ","
    "\"resource_class\":" + resource_class +
    "}";

    return json;
}



// TODO: Deprecated
//Event::Event::Event() { }

/*
Espc::Event::Event(const string &str, const long instance ) : 
    tstamp{instance} {
    etype = static_cast<EventType>(stoi(str));
}

Espc::Event::Event(const EventType event, const long instance ) : 
    etype{event},
    tstamp{instance} { }
    


long Espc::Event::timestamp() const {
    return tstamp;
}


Espc::EventType Espc::Event::type() const{
    return etype;
}
*/

bool Event::validateStateChange(EventType current, EventType next) {
    bool result;
    
    switch(current) {

        case EventType::submit:
            result = next == EventType::update_pending || next == EventType::schedule ||
                     next == EventType::fail || next == EventType::kill ||
                     next == EventType::lost;
            break;

        case EventType::schedule:
            result = next == EventType::update_running || next == EventType::evict ||
                     next == EventType::fail || next == EventType::finish ||
                     next == EventType::kill || next == EventType::lost;
            break;

/*      Disconsidered transitions      

        case EventType::evict:
            result = next == EventType::submit;
            break;

        case EventType::fail:
            result = next == EventType::submit;
            break;

        case EventType::finish:
            result = next == EventType::submit;
            break;

        case EventType::kill:
            result = next == EventType::submit;
            break;
        
        case EventType::lost:
            result = next == EventType::submit;
            break;
*/

        case EventType::update_pending:
            result = next == EventType::schedule || next == EventType::fail || 
                     next == EventType::kill || next == EventType::lost ||
                     next == EventType::update_pending;
            break;

        case EventType::update_running:
            result = next == EventType::evict || next == EventType::fail ||
                     next == EventType::finish || next == EventType::kill ||
                     next == EventType::lost || next == EventType::update_running;
            break;

        default:
            result = false;
            break;
    }

    return result;
}


int Event::reindex(EventType event) {
    int final_events[] = { 2, 3, 4, 5, 6 };
    int trans_events[] = { 1, 0 };
    //int loop_events[] =  { 7, 8 };
    int i = static_cast<int>(event);

    bool result = std::find(std::begin(final_events), std::end(final_events), i) != std::end(final_events);
    if( result )
        return 2;

    result = std::find(std::begin(trans_events), std::end(trans_events), i) != std::end(trans_events);
    if( result )
        return i;

    // state correspond to State::update_pending or State::update_running
    return i - 4;
}
