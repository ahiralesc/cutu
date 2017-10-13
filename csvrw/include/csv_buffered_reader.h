#ifndef _CSV_BUFFERED_READER_
#define _CSV_BUFFERED_READER_

//#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//using namespace std;


class csv_buffered_reader {

        /*
        * The number of rows in the CSV file
        */
        long num_rows;

        /*
        * A queue holding size rows from the file
        */
        std::vector<std::string> rows;


        /*
        * The input file stream  
        */
        std::ifstream ifstrm;

        /*
        * Buffer reader initialization/finaliztion
        */
        bool initialized{false};
        bool finalized{false};
        

    public:
        
        /*
        * Initializes the CSV parser
        *
        * Input:
        * - The file input stream
        * - The number of rows in the CSV file
        */
        csv_buffered_reader(std::string ifs, long nrows);

        /*
        * Destructore closes the file strem
        */
        ~csv_buffered_reader() { ifstrm.close(); };


        /*
        *  Ensures the queue is refilled if empty
        */ 
        void ensureBufferRefill( );


        /*
        * Gets the next available row
        */ 
        std::string next( );


        /*
        * Returns true if rows size is different from zero
        */
        bool empty();
};

#endif
