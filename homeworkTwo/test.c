#include <stdio.h>
#include <ctype.h>
#include <string.h>

//Simple program that checks if user input is an integer
void main(int argc, char* argv[]){

int length;
length = strlen(argv[1]);
printf("Length: %d\n", length);

printf("argv[1]: %s\n", argv[1]);
for(int i = 0; i < length; i++){
if(isdigit(argv[1][i]))
  printf("Hello I'm a digit\n");
else
  printf("I'm not a digit\n");
}
}
