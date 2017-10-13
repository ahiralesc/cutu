#ifndef _GUTTACCEPT_
#define _GUTTACCEPT_

#include <string>
#include <map>
#include <set>
#include "Trace.h"


class GUTTAccept {
    private :
    
    /* The number of rows */
    int num_rows;

    /* the input file name */
    std::string in_file;

    /* The output file name */
    std::string out_file; 

    /* The accepted traces output file name */
    std::string aos;

    /* Teh rejected traves output file name */
    std::string ros;


    /* The of Task execution traces */
    std::map<unsigned long long,ETrace::Trace> traces;
  
    /* The set of job runtimes */
    void translate (std::string&);


    public :

    /* class constructor */
    GUTTAccept(int nrows, std::string infile, std::string _aos, std::string _ros) : 
        num_rows{ nrows }, in_file { infile }, aos { _aos }, ros{ _ros }  {
    };

    /* Does the translation process */
    void process( );
};

#endif
