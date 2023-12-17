[comment]: <> ([![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-f4981d0f882b2a3f0472912d15f9806d57e124e0fc890972558857b51b24a6f9.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=9991287)
PHAS0100ASSIGNMENT1)
------------------

Purpose
-------

This project serves as a starting point for the PHAS0100 2022/23 Assignment 1 Game of Life Simulation coursework. It has a reasonable folder structure for [CMake](https://cmake.org/) based projects that use [CTest](https://cmake.org/) to run unit tests via [Catch](https://github.com/catchorg/Catch2). 

Credits
---------
---------
This project is maintained by [Dr. Jamie Quinn](http://jamiejquinn.com/). It is based on [CMakeCatch2](https://github.com/UCL/CMakeCatch2.git) that was originally developed as a teaching aid for UCL's ["Research Computing with C++"](https://github-pages.ucl.ac.uk/research-computing-with-cpp/) course developed by [Dr. James Hetherington](http://www.ucl.ac.uk/research-it-services/people/james) and [Dr. Matt Clarkson](https://iris.ucl.ac.uk/iris/browse/profile?upi=MJCLA42).
-----------------
-----------------

This assignment is done and completed by Pak To Lau Ryan 

(Student Number: 19070595)

Build Instructions
------------------

To run cmake:

```
cmake -B build
```

To compile:

```
cmake --build build
```

Additional build and running instructions will be shown below.

Brief Description and User Instructions
----------------
----------------
## Section 1  to Section 2.1:
## Data Structure for 2D grid of cells and Implementation of the Game of Life
### Unit Tests
All unit tests are written within *test/golTests.cpp*

Tests for each section (e.g. section 1.1) are indicated within this file for clarity.

To run tests for code regarding this section:
```
./build/bin/golTests
```
Known error: Even if run with filters, for example:
```
./build/bin/golTests [1.2_IsSame]
```
Unit tests will run altogether.
### Code Implementation
For section 1, code is written in *src/lib/golInitCells.cpp* and *src/lib/golInitCells.h*

For section 2.1, code is written in *src/lib/golInitCells.cpp* and *src/lib/golAction.cpp*

The user index for cell grids starts from 1, rather than the traditional 0.

## Section 2.2: Command-Line Application

### Code Implementation

For section 2.2 onwards, code is written in *src/app/gol_simulator.cpp*

To get instructions on how to run the command-line application:
```
./build/bin/golSimulator -h
```
This will print a help message on how to run the application and one may follow it accordingly. The help message is as follows:
```
Usage: ./build/bin/golSimulator [-h] [--help] [-f <filename> <num_gen>] [-r <row_num> <col_num> <num_alive> <num_gen>]
                                [-ssl  <row_num> <col_num> <num_alive> <num_gen> <num_init> <min_alive>]

Options:
Commands                                                                Description

-h | --help                                                             Shows this help message
-f | --file <filename> <num_gen>                                        Choosing the text file input and looping through a specified number of steps.
-r | --rand <row_num> <col_num> <num_alive> <num_gen>                   Initialising a random cell grid of specified 
                                                                        rows and columns, and a chosen number of alive cells generated at random positions, with required number of generations to simulate 
-sls  <row_num> <col_num> <num_alive> <num_gen> <num_init> <min_alive>  Searching for still lifes. A random cell grid is generated, with specified
                                                                        rows and columns and a chosen number of alive cells generated at random positions.
                                                                        The program will generate <num_init> times of the random cell grid. 
                                                                        There will be a specified, minimum number of alive cells in the grid
                                                                        Within the required number of generations, the application will print out, for each random configuration:
                                                                        2. The resulting still life pattern
                                                                        3. The number of steps required to generate the final still life pattern from the initial cell grid

Arguments are separated by a single whitespace.

Variable explanations:

        <filename>       name of the text file (e.g. "test/data/glider.txt") 
        <row_num>        number of rows in the cell grid (e.g. 5)
        <col_num>        number of columns in the cell grid (e.g. 6)
        <num_alive>      number of live cells in the cell grid, with randomly generated positions (e.g. 7)
        <num_gen>        number of generations, or steps of cellular automata required (e.g. 7)
        <min_alive>      minumum number of alive cells present in the cell grid
        <num_init>       number of random cell grids to be tried to find still life patterns

Example inputs:

        -f test/data/input.txt 4         Reading from text file named "test/data/input.txt", with 4 steps of generation
        -r 6 4 10 6                      Initialising a random cell grid of 6 rows and 4 columns,
                                         with 10 alive cells generated at random positions, and with 6 steps of generation
        -h                               Showing this help message
        -sls 6 4 6 9 5                   Initialising a random cell grid of 6 rows and 4 columns,
                                         with 6 alive cells generated at random positions, and with 9 steps of generation.
                                         5 different configurations of the cell grid will be run to find still life patterns.
                                         Still lives will be found and printed on the screen.

If there are no inputs, the help message will be shown
```

## Section 2.3: Running the Application
All screenshots are contained within the *Screenshots* directory

Other than results, the application will also print out the steps taken.
### 1. 
We are running with the *glider.txt* input file with 4 evolutions. Run the following to get the result:
```
./build/bin/golSimulator -f test/data/glider.txt 4
```
See below for the screenshot of the result:

![Glider Result](./Screenshots/GOLGliderResult_(4_evol).jpg)

### 2. 
We are running with a 7 by 7 grid with random initial cell values, with 15 cells alive and run with 4 evolutions. Run the following to get the result
```
./build/bin/golSimulator -r 4 4 15 4
```
See below for the screenshot of the whole run and result for two different runs:

![4x4_15alive_4evol_one](./Screenshots/GOL_4x4_15alive_(4_evol)_one.jpg)
![4x4_15alive_4evol_two](./Screenshots/GOL_4x4_15alive_(4_evol)_two.jpg)

## Section 3: Finding Still Lives
Again for user instructions, run the following to get the help message for commands used in this section:
```
./build/bin/golSimulator -h
```
<!-- For example, we can run the following to get the result:
```
./build/bin/golSimulator -f test/data/glider.txt 4
``` -->
<!-- See below for the screenshot of the result: -->
### 3d: Finding still lives of a 4 by 4 grid of cells
See the following subsection for the results and screenshots of each commands I have run. Note that as random cell configurations are initiated, results may vary.

#### Case 1
<!-- case 1 -->
For the following command, <ins>**1** still life is found</ins>.
```
./build/bin/golSimulator -sls 4 4 4 4 4 4
```
![sls case1](./Screenshots/GOL_4x4_SLS_(4_4_4_4_4_4).jpg)

#### Case 2
<!-- case 2 -->
For the following command, <ins>**3** still lives are found</ins>.
```
./build/bin/golSimulator -sls 4 4 10 7 4 6
```
![sls case2](./Screenshots/GOL_4x4_SLS_(4_4_10_7_4_6).jpg)

#### Case 3
<!-- case 3 -->
For the following command, <ins>**1** still life is found</ins>.
```
./build/bin/golSimulator -sls 4 4 7 3 3 7
```
![sls case3](./Screenshots/GOL_4x4_SLS_(4_4_7_3_3_7).jpg)

#### Case 4
<!-- case 4 -->
For the following command, <ins>**no** still lives are found</ins>.
```
./build/bin/golSimulator -sls 4 4 3 3 1 6
```
![sls case1](./Screenshots/GOL_4x4_SLS_(4_4_3_3_1_6).jpg)



