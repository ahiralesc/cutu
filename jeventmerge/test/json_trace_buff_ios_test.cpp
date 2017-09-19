//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "JSONTraceBuffIOST"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "JSONTraceBuffIOS.h"
#include "Trace.h"

using namespace std;
using namespace ETrace;

BOOST_AUTO_TEST_SUITE(json_IOS_test)

BOOST_AUTO_TEST_CASE(empty_json)
{
    string path{"../logs/empty.json"};
    JSONTraceBuffIOS reader{path,5};

    Trace t = reader.next();
    bool result = ( t.tid.length() != 0 )? true : false;
    BOOST_CHECK_EQUAL(result,true);
}

BOOST_AUTO_TEST_CASE(not_empty)
{
    int count = 0;

    string path{"../logs/four.json"};
    JSONTraceBuffIOS reader{path,2};
    
    while(!reader.empty()){
        Trace t  = reader.next();
        if(t.tid.length() != 0 ){
            count++;
            cout << t.jid << endl;
        }else break;
    }
    BOOST_CHECK_EQUAL(count,4);
}

BOOST_AUTO_TEST_SUITE_END()
