# happly
![](https://travis-ci.com/nmwsharp/happly.svg?branch=master) A header-only C++ reader/writer for the PLY file format.

## The .ply format and Happly

The .ply format is a general-purpose flat file format useful for recording numerical data on unstructured domains. The format has been kicking around since the 90s, [Paul Bourke's webpage](http://paulbourke.net/dataformats/ply/) is the closest we have to an official specification.
The format includes both ASCII and binary represetations. Happly grew out of my own personal code for working with .ply data, although the format is extremely useful for working with 3D meshes and other geometric data, there did not seem to be an accessible C++ implementation.

Although the .ply format is commonly used to store 3D mesh and point cloud data, the format itself has nothing to do with meshes or point clouds; it simply specifies a collection **elements**, and data (called **properties**) associated with those elements.  For instance in a mesh, the elements are vertices and faces; vertices then have properties like "position" and "color", while faces have a property which is a list of vertex indices. Happly exposes a general API for reading and writing elements and properties, as well as special-purpose helpers for the common conventions surrounding mesh data.

## API

## Examples

## Known issues:
- Writing floating-point values of `inf` or `nan` in ASCII mode is not supported, because C++'s ofstream and ifstream do not treat them consistently. They work just fine in binary mode.
- The .ply file format allows binary files to be big-endian or little-endian; this library only handles little-endian files.
