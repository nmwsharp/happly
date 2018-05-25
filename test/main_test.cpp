#include "happly.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

using std::cout;
using std::endl;

// === Test helpers
void DoubleArrayVecEq(std::vector<std::array<double, 3>>& arr1, std::vector<std::array<double, 3>>& arr2) {

  EXPECT_EQ(arr1.size(), arr2.size());

  for (size_t i = 0; i < arr1.size(); i++) {
    for (int j = 0; j < 3; j++) {
      EXPECT_DOUBLE_EQ(arr1[i][j], arr2[i][j]);
    }
  }
}
//void VecEq(std::vector<double>& arr1, std::vector<double>& arr2) {
  //EXPECT_EQ(arr1.size(), arr2.size());
  //for (size_t i = 0; i < arr1.size(); i++) {
    //EXPECT_DOUBLE_EQ(arr1[i], arr2[i]);
  //}
//}

// == Test typed reading/writing

// = char
TEST(TypedReadWriteTest, ReadWriteCharASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<char> testData{-3, 4, 11, -12, 122};
  plyOut.getElement("test_elem").addProperty<char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<char> testDataASCII = plyIn.getProperty<char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteCharBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<char> testData{-3, 4, 11, -12, 122};
  plyOut.getElement("test_elem").addProperty<char>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<char> testDataBinary = plyInB.getProperty<char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned char
TEST(TypedReadWriteTest, ReadWriteUCharASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned char> testData{3, 0, 11, 255, 122};
  plyOut.getElement("test_elem").addProperty<unsigned char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<unsigned char> testDataASCII = plyIn.getProperty<unsigned char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteUCharBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned char> testData{3, 0, 11, 255, 122};
  plyOut.getElement("test_elem").addProperty<unsigned char>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<unsigned char> testDataBinary = plyInB.getProperty<unsigned char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}


// = short
TEST(TypedReadWriteTest, ReadWriteShortASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<short> testData{-3, 4, 32767, -32768, 122};
  plyOut.getElement("test_elem").addProperty<short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<short> testDataASCII = plyIn.getProperty<short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteShortBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<short> testData{-3, 4, 32767, -32768, 122};
  plyOut.getElement("test_elem").addProperty<short>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<short> testDataBinary = plyInB.getProperty<short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned short
TEST(TypedReadWriteTest, ReadWriteUShortASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned short> testData{3, 0, 11, 65535, 122};
  plyOut.getElement("test_elem").addProperty<unsigned short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<unsigned short> testDataASCII = plyIn.getProperty<unsigned short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteUShortBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned short> testData{3, 0, 11, 65535, 122};
  plyOut.getElement("test_elem").addProperty<unsigned short>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<unsigned short> testDataBinary = plyInB.getProperty<unsigned short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = int
TEST(TypedReadWriteTest, ReadWriteIntASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<int> testData{-3, 0, 2147483647, -2147483648, 122};
  plyOut.getElement("test_elem").addProperty<int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<int> testDataASCII = plyIn.getProperty<int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteIntBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<int> testData{-3, 0, 2147483647, -2147483648, 122};
  plyOut.getElement("test_elem").addProperty<int>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<int> testDataBinary = plyInB.getProperty<int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned int
TEST(TypedReadWriteTest, ReadWriteUIntASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned int> testData{3, 0, 4294967295, 15, 122};
  plyOut.getElement("test_elem").addProperty<unsigned int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<unsigned int> testDataASCII = plyIn.getProperty<unsigned int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteUIntBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<unsigned int> testData{3, 0, 4294967295, 15, 122};
  plyOut.getElement("test_elem").addProperty<unsigned int>("test_data", testData);

  // Binary read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyInB("temp.ply");
  std::vector<unsigned int> testDataBinary = plyInB.getProperty<unsigned int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = float
TEST(TypedReadWriteTest, ReadWriteFloatASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  std::vector<float> testData{
      3.141592653589793238,
      -3.141592653589793238,
      std::numeric_limits<float>::min(),
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::epsilon(),
      //std::numeric_limits<float>::infinity(),     // these DO NOT work right now, due to how ostream works
      //-std::numeric_limits<float>::infinity(),
      0.0,
      -0.0,
      1e24,
  };
  plyOut.addElement("test_elem", testData.size());
  plyOut.getElement("test_elem").addProperty<float>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<float> testDataASCII = plyIn.getProperty<float>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteFloatBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  std::vector<float> testData{
      3.141592653589793238,
      -3.141592653589793238,
      std::numeric_limits<float>::min(),
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::epsilon(),
      std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity(),
      0.0,
      -0.0,
      1e24,
  };
  plyOut.addElement("test_elem", testData.size());
  plyOut.getElement("test_elem").addProperty<float>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<float> testDataBinary = plyIn.getProperty<float>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}


// = double
TEST(TypedReadWriteTest, ReadWriteDoubleASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  std::vector<double> testData{
      3.141592653589793238,
      -3.141592653589793238,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max(),
      std::numeric_limits<double>::lowest(),
      std::numeric_limits<double>::epsilon(),
      //std::numeric_limits<double>::infinity(),     // these DO NOT work right now, due to how ostream works
      //-std::numeric_limits<double>::infinity(),
      0.0,
      -0.0,
      1e24,
  };
  plyOut.addElement("test_elem", testData.size());
  plyOut.getElement("test_elem").addProperty<double>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<double> testDataASCII = plyIn.getProperty<double>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedReadWriteTest, ReadWriteDoubleBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  std::vector<double> testData{
      3.141592653589793238,
      -3.141592653589793238,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max(),
      std::numeric_limits<double>::lowest(),
      std::numeric_limits<double>::epsilon(),
      std::numeric_limits<double>::infinity(), 
      -std::numeric_limits<double>::infinity(),
      0.0,
      -0.0,
      1e24,
  };
  plyOut.addElement("test_elem", testData.size());
  plyOut.getElement("test_elem").addProperty<double>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<double> testDataBinary = plyIn.getProperty<double>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}


// = signed char list
TEST(TypedListReadWriteTest, ReadWriteCharASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<char>> testData{
    {3},
    {3, 0, 11, -128, 127},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< char>> testDataASCII = plyIn.getListProperty< char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteCharBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector< char>> testData{
    {3},
    {3, 0, 11, -128, 127},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< char>> testDataBinary = plyIn.getListProperty< char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned char list
TEST(TypedListReadWriteTest, ReadWriteUCharASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned char>> testData{
    {3},
    {3, 0, 11, 255, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned char>> testDataASCII = plyIn.getListProperty<unsigned char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteUCharBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned char>> testData{
    {3},
    {3, 0, 11, 255, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned char>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned char>> testDataBinary = plyIn.getListProperty<unsigned char>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}



// = signed short list
TEST(TypedListReadWriteTest, ReadWriteShortASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<short>> testData{
    {3},
    {-3, 4, 32767, -32768, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< short>> testDataASCII = plyIn.getListProperty< short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteShortBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector< short>> testData{
    {3},
    {-3, 4, 32767, -32768, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< short>> testDataBinary = plyIn.getListProperty< short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned short list
TEST(TypedListReadWriteTest, ReadWriteUShortASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned short>> testData{
    {3},
    {3, 0, 11, 65535, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned short>> testDataASCII = plyIn.getListProperty<unsigned short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteUShortBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned short>> testData{
    {3},
    {3, 0, 11, 65535, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned short>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned short>> testDataBinary = plyIn.getListProperty<unsigned short>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}


// = signed int list
TEST(TypedListReadWriteTest, ReadWriteIntASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<int>> testData{
    {3},
    {-3, 0, 2147483647, -2147483648, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< int>> testDataASCII = plyIn.getListProperty< int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteIntBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector< int>> testData{
    {3},
    {-3, 0, 2147483647, -2147483648, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty< int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector< int>> testDataBinary = plyIn.getListProperty< int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = unsigned int list
TEST(TypedListReadWriteTest, ReadWriteUIntASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned int>> testData{
    {3},
    {3, 0, 4294967295, 15, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned int>> testDataASCII = plyIn.getListProperty<unsigned int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteUIntBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<unsigned int>> testData{
    {3},
    {3, 0, 4294967295, 15, 122},
    {},
    {},
    {3, 11},
  };
  plyOut.getElement("test_elem").addListProperty<unsigned int>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<unsigned int>> testDataBinary = plyIn.getListProperty<unsigned int>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}


// = float list
TEST(TypedListReadWriteTest, ReadWriteFloatASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<float>> testData{
    {3., 14.44, 42.4242},
    {3.141592653589793238, -3.141592653589793238, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::epsilon(), 0.0, -0.0, 1e24,},
    {},
    {1.1},
    {-121.5, 1.111},
  };
  plyOut.getElement("test_elem").addListProperty<float>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<float>> testDataASCII = plyIn.getListProperty<float>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteFloatBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<float>> testData{
    {3., 14.44, 42.4242},
    {3.141592653589793238, -3.141592653589793238, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::epsilon(), 0.0, -0.0, 1e24,},
    {},
    {1.1},
    {-121.5, 1.111},
  };
  plyOut.getElement("test_elem").addListProperty<float>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<float>> testDataBinary = plyIn.getListProperty<float>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// = double list
TEST(TypedListReadWriteTest, ReadWriteDoubleASCII) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<double>> testData{
    {3., 14.44, 42.4242},
    {3.141592653589793238, -3.141592653589793238, std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::epsilon(), 0.0, -0.0, 1e24,},
    {},
    {1.1},
    {-121.5, 1.111},
  };
  plyOut.getElement("test_elem").addListProperty<double>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::ASCII);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<double>> testDataASCII = plyIn.getListProperty<double>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataASCII);
}
TEST(TypedListReadWriteTest, ReadWriteDoubleBinary) {

  // Create a simple file
  happly::PLYData plyOut;
  plyOut.addElement("test_elem", 5);
  std::vector<std::vector<double>> testData{
    {3., 14.44, 42.4242},
    {3.141592653589793238, -3.141592653589793238, std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::epsilon(), 0.0, -0.0, 1e24,},
    {},
    {1.1},
    {-121.5, 1.111},
  };
  plyOut.getElement("test_elem").addListProperty<double>("test_data", testData);

  // ASCII read/write
  plyOut.write("temp.ply", happly::DataFormat::Binary);
  happly::PLYData plyIn("temp.ply");
  std::vector<std::vector<double>> testDataBinary = plyIn.getListProperty<double>("test_elem", "test_data");
  EXPECT_EQ(testData, testDataBinary);
}

// === Test error and utility behavior

// Errors get thrown

// Removal

// Type promotion

// === Test reading mesh-like files
TEST(MeshTest, ReadWriteASCIIMesh) {

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

TEST(MeshTest, ReadWriteBinaryMesh) {

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
  plyOut.write("temp.ply", happly::DataFormat::Binary);


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
