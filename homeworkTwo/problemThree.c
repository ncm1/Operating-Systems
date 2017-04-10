#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#include <unistd.h>

#define true 1
#define false 0

sem_t serviceDesk;
sem_t seating;
int K_Seats = 3;

void take_a_walk()
{
  printf("Customer %d is taking a walk\n", (int)pthread_self());
  sleep(2); //Taking a walk for 2 seconds...
}

void make_transaction()
{
  sleep(2);
  printf("Customer %u made transaction successfully\n", (int)pthread_self());
  sem_post(&seating); //when going to make transaction seating is avaliable
}

void take_a_seat()
{
  printf("Customer %u has been seated\n", (int)pthread_self());
  int numOfSeats;
  sem_getvalue(&seating, &numOfSeats); //numOfSeats = seating
  printf("There are %d seats avaliable\n", numOfSeats);
}

void return_home()
{
  printf("Customer %u is going home\n", (int)pthread_self());
  sem_post(&serviceDesk); //Customer leaves after going to service desk
}

void bank_client()
{

  while(true)
  {
    int numOfSeats;
    sem_getvalue(&seating, &numOfSeats); //numOfSeats = seating

    if( numOfSeats > 0 )
    {
      sem_wait(&seating); //if there is no seating block transaction
      take_a_seat();      //after seating is avaliable

      sem_wait(&serviceDesk); //if there is no service desk block transaction
      make_transaction();   //make transaction after seat and desk are avaliable
      break;
    }
    else
    {
      take_a_walk();
    }
  }
  //Customer has finished successfully, can go home now
  return_home();
}

int main(int argc, char* argv[])
{

  //////////////////////////////////////////////////////////////////////
 //           Verify the input is as specified in README             //
//////////////////////////////////////////////////////////////////////
if(argc != 2)
{
  printf("Improper input, refer to README.txt\n");
  printf("Ending program\n");
  return -1;
} //end first check for input size

int length = strlen(argv[1]);  //store the amount of char in integer length
for(int i = 0; i < length; i++){
  if(!isdigit(argv[1][i])){
    printf("Input is not an Integer!\n");
    return -1;
  }//end for: checking that input is an integer
}

int input;             //integer input used for storing the input
input = atoi(argv[1]); //convert the input into an integer using atoi

if(!(input > 4))
{
  printf("Number not greater than 4...ending\n");
  return -1;
}

/*                                         /
          Begin Bank Simulation here
/                                         */
sem_init(&seating, 0, K_Seats); //initialize semaphore to 4 avaliable seats
sem_init(&serviceDesk, 0, 1);   //initializes semaphore to one avaliable Desk

int numberOfThreads = input;
void *threadStatus;
int  threadCreate;

pthread_t threads[numberOfThreads];

printf("Starting Bank Simulation with %d Customers", numberOfThreads);

for(int j = 0; j < numberOfThreads; j++)
{
    threadCreate = pthread_create(&threads[j], NULL, (void *)bank_client, NULL);
}
for(int k = 0; k < numberOfThreads; k++)
{
    threadCreate = pthread_join(threads[k], &threadStatus);
}
  printf("Ending Bank Simulation...\n");
  return 0;
}
