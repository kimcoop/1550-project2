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

static void sigHandler( int signum ) {
  if ( signum == SIGUSR1 ) {
    println("*************");
    println("All sorters have completed execution.");
  }
}

int main( int argc, char *argv[] ) {

  int numFlags = argc-1, numWorkers, sortAttr;
  char flag[2]; char flagValue[MAX_ARG_SIZE];
  char filename[MAX_ARG_SIZE];
  char executableName[MAX_ARG_SIZE]; char type[MAX_ARG_SIZE]; char order[MAX_ARG_SIZE]; 
  char outputFile[MAX_ARG_SIZE];

  strcpy( executableName, EXEC_NAME );
  strcpy( filename, INPUTFILE );
  strcpy( outputFile, OUTFILE );

  numWorkers = NUM_WORKERS;
  sortAttr = SORT_ATTR;

  if ( numFlags % 2 != 0 ) {
    println( "Malformed flags. Please re-enter." );
  } else { // overwrite defaults
    
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

  if ( filename == INPUTFILE )
    println( "No input file given. Using %s", filename );
  if ( outputFile == OUTFILE ) // not user-provided; using default
    println( "No output file given. Using %s", outputFile );

  Coordinator* coord = initCoordinator( filename, numWorkers, sortAttr, executableName );
  Merger* merger = initMerger( numWorkers ); // generates pipes

  if ( signal(SIGUSR1, sigHandler) == SIG_ERR ) {
    println("An error occurred while setting a signal handler.");
    return EXIT_FAILURE;
  }

  deploySorters( merger, coord );
  return 0;
 
}
