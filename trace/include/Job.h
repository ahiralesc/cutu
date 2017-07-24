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


#ifndef _JOB__
#define _JOB__

namespace Job {

class Job {
    private:
        std::string id;
        JobType     type;

    public:
        Job(const std:string &jid, const JobType &jtype) : id{jid}, type{jtype};
        string::string get_type() const;
        JobType get_type() const;
};


class Rigid : Job {
    public:
        unsigned long   allocated_cores;
        unsigned long   allocated_disk;
        unsigned long   allocated_ram;
        unsigned long   allocated_time;
        std:string      comp_job_id;
        JobClass        job_class;
        User            proprietor;
        unsigned short  priority;
        unsigned long   requested_cores;
        unsigned long   requested_disk;
        unsigned long   requested_ram;
        unsigned long   requested_time;
        unsigned long   run_time;
        JobStatus       job_status;
        unsigned long long submit_time
        unsigned long   wait_time;
        Provenance      provenance;

       Rigid() {
            Job::Job{};
        }
};



class Composite : Job {
    public:
        unsigned long long  submit_time;
        unsigned long   cp_wait_time;
        unsigned long   cp_run_time
};


class User {
    public:
        std::string  id;
        std::string  gid;
};


enum class JobStatus : int {
    FAILED = 0,
    COMPLETED,
    PARTIAL_EXEC,
    PARTIAL_EXEC_COMP,
    PARTIAL_EXEC_FAILED,
    CANCELED
};


enum class JobType : int {
    RIGID,
    DAG
};


enum class JobClass : int {
    SCRIPT,
    COMPUTE,
    DATA,
    COMUNICATION
};

#endif
