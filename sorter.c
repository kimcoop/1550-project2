Sorter* initSorter( Coordinator* coord, int numRecsPerSorter, int pos ) {

    Sorter* sorter = (Sorter*) malloc( sizeof(Sorter)+1 );
    strcpy( sorter->filename, coord->filename );
    sorter->id = pos;
    sorter->begin = numRecsPerSorter * pos;
    log("sorter begin is %d", sorter->begin);
    sorter->numBytes = numRecsPerSorter;
    sorter->sortAttr = coord->sortAttr;
    strcpy( sorter->sortProgram, coord->sortProgram );
    strcpy( sorter->sortAttrType, "test" ); // TODO -sortAttrType

} // initSorter

void deploySorter( Merger* merger, Sorter* sorter ) {
	// close( m_pipe[WRITE] );
 //  // convertToString( m_pipe[READ], stdout );
 //  close( m_pipe[READ] );

  if ( signal(SIGUSR1, my_handler ) == SIG_ERR ) {
    perror("Could not set a handler for SIGUSR1");
    exit(4);
  }

  println("Sorter sortRange: from [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
  println("Leaf: processing file in sorter [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
  readFile( sorter->filename );
  sortArray();

	int fd;
  if (( fd = open(sorter->filename, O_RDONLY, 0 )) < 0 ){
    perror("Bad filename.");
    exit(3);
  }

  writeArray( fd );
	char c[BUFF_SIZE];

 	// get( fd, sorter->begin, &c, sorter->numBytes);

	// char c[BUFSIZ];
	// int n;
	// pid_t pid;
	// int child_status;

	lseek( fd, 0L, SEEK_SET );
	writeFile( OUTFILE, "\n---WRITING---\n" );
	while((read(fd, c, sorter->numBytes)) != 0) {
	  writeFile( OUTFILE, c );
	}
	close( fd );



  println("Leaf: finished sorter [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);

} // deploySorter

// *get:  read n bytes from position pos 
int get(int fd, long pos, char *buf, int n)  {
 if ( lseek( fd, pos, 0 ) >= 0 )
   return read( fd, buf, n );
 else
   return -1;
}