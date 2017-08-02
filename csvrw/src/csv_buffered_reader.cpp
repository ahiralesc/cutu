#include <iostream>
#include "csv_buffered_reader.h"

using namespace std;


csv_buffered_reader::csv_buffered_reader(string ifs, long nrows) : 
		num_rows{nrows} {
    rows.clear();

    if( !ifs.empty() ) {
        if_str.open(ifs);
        if( !if_str ) {
            cerr << "Error: could not open file" << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << "Error: no file or incorrect path provided" << endl;
        exit(EXIT_FAILURE);
    }
}


void csv_buffered_reader::parse( ) {
    string row;

    for(int i=0; i<=num_rows; i++) {
        getline( if_str, row );
        if( row.size() > 0 )
            rows.push_back( row );
        else
            break;
    }
}


string csv_buffered_reader::next( ) {
    string row;

    if( rows.empty() ) {
        parse();
    } else {
        row = rows[0];
        rows.erase( rows.begin());
    }       
    
    return row;
} 


bool csv_buffered_reader::empty() {
    return rows.empty();
}


