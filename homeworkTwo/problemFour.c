#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h> //for sleep
#include <math.h>

//Shared variables between Parent() Teacher() and Child()
int numOfParents = 0, numOfChildren = 0, numOfTeachers = 0;
int Ratio = 3; //set the Child/Teacher Ratio as 3
sem_t mutex;
sem_t door;
sem_init(&door, 0, 0);
sem_init(&mutex,0, 1);

#define true 1
#define false 0

void update_currRatio()
{
  currRatio = numOfChildren / numOfTeachers + (numOfChildren % numOfTeachers != 0);
}
void teacher_exit()
{
  numOfTeachers--;
  printf("A teacher has left!\n");
  printf("There are a total of %d teachers are present\n", numOfTeachers);
}
void teacher_enter()
{
  numOfTeachers++;
  printf("A teacher has entered!\n");
  printf("There are a total of %d teachers are present\n", numOfTeachers);
}

void teach()
{
  printf("Teacher is teaching\n");
}
void Teacher()
{

  while(true)
  {
    wait(mutex);
    //Start First Critical Section of teacher
    teacher_enter();
    update_currRatio();
    while(currRatio < R)
    {
      teacher_enter();
      update_currRatio();
    } //to enforce the ratio
    if( currRatio > R)
      signal(door);
    signal(mutex);
    //End First Critial Section
    while(true)
    {
    teach();
      if(currRatio > R)
      {
        teacher_exit();
        update_currRatio();
        break;
      }
    }
    signal(mutex);
  }
}

void learn()
{
  printf("Child is learning\n");
}
void child_enter()
{
  numOfChildren++;
  printf("A child has entered!\n");
  printf("There are a total of %d children are present\n", numOfChildren);
}
void child_exit()
{
  numOfChildren--;
  printf("A parent has left!\n");
  printf("There are a total of %d children are present\n", numOfChildren);
}
void Child()
{

  while(true)
  {
    while(true)
    {
      wait(mutex);
      wait(door);
      //Critical Section of child
      update_currRatio();
      if(currRatio == R)
      {
        child_enter(); //child enters the classroom
        update_currRatio();
        if(currRatio == R)
          signal(door);
        signal(mutex);
        break;
      }
      signal(mutex);
      //End critical Section of child
    }
      learn();
      wait(mutex);
      //Critical Section of child
      child_exit();
      //End Critical Section of child
      signal(mutex);
    }
  }

}

void parent_enter()
{
  numOfParents++;
  printf("A parent has entered!\n");
  printf("There are a total of %d parents are present\n", numOfParents);
}

void parent_exit()
{
  numOfParents--;
  printf("A parent has left!\n");
  printf("There are a total of %d parents are present\n", numOfParents);
}

void Parent()
{
  while(true)
  {
    parent_enter();
    wait(mutex);
    //Critical Section
    verify_compliance();
    //End Critical Section
    signal(mutex);
    parent_exit();
  }
}

void verify_compliance()
{
  update_currRatio(); //update the current ratio to verify the compliance
  if(currRatio == R)
    printf("Student / Teacher Ratio is meant...\n");
  else
    printf("Student / Teacher Ratio not in place...\n");
}
