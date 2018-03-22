#!/bin/bash

# Captura de variables

PROGRAM=$0
CRITERION=$1
LOG=$2
echo $PROGRAM
echo $CRITERION
echo $LOG

# Pasar variables a comando
awk -v criterio="$CRITERION"  -F "\"*,\"*" '
BEGIN{
    startTime=1304275800000000;
    chunck=10000000000000
    endTime=startTime+chunck;
    fileName="eventlong"
    type=".cvs"
    i=1
}
{
    releaseTime=startTime+$1;
    if(releaseTime<=endTime){
        file=(fileName i type)
        print file
        print $0 > file
    }else {
        i=i+1;
        endTime=endTime+chunk;
    }
}' $LOG
