     
     void
     write_to_pipe (int file)
     {
      println(" write_to_pipe");
       FILE *stream;
       stream = fdopen (file, "w");
       fprintf (stream, "hello, world!\n");
       fprintf (stream, "goodbye, world!\n");
       fclose (stream);
     }

Sorter* initSorter( Coordinator* coord, int numRecsPerSorter, int pos ) {

    Sorter* sorter = (Sorter*) malloc( sizeof(Sorter)+1 );
    strcpy( sorter->filename, coord->filename );
    sorter->pos = pos;
    sorter->begin = numRecsPerSorter * pos;
    sorter->numBytes = numRecsPerSorter;
    sorter->sortAttr = coord->sortAttr;
    strcpy( sorter->sortProgram, coord->sortProgram );
    strcpy( sorter->sortAttrType, "test" ); // TODO -sortAttrType
    return sorter;

} // initSorter

void deploySorter( int* m_pipe, Sorter* sorter ) {

  println("deploySorter: m_pipe**");
  close(m_pipe[READ]);
  write_to_pipe( m_pipe[WRITE] );
  close(m_pipe[WRITE]);

  // if ( signal(SIGUSR1, my_handler) == SIG_ERR ) {
  //   fputs("An error occurred while setting a signal handler.\n", stderr);
  // }
  // if ( kill(getpid(), SIGUSR1) != 0 ) {
  //   fputs("Error raising the signal.\n", stderr);
  // }



 //  println("Sorter sortRange: from [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
 //  println("Leaf: processing file in sorter [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
 //  readFile( sorter->filename );
 //  sortArray();

	// int fd;
 //  if (( fd = open(sorter->filename, O_RDONLY, 0 )) < 0 ){
 //    perror("Bad filename.");
 //    exit(3);
 //  }

 //  writeArray( fd );
	// char c[BUFF_SIZE];

 // 	// get( fd, sorter->begin, &c, sorter->numBytes);

	// // char c[BUFSIZ];
	// // int n;
	// // pid_t pid;
	// // int child_status;

	// lseek( fd, 0L, SEEK_SET );
	// writeFile( OUTFILE, "\n---WRITING---\n" );
	// while((read(fd, c, sorter->numBytes)) != 0) {
	//   writeFile( OUTFILE, c );
	// }
	// close( fd );

  println("Leaf: finished sorter [%d..%d]", sorter->begin, sorter->begin+sorter->numBytes);
  println("sorter pos %d", sorter->pos);

  int rc = 37;
  log("Child exiting (status = %d (0x%.2X))\n", rc, rc);
  exit(rc);

} // deploySorter

// *get:  read n bytes from position pos 
int get(int fd, long pos, char *buf, int n)  {
 if ( lseek( fd, pos, 0 ) >= 0 )
   return read( fd, buf, n );
 else
   return -1;
}