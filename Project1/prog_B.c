#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int bufferSize, char* buffer[])
{
  //make sure that the program is run with proper input parameters
  //EXAMPLE: <program> <input path> <number of integers>
  //        ./sum input/input_10.txt 10
  if(bufferSize != 3)
  {
    printf("Improper input, refer to README.txt\n");
    printf("Ending program\n");
    return 0;
  }
  //fp is declared as a pointer to a file, use fopen to open the buffer parameter passed
  FILE *fp;
  long long sum = 0;           // declare sum as long long and initialize to 0
  int max = 1;                 // declare the max as INT_MIN
  int min = atoi(buffer[2])*10;// declare the min as INT_MAX
  int mypipefd[2];

  fp = fopen(buffer[1], "r");
  int maxSize = 0;           //declare and initialize maxSize for array
  maxSize = atoi(buffer[2]); //the max size of the array converted to integer
  int *numberArray;          //declaring a pointer variable to point to allocated heap space

  //dynamically allocating memory, because C doesn't spoil us like C++
  numberArray = (int*)malloc(sizeof(int)*maxSize);

  for(int l = 0; l < maxSize; l++)
  {
    fscanf(fp, "%d", &numberArray[l]);
  }

  int   status;
  int   mypipe[2];
  pid_t pid, pid1, pid2, pid3;      // declare pid's for sum, max, and min


  pipe(mypipe);
  pid1 = fork();
  if(pid1 == 0)
  {
    printf("1. Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    for(int i = 0; i < maxSize; i++)
    {
      if(max < numberArray[i])
        max = numberArray[i];
    }
    close(mypipe[0]);
    write(mypipe[1], &max, sizeof(max));
    pid2 = fork();
  }

  if( pid2 == 0)
  {
    printf("2. Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    min = numberArray[0];
    for(int j = 0; j < maxSize; j++)
    {
      if(min > numberArray[j])
        min = numberArray[j];
    }
    close(mypipe[0]);
    write(mypipe[1], &min, sizeof(min));
    pid3 = fork();
  }*/

  /*if( pid3 == 0)
  {
    printf("3. Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    for(int k = 0; k < maxSize; k++)
    {
      sum = sum + numberArray[k];
    }
    close(mypipe[0]);
    write(mypipe[1], &sum, sizeof(sum));
  }*/

  else
  {
    printf("Parent process %d\n", getpid());

    close(mypipe[1]);
    read(mypipe[0], &max, sizeof(max));
    read(mypipe[0], &min, sizeof(min));
    read(mypipe[0], &sum, sizeof(sum));
    printf("Max is: %d\n", max);
    printf("Min is: %d\n", min);
    printf("Sum is: %d\n", sum);
  }

    //close(mypipefd[1]);
    //read(mypipefd[0], &sum, sizeof(sum));

    //printf("Sum is: %d\n", sum);
    //fclose(fp);
    //free(numberArray);  //free up the allocated memory

printf("Ending process %d with parent %d\n", getpid(), getppid());
return 0;

}

/*References:
http://stackoverflow.com/questions/22749754/c-user-input-size-of-array
https://www.cs.swarthmore.edu/~newhall/unixhelp/C_arrays.html
http://stackoverflow.com/questions/7021725/converting-string-to-integer-c
http://stackoverflow.com/questions/23995638/i-get-an-error-with-fopen
*/
