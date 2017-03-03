#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>

int getMax(int [], int[]);
int getMin(int [], int[]);
int getSum(int [], int[]);
int *partition(FILE *, int, int[]);
void print(int [], int []);

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

  uint64_t sum;                 // declare sum as uint64 and initialize to 0
  int max;                      // declare the max as INT_MIN
  int min;                      // declare the min as INT_MAX
  int maxSize;                  // declare and initialize maxSize for array
  maxSize = atoi(buffer[2]);    // the max size of the array converted to integer

  FILE *fp;                     // fp is declared as a pointer to a file,
  fp = fopen(buffer[1], "r");   // use fopen to open the buffer parameter passed

  int limits1[2], limits2[2], limits3[2];                            //limit arrays for partitioning of the array
  int *numberArray_Proc1, *numberArray_Proc2 , *numberArray_Proc3;   //declaring integer arrays and allocating memory

  //dynamically allocating memory, because C doesn't spoil us like C++
  limits1[0] = 0;
  limits1[1] = maxSize / 3;
  numberArray_Proc1 =  partition(fp, maxSize, limits1);

  printf("First partition\n------------------------\n");
  print(numberArray_Proc1, limits1);

  //dynamically allocating memory, because C doesn't spoil us like C++
  limits2[0] = limits1[1] + 1;
  limits2[1] = 2 * limits1[1];
  numberArray_Proc2 = partition(fp, maxSize, limits2);

  printf("Second partition\n------------------------\n");
  print(numberArray_Proc2, limits2);

  //dynamically allocating memory, because C doesn't spoil us like C++
  limits3[0] = limits2[1];
  limits3[1] = maxSize + 1;
  numberArray_Proc3 = partition(fp, maxSize, limits3);

  printf("Third partition\n------------------------\n");
  print(numberArray_Proc3, limits3);

  printf("------------------------------\n");

  int   status;                       // integer status for waiting for pids to finish
  int   vals1[3], vals2[3], vals3[3]; // integer array storing the results of min, max, and sum
  int   mypipe1[2];                   // pipe for communication between the children processes
  int   mypipe2[2];                   // pipe for communication between the children processes
  int   mypipe3[2];                    // pipe for communication between the children processes
  pid_t pid, pid1, pid2, pid3;        // declare pid's for sum, max, and min

  //Open up a pipe for communication between the children
  pipe(mypipe1);
  /* Spawn the child process */
  if((pid1 = fork()) ==  0)
  {
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    vals1[0] = getMax(numberArray_Proc1, limits1);
    vals1[1] = getMin(numberArray_Proc1, limits1);
    vals1[2] = getSum(numberArray_Proc1, limits1);


    close(mypipe1[0]);
    write(mypipe1[1], &vals1, sizeof(vals1));
    return 0;
    //exit(0);
  }
  pipe(mypipe2);
  if( (pid2 = fork()) ==  0)
  {
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    vals2[0] = getMax(numberArray_Proc2, limits2);
    vals2[1] = getMin(numberArray_Proc2, limits2);
    vals2[2] = getSum(numberArray_Proc2, limits2);

    close(mypipe2[0]);
    write(mypipe2[1], &vals2, sizeof(vals2));
    return 0;
    //exit(0);
  }
  pipe(mypipe3);
  if((pid3 = fork()) ==  0)
  {
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    vals3[0] = getMax(numberArray_Proc3, limits3);
    vals3[1] = getMin(numberArray_Proc3, limits3);
    vals3[2] = getSum(numberArray_Proc3, limits3);

    close(mypipe3[0]);
    write(mypipe3[1], &vals3, sizeof(vals3));
    return 0;
    //exit(0);
  }

  else
  {
    //wait(&status);
    /* Parent Process */
    printf("Hello I'm process %d and my parent is %d\n", getpid(), getppid());
    // Close the first child pipe, and read the values
    close(mypipe1[1]);
    read(mypipe1[0], &vals1, sizeof(vals1));

    /* DEBUGGING */
    printf("vals1[0]: %d\n", vals1[0]);
    printf("vals1[1]: %d\n", vals1[1]);
    printf("vals1[2]: %d\n", vals1[2]);
    /* DEBUGGING ENDS */

    max = vals1[0];
    min = vals1[1];
    sum = vals1[2];
    printf("1. max: %d\n", max);
    printf("1. min: %d\n", min);

    //wait(&status);
    close(mypipe2[1]);
    read(mypipe2[0], &vals2, sizeof(vals2));

    /* DEBUGGING */
    printf("vals2[0]: %d\n", vals2[0]);
    printf("vals2[1]: %d\n", vals2[1]);
    printf("vals2[2]: %d\n", vals2[2]);
    /* DEBUGGING ENDS */

    if(vals2[0] >= vals1[0])
      max = vals2[0];
    if(vals2[1] <= vals1[1])
      min  = vals2[1];
    printf("2.max: %d\n", max);
    printf("2.min: %d\n", min);

    sum = sum + vals2[2];

    //wait(&status);
    close(mypipe3[1]);
    read(mypipe3[0], &vals3, sizeof(vals3));

    /* DEBUGGING */
    printf("vals3[0]: %d\n", vals3[0]);
    printf("vals3[1]: %d\n", vals3[1]);
    printf("vals3[2]: %d\n", vals3[2]);
    /* DEBUGGING ENDS */

    if(vals3[0] >= max)
        max = vals3[0];
    if(vals3[1] <= min)
        min  = vals2[1];
    printf("3. max: %d\n", max);
    printf("3. min: %d\n", min);
    sum = sum + vals3[2];


    printf("Max is: %d\n", max);
    printf("Min is: %d\n", min);
    printf("Sum is: %"PRIu64"\n", sum);

    //fclose(fp);
    //free(numberArray);  //free up the allocated memory
  }

wait(&status);
printf("Ending process %d with parent %d\n", getpid(), getppid());
return 0;

}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the maximum of the INTEGERS in ARRAY num[]
*/
int getMax(int num[], int limits[])
{
  int max = num[0];
  int START = limits[0];
  int END   = limits[1];
  for(int i = START; i < END; i++)
  {
    if(max < num[i])
      max = num[i];
  }
  return max;
}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the minimum of the INTEGERS in ARRAY num[]
*/
int getMin(int num[], int limits[])
{
  int min = limits[1];
  int START = limits[0];
  int END   = limits[1];
  for(int i = START; i < END; i++)
  {
    if(min > num[i])
      min = num[i];
  }
  return min;
}
/* PRECONDITIONS: num is an ARRAY of INTEGERS and maxsize is an ARRAY
   PRECONDITIONS: Returns the sum of the INTEGERS in ARRAY num[]
*/
int getSum(int num[], int limits[])
{
  int sum = 0;
  int START = limits[0];
  int END   = limits[1];
  for(int i = START; i < END; i++)
  {
    sum = sum + num[i];
  }
  return sum;
}
/* PRECONDITIONS:
   PRECONDITIONS:
*/
int *partition(FILE *fp, int maxSize, int limits[])
{
  int *numberArray = (int*)malloc(sizeof(int)*(1 + maxSize / 3));
  int START = limits[0];
  int END   = limits[1];
  for(int i = START; i < END; i++)
  {
    fscanf(fp, "%d", &numberArray[i]);
  }
  return numberArray;
}

void print(int arr[], int limits[])
{
  int START = limits[0];
  int END   = limits[1];
  for(int i = START; i < END; i++)
    printf("arr[%d]: %d\n", i, arr[i]);

}
/*References:
http://stackoverflow.com/questions/22749754/c-user-input-size-of-array
https://www.cs.swarthmore.edu/~newhall/unixhelp/C_arrays.html
http://stackoverflow.com/questions/7021725/converting-string-to-integer-c
http://stackoverflow.com/questions/23995638/i-get-an-error-with-fopen
*/
