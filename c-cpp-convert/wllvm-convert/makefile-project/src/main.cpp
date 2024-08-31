#include "utils/myfunctions.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  int x = 42;
  int y = 13;
  int z = add(x, y);
  cout << "x + y = " << z << '\n';
  return 0;
}
