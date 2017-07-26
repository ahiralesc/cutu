#include <iostream>
#include <string>
#include <vector>
#include "State.h"
#include <boost/algorithm/string.hpp>


using namespace GUCT_FSM;
using namespace std;
using namespace boost;

main(int argc, char** argv){

    string input_line;
    vector<string> values;

    while(cin) {
        getline( cin, input_line );

        if( !input_line.empty() ) {
            split( values, input_line , is_any_of(","));
            string task_id      = values[2] + values[3];
            long timestamp      = atol(values[0].c_str());
            string event_type   = values[5];

            cout << "task id : " << task_id << ", time : " << values[0] << ", event type : " << event_type << endl;
        }
    }

    
}
