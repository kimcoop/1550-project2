     
void write_to_pipe( MyRecord** records, int file, int len, int attr ) {  // TODO - assumes string (sorting attr)
  println(" write_to_pipe");
  FILE *stream;
  stream = fdopen (file, "w");
  fprintf( stream, "hello, world!xx\n" );
  fprintf( stream, "goodbye, world!xx\n" );
  
  int i;
  for ( i = 0; i < len; i++ ) {
    MyRecord* rec = (MyRecord*) records[i];
    if ( attr == KEY_LASTNAME ) { // TODO - this isn't DRY :(
      fprintf( stream, "%s\n", rec->LastName );
    } else {
      fprintf( stream, "%s\n", rec->FirstName ); // firstName
    } // for
  }

   fclose( stream );
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
      } else {
        vals[ i ] = rec->FirstName; // firstName
      } // for
    }
    sortStrings( vals, numRecs );
    println("???");
    print_cstring_array( vals, numRecs );
    
  } else {
    int vals[ numRecs ];
    for ( i = 0; i < numRecs; i++ ) {
      MyRecord* rec = (MyRecord*) records[i];
      if ( attr == KEY_SSN ) {
        vals[ i ] = rec->ssn; // ssn
      } else {
        vals[ i ] = rec->income; // income
      } // for
    }
    for ( i = 0; i < numRecs; i++ ) {
      println( "vals[%d] = %d ", i, vals[i]);
    }
    sortInts( vals, numRecs );
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
  write_to_pipe( records, m_pipe[WRITE], sorter->numRecs, sorter->sortAttr );
  close(m_pipe[WRITE]);


  println("sorter pos %d", sorter->pos);

  int rc = 37;
  log("Child exiting (status = %d (0x%.2X))\n", rc, rc);
  exit(rc);

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

void print_cstring_array( char **array, int len ) { 
    int i;
    for ( i = 0; i < len; i++ ) 
      printf("%s | ", array[i]);
    putchar('\n');
}

void print_int_array( int *array, int len ) { 
    int i;
    for ( i = 0; i < len; i++ ) 
      printf("%d | ", array[i]);
    putchar('\n');
} 

int intcmp( const void *n1, const void *n2 ) {
    const int num1 = *(const int *) n1;
    const int num2 = *(const int *) n2;
    return (num1 < num2) ? -1 : (num1 > num2);
}


int cstring_cmp( const void *a, const void *b ) { 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}


void sortStrings( char** strings, int len ) {
  println(" sortStrings ");
  qsort(strings, len, sizeof(char *), cstring_cmp);
  print_cstring_array( strings, len );
}

void sortInts( int* vals, int len ) {
  println(" sortInts");
  qsort(vals, len, sizeof(int), intcmp);
  print_int_array( vals, len );
}
