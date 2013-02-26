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
#include <signal.h>
#include  "my_header.h"

/*************************************/

static void catch_signal( int );
Coordinator* initCoordinator( char*, int, int, char*);
void writeFile( char* );
void loadFile( char* );

/*************************************/


static void catch_signal( int signum ) {
  if ( signum == SIGUSR1 ) {
    printf("Received SIGUSR1!\n");
  }
} // catch_signal

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {
  Coordinator *coord =  (Coordinator*) malloc( sizeof( Coordinator )+1 );
  strcpy( coord->filename, filename );
  coord->numWorkers = numWorkers;
  coord->sortAttr = sortAttr;
  strcpy( coord->sortProgram, sortProgram );
  println(" initCoordinator ");
  loadFile( filename );
  return coord;
} // initCoordinator

void printRecord( MyRecord* record ) {
  println(" -- record -- ");
  println(" -- %d -- ", record->ssn);
  println(" -- %s -- ", record->FirstName);
  println(" -- %s -- ", record->LastName);
  println(" -- %d -- ", record->income);
} // printRecord

void writeFile( char* filename ) {
  FILE *file;
  file = fopen( filename, "a+" ); // append file (add text to a file or create a file if it does not exist.
  fprintf( file, "%s", "This is just an example :)" ); // write
  fclose( file );
} // writeFile

void loadFile( char* filename ) {
  FILE  *fp = NULL;
  char separator;
  if ( (fp = fopen( filename, "r" )) == NULL ) {
    println("Unknown file");
    exit(1);
  } else {
    while ( !feof(fp) ) {
      // struct MyRecord* record = (struct MyRecord*) malloc( sizeof( struct MyRecord)+1 );
      MyRecord record;
      fscanf( fp, "%d %s %s %d", &record.ssn, record.LastName, record.FirstName, &record.income);
      printf("%d %s %s %d \n", record.ssn, record.LastName, record.FirstName, record.income);
    } // end while 
    fclose(fp);
  }
} // loadFile

int main( int argc, char *argv[] ) {
  
  if ( signal(SIGUSR1, catch_signal) == SIG_ERR ) {
    fputs("An error occurred while setting a signal handler.\n", stderr);
    return EXIT_FAILURE;
  }
  println("Raising the interactive attention signal.");
  if ( raise(SIGUSR1) != 0 ) {
    fputs("Error raising the signal.\n", stderr);
    return EXIT_FAILURE;
  }

  int numFlags = argc-1, numWorkers, sortAttr;
  char flag[2]; char flagValue[MAX_ARG_SIZE];
  char filename[MAX_ARG_SIZE];
  char executableName[MAX_ARG_SIZE]; char type[MAX_ARG_SIZE]; char order[MAX_ARG_SIZE]; char outputFile[MAX_ARG_SIZE];

  if ( numFlags < 2 ) {
    log( "No commands provided." );
    numWorkers = 2; // some defaults for debugging/testing
    sortAttr = 2;
    strcpy( executableName, "testExe" );
    strcpy( filename, "records100.txt" );
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

  initCoordinator( filename, numWorkers, sortAttr, executableName );
  // writeFile( "testoutput.txt" );

  return 0;
 
}
