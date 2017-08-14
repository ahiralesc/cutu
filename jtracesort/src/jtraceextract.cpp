#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
#include "tclap/CmdLine.h"
#include "jtraceextract.h"
#include "Trace.h"
#include "JSONTraceBuffIOS.h"

using namespace std;
using namespace ETrace;
using namespace TCLAP;
using namespace boost;


/* Input file stream */
string ifs{};


/* Output file stream */
string ofs{}; 



/**
*  Parses the Command Line Arguments (CLA). Valid switches for this application are the following:
*  - r, specifies the number of rows that will be loaded in memory.
*
*/
void parseCLA(int argc, char** argv) {
    try{
        // Usage: zcat workload.csv.gz | csv2swf -r 1000 | tee transformed_worload.swf
        CmdLine cmd("jtraceextract (JSON Task Trace Extract) extracts the trace UUID, ....\n\
        \n\
        Modes of execution: \n\
        \t jtraceextract -i <input JSON trace> -p <comma separated list of parameters> -o <csv output file>",' ', "0.1");
 
        // List of value arguments
        ValueArg<string> in_file("i", "in_file", "JSON trace file", false, "", "string");
        cmd.add( in_file );

        ValueArg<string> out_file("o", "out_file", "CSV file with extracted parameters", true, "extracted.json", "string");
        cmd.add( out_file );

        // Parse the argumnets
        cmd.parse( argc, argv );
        ifs = in_file.getValue();
        ofs = out_file.getValue();

            
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

    JTraceExtract parser {ifs, ofs}; 
    parser.process();
    
    return EXIT_SUCCESS;
}


/**
*   Coordiantes I/O with the translation process 
*/
void JTraceExtract::process( )
{
    std::vector<string> buffer;
    // Parse the file from a given file
        
    JSONTraceBuffIOS reader{in_file, 500};
    do {
        Trace trace = reader.next();
        // cout <<  trace.jid << " : " << trace.empty() << std::endl;
        if( trace.empty() )
            break;
        else {
            string l = trace.uuid  + ',' + to_string(trace.jid) + ',' + to_string(trace.startTime);
            buffer.push_back(l);
        }
    }while(!reader.empty());

    ofstream fos( out_file );
    if( !fos ) {
        cerr << "Error: could not open file for output" << endl;
        exit(EXIT_FAILURE);
    }
        
    // Send to file
    if(buffer.size() > 0 )
        for(string line : buffer)
            fos << line << endl;        
    fos.close();
}
