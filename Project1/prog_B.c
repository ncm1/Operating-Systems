#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>

int getMax(int [], int);
int getMin(int [], int);
int getSum(int [], int);

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


  uint64_t sum = 0;             // declare sum as uint64 and initialize to 0
  int max = 1;                  // declare the max as INT_MIN
  int min = atoi(buffer[2])*10; // declare the min as INT_MAX

  FILE *fp;                     // fp is declared as a pointer to a file,
  fp = fopen(buffer[1], "r");   // use fopen to open the buffer parameter passed
  int maxSize = 0;              // declare and initialize maxSize for array
  maxSize = atoi(buffer[2]);    // the max size of the array converted to integer
  int *numberArray;             // declaring a pointer variable to point to allocated heap space

  //dynamically allocating memory, because C doesn't spoil us like C++
  numberArray = (int*)malloc(sizeof(int)*maxSize);

  for(int l = 0; l < maxSize; l++)
  {
    fscanf(fp, "%d", &numberArray[l]);
  }

  int   status;                     // integer status for waiting for pids to finish
  int   vals[3];                    // integer array storing the results of min, max, and sum
  int   mypipe[2];                  // pipe for communication between the children processes
  pid_t pid, pid1, pid2, pid3;      // declare pid's for sum, max, and min

  //Open up a pipe for communication between the children
  pipe(mypipe);
  /* Spawn the child process */
  pid1 = fork();
  if(pid1 == 0)
  {
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    vals[0] = getMax(numberArray,maxSize);

    /*Spawn the second child process */
    pid2 = fork();
    if( pid2 == 0)
    {
      printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
      vals[1] = getMin(numberArray,maxSize);

      /* Spawn the third child process */
      pid3 = fork();
      if( pid3 == 0)
      {
        printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
        vals[2] = getSum(numberArray, maxSize);
        //Write the results for the parent process to read
        close(mypipe[0]);
        write(mypipe[1], &vals, sizeof(vals));
      }
    }
  }


  else
  {
    /* Parent Process */
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    // Close the first child pipe, and read the values
    close(mypipe[1]);
    read(mypipe[0], &vals, sizeof(vals));
    max = vals[0];
    min = vals[1];
    sum = vals[2];

    printf("Max is: %d\n", max);
    printf("Min is: %d\n", min);
    printf("Sum is: %"PRIu64"\n", sum);

    fclose(fp);
    free(numberArray);  //free up the allocated memory
  }

wait(&status);
printf("Ending process %d with parent %d\n", getpid(), getppid());
return 0;

}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the maximum of the INTEGERS in ARRAY num[]
*/
int getMax(int num[], int maxSize)
{
  int max = num[0];
  for(int i = 0; i < maxSize; i++)
  {
    if(max < num[i])
      max = num[i];
  }
  return max;
}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the minimum of the INTEGERS in ARRAY num[]
*/
int getMin(int num[], int maxSize)
{
  int min = num[0];
  for(int i = 0; i < maxSize; i++)
  {
    if(min > num[i])
      min = num[i];
  }
  return min;
}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the sum of the INTEGERS in ARRAY num[]
*/
int getSum(int num[], int maxSize)
{
  int sum = 0;
  for(int i = 0; i < maxSize; i++)
  {
    sum = sum + num[i];
  }
  return sum;
}

/*References:
http://stackoverflow.com/questions/22749754/c-user-input-size-of-array
https://www.cs.swarthmore.edu/~newhall/unixhelp/C_arrays.html
http://stackoverflow.com/questions/7021725/converting-string-to-integer-c
http://stackoverflow.com/questions/23995638/i-get-an-error-with-fopen
*/
