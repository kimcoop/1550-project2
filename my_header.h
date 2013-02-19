#include	<stdio.h>

#define MAX_COMMAND_SIZE 200
#define YES 1
#define NO 0
#define println(...) printf( __VA_ARGS__ ); printf("\n")
#define strEqual(a, b) !strcmp(a, b)
/*****
* This function prints out line numbers with each print statement. Very useful for debugging.
* To use, just overwrite the other (above) println function.
*
   #define println(...) printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n")
******/

struct item {
  struct item *next;
  char courseId[7], courseName[8], grade[3];
};

struct nodeIndex {
  struct node *node;
  int wasFound;
  int index;
};

struct courseFreq {
  char courseId[7];
  char courseName[8];
  int freq;
};

struct node {
  int numChildren; // number of keys currently stored
  int keys[4]; // the n keys themselves
  int isLeafNode; // true if node is a leaf node
  struct node* children[4]; // array with four pointers to node structs
  struct item* courseList[4]; // array with four pointers to first courseData in linkedlist for each child node
  struct node* nextLeaf; // children[3] will have a pointer to the next right node
};

void PrintItem( struct item* );

struct node* insert( struct node*, int, struct item* );
struct nodeIndex* search( struct node*, int );

void printCoursesInList( struct item* );
void getInfoInRange( struct node*,int, int );

float calculateGPA( struct node*, int );
void getGPAinRange( struct node*, int, int );
float gradepointForGrade( char* );
float calculateGPAofList( struct item* );

struct nodeIndex* nodeIndexForKey( struct node*, int );

void traverseLeaves( struct node* );
struct node* getMinNode( struct node* );
void printKeysInNode( struct node* );
int indexOfCourseId( char*, struct courseFreq* );
void getTopCourses( struct node*, int, int );
