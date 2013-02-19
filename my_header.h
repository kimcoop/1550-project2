#include	<stdio.h>

#define MAX_ARG_SIZE 200
#define YES 1
#define NO 0
#define DEBUG 1 /* set to false for final submission */
#define println(...) printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)