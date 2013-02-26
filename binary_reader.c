#include <stdio.h>

#include "myRecordDef.h"

int main (int argc, char** argv) {
   FILE *fpb;
   MyRecord rec;
   long lSize;
   int numOfrecords, i;
   
   if (argc!=2) {
      	printf("Correct syntax is: %s BinaryFile\n", argv[0]);
      	return(1);
   	}
   fpb = fopen (argv[1],"rb");
   if (fpb==NULL) {
      	printf("Cannot open binary file\n");
      	return 1;
   	}

   // check number of records
   fseek (fpb , 0 , SEEK_END);
   lSize = ftell (fpb);
   rewind (fpb);
   numOfrecords = (int) lSize/sizeof(rec);

   printf("Records found in file %d \n", numOfrecords);
   sleep(3);
   
   for (i=0; i<numOfrecords ; i++) {
      	fread(&rec, sizeof(rec), 1, fpb);
      	printf("%d %s %s %d \n",rec.ssn, rec.LastName, rec.FirstName, rec.income);
   	}

   fclose (fpb);
}