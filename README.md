# Soukaku

## About
Soukaku is a Sudoku puzzle solver. Soukaku will solve any puzzle by using a brute force algorithm called backtracing.

## Dependencies
- [CMake](https://cmake.org)

### Windows Dependencies 
- Visual Studio Community 2022
- [Desktop development with C++](https://imgur.com/a/visual-studio-community-2019-installer-c-options-7zs51IT) in Visual Studio installer

## Installation
You will need to use cmake to create the build scripts, you can download cmake [here](https://cmake.org) or use a package manager.

First clone the repo. And execute the following commands in the repo.
```
mkdir build
cmake ..
```

This will create the build scripts and you can just use make to compile the code. If you are on windows go to [Windows Details](#windows-details).
```
make
```

### Windows Details
If you are on Windows by default it will create a Visual Studio Solution. Open up `Developer Command Prompt for VS 2022` by searching that in the start menu. The run the following command in the build directory you should have made.
```
msbuild soukaku.sln /p:Configuration=Release
```
This will compile the Visual Studio Solution and put the executable in `release/`

## Usage
The command syntax is the following.
```
soukaku <PUZZLE FILE>
```

The Sudoku puzzle file format should be formatted as it is bellow.
```
030|050|407
100|040|002
900|070|000
---+---+---
600|008|003
000|000|970
090|002|060
---+---+---
005|020|006
300|001|004
000|000|000
``` 
A zero means it's just a empty cell.

There is a puzzle file already provided in the base directory of this repo.

Once you have your puzzle file ready, you give that to SouKaku and Soukaku will solve it for you.
```
soukaku puzzle.txt
```
```
2 3 8 | 1 5 6 | 4 9 7
1 5 7 | 8 4 9 | 6 3 2
9 6 4 | 2 7 3 | 1 8 5
------+-------+------
6 7 1 | 5 9 8 | 2 4 3
5 8 2 | 3 6 4 | 9 7 1
4 9 3 | 7 1 2 | 5 6 8
------+-------+------
8 4 5 | 9 2 7 | 3 1 6
3 2 9 | 6 8 1 | 7 5 4
7 1 6 | 4 3 5 | 8 2 9

Solution Found
Solve Time: 0us
Cycles: 174262
```

The following is a solve puzzle of the puzzle file showed in the puzzle file.