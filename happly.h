#pragma once

/* A header-only implementation of the .ply file format.
 * https://github.com/nmwsharp/happly
 * By Nicholas Sharp - nsharp@cs.cmu.edu
 */

#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace happly {

class Property {

public:
  Property(std::string name_) : name(name_){};

  std::string name;

  // ASCII parsing
  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) = 0;

  // binary copying
  virtual void readNext(std::ifstream& stream) = 0;
};

template <class T>
class TypedProperty : public Property {

public:
  TypedProperty(std::string name_) : Property(name_){};

  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) {
    T val;
    std::istringstream iss(tokens[currEntry]);
    iss >> val;
    data.push_back(val);
    currEntry++;
  };

  virtual void readNext(std::ifstream& stream) {
    T val;
    stream.read((char*)&val, sizeof(T));
    data.push_back(val);
  }

  std::vector<T> data;
};

template <class T>
class TypedListProperty : public Property {

public:
  TypedListProperty(std::string name_, int listCountBytes_) : Property(name_), listCountBytes(listCountBytes_){};

  virtual void parseNext(std::vector<std::string>& tokens, size_t& currEntry) {

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

  virtual void readNext(std::ifstream& stream) {

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

  std::vector<std::vector<T>> data;
  int listCountBytes;
};

inline std::shared_ptr<Property> createPropertyWithType(std::string name, std::string typeStr, bool isList,
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
      return std::shared_ptr<Property>(new TypedListProperty<uint8_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<uint8_t>(name));
    }
  }

  // 16 bit unsigned
  else if (typeStr == "ushort" || typeStr == "uint16") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<uint16_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<uint16_t>(name));
    }
  }

  // 32 bit unsigned
  else if (typeStr == "uint" || typeStr == "uint32") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<uint32_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<uint32_t>(name));
    }
  }

  // = Signed int

  // 8 bit signed
  if (typeStr == "char" || typeStr == "int8") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<int8_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<int8_t>(name));
    }
  }

  // 16 bit signed
  else if (typeStr == "short" || typeStr == "int16") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<int16_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<int16_t>(name));
    }
  }

  // 32 bit signed
  else if (typeStr == "int" || typeStr == "int32") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<int32_t>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<int32_t>(name));
    }
  }

  // = Float

  // 32 bit float
  else if (typeStr == "float" || typeStr == "float32") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<float>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<float>(name));
    }
  }

  // 64 bit float
  else if (typeStr == "double" || typeStr == "float64") {
    if (isList) {
      return std::shared_ptr<Property>(new TypedListProperty<double>(name, listCountBytes));
    } else {
      return std::shared_ptr<Property>(new TypedProperty<double>(name));
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
  std::vector<std::shared_ptr<Property>> properties;

  std::shared_ptr<Property> getProperty(std::string target) {
    for (std::shared_ptr<Property> prop : properties) {
      if (prop->name == target) {
        return prop;
      }
    }
    throw std::runtime_error("PLY parser: element " + name + " does not have property " + target);
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
};

// Template hackery that makes getProperty<T>() and friends pretty while automatically picking up smaller types
namespace {

// A pointer for the smaller equivalent of a type (eg. when a double is requested a float works too, etc)
template <class T>
struct HalfSize {
  bool isSmaller = false;
  typedef T type;
};

template <>
struct HalfSize<int64_t> {
  bool isSmaller = true;
  typedef int32_t type;
};
template <>
struct HalfSize<int32_t> {
  bool isSmaller = true;
  typedef int16_t type;
};
template <>
struct HalfSize<int16_t> {
  bool isSmaller = true;
  typedef int8_t type;
};
template <>
struct HalfSize<uint64_t> {
  bool isSmaller = true;
  typedef uint32_t type;
};
template <>
struct HalfSize<uint32_t> {
  bool isSmaller = true;
  typedef uint16_t type;
};
template <>
struct HalfSize<uint16_t> {
  bool isSmaller = true;
  typedef uint8_t type;
};
template <>
struct HalfSize<double> {
  bool isSmaller = true;
  typedef float type;
};
}


class PLYData {

public:
  PLYData();

  // Read from file
  PLYData(std::string filename, bool verbose = false) {

    using std::vector;
    using std::string;
    using std::cout;
    using std::endl;

    if (verbose) cout << "PLY parser: Reading ply file: " << filename << endl;

    // Open a file in binary always, in case it turns out to have binary data.
    std::ifstream inStream(filename, std::ios::binary);
    if (inStream.fail()) {
      throw std::runtime_error("PLY parser: Could not open file " + filename);
    }


    // == Process the header
    parseHeader(inStream, verbose);


    // === Parse data from a binary file
    if (isBinary) {
      parseBinary(inStream, verbose);
    }
    // === Parse data from an ASCII file
    else {
      parseASCII(inStream, verbose);
    }


    if (verbose) {
      cout << "  - Finished parsing file." << endl;
    }
  }


  void write(std::string filename, bool binary = true);

  // === Get data out of the representation
  template <class T>
  std::vector<T> getProperty(std::string elementName, std::string propertyName) {

    // Find the property
    std::shared_ptr<Property> prop = getElement(elementName).getProperty(propertyName);

    // Get a copy of the data with auto-promoting type magic
    return getDataFromPropertyRecursive<T, T>(prop.get());
  }

  template <class T>
  std::vector<std::vector<T>> getListProperty(std::string elementName, std::string propertyName) {

    // Find the property
    std::shared_ptr<Property> prop = getElement(elementName).getProperty(propertyName);

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

  std::vector<std::vector<size_t>> getFaceIndices(std::string faceElementName = "face",
                                                  std::string indexPropertyName = "vertex_indices");


private:
  std::vector<Element> elements;
  std::vector<std::string> comments;
  float version = 1.0;
  bool isBinary = false;

  // Helpers
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


  // Parsing helpers

  void parseHeader(std::ifstream& inStream, bool verbose) {

    using std::vector;
    using std::string;
    using std::cout;
    using std::endl;

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
        isBinary = false;
        if (verbose) cout << "  - Type: ascii" << endl;
      } else if (typeStr == "binary_little_endian") {
        isBinary = true;
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

    using std::vector;
    using std::string;

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

    using std::vector;
    using std::string;

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
      throw std::runtime_error("PLY parser: property " + prop->name + " cannot be coerced to requested type");
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
};

// Specialization for size_t. It's useful to always look for indices as size_t, but some files store them with a signed
// type (usually int).
// This automatically handles that case.
template <>
inline std::vector<std::vector<size_t>> PLYData::getListProperty(std::string elementName, std::string propertyName) {

  // Find the property
  std::shared_ptr<Property> prop = getElement(elementName).getProperty(propertyName);

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


inline std::vector<std::vector<size_t>> PLYData::getFaceIndices(std::string faceElementName,
                                                                std::string indexPropertyName) {
  return getListProperty<size_t>(faceElementName, indexPropertyName);
}
}
