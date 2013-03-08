

long numRecordsPerSorter( FILE* fp, int numWorkers ) {

  fseek (fp, 0, SEEK_END); // check number of records
  long lSize = ftell( fp );
  rewind( fp );// TODO: this doesn't include lines that are less than the BUFF_SIZE from struct
  int numRecs = lSize / sizeof( MyRecord );

  long recsPerSorter = (long) numRecs / numWorkers;

  // log("NumWorkers is %d", numWorkers);
  // log("Records found in file %d", numRecs);
  log("Records per worker is %lu ", recsPerSorter );
  log("---");

  return ( lSize > 0 && sizeof(MyRecord) >lSize) ? 1  : recsPerSorter;
} // determineNumSorters

void read_from_pipe (int file) {
   println(" read_from_pipe ");
   FILE *stream;
   int c;
   stream = fdopen (file, "r");
   while ((c = fgetc (stream)) != EOF)
     putchar (c);
   fclose (stream);
}

void deploySorters( Merger* merger, Coordinator* coord ) { //coord = ( filename, numWorkers, sortAttr, executableName );
  
  FILE* fp = fopen( coord->filename, "r" );
  long numRecsPerSorter;
  if ( fp==NULL ) {
    println("Cannot open file\n");
    return;
  } else {
    numRecsPerSorter = numRecordsPerSorter( fp, coord->numWorkers );
  }

  fclose( fp );
  if ( numRecsPerSorter > 0 ) {
    int i;
    for ( i = 0; i < coord->numWorkers; i++ ) {
      int child_status;
      int m_pipe[2];
      pid_t pid;
      if ( pipe(m_pipe) < 0 ) {
          println("Failed to create master pipe");
      }
      if ( (pid = fork()) < 0 ) {
          println("Failed to fork master");
      } else if ( pid == 0 ) {
          println("CHILD PROCESS");
          
          // Exec the sort program execl("/bin/sort", "sort",  (char*) NULL);
          // sort here
          Sorter* sorter = initSorter( coord, numRecsPerSorter, i );
          deploySorter( m_pipe, sorter );
          
      } else {   
          wait(&child_status);
          println("PARENT PROCESS");
          close(m_pipe[WRITE]);
          read_from_pipe( m_pipe[READ] );
          close(m_pipe[READ]);
          merger->write_pipes[n_pipes++] = m_pipe[WRITE];
          // when the child at ( read_pipe = sorter->pos, write_pipe = sorter->pos+1 )
          // is done sorting, it signals via a signal handler that it is done / contents sorted.
          // then the merger node will retrieve the sorted contents from the pipe.
      }

    }

  } // numRecsPerSorter > 0
} // deploySorters

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {

  Coordinator *coord =  (Coordinator*) malloc( sizeof( Coordinator )+1 );
  strcpy( coord->filename, filename );
  coord->numWorkers = numWorkers;
  coord->sortAttr = sortAttr;
  switch ( sortAttr ) {
    case KEY_SSN:
      strcpy( coord->sortType, "int" ); // SSN
      break;
    case KEY_LASTNAME:
      strcpy( coord->sortType, "string" ); // firstName
      break;
    case KEY_FIRSTNAME:
      strcpy( coord->sortType, "string" ); // lastName
      break;
    case KEY_INCOME:
      strcpy( coord->sortType, "int" ); // income
      break;
    default:
      strcpy( coord->sortType, "int" );
  }
  strcpy( coord->sortProgram, sortProgram );
  return coord;
} // initCoordinator
