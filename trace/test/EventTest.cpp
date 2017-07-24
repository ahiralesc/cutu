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

BOOST_AUTO_TEST_CASE( Transitions_0_1_2 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::evict));
    BOOST_CHECK_EQUAL(states.to_ulong(),7);
}


BOOST_AUTO_TEST_CASE( Transitions_0_1_3 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::fail));
    BOOST_CHECK_EQUAL(states.to_ulong(),7);
}


BOOST_AUTO_TEST_CASE( Transitions_0_1_4 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::finish));
    BOOST_CHECK_EQUAL(states.to_ulong(),7);
}


BOOST_AUTO_TEST_CASE( Transitions_0_1_5 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::kill));
    BOOST_CHECK_EQUAL(states.to_ulong(),7);
}


BOOST_AUTO_TEST_CASE( Transitions_0_1_6 )
{
    bitset<5> states;
    states.reset();
    
    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::lost));
    BOOST_CHECK_EQUAL(states.to_ulong(),7);
}


BOOST_AUTO_TEST_CASE( Transitions_0_7_1_2 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::update_pending));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::evict));
    BOOST_CHECK_EQUAL(states.to_ulong(),15);
}


BOOST_AUTO_TEST_CASE( Transitions_0_7_1_3 )
{
    bitset<5> states;
    states.reset();

    states.set(reindex(EventType::submit));
    states.set(reindex(EventType::update_pending));
    states.set(reindex(EventType::schedule));
    states.set(reindex(EventType::fail));
    BOOST_CHECK_EQUAL(states.to_ulong(),15);
}

BOOST_AUTO_TEST_SUITE_END()
