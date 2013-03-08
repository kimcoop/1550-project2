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
#include <signal.h>
#include  "my_header.h"
#include  "my_pipes.c"
#include  "coordinator.c"
#include  "merger.c"
#include  "sorter.c"


static void my_handler( int signum ) {
  if ( signum == SIGUSR1 ) {
    completed_sorters = completed_sorters+1;
    println("Received SIGUSR1.Incrementing completed_sorters to %d", completed_sorters);
  }
} // my_handler

void writeFile( char* filename, char* str ) {
  FILE *file;
  file = fopen( filename, "a+" ); // append file (add text to a file or create a file if it does not exist)
  fprintf( file, "%s", str ); // write
  fclose( file );
} // writeFile

// MyRecord** loadRecords( int numRecords, char* filename ) {
//   MyRecord* records[ numRecords ];
//   FILE  *fp = NULL;
//   char separator;
//   if ( (fp = fopen( filename, "r" )) == NULL ) {
//     println("Unknown file");
//     exit(1);
//   } else {
//     while ( !feof(fp) ) {
//       int i = 0;
//       // struct MyRecord* record = (struct MyRecord*) malloc( sizeof( struct MyRecord)+1 );
//       MyRecord record;
//       fscanf( fp, "%d %s %s %d", &record.ssn, record.LastName, record.FirstName, &record.income);
//       printf("Parsing record %d: %d %s %s %d \n", i, record.ssn, record.LastName, record.FirstName, record.income);
//       records[ i ] = &record;
//       i++;
//     } // end while

//     fclose(fp);
//   }
//   return records;
// } // loadFile

int main( int argc, char *argv[] ) {

  int numFlags = argc-1, numWorkers, sortAttr;
  char flag[2]; char flagValue[MAX_ARG_SIZE];
  char filename[MAX_ARG_SIZE];
  char executableName[MAX_ARG_SIZE]; char type[MAX_ARG_SIZE]; char order[MAX_ARG_SIZE]; char outputFile[MAX_ARG_SIZE];

  if ( numFlags < 2 ) {
    log( "No commands provided." );
    numWorkers = 2; // some defaults for debugging/testing
    sortAttr = SORT_ATTR;
    strcpy( executableName, "testExe" );
    strcpy( filename, INPUTFILE );

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
        else {
          println( "Flag %s not recognized.", flag  );
        }
      }
    }
  }


  Coordinator* coord = initCoordinator( filename, numWorkers, sortAttr, executableName );
  Merger* merger = initMerger( numWorkers );
  deploySorters( merger, coord );

  return 0;
 
}
