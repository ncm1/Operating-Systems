#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;
ofstream fileSelect(int);

int main()
{
 random_device rand;

 ofstream fileOutput;
 int n[] = {10, 100, 1000, 10000, 100000};


 for(int i = 0; i < 5; i++)
 {
   default_random_engine gen(rand());
   uniform_int_distribution<int> ranNum(1, 10*n[i]);
   fileOutput = fileSelect(i+1);
    for(int j = 0; j < n[i]; j++)
    {
      fileOutput << ranNum(gen) << endl;
    }
    fileOutput.close();
 }

return 0;
}

ofstream fileSelect(int sel)
{

ofstream fileOutput;
switch(sel)
{
  case 1:
    fileOutput.open("./input/input_10.txt");
    return fileOutput;
  case 2:
    fileOutput.open("./input/input_100.txt");
    return fileOutput;
  case 3:
    fileOutput.open("./input/input_1k.txt");
    return fileOutput;
  case 4:
    fileOutput.open("./input/input_10k.txt");
    return fileOutput;
  case 5:
    fileOutput.open("./input/input_100k.txt");
    return fileOutput;
}

}
