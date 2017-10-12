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

//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "trace test"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "Trace.h"
#include "JSONTraceBuffIOS.h"

using namespace ETrace;
using namespace std;

BOOST_AUTO_TEST_SUITE(JBIOTest) 

BOOST_AUTO_TEST_CASE( emptyness ) 
{
    string json =  "{"
    "\"job_id\":2902878580,"
    "\"user_name\":\"\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
    "\"events\": ["
        "{"
        "\"timestamp\":502339956266,"
        "\"missing_info\":4294967295,"
        "\"job_id\":2902878580,"
        "\"task_index\":1155,"
        "\"machine_id\":370132118,"
        "\"event_type\":1,"
        "\"user_name\":\"wCn+ghu8PJtxrZGYHh7myaXzhOnxiWYu5XZf/2M2slc=\","
        "\"scheduling_class\":2,"
        "\"priority\":11,"
        "\"norm_req_cores\":0.062500,"
        "\"norm_req_ram\":0.025600,"
        "\"norm_req_disk\":0.000070,"
        "\"constraints\":1},"
        "{"
        "\"timestamp\":505497139144,"
        "\"missing_info\":4294967295,"
        "\"job_id\":2902878580,"
        "\"task_index\":1155,"
        "\"machine_id\":370132118,"
        "\"event_type\":5,"
        "\"user_name\":\"wCn+ghu8PJtxrZGYHh7myaXzhOnxiWYu5XZf/2M2slc=\","
        "\"scheduling_class\":2,"
        "\"priority\":11,"
        "\"norm_req_cores\":0.062500,"
        "\"norm_req_ram\":0.025600,"
        "\"norm_req_disk\":0.000070,"
        "\"constraints\":1}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\":0.062500,"
    "\"avg_norm_alloc_ram\":0.025600,"
    "\"avg_norm_alloc_disk\":0.000070"
    "}}";


    Trace trace{json};
    BOOST_CHECK_EQUAL(trace.empty(),false);
}


BOOST_AUTO_TEST_CASE( first_element_access )
{
     string json =  "{"
    "\"job_id\":2902878580,"
    "\"user_name\":\"\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
    "\"events\": ["
        "{"
        "\"timestamp\":502339956266,"
        "\"missing_info\":4294967295,"
        "\"job_id\":2902878580,"
        "\"task_index\":1155,"
        "\"machine_id\":370132118,"
        "\"event_type\":1,"
        "\"user_name\":\"wCn+ghu8PJtxrZGYHh7myaXzhOnxiWYu5XZf/2M2slc=\","
        "\"scheduling_class\":2,"
        "\"priority\":11,"
        "\"norm_req_cores\":0.062500,"
        "\"norm_req_ram\":0.025600,"
        "\"norm_req_disk\":0.000070,"
        "\"constraints\":1},"
        "{"
        "\"timestamp\":505497139144,"
        "\"missing_info\":4294967295,"
        "\"job_id\":2902878580,"
        "\"task_index\":1155,"
        "\"machine_id\":370132118,"
        "\"event_type\":5,"
        "\"user_name\":\"wCn+ghu8PJtxrZGYHh7myaXzhOnxiWYu5XZf/2M2slc=\","
        "\"scheduling_class\":2,"
        "\"priority\":11,"
        "\"norm_req_cores\":0.062500,"
        "\"norm_req_ram\":0.025600,"
        "\"norm_req_disk\":0.000070,"
        "\"constraints\":1}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\":0.062500,"
    "\"avg_norm_alloc_ram\":0.025600,"
    "\"avg_norm_alloc_disk\":0.000070"
    "}}";


    Trace t1{json};

    string in_file{"../../schema/accepted.json"};
    JSONTraceBuffIOS reader{in_file,5};

    Trace t2 = reader.next();
    
    bool eval = (t2.get_jid() == t1.get_jid());

    BOOST_CHECK_EQUAL(eval,true);
}

BOOST_AUTO_TEST_CASE( last_element_access )
{
    string json = "{"
    "\"job_id\":15,"
    "\"user_name\":\"AA\","
    "\"uuid\":\"484f04e2-545a-40ff-84e3-a3a6b5eb791f\","
    "\"timestamp\":502062044694,"
    "\"events\": ["
        "{"
        "\"timestamp\":504524563872,"
        "\"missing_info\":4294967295,"
        "\"job_id\":698645235,"
        "\"task_index\":0,"
        "\"machine_id\":18446744073709551615,"
        "\"event_type\":0,"
        "\"user_name\":\"9bg757+k8xeWFApxOzOS5nnVAHYpBBQ+Pf1mEnsTZsQ=\","
        "\"scheduling_class\":2,"
        "\"priority\":9,"
        "\"norm_req_cores\":0.012500,"
        "\"norm_req_ram\":0.023860,"
        "\"norm_req_disk\":0.000193,"
        "\"constraints\":0"
        "},{"
        "\"timestamp\":504526396850,"
        "\"missing_info\":4294967295,"
        "\"job_id\":698645235,"
        "\"task_index\":0,"
        "\"machine_id\":6565363,"
        "\"event_type\":1,"
        "\"user_name\":\"9bg757+k8xeWFApxOzOS5nnVAHYpBBQ+Pf1mEnsTZsQ=\","
        "\"scheduling_class\":2,"
        "\"priority\":9,"
        "\"norm_req_cores\":0.012500,"
        "\"norm_req_ram\":0.023860,"
        "\"norm_req_disk\":0.000193,"
        "\"constraints\":0"
        "},{"
        "\"timestamp\":504704099910,"
        "\"missing_info\":4294967295,"
        "\"job_id\":698645235,"
        "\"task_index\":0,"
        "\"machine_id\":6565363,"
        "\"event_type\":3,"
        "\"user_name\":\"9bg757+k8xeWFApxOzOS5nnVAHYpBBQ+Pf1mEnsTZsQ=\","
        "\"scheduling_class\":2,"
        "\"priority\":9,"
        "\"norm_req_cores\":0.012500,"
        "\"norm_req_ram\":0.023860,"
        "\"norm_req_disk\":0.000193,"
        "\"constraints\":0"
        "}],"
        "\"avg_alloc_resources\": {"
        "\"avg_norm_alloc_cores\":0.012500,"
        "\"avg_norm_alloc_ram\":0.023860,"
        "\"avg_norm_alloc_disk\":0.000193 }"
    "}";


    Trace t1{json};
    string in_file{"../../schema/small.json"};
    JSONTraceBuffIOS reader{in_file,5};
    Trace t2;
    bool eval = false;

    do{
        t2 = reader.next();
        eval = (t2.get_jid() == t1.get_jid());
        if(eval)
            break;
    }while(!reader.empty());

    BOOST_CHECK_EQUAL(eval,true);
}

BOOST_AUTO_TEST_SUITE_END()
