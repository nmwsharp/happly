#Happly
![](https://travis-ci.com/nmwsharp/happly.svg?branch=master)

A header-only C++ reader/writer for the PLY file format. Parse `.ply` happily!

### Features:
- Header only-- drop in and use!
- Read and write to plaintext and binary variants of format with same API!
- Supports general data in `.ply` files, along with common-case helpers for reading/writing mesh data!
- Automatic type promotion-- eg, if a file contains a `float` field, you can seamlessly read it as a `double`!
- Tested, documented, and MIT-licensed!

## The `.ply` format and Happly

The `.ply` format is a general-purpose flat file format useful for recording numerical data on unstructured domains, which includes both plaintext and binary representations. The format has been kicking around since the 90s: [Paul Bourke's webpage](http://paulbourke.net/dataformats/ply/) serves as both an introduction and the most official specification. [Happly](https://github.com/nmwsharp/happly) grew out of my own personal code for `.ply` files-- the format is extremely useful for working with 3D meshes and other geometric data, but no easily accessible C++ implementation was available.

Although the `.ply` format is commonly used to store 3D mesh and point cloud data, the format itself technically has nothing to do with meshes or point clouds; it simply specifies a collection **elements**, and data (called **properties**) associated with those elements.  For instance in a mesh, the elements are vertices and faces; vertices then have properties like "position" and "color", while faces have a property which is a list of vertex indices. Happly exposes a general API for reading and writing elements and properties, as well as special-purpose helpers for the common conventions surrounding mesh data.

## Examples

Read basic data
```cpp
#include "happly.h"

// Construct a data object by reading from file
happly::PLYData plyIn("my_file.ply");

// Get data from the object
std::vector<float> elementA_prop1 = plyIn.getElement("elementA").getProperty<float>("prop1");
std::vector<int> elementA_prop2 = plyIn.getElement("elementA").getProperty<double>("prop1");
std::vector<std::vector<double>> elementB_listProp = plyIn.getElement("elementA").getListProperty<double>("listprop1");

// Type promotion is automatic for numeric types: even if this property was stored as a float, 
// we can access it as a double
std::vector<double> elementA_prop1_as_double = plyIn.getElement("elementA").getProperty<double>("prop1"); 
```

Write basic data
```cpp
#include "happly.h"

// Suppose these hold your data
std::vector<float> elementA_prop1;
std::vector<int> elementA_prop2;
std::vector<std::vector<double>> elementB_listProp;

// Create an empty object
happly::PLYData plyOut();

// Add elements
plyOut.addElement("elementA", 20);
plyOut.addElement("elementB", 20);

// Add properties to those elements
plyOut.getElement("elementA").addProperty<float>("prop1", elementA_prop1);
plyOut.getElement("elementA").addProperty<int>("prop2", elementA_prop2);
plyOut.getElement("elementB").addListProperty<double>("listprop1", elementB_listProp);

// Write the object to file
plyOut.write("my_output_file.ply", happly::DataFormat::Binary);

```

Read mesh-like data
```cpp
#include "happly.h"

// Construct the data object by reading from file
happly::PLYData plyIn("my_mesh_file.ply");

// Get mesh-style data from the object
std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices();
```

Write mesh-like data
```cpp
#include "happly.h"

// Suppose these hold your data
std::vector<std::array<double, 3>> meshVertexPositions;
std::vector<std::array<double, 3>> meshVertexColors;
std::vector<std::vector<size_t>> meshFaceIndices;

// Create an empty object
happly::PLYData plyOut();

// Add mesh data (elements are created automatically)
plyOut.addVertexPositions(meshVertexPositions);
plyOut.addVertexColors(meshVertexColors);
plyOut.addFaceIndices(fInd);


// Write the object to file
plyOut.write("my_output_mesh_file.ply", happly::DataFormat::ASCII);

```


## API

This assumes a basic familiarity with the file format; I suggest reading [Paul Bourke's webpage](http://paulbourke.net/dataformats/ply/) if you are new to `.ply`. 

All of the outward-facing functionality of Happly is grouped under a single (namespaced) class called `happly::PLYData`, which represents a collection of elements and their properties. `PLYData` objects can be constructed from an existing file `PLYData::PLYData("my_input.ply")`, or you can fill with your own data and then write to file `PLYData::write("my_output.ply", DataFormat::ASCII)`.

Generally speaking, Happly uses C++ exceptions to communicate errors-- most of these methods will throw if something is wrong. Happly attempts to provide basic sanity checks and informative errors, but does not guarantee robustness to malformed input.

**Reading and writing objects**:

- `PLYData()` Construct an empty PLYData containing no elements or properties.

- `PLYData(std::string filename, bool verbose = false)` Construct a new PLYData object from a file, automatically detecting whether the file is plaintext or binary. If `verbose=true`, useful information about the file will be printed to `stdout`.

- `PLYData::validate()` Perform some basic sanity checks on the object, throwing if any fail. Called internally before writing.

- `PLYData::write(std::string filename, DataFormat format = DataFormat::ASCII)` Write the object to file. Specifying `DataFormat::ASCII` or `DataFormat::Binary` controls the kind of output file.

**Accessing and adding data to an object**:

- `void addElement(std::string name, size_t count)` Add a new element type to the object, with the object, with a specified number of elements.

- `Element& getElement(std::string target)` Get a reference to an element type contained in the object.
  
- `bool hasElement(std::string target)` Check if an element type is contained in the object.

- `std::vector<T> Element::getProperty(std::string propertyName)` Get a vector of property data for an element. Will automatically promote types if possible, eg `getProperty<int>("my_prop")` will succeed even if the object contains "my_prop" with type `short`.

- `std::vector<std::vector<T>> Element::getListProperty(std::string propertyName)` Get a vector of list property data for an element. Supports type promotion just like `getProperty()`.


- `void Element::addProperty(std::string propertyName, std::vector<T>& data)` Add a new property to an element type. `data` must be the same length as the number of elements of that type.
  
- `void addListProperty(std::string propertyName, std::vector<std::vector<T>>& data)` Add a new list property to an element type. `data` must be the same length as the number of elements of that type.


**Common-case helpers for mesh data**:

- `std::vector<std::array<double, 3>> getVertexPositions(std::string vertexElementName = "vertex")` Returns x,y,z vertex positions from an object. `vertexElementName` specifies the name of the element type holding vertices, which is conventionally "vertex".
  
- `void addVertexPositions(std::vector<std::array<double, 3>>& vertexPositions)` Adds x,y,z vertex positions to an object, under the element name "vertex".

- `std::vector<std::array<unsigned char, 3>> getVertexColors(std::string vertexElementName = "vertex")` Returns r,g,b vertex colors from an object. `vertexElementName` specifies the name of the element type holding vertices, which is conventionally "vertex".

- `void addVertexColors(std::vector<std::array<unsigned char, 3>>& vertexColors)` Adds r,g,b vertex colors positions to an object, under the element name "vertex".

- `void addVertexColors(std::vector<std::array<double, 3>>& vertexColors)` Adds r,g,b vertex colors positions to an object, under the element name "vertex". Assumes input is in [0.0,1.0], and internally converts to 0-255 char values

- `std::vector<std::vector<size_t>> getFaceIndices()` Returns indices in to a vertex list for each face. Usually 0-indexed, but there are no formal rules in the format.

- `void addFaceIndices(std::vector<std::vector<size_t>>& indices)` Adds vertex indices for faces to an object, under the element name "face" with the property name "vertex_indices".


## Known issues:
- Writing floating-point values of `inf` or `nan` in ASCII mode is not supported, because C++'s ofstream and ifstream do not treat them consistently, and the .ply format does not specify how they should be written. They work just fine in binary mode.
- The `.ply` file format allows binary files to be big-endian or little-endian; Happly only explicitly supports little-endian files, and basically just assumes your machine is little-endian.


## Current TODOs:
- Add more common-case helpers for meshes (texture coordinates, etc)
- Add common-case helpers for point clouds
- Bindings for Python, Matlab?
