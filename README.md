# happly
A header-only parser for the PLY file format

**WORK IN PROGRESS.**

As of 3/14/18, supports basic reading from ASCII/Binary, no writing, and is very unpolished.

## About the .ply format

## API

## Examples

## Known issues:
- Writing floating-point values of `inf` or `nan` in ASCII mode is not supported, because C++'s ofstream and ifstream do not treat them consistently. They work just fine in binary mode.
