     
 void
 write_to_pipe (int file)
 {
  println(" write_to_pipe");
   FILE *stream;
   stream = fdopen (file, "w");
   fprintf (stream, "hello, world!\n");
   fprintf (stream, "goodbye, world!\n");
   fclose (stream);
 }


Sorter* initSorter( Coordinator* coord, int numRecsPerSorter, int pos ) {

    Sorter* sorter = (Sorter*) malloc( sizeof(Sorter)+1 );
    strcpy( sorter->filename, coord->filename );
    sorter->pos = pos;
    sorter->begin = numRecsPerSorter * pos;
    sorter->numRecs = numRecsPerSorter;
    sorter->sortAttr = coord->sortAttr;
    strcpy( sorter->sortProgram, coord->sortProgram );
    strcpy( sorter->sortType, coord->sortType );
    return sorter;

} // initSorter

void sortRecords( MyRecord** records, int numRecs, int attr, char* attrType ) {
  println("sortRecords");
  int i;

  if ( strEqual(attrType, "string") ) {
    char* vals[ numRecs ];
    for ( i = 0; i < numRecs; i++ ) {
      MyRecord* rec = (MyRecord*) records[i];
      if ( attr == KEY_LASTNAME ) {
        vals[ i ] = rec->LastName; // lastName
        println( "records attr  = %s",  rec->LastName );
      } else {
        vals[ i ] = rec->FirstName; // firstName
        println( "records attr  = %s",  rec->FirstName );
      } // for
    }
    sortStrings( vals );
    
  } else {
    int vals[ numRecs ];
    for ( i = 0; i < numRecs; i++ ) {
      MyRecord* rec = (MyRecord*) records[i];
      if ( attr == KEY_SSN ) {
        vals[ i ] = rec->ssn; // ssn
        println( "records attr  = %d",  rec->ssn );
      } else {
        vals[ i ] = rec->income; // income
        println( "records attr  = %d",  rec->income );
      } // for
    }
    sortInts( vals );
  }
  
} // sortRecords

void deploySorter( int* m_pipe, Sorter* sorter ) {

  println("deploySorter begin at: %d", sorter->begin);

  MyRecord* records[ sorter->numRecs ];
  FILE  *fp = NULL;
  char separator;
  if ( (fp = fopen( sorter->filename, "r" )) == NULL ) {
    println("Unknown file");
    exit(1);
  } else {
    // fseek( fp, sorter->begin, SEEK_SET );
    int i = 0, numRecsSkipped = 0;
    while ( !feof(fp) && i < sorter->numRecs ) {
      MyRecord* recordPtr = ( MyRecord* ) malloc( sizeof( MyRecord)+1 );
      MyRecord record;
      fscanf( fp, "%d %s %s %d", &record.ssn, record.LastName, record.FirstName, &record.income);
      if ( numRecsSkipped >= sorter->begin ) { // TODO: this is SUPER HACKY
        printf("Parsing record %d: %d %s %s %d \n", i, record.ssn, record.LastName, record.FirstName, record.income);
        recordPtr->ssn = record.ssn;
        strcpy( recordPtr->LastName, record.LastName );
        strcpy( recordPtr->FirstName, record.FirstName );
        recordPtr->income = record.income;
        records[ i ] = recordPtr;
        i++;
      }
      numRecsSkipped++;
    } // end while

    fclose(fp);
  }
  sortRecords( records, sorter->numRecs, sorter->sortAttr, sorter->sortType );

  close(m_pipe[READ]);
  write_to_pipe( m_pipe[WRITE] );
  close(m_pipe[WRITE]);

  // if ( signal(SIGUSR1, my_handler) == SIG_ERR ) {
  //   fputs("An error occurred while setting a signal handler.\n", stderr);
  // }
  // if ( kill(getpid(), SIGUSR1) != 0 ) {
  //   fputs("Error raising the signal.\n", stderr);
  // }



 //  println("Sorter sortRange: from [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
 //  println("Leaf: processing file in sorter [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
 //  readFile( sorter->filename );
 //  sortArray();

	// int fd;
 //  if (( fd = open(sorter->filename, O_RDONLY, 0 )) < 0 ){
 //    perror("Bad filename.");
 //    exit(3);
 //  }

 //  writeArray( fd );
	// char c[BUFF_SIZE];

 // 	// get( fd, sorter->begin, &c, sorter->numBytes);

	// // char c[BUFSIZ];
	// // int n;
	// // pid_t pid;
	// // int child_status;

	// lseek( fd, 0L, SEEK_SET );
	// writeFile( OUTFILE, "\n---WRITING---\n" );
	// while((read(fd, c, sorter->numBytes)) != 0) {
	//   writeFile( OUTFILE, c );
	// }
	// close( fd );

  println("sorter pos %d", sorter->pos);

  int rc = 37;
  log("Child exiting (status = %d (0x%.2X))\n", rc, rc);
  exit(rc);

} // deploySorter

// *get:  read n bytes from position pos 
int get(int fd, long pos, char *buf, int n)  {
 if ( lseek( fd, pos, 0 ) >= 0 )
   return read( fd, buf, n );
 else
   return -1;
}

/******************************************
 SORTING
******************************************/

void sortStrings( char** strings ) {
  println(" sortStrings ");   
  size_t strings_len = sizeof(strings) / sizeof(char *);
  qsort(strings, strings_len, sizeof(char *), cstring_cmp);
}

void sortInts( int* vals ) {
  println(" sortInts");
}

