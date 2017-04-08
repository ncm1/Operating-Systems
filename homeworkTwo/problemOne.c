#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define true 1
#define false 0
//Sieve of Eratosthenes for finding the primes up to the specified input
void *sieveEratosthenes(void *arg){

  int input = (int) arg;

  int *A;                              //A: pointer for the array of booleans
  A = (int*)malloc(sizeof(int)*input); //allocating enough memory for array

  // Set all of the initial values to true in the Array
  for(int j = 2; j < input; j++){
    A[j - 2] = true;
  }//end for

  int sqrtPart;                        //integer sqrtPart declared
  sqrtPart = (int)sqrt(input) + 1;     //store the integer part of sqrt(input)

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
  printf("Outputting the results of the sieve for %d...\n", input);
  for(int m = 2; m < input; m++){
    if(A[m - 2] == true)
      printf("%d\n", m);
  }//end for
}

/* Iterative function to reverse digits of num*/
int reversDigits(int num)
{
	int rev_num = 0;
	while(num > 0)
	{
		rev_num = rev_num*10 + num%10;
		num = num/10;
	}
	return rev_num;
} //Reference: http://ide.geeksforgeeks.org/index.php

int isGreaterThanOne(int input){
  if(input > 1)
    return true;
  else
    return false;
}

//Main program for creating the two threads
void main(int argc, char* argv[]){


  //////////////////////////////////////////////////////////////////////
 //           Verify the input is as specified in README             //
//////////////////////////////////////////////////////////////////////
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

int input;             //integer input used for storing the input
input = atoi(argv[1]); //convert the input into an integer using atoi

if(!isGreaterThanOne(input)){
  printf("Number not greater than 1...ending\n");
  return;
}

int pt1_status;
int pt2_status;
pthread_t tid1;
pthread_t tid2;

pt1_status = pthread_create(&tid1, NULL, sieveEratosthenes, (void*)input);
pthread_join(tid1, NULL);

int input_reverse = reversDigits(input);
if(!isGreaterThanOne(input_reverse)){
  printf("Reverse number smaller than one\n");
}

pt2_status = pthread_create(&tid2, NULL, *sieveEratosthenes,(void*)input_reverse);
pthread_join(tid2, NULL);

}
