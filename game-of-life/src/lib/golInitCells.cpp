/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <string>
#include <iostream>
#include <memory>
#include <exception>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include "golInitCells.h"

namespace gol {

  // initialising a 2d grid of dead cells with specified number of rows and columns
  Cells::Cells(int row_num, int col_num) : num_rows(row_num), num_cols(col_num), size({row_num, col_num}) 
  {
    cell_grid = std::vector<std::vector<std::string>>(num_rows, std::vector<std::string>(num_cols, "-"));
  }
  
  // takes a custom 2d custom grid and initialises it as type Cells
  // contents within custom_grid must be dead (-) or alive (o)
  Cells::Cells(std::vector<std::vector<std::string>> custom_grid)
  { 
    cell_grid = custom_grid;
    num_rows = cell_grid.size();
    num_cols = cell_grid[0].size(); 
    size={num_rows, num_cols};
  }

  // printing the cell 
  std::string Cells::PrintCells()
  {
    std::string cellstring = "";
    for(auto &row : cell_grid)
    {
      for(auto &cell: row)
      {
        cellstring += cell;
      }
      cellstring += "\n";
    }
    cellstring.pop_back(); // removing the "\n" at the end of the cellstring
    return cellstring;
  }

   // initialising a 2d grid of dead cells with specified number of rows and columns,
   // with specified number of alive cells generated at random positions
  Cells::Cells(int row_num, int col_num, int num_alive) : Cells(row_num, col_num)
  { 
    auto total_cells = row_num * col_num;
    if (num_alive > total_cells)
    { 
      auto text_num_alive = std::to_string(num_alive);
      auto text_total_cells = std::to_string(total_cells);
      throw std::logic_error("Number of requested alive cells (" + text_num_alive + ") exceed number of total cells (" + text_total_cells + ")");
    }

    // random seeds for generating random integers
    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution<> rand_row_index(1, row_num);
    std::uniform_int_distribution<> rand_col_index(1, col_num);

    int loops = 0;
    while (loops < num_alive)
    { 
      int row_index = rand_row_index(seed);
      int col_index = rand_col_index(seed);

      if( GetCell(row_index,col_index) == "-") //only turns dead cells into alive cells
      {
        SetCell(row_index, col_index, "o");
        loops++;
      }
    }
  }

  // initialsing the cell grid from a file
  Cells::Cells(const std::string filename)
  {
    //opening the file
    std::ifstream file;
    file.open(filename);
    if(file.is_open() == false)
    { 
      throw std::runtime_error("Failed to open " + filename + ". Check if file exists");
    }
    if(file.peek() == std::ifstream::traits_type::eof()) //checking if file is empty, closes if file is empty
    { 
      std::cout<< "Closing file: " + filename <<std::endl;
      file.close();
      throw std::runtime_error("Empty file: " + filename);
    }
    std::cout << "File is opened: " + filename << std::endl;

    //reading the file
    std::string row_string;
    std::string deadcell = "-";
    std::string alivecell = "o";
    std::string whitespace = " ";
    std::string nextline = "\n";
    std::string carriagereturn = "\r"; // return key word when starting a new line of writing, exists and found in files


    int check_row_numbers;
    int rowcount = 0;

    while( std::getline(file, row_string))
    { 
      std::vector<std::string> row_vector = {}; 
      for (auto cell : row_string)
      { 
        std::string cell_elem(1,cell);

        if(cell_elem == deadcell)
        { 
          // std::cout << "Dead!" << std::endl;
          row_vector.push_back(deadcell);
        }

        else if(cell_elem == alivecell )
        { 
          // std::cout << "Alive!" << std::endl;
          row_vector.push_back(alivecell);
        }

        //do not take in the element to initialise the cell grid if the element in the file is a whitespace, \n or \r
        else if (cell_elem == whitespace || cell_elem == nextline || cell_elem == carriagereturn)
        { 
          // std::cout << "whitespace or change line" << std::endl;
          continue;
        }

        else
        { 
          file.close();
          std::cout << "Closing file: " + filename << std::endl;
          std::string exception_msg = "Cells should either be dead (-) or alive (o). Invalid cell input with ASCII number: " + std::to_string(int(cell));
          throw std::logic_error(exception_msg);
        }
      }
      cell_grid.push_back(row_vector);
    }

    // checking uneven rows and columns
    bool first = false;
    for (auto &row : cell_grid)
    { 
      int rowcount = 0;

      for (auto &cell : row)
      {
        if (cell == deadcell || cell == alivecell)
        {
          rowcount++;
        }
      }
      
      if (first == false)
      {
        check_row_numbers = rowcount; //storing number of row elements of the first row with check_row_numbers
        first = true; // "flips the switch", so check_row_numbers will always store the number of elements of the first row and will not be overwritten
      }

      if (first)
      {
        if(rowcount == 0) // if there are no elements in that column, rowcount will be zero, hence an empty column, close file due to invalidity
        { 
          file.close();
          std::cout << "Closing file: " + filename << std::endl;
          throw std::logic_error("Empty column. Number of column elements should be the same.");
        }

        // std::cout<<check_row_numbers << "\t " << rowcount <<std::endl;
        if (check_row_numbers != rowcount) // if the number of first row elements is not equal to that of other row elements, close file due to invalidity
        {
          file.close();
          std::cout << "Closing file: " + filename << std::endl;
          throw std::logic_error("Uneven number of row elements. Row elements should be the same.");
        }
      }
    }
    
    num_rows = cell_grid.size();
    num_cols = cell_grid[0].size();
    size = {num_rows,num_cols};

  //closing the file
    if(file.is_open()) 
    {
      file.close();
      std::cout << "File is closed: " + filename << std::endl;
    }
  }

  int Cells::NumRows()
  {
    return num_rows;
  }

  int Cells::NumCols()
  {
    return num_cols;
  }

  std::vector<int> Cells::Size()
  { 
    return size;
  }

  std::vector<std::vector<std::string>> Cells::GetGrid()
  {
    return cell_grid;
  }

  //comparing one cell grid to the other
  bool Cells::IsSame(Cells othergrid)
  { 
    // comparison of sizes
    if (size != othergrid.size)
    {
      return false;
    }

    else
    {
      for (int m = 1; m < num_rows+1; m++) {
        for (int n = 1; n < num_cols+1; n++) {
            if (GetCell(m, n) != othergrid.GetCell(m, n)) {
                return false;
            }
        }
    }
    return true;
    }
  }

  bool Cells::IsDead(const int row_index, const int col_index)
  {
    return (cell_grid[row_index-1][col_index-1] == "-" ? 1:0);
  }

  bool Cells::IsAlive(const int row_index, const int col_index)
  {
    return (cell_grid[row_index-1][col_index-1] == "o" ? 1:0);
  }

  void Cells::SetAll(std::string status)
  { 
    if(status == "o" || status == "-")
    {
      for(auto &row : cell_grid)
      {
        for(auto &cell: row)
        {
          cell = status;
        }
      }
    }
    else
    {
      throw std::logic_error("Undefined cell status. Cell should either be dead (-) or alive (o).");
    }
    
    std::cout << std::endl;
  }
  
  // note: The user index for cell grids starts from 1, rather than the traditional 0.
  // gets the status of individual cell contents
  std::string Cells::GetCell(int row_index, int col_index)
  { 
    return cell_grid[row_index-1][col_index-1];
  }

  // note: The user index for cell grids starts from 1, rather than the traditional 0.
  // sets individual cell contents
  void Cells::SetCell(int row_index, int col_index, std::string status) 
  {
    if(status == "o" || status == "-")
    {
      cell_grid[row_index-1][col_index-1] = status;
    }
    else
    {
      throw std::logic_error("Undefined cell status. Cell should either be dead (-) or alive (o).");
    }
  }

  // counts number of dead cells in a grid
  int Cells::CountDead()
  {
    int dead_num = 0;
    for(auto &row : cell_grid)
    {
      for(auto &cell: row)
      {
        if (cell == "-")
        {
          dead_num ++;
        }
      }
      
    }
    return dead_num;
  }

  // counts number of alive cells in a grid
  int Cells::CountAlive()
  {
    int live_num = 0;
    for(auto &row : cell_grid)
      {
        for(auto &cell: row)
        {
          if (cell == "o")
          {
            live_num ++;
          }
        }
      }
    return live_num;
  }

  // gets the number of nearest neighbour of an individual cell in the cell grid
  int Cells::GetNearestAlive(int row, int col)
  { 
    int num_nearest_alive = 0;
    if (row <= 0 || col <= 0 )
    {
      throw std::logic_error("Only integers larger than 0 are allowed");
    }
    else if (row > num_rows || col > num_cols)
    {
      throw std::logic_error("Index exceeded. Grid is of size: " + std::to_string(num_rows) + " times " + std::to_string(num_cols) );
    }
    // neighbour locations in the following order:
    // top, bottom, right, left, top right, top left, bottom right, bottom left (total 8 nearest neighbours)
    const std::vector<std::vector<int>> neighbour_vector = {{row+1 , col}, // top
                                                            {row-1 , col}, // bottom
                                                            {row , col+1}, // right
                                                            {row , col-1}, // left
                                                            {row+1,col+1}, // top right
                                                            {row+1,col-1}, // top left
                                                            {row-1,col+1}, // bottom right
                                                            {row-1,col-1}};// bottom left 

    for ( auto &position : neighbour_vector)
    { 
      int row_index = position[0];
      int col_index = position[1];

      //disregarding edge cases to avoid segmentation errors

      if (row_index < 1 || col_index < 1) //top cases and left-most cases
      {
        continue;
      }

      else if (row_index > num_rows || col_index > num_cols) //bottom cases and right-most cases
      {
        continue;
      }

      else
      {
        if (IsAlive(row_index,col_index))
        {
          num_nearest_alive++;
        }
      }
    }
    return num_nearest_alive;
  }
} // end namespace


