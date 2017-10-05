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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "bitmap test"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "Event.h"

using namespace Event;
using namespace std;

BOOST_AUTO_TEST_SUITE(bitmap_test)


BOOST_AUTO_TEST_CASE( valid_transitions )
{
    bitset<5> states;
    states.reset();
        
    if(validateEventChange(EventType::submit, EventType::schedule)) 
        states.set(0);
    if(validateEventChange(EventType::schedule, EventType::finish)) 
        states.set(1);
    if(validateEventChange(EventType::evict, EventType::submit)) 
        states.set(2);
    if(validateEventChange(EventType::submit, EventType::update_pending)) 
        states.set(3);
    if(validateEventChange(EventType::update_running, EventType::lost)) 
        states.set(4);
    
    BOOST_CHECK_EQUAL(states.all(),true);
}


BOOST_AUTO_TEST_CASE( invalid_transition )
{
    bitset<2> states;
    states.reset();

    if(!validateEventChange(EventType::submit, EventType::evict))
        states.set(0);
    if(!validateEventChange(EventType::lost, EventType::schedule))
        states.set(1);
    
    BOOST_CHECK_EQUAL(states.all(), true);
}

BOOST_AUTO_TEST_SUITE_END()
