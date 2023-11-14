/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef golMyFunctions_h
#define golMyFunctions_h

/**
* \file golMyFunctions.h
* \brief Various Utilities.
* \ingroup utilities
*/

#include <string>
#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <golInitCells.h>
namespace gol
{

// class GolTests // for debugging, unrelated to the project
// { 
//   public:

//   std::vector<std::vector<std::string>> InitiateCells();

//   void PrintInitiateCells();

//   void RandomiseCells();

//   void PrintFileCells(std::string);
// };


class GolActionGrid : public Cells
{ 

  //attributes
  private:
  std::vector<std::vector<std::string>> next_grid;

  std::vector<std::vector<std::string>> current_grid;

  public:
  //instance to store the 2D grid of cells class from Part A
  GolActionGrid(int row, int col);


  GolActionGrid(int row, int col, int num_alive);


  GolActionGrid(const std::string filename);

  

  //methods

  std::vector<std::vector<std::string>> GetNextGrid();

  std::vector<std::vector<std::string>> GetCurrentGrid();

  void GolSet(int row_index, int col_index, std::string status);  

  std::string AddDelimiter();

  std::string PrintGrid();

  void TakeStep(bool show_debug);

  void Update(int num_gen);

};


void Sleeper(int sec);

} // end namespace

#endif
