#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <climits>
#include "tclap/CmdLine.h"
#include "jtracesort.h"
#include "Trace.h"
#include "JSONTraceBuffIOS.h"
#include "csv_buffered_reader.h"

using namespace std;
using namespace ETrace;
using namespace TCLAP;
using namespace boost;


/* Input file stream */
string _ifs{};

/* sorted parameter file */
string _sfs{};

/* Output file stream */
string _ofs{}; 



/**
*  Parses the Command Line Arguments (CLA).
*
*/
void parseCLA(int argc, char** argv) {
    try{
        // Usage: zcat workload.csv.gz | csv2swf -r 1000 | tee transformed_worload.swf
        CmdLine cmd("jtracesort (JSON Task Trace Sort) sorts an unsorted JSON. \n\
        Usage: \n\
            jtracesort -[OPTION]... [FILE]...",' ', "0.1");
 
        // List of value arguments
        ValueArg<string> in_file("i", "in_file", "Unsorted JSON", true, "", "string");
        cmd.add( in_file );

        ValueArg<string> sin_file("s","sorted_parameters","Sorted parameters file",true,"sorted_parameters.cvs","string");
        cmd.add( sin_file );

        ValueArg<string> out_file("o", "sorted_JSON", "Sorted JSON", true, "", "string");
        cmd.add( out_file );

        // Parse the argumnets
        cmd.parse( argc, argv );
        _ifs = in_file.getValue();
        _sfs = sin_file.getValue();
        _ofs = out_file.getValue();

            
        }catch(ArgException &e) {
            cerr << "Error: " << e.error() << " for argument " << e.argId() << endl;
            exit(EXIT_FAILURE);
        }
}


/**
*   The main program
*/
int main(int argc, char** argv)
{
    parseCLA( argc, argv );

    JTraceSort parser {_ifs, _sfs, _ofs}; 
    parser.process();
    
    return EXIT_SUCCESS;
}


/**
*   Loads the unsorted JSON and the sorted_parameters files.
*   Iterates over the sorted_parameter file and stores the
*   unsorted JSON in Radix order. 
*/
void JTraceSort::process( )
{
    std::map<string,Trace> traces;
    std::vector<string> straces;

    // Load the unsorted JSON file
    JSONTraceBuffIOS JSONreader{ifs, 1000};
    do {
        Trace trace = JSONreader.next();
        if( trace.empty() )
            break;
        else
            traces[trace.uuid] = trace;
    }while(!JSONreader.empty());

    // Loads the sorted parameter file
    long rows = 1000;
    csv_buffered_reader CSVreader{sfs, rows};
    char sep = ',';
    do{
        string line = CSVreader.next();
        if( !line.empty() ) {
            // UUID must be the first parameter in sorted parameter file
            std::istringstream stm(line);
            std::string uuid;
            getline(stm,uuid,sep);
            straces.push_back(traces[uuid].to_json());
            traces.erase(uuid);
        }    
    }while( !CSVreader.empty() );

    // Store traces in sorted order
    if( straces.size() > 0 ) {
        ofstream fos(ofs);
        if( !fos ) {
            cerr << "Error: could not open file for output" << endl;
            exit(EXIT_FAILURE);
        }

        fos << "{\"traces\":[";
        for(unsigned int i=0; i<straces.size(); i++) 
            fos << straces[i];
        fos << "]}";
        fos.close();
    }
}
