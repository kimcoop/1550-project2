

void write_to_pipe( MyRecord** records, int file, int len, int attr ) {
  println(" write_to_pipe");
  FILE *stream = fdopen( file, "w" );
  
  int i;
  for ( i = 0; i < len; i++ ) {
    MyRecord* rec = (MyRecord*) records[i];
    if ( attr == KEY_LASTNAME )
      fprintf( stream, "%s\n", rec->LastName );
    else if ( attr == KEY_FIRSTNAME )
      fprintf( stream, "%s\n", rec->FirstName );
    else if ( attr == KEY_SSN )
      fprintf( stream, "%d\n", rec->ssn );
    else 
      fprintf( stream, "%d\n", rec->income );
  } // for

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

  println("deploySorter begin at: %d", sorter->begin);

  MyRecord* records[ sorter->numRecs ];
  sorter->records = records; // attach records
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
  write_to_pipe( records, myPipe[WRITE], sorter->numRecs, sorter->sortAttr );
  close( myPipe[WRITE] );

  log( "Child exiting." );
  exit( SORTER_SUCCESS );

} // deploySorter

// // *get:  read n bytes from position pos 
// int get(int fd, long pos, char *buf, int n)  {
//  if ( lseek( fd, pos, 0 ) >= 0 )
//    return read( fd, buf, n );
//  else
//    return -1;
// }

/******************************************
 SORTING
******************************************/


int intcmp( const void *n1, const void *n2 ) {
  const int num1 = *(const int *) n1;
  const int num2 = *(const int *) n2;
  return (num1 < num2) ? -1 : (num1 > num2);
}

int structCmpFirst( const void *struct_a, const void *struct_b ) { 
  // struct_a and struct_b are pointers to an element of the array, aka pointers to pointers to structs. must cast it, then dereference it
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  println (" comparing %s and %s ", a->FirstName, b->FirstName ) ;
  return strcmp(  a->FirstName, b->FirstName ); 
} //  structCmpFirst

int structCmpLast( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  println (" comparing %s and %s ", a->LastName, b->LastName ) ;
  return strcmp(  a->LastName, b->LastName ); 
} //  structCmpLast

int structCmpSsn( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return intcmp(  &a->ssn, &b->ssn );
} // structCmp structCmpSsn

int structCmpIncome( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return intcmp(  &a->income, &b->income );
} // structCmp structCmpIncome

void sortRecords( MyRecord** records, int numRecs, int attr ) {
  println(" sortRecords ");

  if ( attr == KEY_LASTNAME )
    qsort( records, numRecs, sizeof( MyRecord* ), structCmpLast );
  else if ( attr == KEY_FIRSTNAME  )
    qsort( records, numRecs, sizeof( MyRecord* ), structCmpFirst );
  else if ( attr == KEY_SSN )
    qsort( records, numRecs, sizeof( MyRecord ), structCmpSsn );
  else
    qsort( records, numRecs, sizeof( MyRecord ), structCmpIncome );


} // sortRecords



