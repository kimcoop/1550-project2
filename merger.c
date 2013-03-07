
Merger* initMerger( int numWorkers ) {
	println("initMerger");
	Merger* merger = ( Merger* ) malloc( sizeof( Merger) +1 );
  int write_pipes[ numWorkers ];
  merger->write_pipes = write_pipes;
	return merger;	
}