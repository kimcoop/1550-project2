#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>



static void convertToString(int fd, FILE *fp)
{
    int value;
    while (read(fd, &value, sizeof(int)) == sizeof(int)) {
      // fprintf(fp, "%d\n", value);
    }
}

static int readInteger(int fd, int *value)
{
    if (read(fd, value, sizeof(int)) != sizeof(int))
      return EOF;
    return 0;
}

static void writeInteger(int fd, int value)
{
    if (write(fd, &value, sizeof(int)) != sizeof(int))
        println("Failed to write integer to fd %d", fd);
}

static void mergeFiles(int fd_in1, int fd_in2, int fd_out)
{
    int value_1;
    int value_2;
    int eof_1 = readInteger(fd_in1, &value_1);
    int eof_2 = readInteger(fd_in2, &value_2);

    while (eof_1 != EOF && eof_2 != EOF)
    {
        println("v1: %d; v2: %d", value_1, value_2);
        if (value_1 <= value_2)
        {
            writeInteger(fd_out, value_1);
            eof_1 = readInteger(fd_in1, &value_1);
        }
        else
        {
            writeInteger(fd_out, value_2);
            eof_2 = readInteger(fd_in2, &value_2);
        }
    }

    while (eof_1 != EOF)
    {
        println("v1: %d", value_1);
        writeInteger(fd_out, value_1);
        eof_1 = readInteger(fd_in1, &value_1);
    }

    while (eof_2 != EOF)
    {
        println("v2: %d", value_2);
        writeInteger(fd_out, value_2);
        eof_2 = readInteger(fd_in2, &value_2);
    }
}

// static void sortMergeFiles(int fd, int number, char **names)
// {
//     assert(number >= 0);
//     if (number == 0)
//         return;
//     else if (number == 1)
//         sortOneFile(fd, names[0]);
//     else
//     {
//         println("Non-Leaf: processing %d files (%s .. %s)", number, names[0], names[number-1]);
//         int mid = number / 2;
//         int l_pipe[2];
//         int r_pipe[2];
//         pid_t l_pid;
//         pid_t r_pid;
//         if (pipe(l_pipe) < 0 || pipe(r_pipe) < 0)
//             perror("Failed to create pipes");
//         if ((l_pid = fork()) < 0)
//             perror("Failed to fork left child");
//         else if (l_pid == 0)
//         {
//             close(l_pipe[READ]);
//             close(r_pipe[READ]);
//             close(r_pipe[WRITE]);
//             sortMergeFiles(l_pipe[WRITE], mid, names);
//             close(l_pipe[WRITE]);
//             exit(0);
//         }
//         else if ((r_pid = fork()) < 0)
//             perror("Failed to fork right child");
//         else if (r_pid == 0)
//         {
//             close(r_pipe[READ]);
//             close(l_pipe[READ]);
//             close(l_pipe[WRITE]);
//             sortMergeFiles(r_pipe[WRITE], number - mid, names + mid);
//             close(r_pipe[WRITE]);
//             exit(0);
//         }
//         else
//         {
//             close(l_pipe[WRITE]);
//             close(r_pipe[WRITE]);
//             mergeFiles(l_pipe[READ], r_pipe[READ], fd);
//             close(l_pipe[READ]);
//             close(r_pipe[READ]);
//             println("Non-Leaf: finished %d files (%s .. %s)", number, names[0], names[number-1]);
//         }
//     }
// }

static void addNumberToArray( int number ) {

    if (a_used == a_size) { // if we need to allocate more space
      println("a_used == a_size. space in array used up");
      int  n_size = (a_size + 1) * 2;
      int *n_data = realloc(a_data, sizeof(*n_data) * n_size);
      if (n_data == 0)
          println("Failed to allocate space for %d numbers", n_size);
      a_data = n_data;
      a_size = n_size;
    }

    a_data[a_used++] = number;
}

/* Could be compressed to write(fd, a_data, a_used * sizeof(int)); */
/* Arguably should check for write errors - but not SIGPIPE */
static void writeArray(int fd)
{   int i;
    for (i=0; i < a_used; i++)
    {
        println("Write: %d", a_data[i]);
        write(fd, &a_data[i], sizeof(int));
    }
}

void readFile(char *fileName)
{
    char buffer[BUFFER_SIZE];
    FILE *fp;
    fp = fopen(fileName, "r");

    if (fp == NULL)
        println("Failed to open file %s for reading", fileName);

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        char *nl = strchr(buffer, '\n');
        if (nl != 0)
            *nl = '\0';
        // println("Line: %s", buffer); // TODO - this is where attribute comes in
        addNumberToArray(atoi(buffer)); // convert line to integer and add to array
    }

    fclose(fp);
}


void sortArray(void)
{
    log("sortArray");
    qsort(a_data, a_used, sizeof(int), intcmp);
}

void freeMem(void)
{
    free(a_data);
}

// static void sortOneFile(int fd, const char *file)
// {
//     println("Leaf: processing file %s", file);
//     readFile(file);
//     sortArray();
//     writeArray(fd);
//     freeMem();
//     println("Leaf: finished file %s", file);
// }