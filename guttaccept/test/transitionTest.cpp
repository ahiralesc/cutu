#include <iostream>
#include <string>
#include "State.h"

using namespace GUCT_FSM;
using namespace std;

main(int argc, char** argv){
    
    int i, j;
    string states[] = { "submit", "schedule", "evict", "fail", "finish", "kill", "lost", "update_pending", "update_running"  };
    
    for ( i = 0; i < 9; i++ ){
        for ( j = 0; j < 9; j++) {
            cout << " Assert " << states[i] << " to " << states[j] << " :\t\t " << validateStateChange(st2str.at(states[i]), st2str.at(states[j])) << endl;
             
        }
        cout << "\n" << endl;
    }
    
}
