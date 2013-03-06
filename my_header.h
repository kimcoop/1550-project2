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
#define INPUTFILE "records_short.txt"

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 256

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

/* ERRORS */
static int *a_data = 0;
static int  a_used = 0;
static int  a_size = 0;
void readFile(char const *);
void freeMem(void);
void sortArray(void);
int  intcmp(void const *n1, void const *n2);
static void sortMergeFile(int, char *);
static void sortMergeFiles(int, int, char **);
static void sortFile(int, const char *);
static void convertToString(int, FILE *);