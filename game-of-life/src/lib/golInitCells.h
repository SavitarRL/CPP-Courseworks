/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef golBasicTypes_h
#define golBasicTypes_h

/**
* \defgroup internal internal
* \brief Internal stuff, not for end-users.
*/

/**
* \defgroup types types
* \brief Package-wide data types.
*/

/**
* \defgroup utilities utilities
* \brief Groups of c-style functions.
*/

/**
* \defgroup applications applications
* \brief Small, end-user applications, most likely command line.
*/

/**
* \file golBasicTypes.h
* \brief Defines types and typedefs used in this library.
* \ingroup types
*/

//! Single namespace for all code in this package

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <exception>
#include <vector>
namespace gol
{

class Cells 
{ 
  // attributes
  private: 
    int num_rows;
    int num_cols;
    std::vector<int> size;

  protected:
  std::vector<std::vector<std::string>> cell_grid;

  // methods
  public:
    Cells(int row_num, int col_num);

    Cells(int row_num, int col_num, int num_alive);

    Cells(const std::string filename);

    Cells(std::vector<std::vector<std::string>> custom_grid);

    int NumRows();

    int NumCols();

    std::vector<int> Size();

    std::vector<std::vector<std::string>> GetGrid();

    bool IsSame(const Cells);

    bool IsDead(const int row_index, const int col_index);

    bool IsAlive(const int row_index, const int col_index);

    std::string PrintCells();

    void SetAll(std::string status);
    
    std::string GetCell(int row_index, int col_index);
    
    void SetCell(int row_index, int col_index, std::string status);
    
    int CountDead();

    int CountAlive();

    int GetNearestAlive(int row, int col);
};

} // end namespace

#endif
