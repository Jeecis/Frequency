# Frequency
Linux CLI tool to count character frequency in c++

## Build

```sh
g++ -std=c++20 -o frequency main.cpp
```

## Usage
Build the program yourself or download it from a release

```sh
./frequency -i dancing_men.txt -l -o dancing_men.tsv
```

```
-i  mandatory input file name
-o  optional output file name
-l  optional flag to lowercase all symbols in .txt file before counting
```