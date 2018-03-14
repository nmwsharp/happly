#include "happly.h"

#include <string>
#include <iostream>

using std::cout; using std::endl;

int main(int argc, char** argv) {

  if(argc != 2) {
    cout << "Usage: reader file.ply" << endl;
    std::exit(-1);
  }

  happly::PLYData ply(std::string(argv[1]), true); 

  std::vector<int> vec = ply.getProperty<int>("vertex", "x");
  for(auto x : vec) {
    cout << x << endl;
  }

  return 0;
}
