# Happly
![](https://travis-ci.com/nmwsharp/happly.svg?branch=master)
A header-only C++ reader/writer for the PLY file format. Parse `.ply` happily!

### Features:
- Header only-- drop in and use!
- Read and write to plaintext and binary variants of format with same API!
- Supports general data in `.ply` files, along with common-case helpers for reading/writing mesh data!
- Automatic type promotion-- eg, if a file contains a `float` field, you can seamlessly read it as a `double`!
- Tested, documented, and MIT-licensed!

## The `.ply` format and Happly

The `.ply` format is a general-purpose flat file format useful for recording numerical data on unstructured domains, which includes both plaintext and binary representations. The format has been kicking around since the 90s: [Paul Bourke's webpage](http://paulbourke.net/dataformats/ply/) serves as both an introduction and the most official specification. [Happly](https://github.com/nmwsharp/happly) grew out of my own personal code for `.ply` data-- the format is extremely useful for working with 3D meshes and other geometric data, but no easily accessible C++ implementation was available.

Although the `.ply` format is commonly used to store 3D mesh and point cloud data, the format itself technically has nothing to do with meshes or point clouds; it simply specifies a collection **elements**, and data (called **properties**) associated with those elements.  For instance in a mesh, the elements are vertices and faces; vertices then have properties like "position" and "color", while faces have a property which is a list of vertex indices. Happly exposes a general API for reading and writing elements and properties, as well as special-purpose helpers for the common conventions surrounding mesh data.

## API

All of the outward-facing functionality of Happly is grouped under a single (namespaced) class called `happly::PLYData`.


## Examples

## Known issues:
- Writing floating-point values of `inf` or `nan` in ASCII mode is not supported, because C++'s ofstream and ifstream do not treat them consistently. They work just fine in binary mode.
- The `.ply` file format allows binary files to be big-endian or little-endian; Happly only handles little-endian files.


## Current TODOs:
- Add more common-case helpers for meshes (texture coordinates, etc)
- Add common-case helpers for point clouds
- Bindings for Python, Matlab?
