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

typedef struct{
	int ssn;
	char FirstName[BUFF_SIZE];
	char LastName[BUFF_SIZE];
	int  income;
} MyRecord;

struct MyRecord {
	int ssn;
	char FirstName[BUFF_SIZE];
	char LastName[BUFF_SIZE];
	int  income;
};

typedef struct {
	char filename[FILENAME_BUFF_SIZE];
	int numWorkers;
	int sortAttr;
	char sortProgram[BUFF_SIZE];
} Coordinator;

typedef struct {
	char filename[FILENAME_BUFF_SIZE];
	int rangeBegin, rangeEnd;
	int sortAttr;
	char sortAttrType[BUFF_SIZE];
	char sortProgram[BUFF_SIZE];
} Sorter;