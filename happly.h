#pragma once

/* A header-only implementation of the .ply file format.
 * https://github.com/nmwsharp/happly
 * By Nicholas Sharp - nsharp@cs.cmu.edu
 */

#include <array>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace happly {

enum class DataFormat { ASCII, Binary };

// Type names
// clang-format off
template <typename T> std::string typeName() { return "unknown"; }
template<> inline std::string typeName<char>() { return "char"; }
template<> inline std::string typeName<unsigned char>() { return "uchar"; }
template<> inline std::string typeName<short>() { return "short"; }
template<> inline std::string typeName<unsigned short>() { return "ushort"; }
template<> inline std::string typeName<int>() { return "int"; }
template<> inline std::string typeName<unsigned int>() { return "uint"; }
template<> inline std::string typeName<float>() { return "float"; }
template<> inline std::string typeName<double>() { return "double"; }
// clang-format on


class Property {

public:
  Property(std::string name_) : name(name_){};
  virtual ~Property(){};

  std::string name;

  // ASCII parsing
  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) = 0;

  // binary copying
  virtual void readNext(std::ifstream& stream) = 0;


  virtual void writeHeader(std::ofstream& outStream) = 0;
  virtual void writeDataASCII(std::ofstream& outStream, size_t iElement) = 0;
  virtual void writeDataBinary(std::ofstream& outStream, size_t iElement) = 0;


  virtual size_t size() = 0;
  virtual std::string propertyTypeName() = 0;
};

template <class T>
class TypedProperty : public Property {

public:
  TypedProperty(std::string name_) : Property(name_){};
  TypedProperty(std::string name_, std::vector<T>& data_) : Property(name_), data(data_){};
  virtual ~TypedProperty() override{};

  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) override {
    T val;
    std::istringstream iss(tokens[currEntry]);
    iss >> val;
    data.push_back(val);
    currEntry++;
  };

  virtual void readNext(std::ifstream& stream) override {
    T val;
    stream.read((char*)&val, sizeof(T));
    data.push_back(val);
  }

  virtual void writeHeader(std::ofstream& outStream) override {
    outStream << "property " << typeName<T>() << " " << name << "\n";
  }

  virtual void writeDataASCII(std::ofstream& outStream, size_t iElement) override {
    outStream.precision(std::numeric_limits<T>::max_digits10);
    outStream << data[iElement];
  }

  virtual void writeDataBinary(std::ofstream& outStream, size_t iElement) override {
    outStream.write((char*)&data[iElement], sizeof(T));
  }

  virtual size_t size() override { return data.size(); }

  virtual std::string propertyTypeName() override { return typeName<T>(); }

  std::vector<T> data;
};

// outstream doesn't do what we want with chars
template <>
inline void TypedProperty<unsigned char>::writeDataASCII(std::ofstream& outStream, size_t iElement) {
  outStream << (int)data[iElement];
}
template <>
inline void TypedProperty<char>::writeDataASCII(std::ofstream& outStream, size_t iElement) {
  outStream << (int)data[iElement];
}
template <>
inline void TypedProperty<unsigned char>::parseNext(std::vector<std::string>& tokens, size_t& currEntry) {
  std::istringstream iss(tokens[currEntry]);
  int intVal;
  iss >> intVal;
  data.push_back((unsigned char)intVal);
  currEntry++;
}
template <>
inline void TypedProperty<char>::parseNext(std::vector<std::string>& tokens, size_t& currEntry) {
  std::istringstream iss(tokens[currEntry]);
  int intVal;
  iss >> intVal;
  data.push_back((char)intVal);
  currEntry++;
}


template <class T>
class TypedListProperty : public Property {

public:
  TypedListProperty(std::string name_, int listCountBytes_) : Property(name_), listCountBytes(listCountBytes_){};
  TypedListProperty(std::string name_, std::vector<std::vector<T>>& data_) : Property(name_), data(data_){};

  virtual ~TypedListProperty() override{};

  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) override {

    std::istringstream iss(tokens[currEntry]);
    size_t count;
    iss >> count;
    currEntry++;

    std::vector<T> thisVec;
    for (size_t iCount = 0; iCount < count; iCount++) {
      std::istringstream iss(tokens[currEntry]);
      T val;
      iss >> val;
      thisVec.push_back(val);
      currEntry++;
    }
    data.push_back(thisVec);
  }

  virtual void readNext(std::ifstream& stream) override {

    // Read the size of the list
    size_t count = 0;
    stream.read(((char*)&count), listCountBytes);

    // Read list elements
    std::vector<T> thisVec;
    for (size_t iCount = 0; iCount < count; iCount++) {
      T val;
      stream.read((char*)&val, sizeof(T));
      thisVec.push_back(val);
    }
    data.push_back(thisVec);
  }

  virtual void writeHeader(std::ofstream& outStream) override {
    // NOTE: We ALWAYS use int as the list count output type
    outStream << "property list uint " << typeName<T>() << " " << name << "\n";
  }

  virtual void writeDataASCII(std::ofstream& outStream, size_t iElement) override {
    std::vector<T>& elemList = data[iElement];
    outStream << elemList.size();
    outStream.precision(std::numeric_limits<T>::max_digits10);
    for (size_t iEntry = 0; iEntry < elemList.size(); iEntry++) {
      outStream << " " << elemList[iEntry];
    }
  }

  virtual void writeDataBinary(std::ofstream& outStream, size_t iElement) override {
    std::vector<T>& elemList = data[iElement];
    unsigned int count = elemList.size();
    outStream.write((char*)&count, sizeof(int));
    for (size_t iEntry = 0; iEntry < elemList.size(); iEntry++) {
      outStream.write((char*)&elemList[iEntry], sizeof(T));
    }
  }

  virtual size_t size() override { return data.size(); }
  virtual std::string propertyTypeName() override { return typeName<T>(); }

  std::vector<std::vector<T>> data;
  int listCountBytes = -1;
};

// outstream doesn't do what we want with chars
template <>
inline void TypedListProperty<unsigned char>::writeDataASCII(std::ofstream& outStream, size_t iElement) {
  std::vector<unsigned char>& elemList = data[iElement];
  outStream << elemList.size();
  outStream.precision(std::numeric_limits<unsigned char>::max_digits10);
  for (size_t iEntry = 0; iEntry < elemList.size(); iEntry++) {
    outStream << " " << (int)elemList[iEntry];
  }
}
template <>
inline void TypedListProperty<char>::writeDataASCII(std::ofstream& outStream, size_t iElement) {
  std::vector<char>& elemList = data[iElement];
  outStream << elemList.size();
  outStream.precision(std::numeric_limits<char>::max_digits10);
  for (size_t iEntry = 0; iEntry < elemList.size(); iEntry++) {
    outStream << " " << (int)elemList[iEntry];
  }
}
template <>
inline void TypedListProperty<unsigned char>::parseNext(std::vector<std::string>& tokens, size_t& currEntry) {

  std::istringstream iss(tokens[currEntry]);
  size_t count;
  iss >> count;
  currEntry++;

  std::vector<unsigned char> thisVec;
  for (size_t iCount = 0; iCount < count; iCount++) {
    std::istringstream iss(tokens[currEntry]);
    int intVal;
    iss >> intVal;
    thisVec.push_back((unsigned char)intVal);
    currEntry++;
  }
  data.push_back(thisVec);
}
template <>
inline void TypedListProperty<char>::parseNext(std::vector<std::string>& tokens, size_t& currEntry) {

  std::istringstream iss(tokens[currEntry]);
  size_t count;
  iss >> count;
  currEntry++;

  std::vector<char> thisVec;
  for (size_t iCount = 0; iCount < count; iCount++) {
    std::istringstream iss(tokens[currEntry]);
    int intVal;
    iss >> intVal;
    thisVec.push_back((char)intVal);
    currEntry++;
  }
  data.push_back(thisVec);
}

inline std::unique_ptr<Property> createPropertyWithType(std::string name, std::string typeStr, bool isList,
                                                        std::string listCountTypeStr) {

  // == Figure out how many bytes the list count field has, if this is a list type
  // Note: some files seem to use signed types here, we read the width but always parse as if unsigned
  int listCountBytes = -1;
  if (isList) {
    if (listCountTypeStr == "uchar" || listCountTypeStr == "uint8" || listCountTypeStr == "char" ||
        listCountTypeStr == "int8") {
      listCountBytes = 1;
    } else if (listCountTypeStr == "ushort" || listCountTypeStr == "uint16" || listCountTypeStr == "short" ||
               listCountTypeStr == "int16") {
      listCountBytes = 2;
    } else if (listCountTypeStr == "uint" || listCountTypeStr == "uint32" || listCountTypeStr == "int" ||
               listCountTypeStr == "int32") {
      listCountBytes = 4;
    } else {
      throw std::runtime_error("Unrecognized list count type: " + listCountTypeStr);
    }
  }

  // = Unsigned int

  // 8 bit unsigned
  if (typeStr == "uchar" || typeStr == "uint8") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<unsigned char>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<unsigned char>(name));
    }
  }

  // 16 bit unsigned
  else if (typeStr == "ushort" || typeStr == "uint16") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<unsigned short>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<unsigned short>(name));
    }
  }

  // 32 bit unsigned
  else if (typeStr == "uint" || typeStr == "uint32") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<unsigned int>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<unsigned int>(name));
    }
  }

  // = Signed int

  // 8 bit signed
  if (typeStr == "char" || typeStr == "int8") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<char>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<char>(name));
    }
  }

  // 16 bit signed
  else if (typeStr == "short" || typeStr == "int16") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<short>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<short>(name));
    }
  }

  // 32 bit signed
  else if (typeStr == "int" || typeStr == "int32") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<int>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<int>(name));
    }
  }

  // = Float

  // 32 bit float
  else if (typeStr == "float" || typeStr == "float32") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<float>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<float>(name));
    }
  }

  // 64 bit float
  else if (typeStr == "double" || typeStr == "float64") {
    if (isList) {
      return std::unique_ptr<Property>(new TypedListProperty<double>(name, listCountBytes));
    } else {
      return std::unique_ptr<Property>(new TypedProperty<double>(name));
    }
  }

  else {
    throw std::runtime_error("Unrecognized data type: " + typeStr);
  }
}

class Element {

public:
  Element(std::string name_, size_t count_) : name(name_), count(count_) {}

  std::string name;
  size_t count;
  std::vector<std::unique_ptr<Property>> properties;

  std::unique_ptr<Property>& getProperty(std::string target) {
    for (std::unique_ptr<Property>& prop : properties) {
      if (prop->name == target) {
        return prop;
      }
    }
    throw std::runtime_error("PLY parser: element " + name + " does not have property " + target);
  }

  template <class T>
  void addProperty(std::string propertyName, std::vector<T>& data, bool removeExisting = true) {

    if (data.size() != count) {
      throw std::runtime_error("PLY write: new property " + propertyName + " has size which does not match element");
    }

    // If there is already some property with this name, remove it
    if (removeExisting) {
      for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->name == propertyName) {
          properties.erase(properties.begin() + i);
          i--;
        }
      }
    }

    properties.push_back(std::unique_ptr<Property>(new TypedProperty<T>(propertyName, data)));
  }

  template <class T>
  void addListProperty(std::string propertyName, std::vector<std::vector<T>>& data, bool removeExisting = true) {

    if (data.size() != count) {
      throw std::runtime_error("PLY write: new property " + propertyName + " has size which does not match element");
    }

    // If there is already some property with this name, remove it
    if (removeExisting) {
      for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->name == propertyName) {
          properties.erase(properties.begin() + i);
          i--;
        }
      }
    }

    properties.push_back(std::unique_ptr<Property>(new TypedListProperty<T>(propertyName, data)));
  }

  void validate() {

    // Make sure no properties have duplicate names, and no names have whitespace
    for (size_t iP = 0; iP < properties.size(); iP++) {
      for (char c : properties[iP]->name) {
        if (std::isspace(c)) {
          throw std::runtime_error("Ply validate: illegal whitespace in name " + properties[iP]->name);
        }
      }
      for (size_t jP = iP + 1; jP < properties.size(); jP++) {
        if (properties[iP]->name == properties[jP]->name) {
          throw std::runtime_error("Ply validate: multiple properties with name " + properties[iP]->name);
        }
      }
    }

    // Make sure all properties have right length
    for (size_t iP = 0; iP < properties.size(); iP++) {
      if (properties[iP]->size() != count) {
        throw std::runtime_error("Ply validate: property has wrong size. " + properties[iP]->name +
                                 " does not match element size.");
      }
    }
  }

  void writeHeader(std::ofstream& outStream) {

    outStream << "element " << name << " " << count << "\n";

    for (std::unique_ptr<Property>& p : properties) {
      p->writeHeader(outStream);
    }
  }

  void writeDataASCII(std::ofstream& outStream) {
    // Question: what is the proper output for an element with no properties? Here, we write a blank line, so there is
    // one line per element no matter what.
    for (size_t iE = 0; iE < count; iE++) {
      for (size_t iP = 0; iP < properties.size(); iP++) {
        properties[iP]->writeDataASCII(outStream, iE);
        if (iP < properties.size() - 1) {
          outStream << " ";
        }
      }
      outStream << "\n";
    }
  }


  void writeDataBinary(std::ofstream& outStream) {
    for (size_t iE = 0; iE < count; iE++) {
      for (size_t iP = 0; iP < properties.size(); iP++) {
        properties[iP]->writeDataBinary(outStream, iE);
      }
    }
  }
};

// Some string helpers
namespace {

inline std::string trimSpaces(std::string input) {
  size_t start = 0;
  while (start < input.size() && input[start] == ' ') start++;
  size_t end = input.size();
  while (end > start && (input[end - 1] == ' ' || input[end - 1] == '\n' || input[end - 1] == '\r')) end--;
  return input.substr(start, end - start);
}

inline std::vector<std::string> tokenSplit(std::string input) {
  std::vector<std::string> result;
  size_t curr = 0;
  size_t found = 0;
  while ((found = input.find_first_of(' ', curr)) != std::string::npos) {
    std::string token = input.substr(curr, found - curr);
    token = trimSpaces(token);
    if (token.size() > 0) {
      result.push_back(token);
    }
    curr = found + 1;
  }
  std::string token = input.substr(curr);
  token = trimSpaces(token);
  if (token.size() > 0) {
    result.push_back(token);
  }

  return result;
}

inline bool startsWith(std::string input, std::string query) { return input.compare(0, query.length(), query) == 0; }
}; // namespace

// Template hackery that makes getProperty<T>() and friends pretty while automatically picking up smaller types
namespace {

// A pointer for the smaller equivalent of a type (eg. when a double is requested a float works too, etc)
// clang-format off
template <class T> struct HalfSize { bool isSmaller = false; typedef T type; };
template <> struct HalfSize<int64_t > { bool isSmaller = true; typedef int32_t  type; };
template <> struct HalfSize<int32_t > { bool isSmaller = true; typedef int16_t  type; };
template <> struct HalfSize<int16_t > { bool isSmaller = true; typedef int8_t   type; };
template <> struct HalfSize<uint64_t> { bool isSmaller = true; typedef uint32_t type; };
template <> struct HalfSize<uint32_t> { bool isSmaller = true; typedef uint16_t type; };
template <> struct HalfSize<uint16_t> { bool isSmaller = true; typedef uint8_t  type; };
template <> struct HalfSize<double  > { bool isSmaller = true; typedef float    type; };
// clang-format on
} // namespace


class PLYData {

public:
  PLYData(){};

  // Read from file
  PLYData(std::string filename, bool verbose = false) {

    using std::cout;
    using std::endl;
    using std::string;
    using std::vector;

    if (verbose) cout << "PLY parser: Reading ply file: " << filename << endl;

    // Open a file in binary always, in case it turns out to have binary data.
    std::ifstream inStream(filename, std::ios::binary);
    if (inStream.fail()) {
      throw std::runtime_error("PLY parser: Could not open file " + filename);
    }


    // == Process the header
    parseHeader(inStream, verbose);


    // === Parse data from a binary file
    if (inputDataFormat == DataFormat::Binary) {
      parseBinary(inStream, verbose);
    }
    // === Parse data from an ASCII file
    else if (inputDataFormat == DataFormat::ASCII) {
      parseASCII(inStream, verbose);
    }


    if (verbose) {
      cout << "  - Finished parsing file." << endl;
    }
  }

  void validate() {

    for (size_t iE = 0; iE < elements.size(); iE++) {
      for (char c : elements[iE].name) {
        if (std::isspace(c)) {
          throw std::runtime_error("Ply validate: illegal whitespace in element name " + elements[iE].name);
        }
      }
      for (size_t jE = iE + 1; jE < elements.size(); jE++) {
        if (elements[iE].name == elements[jE].name) {
          throw std::runtime_error("Ply validate: duplcate element name " + elements[iE].name);
        }
      }
    }

    // Do a quick validation sanity check
    for (Element& e : elements) {
      e.validate();
    }
  }

  void write(std::string filename, DataFormat format = DataFormat::ASCII) {
    outputDataFormat = format;

    validate();

    // Open stream for writing
    std::ofstream outStream(filename, std::ios::out | std::ios::binary);
    if (!outStream.good()) {
      throw std::runtime_error("Ply writer: Could not open output file " + filename + " for writing");
    }

    writeHeader(outStream);

    // Write all elements
    for (Element& e : elements) {
      if (outputDataFormat == DataFormat::Binary) {
        e.writeDataBinary(outStream);
      } else if (outputDataFormat == DataFormat::ASCII) {
        e.writeDataASCII(outStream);
      }
    }
  }

  // ===  Helpers
  Element& getElement(std::string target) {
    for (Element& e : elements) {
      if (e.name == target) return e;
    }
    throw std::runtime_error("PLY parser: no element with name: " + target);
  }
  bool hasElement(std::string target) {
    for (Element& e : elements) {
      if (e.name == target) return true;
    }
    return false;
  }
  void addElement(std::string name, size_t count) { elements.emplace_back(name, count); }

  // === Get data out of the representation
  template <class T>
  std::vector<T> getProperty(std::string elementName, std::string propertyName) {

    // Find the property
    std::unique_ptr<Property>& prop = getElement(elementName).getProperty(propertyName);

    // Get a copy of the data with auto-promoting type magic
    return getDataFromPropertyRecursive<T, T>(prop.get());
  }

  template <class T>
  std::vector<std::vector<T>> getListProperty(std::string elementName, std::string propertyName) {

    // Find the property
    std::unique_ptr<Property>& prop = getElement(elementName).getProperty(propertyName);

    // Get a copy of the data with auto-promoting type magic
    return getDataFromListPropertyRecursive<T, T>(prop.get());
  }


  // === Common-case helpers
  std::vector<std::array<double, 3>> getVertexPositions(std::string vertexElementName = "vertex") {

    std::vector<double> xPos = getProperty<double>(vertexElementName, "x");
    std::vector<double> yPos = getProperty<double>(vertexElementName, "y");
    std::vector<double> zPos = getProperty<double>(vertexElementName, "z");

    std::vector<std::array<double, 3>> result(xPos.size());
    for (size_t i = 0; i < result.size(); i++) {
      result[i][0] = xPos[i];
      result[i][1] = yPos[i];
      result[i][2] = zPos[i];
    }

    return result;
  }

  std::vector<std::array<unsigned char, 3>> getVertexColors(std::string vertexElementName = "vertex") {

    std::vector<unsigned char> r = getProperty<unsigned char>(vertexElementName, "red");
    std::vector<unsigned char> g = getProperty<unsigned char>(vertexElementName, "green");
    std::vector<unsigned char> b = getProperty<unsigned char>(vertexElementName, "blue");

    std::vector<std::array<unsigned char, 3>> result(r.size());
    for (size_t i = 0; i < result.size(); i++) {
      result[i][0] = r[i];
      result[i][1] = g[i];
      result[i][2] = b[i];
    }

    return result;
  }

  std::vector<std::vector<size_t>> getFaceIndices();


  // Creates a vertex element (if doesn't already exist) and sets positions
  void addVertexPositions(std::vector<std::array<double, 3>>& vertexPositions) {

    std::string vertexName = "vertex";
    size_t N = vertexPositions.size();

    // Create the element
    if (!hasElement(vertexName)) {
      addElement(vertexName, N);
    }

    // De-interleave
    std::vector<double> xPos(N);
    std::vector<double> yPos(N);
    std::vector<double> zPos(N);
    for (size_t i = 0; i < vertexPositions.size(); i++) {
      xPos[i] = vertexPositions[i][0];
      yPos[i] = vertexPositions[i][1];
      zPos[i] = vertexPositions[i][2];
    }

    // Store
    getElement(vertexName).addProperty<double>("x", xPos);
    getElement(vertexName).addProperty<double>("y", yPos);
    getElement(vertexName).addProperty<double>("z", zPos);
  }

  // Create a vertex element (if doesn't already exist) and sets colors as uchars
  void addVertexColors(std::vector<std::array<double, 3>>& colors) {

    std::string vertexName = "vertex";
    size_t N = colors.size();

    // Create the element
    if (!hasElement(vertexName)) {
      addElement(vertexName, N);
    }

    auto toChar = [](double v) {
      if (v < 0.0) v = 0.0;
      if (v > 1.0) v = 1.0;
      return static_cast<unsigned char>(v * 255.);
    };

    // De-interleave
    std::vector<unsigned char> r(N);
    std::vector<unsigned char> g(N);
    std::vector<unsigned char> b(N);
    for (size_t i = 0; i < colors.size(); i++) {
      r[i] = toChar(colors[i][0]);
      g[i] = toChar(colors[i][1]);
      b[i] = toChar(colors[i][2]);
    }

    // Store
    getElement(vertexName).addProperty<unsigned char>("red", r);
    getElement(vertexName).addProperty<unsigned char>("green", g);
    getElement(vertexName).addProperty<unsigned char>("blue", b);
  }


  void addFaceIndices(std::vector<std::vector<size_t>>& indices) {

    std::string faceName = "face";
    size_t N = indices.size();

    // Create the element
    if (!hasElement(faceName)) {
      addElement(faceName, N);
    }

    // Shrink type to int
    std::vector<std::vector<int>> intInds;
    for (std::vector<size_t>& l : indices) {
      intInds.emplace_back(l.begin(), l.end());
    }

    // Store
    getElement(faceName).addListProperty<int>("vertex_indices", intInds);
  }


private:
  std::vector<Element> elements;
  std::vector<std::string> comments;
  float version = 1.0;

  DataFormat inputDataFormat = DataFormat::ASCII;  // set when reading from a file
  DataFormat outputDataFormat = DataFormat::ASCII; // option for writing files


  // === Reading ===

  void parseHeader(std::ifstream& inStream, bool verbose) {

    using std::cout;
    using std::endl;
    using std::string;
    using std::vector;

    // First two lines are predetermined
    { // First line is magic constant
      string plyLine;
      std::getline(inStream, plyLine);
      if (trimSpaces(plyLine) != "ply") {
        throw std::runtime_error("PLY parser: File does not appear to be ply file. First line should be 'ply'");
      }
    }

    { // second line is version
      string styleLine;
      std::getline(inStream, styleLine);
      vector<string> tokens = tokenSplit(styleLine);
      if (tokens.size() != 3) throw std::runtime_error("PLY parser: bad format line");
      std::string formatStr = tokens[0];
      std::string typeStr = tokens[1];
      std::string versionStr = tokens[2];

      // "format"
      if (formatStr != "format") throw std::runtime_error("PLY parser: bad format line");

      // ascii/binary
      if (typeStr == "ascii") {
        inputDataFormat = DataFormat::ASCII;
        if (verbose) cout << "  - Type: ascii" << endl;
      } else if (typeStr == "binary_little_endian") {
        inputDataFormat = DataFormat::Binary;
        if (verbose) cout << "  - Type: binary" << endl;
      } else if (typeStr == "binary_big_endian") {
        throw std::runtime_error("PLY parser: encountered scary big endian file. Don't know how to parse that");
      } else {
        throw std::runtime_error("PLY parser: bad format line");
      }

      // version
      version = std::atof(versionStr.c_str());
      if (verbose) cout << "  - Version: " << version << endl;
    }

    // Consume header line by line
    while (inStream.good()) {
      string line;
      std::getline(inStream, line);

      // Parse a comment
      if (startsWith(line, "comment")) {
        string comment = line.substr(7);
        if (verbose) cout << "  - Comment: " << comment << endl;
        comments.push_back(comment);
        continue;
      }

      // Parse an element
      else if (startsWith(line, "element")) {
        vector<string> tokens = tokenSplit(line);
        if (tokens.size() != 3) throw std::runtime_error("PLY parser: Invalid element line");
        string name = tokens[1];
        size_t count;
        std::istringstream iss(tokens[2]);
        iss >> count;
        elements.emplace_back(name, count);
        if (verbose) cout << "  - Found element: " << name << " (count = " << count << ")" << endl;
        continue;
      }

      // Parse a property list
      else if (startsWith(line, "property list")) {
        vector<string> tokens = tokenSplit(line);
        if (tokens.size() != 5) throw std::runtime_error("PLY parser: Invalid property list line");
        if (elements.size() == 0) throw std::runtime_error("PLY parser: Found property list without previous element");
        string countType = tokens[2];
        string type = tokens[3];
        string name = tokens[4];
        elements.back().properties.push_back(createPropertyWithType(name, type, true, countType));
        if (verbose)
          cout << "    - Found list property: " << name << " (count type = " << countType << ", data type = " << type
               << ")" << endl;
        continue;
      }

      // Parse a property
      else if (startsWith(line, "property")) {
        vector<string> tokens = tokenSplit(line);
        if (tokens.size() != 3) throw std::runtime_error("PLY parser: Invalid property line");
        if (elements.size() == 0) throw std::runtime_error("PLY parser: Found property without previous element");
        string type = tokens[1];
        string name = tokens[2];
        elements.back().properties.push_back(createPropertyWithType(name, type, false, ""));
        if (verbose) cout << "    - Found property: " << name << " (type = " << type << ")" << endl;
        continue;
      }

      // Parse end of header
      else if (startsWith(line, "end_header")) {
        break;
      }

      // Error!
      else {
        throw std::runtime_error("Unrecognized header line: " + line);
      }
    }
  }

  void parseASCII(std::ifstream& inStream, bool verbose) {

    using std::string;
    using std::vector;

    // Read all elements
    for (Element& elem : elements) {

      if (verbose) {
        std::cout << "  - Processing element: " << elem.name << std::endl;
      }

      for (size_t iEntry = 0; iEntry < elem.count; iEntry++) {

        string line;
        std::getline(inStream, line);

        vector<string> tokens = tokenSplit(line);
        size_t iTok = 0;
        for (size_t iP = 0; iP < elem.properties.size(); iP++) {
          elem.properties[iP]->parseNext(tokens, iTok);
        }
      }
    }
  }

  void parseBinary(std::ifstream& inStream, bool verbose) {

    using std::string;
    using std::vector;

    // Read all elements
    for (Element& elem : elements) {

      if (verbose) {
        std::cout << "  - Processing element: " << elem.name << std::endl;
      }

      for (size_t iEntry = 0; iEntry < elem.count; iEntry++) {
        for (size_t iP = 0; iP < elem.properties.size(); iP++) {
          elem.properties[iP]->readNext(inStream);
        }
      }
    }
  }

  // === Get Data ===

  template <class D, class T>
  std::vector<D> getDataFromPropertyRecursive(Property* prop) {

    { // Try to return data of type D from a property of type T
      TypedProperty<T>* castedProp = dynamic_cast<TypedProperty<T>*>(prop);
      if (castedProp) {
        // Succeeded, return a buffer of the data (copy while converting type)
        return std::vector<D>(castedProp->data.begin(), castedProp->data.end());
      }
    }

    HalfSize<T> halfType;
    if (halfType.isSmaller) {
      return getDataFromPropertyRecursive<D, typename HalfSize<T>::type>(prop);
    } else {
      // No smaller type to try, failure
      throw std::runtime_error("PLY parser: property " + prop->name +
                               " cannot be coerced to requested type. Has type " + prop->propertyTypeName());
    }
  }


  template <class D, class T>
  std::vector<std::vector<D>> getDataFromListPropertyRecursive(Property* prop) {

    TypedListProperty<T>* castedProp = dynamic_cast<TypedListProperty<T>*>(prop);
    if (castedProp) {
      // Succeeded, return a buffer of the data (copy while converting type)
      std::vector<std::vector<D>> result;
      for (std::vector<T>& subList : castedProp->data) {
        result.emplace_back(subList.begin(), subList.end());
      }
      return result;
    }

    HalfSize<T> halfType;
    if (halfType.isSmaller) {
      return getDataFromListPropertyRecursive<D, typename HalfSize<T>::type>(prop);
    } else {
      // No smaller type to try, failure
      throw std::runtime_error("PLY parser: list property " + prop->name + " cannot be coerced to requested type");
    }
  }


  // === Writing ===
  void writeHeader(std::ofstream& outStream) {

    // Magic line
    outStream << "ply\n";

    // Type line
    outStream << "format ";
    if (outputDataFormat == DataFormat::Binary) {
      outStream << "binary_little_endian ";
    } else if (outputDataFormat == DataFormat::ASCII) {
      outStream << "ascii ";
    }
    std::streamsize initPrecision = std::cout.precision();
    outStream << std::setprecision(1) << std::fixed << version << std::defaultfloat << std::setprecision(initPrecision)
              << "\n";

    // Write comments
    for (std::string& comment : comments) {
      outStream << "comment " << comment << "\n";
    }

    // Write elements (and their properties)
    for (Element& e : elements) {
      e.writeHeader(outStream);
    }

    // End header
    outStream << "end_header\n";
  }
};

// Specialization for size_t. It's useful to always look for indices as size_t, but some files store them with a signed
// type (usually int).
// This automatically handles that case.
template <>
inline std::vector<std::vector<size_t>> PLYData::getListProperty(std::string elementName, std::string propertyName) {

  // Find the property
  std::unique_ptr<Property>& prop = getElement(elementName).getProperty(propertyName);

  // Get a copy of the data with auto-promoting type magic
  try {
    return getDataFromListPropertyRecursive<size_t, size_t>(prop.get());
  } catch (std::runtime_error orig_e) {

    try {
      std::vector<std::vector<int>> intResult = getListProperty<int>(elementName, propertyName);

      // Check sign while copying
      std::vector<std::vector<size_t>> copiedResult;
      for (auto& list : intResult) {
        for (auto& val : list) {
          if (val < 0) {
            throw std::runtime_error("converted int is negative");
          }
        }
        copiedResult.emplace_back(list.begin(), list.end());
      }

      return copiedResult;
    } catch (std::runtime_error new_e) {
      throw orig_e;
    }

    throw orig_e;
  }
}


inline std::vector<std::vector<size_t>> PLYData::getFaceIndices() {
  for (std::string f : std::vector<std::string>{"face"}) {
    for (std::string p : std::vector<std::string>{"vertex_indices", "vertex_index"}) {
      try {
        return getListProperty<size_t>(f, p);
      } catch (std::runtime_error e) {
        // that's fine
      }
    }
  }
  throw std::runtime_error("PLY parser: could not find vertex indices attribute under any common name");
}
} // namespace happly
