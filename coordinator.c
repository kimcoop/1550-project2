static void my_handler( int signum ) {
  println("in my_handler");
  println("********");
  if ( signum == SIGUSR1 ) {
    println("Received SIGUSR1!\n");
  }
} // my_handler

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

  void
     read_from_pipe (int file)
     {
      println(" read_from_pipe ");
       FILE *stream;
       int c;
       stream = fdopen (file, "r");
       while ((c = fgetc (stream)) != EOF)
         putchar (c);
       fclose (stream);
     }
     
     /* Write some random text to the pipe. */
     
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

      int m_pipe[2];
      pid_t pid;
      if ( pipe(m_pipe) < 0 ) {
          println("Failed to create master pipe");
      }
      if ( (pid = fork()) < 0 ) {
          println("Failed to fork master");
      } else if ( pid == 0 ) {
          println("CHILD PROCESS");
          close(m_pipe[READ]);
          // Exec the sort program execl("/bin/sort", "sort",  (char*) NULL);
          // sort here
          write_to_pipe( m_pipe[WRITE] );
          initSorter( coord, numRecsPerSorter, i );
          close(m_pipe[WRITE]);
          int rc = 37;
          log("Child exiting (status = %d (0x%.2X))\n", rc, rc);
          exit(rc);
      } else {   
          println("PARENT PROCESS");
          close(m_pipe[WRITE]);
          read_from_pipe( m_pipe[READ] );
          close(m_pipe[READ]);
          merger->write_pipes[n_pipes++] = m_pipe[WRITE];

      }

    }

  } // numRecsPerSorter > 0
} // deploySorters

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {

  Coordinator *coord =  (Coordinator*) malloc( sizeof( Coordinator )+1 );
  strcpy( coord->filename, filename );
  coord->numWorkers = numWorkers;
  coord->sortAttr = sortAttr;
  strcpy( coord->sortProgram, sortProgram );
  // loadFile( filename );
  return coord;
} // initCoordinator
