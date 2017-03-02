#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main(int bufferSize, char* buffer[])
{
  //make sure that the program is run with proper input parameters
  //EXAMPLE: PROGRAM FILE PATH FILE SIZE
  //        ./sum input/input_10.txt 10
  if(bufferSize != 3)
  {
    printf("Improper input, refer to README.txt\n");
    printf("Ending program\n");
    return 0;
  }
  //fp is declared as a pointer to a file, use fopen to open the buffer parameter passed
  FILE *fp;
  fp = fopen(buffer[1], "r");

  int maxSize = 0;           //declare and initialize maxSize for array
  maxSize = atoi(buffer[2]); //the max size of the array converted to integer
  int *numberArray;          //declaring a pointer variable to point to allocated heap space

  //dynamically allocating memory, because C doesn't spoil us like C++
  numberArray = (int*)malloc(sizeof(int)*maxSize);

  //load the integers into the dynamically allocated array numberArray
  for(int i = 0; i < maxSize; i++){
    fscanf(fp, "%d", &numberArray[i]);
    //printf("numberArray[%d]: %d\n", i+1,numberArray[i]);
  }

  uint64_t sum = 0;            // declare sum as long long and initialize to 0
  int max = numberArray[0];    // set the max as the first element
  int min = numberArray[0];    // set the min as the first element
  for(int j = 0; j < maxSize; j++)
  {
    if(max < numberArray[j])
      max = numberArray[j];
    if(min > numberArray[j])
      min = numberArray[j];
    sum = sum + numberArray[j];
  }

  printf("Sum is: %"PRIu64"\n", sum);
  printf("Max is: %d\n", max);
  printf("Min is: %d\n", min);
return 0;
}

/*References:
http://stackoverflow.com/questions/22749754/c-user-input-size-of-array
https://www.cs.swarthmore.edu/~newhall/unixhelp/C_arrays.html
http://stackoverflow.com/questions/7021725/converting-string-to-integer-c
http://stackoverflow.com/questions/23995638/i-get-an-error-with-fopen
*/
