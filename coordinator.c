

long numRecsPerSorter( Coordinator* coord ) {
  FILE* fp = fopen( coord->filename, "r" );
  if ( fp==NULL ) {
    println("Cannot open file.");
    return;
  } else {
    fseek (fp, 0, SEEK_END); // check number of records
    long lSize = ftell( fp );
    rewind( fp );// TODO: this doesn't include lines that are less than the BUFFER_SIZE from struct
    fclose( fp );
    int numRecs = lSize / sizeof( MyRecord );
    log("Number of records found in file = %d ", numRecs );
    if ( numRecs < coord->numWorkers ) {
      println("Too many workers (fewer records in file [%d] than workers [%d])", numRecs, coord->numWorkers);
      println("Decreasing number of workers to %d", numRecs);
      coord->numWorkers = numRecs;
    }
    long recsPerSorter = (long) numRecs / coord->numWorkers;
    log("Calculated number of records per worker = %lu", recsPerSorter );
    return ( lSize > 0 && sizeof(MyRecord) > lSize) ? 1  : recsPerSorter;
  }
} // determineNumSorters

void deploySorters( Merger* merger, Coordinator* coord ) {
  println("Deploying %d sorters.", coord->numWorkers);

  long recsPerSorter = numRecsPerSorter( coord );

  if ( recsPerSorter > 0 ) {
    int i;
    pid_t pid;

    for ( i = 0; i < coord->numWorkers; i++ ) {
      int childStatus;
      
      if ( (pid = fork()) < 0 ) {
        println("Failed to fork pipe.");
      } else if ( pid == 0 ) { // child
        log("In child.");
        
        Sorter* sorter = initSorter( coord, recsPerSorter, i );
        deploySorter( merger->pipes[i], sorter );
          
      } else { // parent
        wait( &childStatus );
        log("In parent.");
        if ( WIFEXITED(childStatus) && WEXITSTATUS(childStatus) == SORTER_SUCCESS ) {
          log("Waiting on child. Child's exit status = OK");
          mergeSorter( coord, merger, i ); // get the data from our pipes array
        }
      }
    }
  } // recsPerSorter > 0
} // deploySorters

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {

  log("Initializing coordinator node.");
  Coordinator *coord =  (Coordinator*) malloc( sizeof( Coordinator )+1 );
  strcpy( coord->filename, filename );
  strcpy( coord->sortProgram, sortProgram );
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
  return coord;
} // initCoordinator
