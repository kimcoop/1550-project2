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

#define SORTER_SUCCESS 37
#define SORTER_ERROR 38

#define KEY_SSN 0
#define KEY_LASTNAME 1
#define KEY_FIRSTNAME 2
#define KEY_INCOME 3

#include "models.h"

static void my_handler( int );

void deploySorters( Merger*, Coordinator* );
long numRecsPerSorter( Coordinator* );
Coordinator* initCoordinator( char*, int, int, char*);

void writeToFile( char*, char* );
MyRecord** loadRecords( int, char* );

/* SORTERS */
Sorter* initSorter( Coordinator*, int, int );
void deploySorter( int*, Sorter* );
void print_cstring_array( char **, int );
void print_int_array( int *, int );
int intcmp( const void *, const void * );
int structCompLast( const void *, const void *  );
int structCompFirst( const void *, const void *  );
int structCompSsn( const void *, const void *  );
int structCompIncome( const void *, const void *  );
void sortRecords( MyRecord**, int, int );

static int *a_data = 0;
static int  a_used = 0;
static int  a_size = 0;


/* MERGER */
int **generatePipes( int );
void mergeSorter( Coordinator*, Merger*, int );
Merger* initMerger( int );
char* readFromPipe( int );

/* ERRORS */
void readFile(char  *);
void freeMem(void);
void sortArray(void);
static void sortMergeFile(int, char *);
static void sortMergeFiles(int, int, char **);
static void sortFile(int, const char *);
static void convertToString(int, FILE *);
// static void sortOneFile(int fd, const char *file);
