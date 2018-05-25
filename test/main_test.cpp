#include "happly.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

using std::cout;
using std::endl;


void DoubleArrayVecEq(std::vector<std::array<double, 3>>& arr1, std::vector<std::array<double, 3>>& arr2) {

  EXPECT_EQ(arr1.size(), arr2.size());

  for (size_t i = 0; i < arr1.size(); i++) {
    for (int j = 0; j < 3; j++) {
      ASSERT_DOUBLE_EQ(arr1[i][j], arr2[i][j]);
    }
  }
}

// Test reading mesh-like files
TEST(MeshTest, ReadASCIIWriteASCIIMesh) {

  // = Read in an interesting mesh file
  happly::PLYData plyIn("../sampledata/spot_quad.ply", false);
  plyIn.validate();

  std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
  std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices();


  // = Write out the mesh file
  happly::PLYData plyOut;
  plyOut.addVertexPositions(vPos);
  plyOut.addFaceIndices(fInd);

  plyOut.validate();
  plyOut.write("temp.ply");


  // = Read the mesh file in again and make sure it hasn't changed
  happly::PLYData plyIn2("temp.ply", false);
  plyIn2.validate();

  std::vector<std::array<double, 3>> vPos2 = plyIn2.getVertexPositions();
  std::vector<std::vector<size_t>> fInd2 = plyIn2.getFaceIndices();

  DoubleArrayVecEq(vPos, vPos2);
  EXPECT_EQ(fInd, fInd2);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
