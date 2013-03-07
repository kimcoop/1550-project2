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
  log("lSize %lu", lSize);
  rewind( fp );
  int numRecs = (int) lSize / sizeof( MyRecord );

  long recsPerSorter = (long) numRecs / numWorkers;

  log("NumWorkers is %d", numWorkers);
  log("Records found in file %d", numRecs);
  log("Records per worker is %lu ", recsPerSorter );
  log("---");

  return recsPerSorter;
} // determineNumSorters

void deploySorters( Coordinator* coord ) { //coord = ( filename, numWorkers, sortAttr, executableName );
  
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
      Sorter* sorter = (Sorter*) malloc( sizeof(Sorter)+1 );
      strcpy( sorter->filename, coord->filename );
      // sorter->fd = fd; // TODO - have this pass as fd? or filename?
      sorter->begin = numRecsPerSorter * i;
      sorter->end = numRecsPerSorter * ( i+1) -1 ;
      sorter->sortAttr = coord->sortAttr;
      strcpy( sorter->sortProgram, coord->sortProgram );
      strcpy( sorter->sortAttrType, "test" ); // TODO -sortAttrType
      deploySorter( sorter );
    }
  }

  // println("in deploySorters\n");
  // int m_pipe[2];
  // pid_t pid;
  // if (pipe(m_pipe) < 0)
  //   println("Failed to create master pipe");
  // if ((pid = fork()) < 0) {
  //   println("Failed to fork master");
  // } else if (pid == 0) {
  //   println(" IN PARENT \n");
  //   close(m_pipe[READ]);
  //   sortOneFile( m_pipe[WRITE], INPUTFILE );
  //   close(m_pipe[WRITE]);
  // } else  {
  //   println(" IN CHILD \n");
  //   close(m_pipe[WRITE]);
  //   convertToString(m_pipe[READ], stdout);
  //   close(m_pipe[READ]);
  // }
  
  

} // deploySorters

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {

  Coordinator *coord =  (Coordinator*) malloc( sizeof( Coordinator )+1 );
  strcpy( coord->filename, filename );
  coord->numWorkers = numWorkers;
  coord->sortAttr = sortAttr;
  strcpy( coord->sortProgram, sortProgram );
  println(" initCoordinator ");
  // loadFile( filename );
  return coord;
} // initCoordinator