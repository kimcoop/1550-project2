/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 2
Due March 07, 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "my_header.h"

int main( int argc, char *argv[] ) {
  
  int numFlags = argc-1, numWorkers, attrNumber;
  char flag[2];
  char flagValue [MAX_ARG_SIZE];
  char numWorkersStr [MAX_ARG_SIZE];
  char executableName [MAX_ARG_SIZE];
  char attrNumberStr [MAX_ARG_SIZE];
  char type [MAX_ARG_SIZE];
  char order [MAX_ARG_SIZE];
  char outputFile [MAX_ARG_SIZE];

  if ( numFlags < 2 ) {
    log( "No commands provided." );
  } else { // commands present
    if ( numFlags % 2 != 0 ) {
      println( "Malformed flags. Please re-enter." );
    } else {
      log( "Argc: %d", argc );

      // parse commands
      int i;
      for ( i = 1; i < argc; i+=2 ) {
        strcpy( flag, argv[i] );
        if ( strEqual(flag, "-k") ) {
          strcpy( numWorkersStr, argv[i+1] );
          numWorkers = atoi( numWorkersStr );
          log(  "numWorkers as int is %i", numWorkers );
        } else if ( strEqual(flag, "-e") ) {
          strcpy( executableName, argv[i+1] );
        } else if ( strEqual(flag, "-a") ) {
          strcpy( attrNumberStr, argv[i+1] );
          attrNumber = atoi( attrNumberStr );
          log(  "attrNumber as int is %i", attrNumber );
        } else if ( strEqual(flag, "-t") ) {
          strcpy( type, argv[i+1] );
        } else if ( strEqual(flag, "-o") ) {
          strcpy( order, argv[i+1] );
        } else if ( strEqual(flag, "-s") ) {
          strcpy( outputFile, argv[i+1] );
        } else {
          println( "Flag %s not recognized.", flag  );
        }
      }
    }
  }

  return 0;
 
}
