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
#include  "coordinator.c"
#include  "merger.c"
#include  "sorter.c"
#include  "sort.c"

static void sigHandler( int signum ) {
  if ( signum == SIGUSR1 ) {
    println("");
    println("*************");
    println("All sorters have completed execution.");
  }
}

void attrName( int attr ) {
  switch ( attr ) {
    case KEY_SSN:
      println( "SSN" ); break;
    case KEY_LASTNAME:
      println( "firstName" ); break;
    case KEY_FIRSTNAME:
      println( "lastName" ); break;
    case KEY_INCOME:
      println( "income" ); break;
    default:
      println( "(invalid)" ); break;
  }
}

int main( int argc, char *argv[] ) {

  
  int numFlags = argc-1, numWorkers, sortAttr;
  char flag[2]; char flagValue[MAX_ARG_SIZE];
  char filename[MAX_ARG_SIZE];
  char executableName[MAX_ARG_SIZE]; char type[MAX_ARG_SIZE]; char order[MAX_ARG_SIZE]; 
  char outputFile[MAX_ARG_SIZE];

  // set defaults from my_header.h
  strcpy( executableName, EXEC_NAME );
  strcpy( filename, INPUTFILE );
  strcpy( outputFile, OUTFILE );
  strcpy( order, ORDER );
  numWorkers = NUM_WORKERS;
  sortAttr = SORT_ATTR;

  if ( numFlags % 2 != 0 || argc == 1 ) { // --help for help

    if (argc ==2 ) strcpy( flagValue, argv[ 1 ] );
    
    if ( !strEqual( flagValue, "--defaults") && !strEqual( flagValue, "-defaults") && !strEqual( flagValue, "--d")) { // --defaults or -defaults or --d for default values
      println("Flags:");
      println( " -s \t output file name");
      println( " -i \t input file name");
      println( " -k \t number of workers");
      println( " -e \t executable name");
      println( " -t \t type");
      println( " -a \t attribute (0-3)");
      println( " -o \t order");
      return EXIT_FAILURE;
    }
  } else { // overwrite defaults
    
    int i;
    for ( i = 1; i < argc; i += 2 ) {
      strcpy( flag, argv[ i ] );
      strcpy( flagValue, argv[ i+1 ] );

      if ( strEqual(flag, "-e") ) 
        strcpy( executableName, flagValue  );
      else if ( strEqual(flag, "-t") ) 
        strcpy( type, flagValue ); 
      else if ( strEqual(flag, "-o") ) 
        strcpy( order, flagValue );
      else if ( strEqual(flag, "-s") ) 
        strcpy( outputFile, flagValue );
      else if ( strEqual(flag, "-a") ) 
        sortAttr = atoi( flagValue );  
      else if ( strEqual(flag, "-k") ) 
        numWorkers = atoi( flagValue );
      else if ( strEqual(flag, "-i") ) 
        strcpy( filename, flagValue ); 
    }
  }

  println( "Input file = %s", filename );
  println( "Output file = %s", outputFile );
  println( "Sort attribute = %d ", sortAttr);
  println( "Sort order = %s", order );
  println( "Number of workers = %d", numWorkers );
  println( "Executable name = %s", executableName );
  println("");

  Coordinator* coord = initCoordinator( filename, numWorkers, sortAttr, executableName );
  Merger* merger = initMerger( outputFile, numWorkers );

  if ( signal(SIGUSR1, sigHandler) == SIG_ERR ) {
    println("An error occurred while setting a signal handler.");
    return EXIT_FAILURE;
  }

  deploySorters( merger, coord );
  return 0;
 
}
