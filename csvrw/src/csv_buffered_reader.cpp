#include <iostream>
#include "csv_buffered_reader.h"

using namespace std;


csv_buffered_reader::csv_buffered_reader(string ifs, long nrows) : 
		num_rows{nrows} {
    rows.clear();
    finalized = false;
    ifstrm.open(ifs);
    if(!ifstrm) {
        cerr << "Error: could not open file" << endl;
        exit(EXIT_FAILURE);
    }
    initialized = true;
}


void csv_buffered_reader::ensureBufferRefill(){
    string row;

    if(rows.empty() && !finalized){
        for(int i=0; i<num_rows; i++) {
            getline(ifstrm,row);
            if(ifstrm.eof()) {
                finalized = true;
                return;
            } else 
                rows.push_back(row);
        }
    }
}


string csv_buffered_reader::next( ) {
    string row;

    ensureBufferRefill();
    if( !empty() ) {
        row = rows[0];
        rows.erase(rows.begin());
    }
    return row;
} 


bool csv_buffered_reader::empty() {
    return finalized && rows.empty();
}
