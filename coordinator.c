

long numRecsPerSorter( Coordinator* coord ) {
  FILE* fp = fopen( coord->filename, "r" );
  if ( fp==NULL ) {
    println("Cannot open file\n");
    return;
  } else {
    fseek (fp, 0, SEEK_END); // check number of records
    long lSize = ftell( fp );
    rewind( fp );// TODO: this doesn't include lines that are less than the BUFF_SIZE from struct
    fclose( fp );
    int numRecs = lSize / sizeof( MyRecord );
    long recsPerSorter = (long) numRecs / coord->numWorkers;
    log("Records per worker is %lu ", recsPerSorter );
    log("---");
    return ( lSize > 0 && sizeof(MyRecord) >lSize) ? 1  : recsPerSorter;
  }
} // determineNumSorters

void deploySorters( Merger* merger, Coordinator* coord ) {
  
  long recsPerSorter = numRecsPerSorter( coord );

  if ( recsPerSorter > 0 ) {
    int i;
    pid_t pid;

    for ( i = 0; i < coord->numWorkers; i++ ) {
      int childStatus;
      
      if ( pipe(merger->pipes[i]) < 0 )
        println("Failed to create master pipe");
      if ( (pid = fork()) < 0 ) {
        println("Failed to fork pipe");
      } else if ( pid == 0 ) { // child
        println("CHILD PROCESS");
        
        // Exec the sort program execl("/bin/sort", "sort",  (char*) NULL);
        // execlp("sort", "sort", (char *)NULL);
        
        Sorter* sorter = initSorter( coord, recsPerSorter, i );
        deploySorter( merger->pipes[i], sorter );
          
      } else { // parent
        wait( &childStatus );
        println("PARENT PROCESS. child status %d", childStatus);
        if ( WIFEXITED(childStatus) && WEXITSTATUS(childStatus) == SORTER_SUCCESS ) {
          println("%d", childStatus);
          mergeSorter( coord, merger, i ); // get the data from our pipes array
        }
      }
    }
  } // recsPerSorter > 0
} // deploySorters

Coordinator* initCoordinator( char* filename, int numWorkers, int sortAttr, char* sortProgram  ) {

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
