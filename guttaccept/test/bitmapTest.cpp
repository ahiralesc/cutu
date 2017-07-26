#include <iostream>
#include <string>
#include "State.h"

using namespace GUCT_FSM;
using namespace std;

main(int argc, char** argv){
bitset<5> states;

cout << "Traces with final value of 7" << endl;

cout << "Transitions: 0 -> 1 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::evict));
cout << "Assert expected value  7 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::fail));
cout << "Assert expected final value  7 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::finish));
cout << "Assert expected final value  7 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::kill));
cout << "Assert expected final value  7 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::lost));
cout << "Assert expected value  7 : " << states.to_ulong() << endl;

cout << "Traces with final value of 15" << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::evict));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::fail));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::finish));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::kill));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::lost));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 7 -> 1 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::evict));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 7 -> 1 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::fail));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 7 -> 1 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::finish));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 7 -> 1 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::kill));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 7 -> 1 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::lost));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

cout << "Traces with final value of 23" << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::evict));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::fail));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::finish));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::kill));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::lost));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 8 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_running));
states.set(reindex(State::evict));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 8 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_running));
states.set(reindex(State::fail));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 8 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_running));
states.set(reindex(State::finish));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 8 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_running));
states.set(reindex(State::kill));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 1 -> 8 -> 8 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_running));
states.set(reindex(State::lost));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

cout << "Traces with final value of 31" << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 8 -> 2" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::evict));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 8 -> 3" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::fail));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 8 -> 4" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::finish));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 8 -> 5" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::kill));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;

cout << "Transitions: 0 -> 7 -> 1 -> 8 -> 6" << endl;
states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::update_pending));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::lost));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;

states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_pending));
states.set(reindex(State::finish));
cout << "Assert expected value  15 : " << states.to_ulong() << endl;

states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::finish));
cout << "Assert expected value  23 : " << states.to_ulong() << endl;

states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::schedule));
states.set(reindex(State::update_running));
states.set(reindex(State::update_pending));
states.set(reindex(State::finish));
cout << "Assert expected value  31 : " << states.to_ulong() << endl;


states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::finish));
cout << "Assert expected value  5 : " << states.to_ulong() << endl;


states.reset();
states.set(reindex(State::submit));
states.set(reindex(State::finish));
states.set(reindex(State::update_pending));
cout << "Assert expected value  13 : " << states.to_ulong() << endl;
    
}
