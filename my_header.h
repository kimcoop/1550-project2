#include	<stdio.h>
#define MAX_ARG_SIZE 200
#define YES 1
#define NO 0
#define DEBUG 1 /* set to false for final submission */
#define println(...) printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)
#define BUFF_SIZE 12
#define FILENAME_BUFF_SIZE 32

#define OUTFILE "testoutput.txt"
#define INPUTFILE "records1.txt"
#define SORT_ATTR KEY_LASTNAME

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 256

#define KEY_SSN 0
#define KEY_LASTNAME 1
#define KEY_FIRSTNAME 2
#define KEY_INCOME 3

#include "models.h"

static void my_handler( int );



void deploySorters( Merger*, Coordinator* );
long numRecordsPerSorter( FILE*, int );
Coordinator* initCoordinator( char*, int, int, char*);

void writeFile( char*, char* );
MyRecord** loadRecords( int, char* );

/* SORTERS */
Sorter* initSorter( Coordinator*, int, int );
// void deploySorter( Merger*, Sorter* );
void deploySorter( int*, Sorter* );
void print_cstring_array( char **, int );
void print_int_array( int *, int );
int intcmp( const void *, const void * );
int cstring_cmp( const void *a, const void * );

static int *a_data = 0;
static int  a_used = 0;
static int  a_size = 0;
static int  n_pipes = 0;
static int 	completed_sorters = 0;

void sortRecords( MyRecord**, int, int, char* ); 
void sortStrings( char**, int );
void sortInts( int*, int );

/* MERGER */
Merger* initMerger( int );

/* ERRORS */
void readFile(char  *);
void freeMem(void);
void sortArray(void);
static void sortMergeFile(int, char *);
static void sortMergeFiles(int, int, char **);
static void sortFile(int, const char *);
static void convertToString(int, FILE *);
// static void sortOneFile(int fd, const char *file);
