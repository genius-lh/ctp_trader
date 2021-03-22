
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <string>
using namespace std;

static map<string, int> mapTest;

int main(int argc, char* argv[])
{
  char a[] = "a";
  char b[] = "b";


  string a1 = a;
  mapTest.insert(map<string, int>::value_type(a1, 1));
  a1 = b;
  mapTest.insert(map<string, int>::value_type(a1, 2));

  a1 = a;
  map<string, int>::iterator iter = mapTest.find(a1);

  if(iter != mapTest.end()){
    printf("find %s:(%d)\n", a1.c_str(), iter->second);
  }
  
  a1 = b;
  iter = mapTest.find(a1);

  if(iter != mapTest.end()){
    printf("find %s:(%d)\n", a1.c_str(), iter->second);
  }
  

  return 0;
}

