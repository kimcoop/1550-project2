int **generatePipes( int numpipes ) { //returns an empty 2-dimensional array
  int **pipes = ( int ** ) malloc( sizeof(int *) * (numpipes) );
  int i;
  for ( i = 0; i < numpipes; i++ ) {
    pipes[i] = (int *) malloc(sizeof(int) * 2);
  }
  return pipes;
}

void mergeSorter( Merger* merger, int pos ) {
  println("in mergeSorter");
  // int i;
  // for ( i=0; i < sorter->numRecs; i++ ) {
  //   MyRecord* rec = ( MyRecord* ) sorter->records[i];
  //   println(" sorter->records[%d]->firstName %s", i, rec->FirstName);
  // }
  close( merger->pipes[ pos ][WRITE] );
  read_from_pipe( merger->pipes[ pos ][READ] );
  close( merger->pipes[ pos ][READ] );
}

Merger* initMerger( int numWorkers ) {
	println("initMerger");
	Merger* merger = ( Merger* ) malloc( sizeof( Merger) +1 );
  int **pipes = generatePipes( numWorkers );
  merger->pipes = pipes;
	return merger;	
}