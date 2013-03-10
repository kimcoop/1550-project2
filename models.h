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
	char sortType[BUFF_SIZE];
	char sortProgram[BUFF_SIZE];
} Coordinator;

typedef struct {
	char filename[FILENAME_BUFF_SIZE];
	int begin, pos, numRecs; // pos tracks pos in merger[pipes]
	int sortAttr;
	char sortType[BUFF_SIZE];
	char sortProgram[BUFF_SIZE];
	MyRecord** records;
} Sorter;

typedef struct{
	char outputFile[FILENAME_BUFF_SIZE];
	int numFinished;
	int **pipes;
} Merger;
