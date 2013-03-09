

void writeToPipe( MyRecord** records, int file, int len, int attr ) {
  log("Writing ordered records to pipe.");
  FILE *stream = fdopen( file, "w" );
  
  int i;
  if ( DEBUG ) fprintf( stream, "Sort attribute: %d\n", attr );
  for ( i = 0; i < len; i++ ) {
    MyRecord* rec = (MyRecord*) records[i];
    fprintf( stream, "%d %s %s %d\n", rec->ssn, rec->LastName, rec->FirstName, rec->income);
  }

   fclose( stream );
 }


Sorter* initSorter( Coordinator* coord, int numRecsPerSorter, int pos ) {

  Sorter* sorter = ( Sorter* ) malloc( sizeof(Sorter) + 1 );
  strcpy( sorter->filename, coord->filename );
  strcpy( sorter->sortProgram, coord->sortProgram );
  strcpy( sorter->sortType, coord->sortType );
  sorter->pos = pos;
  sorter->begin = numRecsPerSorter * pos;
  sorter->numRecs = numRecsPerSorter;
  sorter->sortAttr = coord->sortAttr;
  return sorter;

} // initSorter

void deploySorter( int* myPipe, Sorter* sorter ) {

  MyRecord* records[ sorter->numRecs ];
  sorter->records = records; // attach records
  FILE  *fp = NULL;
  char separator;
  if ( (fp = fopen( sorter->filename, "r" )) == NULL ) {
    println("Unknown file");
    exit(1);
  } else {
    int i = 0, numRecsSkipped = 0;
    while ( !feof(fp) && i < sorter->numRecs ) {
      MyRecord* recordPtr = ( MyRecord* ) malloc( sizeof( MyRecord)+1 );
      MyRecord record;
      fscanf( fp, "%d %s %s %d", &record.ssn, record.LastName, record.FirstName, &record.income);
      if ( numRecsSkipped >= sorter->begin ) { // hacky, should use fseek if possible
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

  sortRecords( records, sorter->numRecs, sorter->sortAttr );

  close( myPipe[READ] );
  writeToPipe( records, myPipe[WRITE], sorter->numRecs, sorter->sortAttr );
  close( myPipe[WRITE] );

  exit( SORTER_SUCCESS );
} // deploySorter



