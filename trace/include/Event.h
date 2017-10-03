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

#ifndef _EVENT__
#define _EVENT__

#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

using boost::assign::map_list_of;
using boost::unordered_map;


namespace Event {


/**
*   For schema documentation see:
*   Reiss, C. Google cluster-usage traces: format + schema 2014
*/
enum class EventType : unsigned int {
    submit,         // 0
    schedule,       // 1
    evict,          // 2
    fail,           // 3
    finish,         // 4
    kill,           // 5
    lost,           // 6
    update_pending, // 7
    update_running  // 8
};

/**
* Task and job final event set
*/
static std::set<EventType> FinalEvents = { EventType::evict, EventType::fail, EventType::finish, EventType::kill, EventType::lost};


enum class ResourceEventType : unsigned int {
    add,            // 0, the resource became available
    remove,         // 1, the resource became unavailable
    update          // 2, the resource capacities were updated
};


class Event {
public:
    unsigned long long timestamp{};
    Event(unsigned long long time_stamp):timestamp{time_stamp}{};
    Event() = default;
    unsigned long long timeStamp() const;
    std::string virtual to_json() const = 0;
};

/*
    The Task class is based on the GCUT-Task format (See Google Cluster Usage Traces: format + schema, pp. 8).
    The GUT-Task schema is the following:

    1 - timestamp, 64 bit integer, time at which the task arrived in microseconds. In the case of GCUT the trave starts
                    600 seconds before the beginning of the trace period. For instance, an event 20s after the start of
                    the trace period would have a time stamp equal to 620s

                    The timestamp is transformed from microseconds to milliseconds (1000 microseconds : 1 millisecond) by
                    applying the following operation: (timestamp/1000) + offset. By calling the fixTimeStamp(offset = 600)
                    method. Note offset in fixTimeStamp is in seconds.

                    In two cases an event will not occur within the trace window, such events are represented by:
                    - 0, represents events that occur before the beginning of the trace window.
                    - (2^63) - 1 (MAXINT), represents events that occur after the end of the trace window.

                    Furthermore, clock driff may occur.

    2 - missing info, 8 bit unsigned char, range [0:2], holds knowledge of an eveluation of consistency between state and time changes.

    3 - job_id, 64 bit integer, a unique job identifyer.

    4 - task_index, 64 bit integer, the task index. Tasks with equal task index are distingueshed by their job index.
                    Tasks might have equal task index within the same job if it was restarted.

    5 - machine_id, 64 bit integer, a unique machine identifyer.

    6 - event_type, 8 bit unsigned char,  range [0:8], holds the event event type value.

    7 - user_name, base64 encoded string, hashed and opaque user name. Encodings can be tested for equality.

    8 - scheduling_class, 8 bit unsigned char, represents how latency-sencible the task is. The classes are the following:
                    3 - latency sencible task
                    0 - non production task (not latency sencible)

    9 - priority, 8 bit unsigned char, represents the task importance, with:
                    0 - the lowest priority

    10 - norm_req_cores, float, normalized relative to the largest capacity of the resource on any machine.

    11 - norm_req_ram, float, same as 10.

    12 - norm_req_disk, float, same as 10.

    13 - constraints, boolean, represents task placement constraints.
*/


class TaskEvent : public Event {
  public :
    // 1 in super class Event
    unsigned int        missing_info{};     // 2
    unsigned long long  job_id{};           // 3
    unsigned long long  task_index{};       // 4
    unsigned long long  machine_id{};       // 5
    EventType           event_type;         // 6, uninitialized
    std::string         user_name;          // 7
    unsigned int        scheduling_class{}; // 8
    unsigned int        priority{};         // 9
    float               norm_req_cores{};   // 10
    float               norm_req_ram{};     // 11
    float               norm_req_disk{};    // 12
    bool                constraints;        // 13
    std::string         job_type;           // Rigid
    std::string         id;                 // job_id + task_index

    TaskEvent(const std::string&);
    TaskEvent() : Event() {};
    std::string to_json() const override;
};



class JobEvent : public Event { 
    public :
    // 1 in super class Event
    unsigned int        missing_info{};
    unsigned long long  job_id{};
    EventType           event_type;         // uninitialized
    std::string         user_name;
    unsigned int        scheduling_class{};
    std::string         job_name;
    std::string         logical_job_name;
    unsigned int        priority{};
    std::string         job_type;            // Composite

    JobEvent(const std::string&);
    JobEvent() : Event() {};
    std::string to_json() const override;
};



class ResourceEvent : public Event {
    public :
    // 1 in super class Event
    std::string         resource_class;
    unsigned long long  resource_id{};
    ResourceEventType   event_type;
    std::string         platform_id;
    unsigned long long  cpu_capacity{};
    unsigned long long  ram_capacity{};
    unsigned long long  disk_capacity{};

    ResourceEvent(const std::string&);
    ResourceEvent() : Event() {};
    std::string to_json() const override;
};


/* Event helper function */
struct Event_Comparator {
    bool operator()(const Event &e1, const Event &e2) const {
    return (e1.timeStamp() < e2.timeStamp());
    }
};


/* EventType helper function: String to EventType */
const unordered_map<std::string, EventType> str2st = map_list_of
    ("submit",EventType::submit)
    ("schedule",EventType::schedule)
    ("evict",EventType::evict)
    ("fail",EventType::fail)
    ("finish",EventType::finish)
    ("kill",EventType::kill)
    ("lost",EventType::lost)
    ("update_pending",EventType::update_pending)
    ("update_running",EventType::update_running);


/* EventType helper function: EventType to String */
const unordered_map<EventType, std::string> st2str = map_list_of
    (EventType::submit,"submit")
    (EventType::schedule,"schedule")
    (EventType::evict,"evict")
    (EventType::fail,"fail")
    (EventType::finish,"finish")
    (EventType::kill,"kill")
    (EventType::lost,"lost")
    (EventType::update_pending,"update_pending")
    (EventType::update_running,"update_running");


/* EventType helper function: validates state changes */
bool validateEventChange(EventType current, EventType next);

}

#endif
