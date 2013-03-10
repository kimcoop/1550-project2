#include	<stdio.h>
#define MAX_ARG_SIZE 200
#define YES 1
#define NO 0

 /*** set to true to log output ****/
#define DEBUG 1

#define println(...) if ( DEBUG ) { printf("%d:\t", __LINE__); }  printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)
#define BUFF_SIZE 12
#define FILENAME_BUFF_SIZE 32

#define OUTFILE "testoutput.txt"
#define INPUTFILE "records1.txt"
#define SORT_ATTR KEY_SSN
#define EXEC_NAME "sort"
#define NUM_WORKERS 4
#define ORDER "ASC"

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 256

#define SORTER_SUCCESS 37
#define SORTER_ERROR 38

#define KEY_SSN 0
#define KEY_LASTNAME 1
#define KEY_FIRSTNAME 2
#define KEY_INCOME 3

#include "models.h"

/* COORDINATOR */
void deploySorters( Merger*, Coordinator* );
long numRecsPerSorter( Coordinator* );
Coordinator* initCoordinator( char*, int, int, char*);

/* SORTERS */
Sorter* initSorter( Coordinator*, int, int );
void deploySorter( int*, Sorter* );

/* SORTING */
int intcmp( const void *, const void * );
int structCompLast( const void *, const void *  );
int structCompFirst( const void *, const void *  );
int structCompSsn( const void *, const void *  );
int structCompIncome( const void *, const void *  );
void sortRecords( MyRecord**, int, int );

/* MERGER */
Merger* initMerger( char*, int );
void writeToFile( char*, char* );
int **generatePipes( int );
void mergeSorter( Coordinator*, Merger*, int );
char* readFromPipe( int );
