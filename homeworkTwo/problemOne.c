#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

/*
Input: an integer n > 1.
Let A be an array of Boolean values, indexed by integers 2 to n,
initially all set to true.
for i = 2, 3, 4, ..., not exceeding √n:
 if A[i] is true:
 for j = i^2, i^2+i, i^2+2i, i^2+3i, ..., not exceeding n:
 A[j] := false.
Output: all i such that A[i] is true.
*/

#define true 1
#define false 0

void main(int argc, char* argv[]){

//Verify the input is as specified in README
if(argc != 2)
{
  printf("Improper input, refer to README.txt\n");
  printf("Ending program\n");
  return;
} //end first check for input size

int length = strlen(argv[1]);  //store the amount of char in integer length
for(int i = 0; i < length; i++){
  if(!isdigit(argv[1][i])){
    printf("Input is not an Integer!\n");
    return;
  }//end for: checking that input is an integer
}
//*********************************************************************

int input;             //integer input used for storing the input
input = atoi(argv[1]); //convert the input into an integer using atoi

int *A;                              //A: pointer for the array of booleans
A = (int*)malloc(sizeof(int)*input); //allocating enough memory for array

// Set all of the initial values to true in the Array
for(int j = 2; j < input; j++){
  A[j - 2] = true;
}//end for

int sqrtPart;                    //integer sqrtPart declared
sqrtPart = (int)sqrt(input);     //store the integer part of sqrt(input)

int k;
int l = 0;
for(k = 2; k < sqrtPart; k++){
  if(A[k - 2] == true){
    l = k*k;
    while(l < input){
      A[l - 2] = false;
      l = l + k;
    }//end while
  }//end if
}//end for

//output the results..
printf("Outputting the results of the sieve...\n");
for(int m = 2; m < input; m++){
  if(A[m - 2] == true)
    printf("%d\n", m);
}//end for

}
