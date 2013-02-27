// 26 child processes write characters to file - parent reads the file

#include<stdio.h>
#include <sys/types.h>
#include<stdio.h>


int main()
{
    FILE *f;
    char c;
    int a = 'A';
    int C = 97; /*'a' = 97*/
    int i;
    int r, pid;
    remove("f.txt");
    for ( i = 1; i <= 26; i++ ){
         if ((pid = fork()) != 0){
                /*parent process  pid != 0*/
                /* wait for chil to terminate */
                waitpid(pid);
                a++;            // increment character
               }
         else
         {
             /*child process pid = 0*/
             /*write next character to file*/
             f = fopen("f.txt", "a");
                 fprintf(f, "%c", a);
                 fclose(f);
                 exit(1);
          }
    }
    // now open resultant file and print it
    f = fopen("f.txt", "r");
    while (1){
       fscanf(f, "%c", &c);
       if ( feof(f))break;
       printf("%c", c);
       }
    fclose(f);
    printf("\n");
}
