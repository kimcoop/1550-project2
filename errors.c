
/* Error reporting */
static void err_vremark(char *fmt, va_list args)
{
    char buffer[256];
    int errnum = errno;
    int buflen = snprintf(buffer, sizeof(buffer), "%d: ", (int)getpid());
    buflen += vsnprintf(buffer + buflen, sizeof(buffer) - buflen, fmt, args);
    if (errnum != 0)
        buflen += snprintf(buffer + buflen, sizeof(buffer) - buflen,
                           ": errno = %d (%s)", errnum, strerror(errnum));
    fprintf(stderr, "%s\n", buffer);
}

static void err_error(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    err_vremark(fmt, args);
    va_end(args);
    exit(1);
}

static void err_remark(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    err_vremark(fmt, args);
    va_end(args);
}