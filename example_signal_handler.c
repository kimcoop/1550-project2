
  if ( signal(SIGUSR1, my_handler) == SIG_ERR ) {
    fputs("An error occurred while setting a signal handler.\n", stderr);
    return EXIT_FAILURE;
  }
  println("Raising the interactive attention signal.");
  if ( raise(SIGUSR1) != 0 ) {
    fputs("Error raising the signal.\n", stderr);
    return EXIT_FAILURE;
  }