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
	int fd;
	char *file;
	char filename[FILENAME_BUFF_SIZE];
	int begin, end;
	int sortAttr;
	char sortAttrType[BUFF_SIZE];
	char sortProgram[BUFF_SIZE];
} Sorter;