void deploySorter( Sorter* sorter ) {


  if ( signal(SIGUSR1, my_handler ) == SIG_ERR ) {
    perror("Could not set a handler for SIGUSR1");
    exit(4);
  }

  println("Sorter sortRange: from %d to %d", sorter->begin, sorter->end);
  println("Leaf: processing file in sorter [%d..%d]", sorter->begin, sorter->end);
  readFile( sorter->filename );
  sortArray();

	int fd;
  if (( fd = open(sorter->filename, O_RDONLY, 0 )) < 0 ){
    perror("Bad filename.");
    exit(3);
  }
  
  writeArray( fd );
  println("Leaf: finished sorter [%d..%d]", sorter->begin, sorter->end);


// char c[BUFSIZ];
// int n;
// pid_t pid;
// int child_status;



// lseek(fd, 0L, SEEK_SET);
// writeFile( OUTFILE, "\n---PARENT WRITING---\n" );
// while((n = read(fd, c, nBytes)) != 0) {
//   writeFile( OUTFILE, c );
// }

} // deploySorter


void sortRange( FILE *fp, int begin, int end ) {
} // sortRange

