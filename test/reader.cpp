#include "happly.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;

int main(int argc, char** argv) {

  if (argc != 2) {
    cout << "Usage: reader file.ply" << endl;
    std::exit(-1);
  }

  happly::PLYData ply(std::string(argv[1]), true);

  std::vector<double> vec = ply.getProperty<double>("vertex", "x");
  for (auto x : vec) {
    cout << x << endl;
  }


  std::vector<std::vector<size_t>> inds = ply.getListProperty<size_t>("face", "vertex_indices");
  inds = ply.getFaceIndices();
  for (auto face : inds) {
    for (auto x : face) {
      cout << x << " ";
    }
    cout << endl;
  }

  return 0;
}
