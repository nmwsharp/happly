#include "happly.h"

#include <string>
#include <iostream>

using std::cout; using std::endl;

int main(int argc, char** argv) {

  if(argc != 2) {
    cout << "Usage: reader file.ply" << endl;
    std::exit(-1);
  }

  happly::PLYData(std::string(argv[1]), true); 

  return 0;
}
