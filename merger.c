Merger* initMerger( int numWorkers ) {
  println("initMerger");
  Merger* merger = ( Merger* ) malloc( sizeof( Merger) +1 );
  int **pipes = generatePipes( numWorkers );
  merger->pipes = pipes;
  return merger;
}

int **generatePipes( int numpipes ) {
  int **pipes = ( int ** ) malloc( sizeof(int *) * (numpipes) );
  int i;
  for ( i = 0; i < numpipes; i++ ) {
    pipes[i] = (int *) malloc( sizeof(int) * 2 );
  }
  return pipes;
}

void mergeSorter( Coordinator* coord, Merger* merger, int index ) {
  println("in mergeSorter");
  close( merger->pipes[ index ][WRITE] );
  println(" readFromPipe ");

  char buffer[BUFFER_SIZE];
  read( merger->pipes[ index ][READ], buffer, BUFFER_SIZE );
  writeToFile( OUTFILE, buffer );

  close( merger->pipes[ index ][READ] );
}

void writeToFile( char* filename, char* str ) {
  FILE *file;
  file = fopen( filename, "a+" ); // append file (add text to a file or create a file if it does not exist)
  fprintf( file, "%s\n", str ); // write
  fclose( file );
}

char* readFromPipe( int file ) {
  println(" readFromPipe ");
  char* string;
  FILE *stream = fdopen( file, "r" );
  int c;
  while (( c = fgetc( stream )) != EOF )
    putchar(c);
  fclose( stream );
  return NULL;
}