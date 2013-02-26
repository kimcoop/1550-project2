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
void loadFile( char* );

/*************************************/


static void catch_signal( int signum ) {
  if ( signum == SIGUSR1 ) {
    printf("Received SIGUSR1!\n");
  }
} // catch_signal

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
    while ( !feof(fp)  ) {
      MyRecord record;
      // MyRecord record = MyRecord* malloc( sizeof(MyRecord)+1 );

      fread(&record, sizeof(record), 1, fp);
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

  int numFlags = argc-1, numWorkers, attrNumber;
  char flag[2];
  char flagValue [MAX_ARG_SIZE];
  char numWorkersStr [MAX_ARG_SIZE];
  char executableName [MAX_ARG_SIZE];
  char attrNumberStr [MAX_ARG_SIZE];
  char type [MAX_ARG_SIZE];
  char order [MAX_ARG_SIZE];
  char outputFile [MAX_ARG_SIZE];

  loadFile( "records100.txt" );
  writeFile( "testoutput.txt" );

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
