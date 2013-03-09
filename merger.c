Merger* initMerger( int numWorkers ) {
  log("Initializing merger node.");
  Merger* merger = ( Merger* ) malloc( sizeof( Merger) +1 );
  int **pipes = generatePipes( numWorkers );
  merger->pipes = pipes;
  return merger;
}

int **generatePipes( int numpipes ) {
  log("Generating %d pipes for merger node.", numpipes);
  int **pipes = ( int ** ) malloc( sizeof(int *) * (numpipes) );
  int i;
  for ( i = 0; i < numpipes; i++ ) {
    pipes[i] = (int *) malloc( sizeof(int) * 2 );
    pipe(pipes[i]);
  }
  return pipes;
}

void mergeSorter( Coordinator* coord, Merger* merger, int index ) {
  log("Reading sorter's ordered results into output file.")
    
  close( merger->pipes[ index ][WRITE] );
  char buffer[BUFFER_SIZE];
  read( merger->pipes[ index ][READ], buffer, BUFFER_SIZE );
  writeToFile( OUTFILE, buffer );
  close( merger->pipes[ index ][READ] );
}

void writeToFile( char* filename, char* str ) {
  FILE *file;
  file = fopen( filename, "a+" ); // append file (add text to a file or create a file if it does not exist)
  fprintf( file, "%s", str ); // write
  if ( DEBUG ) fprintf( file, "\n" );
  fclose( file );
}

char* readFromPipe( int file ) {
  log("Reading from pipe.");
  char* string;
  FILE *stream = fdopen( file, "r" );
  int c;
  while (( c = fgetc( stream )) != EOF )
    putchar(c);
  fclose( stream );
  return NULL;
}