/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 2
Due March 07, 2013

*/

#include <fcntl.h>   
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "my_header.h"
#include  "my_pipes.c"
#include  "coordinator.c"
#include  "merger.c"
#include  "sorter.c"
#include  "sort.c"

int main( int argc, char *argv[] ) {

  int numFlags = argc-1, numWorkers, sortAttr;
  char flag[2]; char flagValue[MAX_ARG_SIZE];
  char filename[MAX_ARG_SIZE];
  char executableName[MAX_ARG_SIZE]; char type[MAX_ARG_SIZE]; char order[MAX_ARG_SIZE]; char outputFile[MAX_ARG_SIZE];

  if ( numFlags < 2 ) {
    log( "No commands provided. Using default values in my_header.h" );
    strcpy( executableName, EXEC_NAME );
    strcpy( filename, INPUTFILE );
    strcpy( outputFile, OUTFILE );

    numWorkers = NUM_WORKERS;
    sortAttr = SORT_ATTR;

  } else { // flags are  present
    if ( numFlags % 2 != 0 ) {
      println( "Malformed flags. Please re-enter." );
    } else {
      
      int i;
      for ( i = 1; i < argc; i+=2 ) {
        strcpy( flag, argv[i] );
        strcpy( flagValue, argv[i+1] );

        if ( strEqual(flag, "-k") ) {  numWorkers = atoi( flagValue );  }
        else if ( strEqual(flag, "-e") ) {  strcpy( executableName, flagValue  ); } 
        else if ( strEqual(flag, "-a") ) {  sortAttr = atoi( flagValue );  } 
        else if ( strEqual(flag, "-t") ) {  strcpy( type, flagValue ); } 
        else if ( strEqual(flag, "-o") ) {  strcpy( order, flagValue );  } 
        else if ( strEqual(flag, "-s") ) {  strcpy( outputFile, flagValue ); } 
        else
          println( "Flag %s not recognized.", flag  );
      }
    }
  }


  Coordinator* coord = initCoordinator( filename, numWorkers, sortAttr, executableName );
  Merger* merger = initMerger( numWorkers ); // generates pipes
  deploySorters( merger, coord );

  return 0;
 
}
