

void printSorter( Sorter* sorter ) {
  printf("rangeBegin: %d - rangeEnd: %d \n", sorter->rangeBegin, sorter->rangeEnd);
} // printSorter

int numRecordsPerSorter( FILE* fd, int numWorkers ) {

  fseek (fd, 0, SEEK_END); // check number of records
  long lSize = ftell( fd );
  log("lSize %lu", lSize);
  rewind( fd );
  int numRecs = (int) lSize / sizeof( MyRecord );

  int recsPerSorter = numRecs / numWorkers;
  log("Records found in file %d \n", numRecs);
  log("Records per worker is %d  \n", recsPerSorter );

  return recsPerSorter;
} // determineNumSorters

void deploySorters( Coordinator* coord ) { //coord = ( filename, numWorkers, sortAttr, executableName );
  
  FILE* fd = fopen( coord->filename, "r" );
  int numRecsPerSorter;
  if ( fd==NULL ) {
    printf("Cannot open file\n");
    return;
  } else {
    numRecsPerSorter = numRecordsPerSorter( fd, coord->numWorkers );
    log("got numRecsPerSorter = %d", numRecsPerSorter);
  }


  printf("in deploySorters\n");
  int m_pipe[2];
  pid_t pid;
  if (pipe(m_pipe) < 0)
    err_error("Failed to create master pipe");
  if ((pid = fork()) < 0)
    err_error("Failed to fork master");
  else if (pid == 0) {
    printf("pid==0\n");
    close(m_pipe[READ]);
    sortOneFile( m_pipe[WRITE], INPUTFILE );
    close(m_pipe[WRITE]);
  } else  {
    printf("process child\n");
    close(m_pipe[WRITE]);
    convertToString(m_pipe[READ], stdout);
    close(m_pipe[READ]);
  }
  
  

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