
  if ( signal(SIGUSR1, my_handler) == SIG_ERR ) {
    fputs("An error occurred while setting a signal handler.\n", stderr);
    return EXIT_FAILURE;
  }
  println("Raising the interactive attention signal.");
  if ( raise(SIGUSR1) != 0 ) {
    fputs("Error raising the signal.\n", stderr);
    return EXIT_FAILURE;
  }

  /*

merger must contain an array size NUM_WORKERS
of ararys size NUM_RECS_PER_WORKER. then the
child processes/sorters can furnish their ordered
results array  to their index, pos, in the array.

merger waits for NUM_WORKERS "finished" signals,
collected in a static global var and incremented every
tiem the children signal they are done.

then merger iterates througha nd does the sorting
on each of its internal arrays, placing the result
in a file or STDOUT (if no filenmae provided).

merger then signals to coord that it is done.


  */


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