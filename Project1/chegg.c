#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    FILE *fp, *fp1;
    int i = 0, j, num[10], total = 0, max, min;
    char word[32];
    fp = fopen("text.txt", "r"); //open text in read mode
    fp1 = fopen("text1.txt", "a+");// open text1 in append mode
    if(fork()==0){
        i=0;
        fprintf(fp1, "\nHi i'm process %d and my parent is %d\n", getpid(), getppid()); //write data into text1 file
        while(fscanf(fp, "%s", word)!=EOF) //read the data from text file
            num[i++] = atoi(word);
        max = num[0];
        for(j=0; j<i; j++)
            if(num[j] > max) // find the max number from file
               max = num[j];

    }
    else
    {

        if(fork()==0){
           i=0;
            fprintf(fp1, "\nHi i'm process %d and my parent is %d\n", getpid(), getppid());
            while(fscanf(fp, "%s", word)!=EOF)
              num[i++] = atoi(word);
            min = num[0];
            for(j=0; j<i; j++)// find the min num
                if(num[j] < min)
                    min = num[j];
            rewind(fp);

        }
        else{
            i=0;
            fprintf(fp1, "\nHi i'm process %d and my parent is %d\n", getpid(), getppid());
            while(fscanf(fp, "%s", word)!=EOF)
              num[i++] = atoi(word);

            for(j=0; j<i; j++) // find the sum
                total = total + num[j];
            rewind(fp);

        }
    }
     fprintf(fp1, "Max= %d\n", max);
     fprintf(fp1, "Min=%d\n", min);
     fprintf(fp1, "Sum=%d\n", total);
}
