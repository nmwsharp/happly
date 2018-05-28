# happly
![](https://travis-ci.com/nmwsharp/happly.svg?branch=master) A header-only C++ reader/writer for the PLY file format.

## About the .ply format

## API

## Examples

## Known issues:
- Writing floating-point values of `inf` or `nan` in ASCII mode is not supported, because C++'s ofstream and ifstream do not treat them consistently. They work just fine in binary mode.
