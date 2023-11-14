/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/
#include "golInitCells.h"
#include "golAction.h"
#include <string>
#include <iostream>
#include <typeinfo>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

namespace gol {

  // Code here with class GolTests is for debugging whilst developing the code, and is unrelated to the final output of the project
  /*=============================================================================
  void GolTests::PrintInitiateCells()
  { 
    auto onecell = Cells(1,1);
    std::cout << onecell.PrintCells() << std::endl;
    auto grid = Cells(5,5);
    std::cout << grid.PrintCells() << std::endl;
    grid.SetAll("o");
    // std::cout << grid.PrintCells() + "\n"<< std::endl;
    grid.SetCell(2, 3, "-");
    grid.SetCell(1, 3, "-");
    grid.SetCell(3, 3, "-");
    grid.SetCell(4, 2, "-");
    grid.SetCell(4, 3, "-");
    std::cout << grid.PrintCells() + "\n" + grid.GetCell(1,3) + "\n" + grid.GetCell(1,4)<< std::endl;
    grid.SetAll("2");
    std::cout << grid.PrintCells() << std::endl;
    std::vector<std::vector<std::string>> customCells{
          {"-", "o", "-"},
          {"o", "-", "o"},
          {"-", "o", "-"}
      };
    Cells customGrid(customCells);
  }

  void GolTests::RandomiseCells()
  {
    // auto grid = Cells(3,3,2);
    // std::cout << grid.PrintCells() + "\n"<< std::endl;
    // std::cout << "Alive: "+std::to_string(grid.CountAlive()) + "\n"<< std::endl;
    // std::cout << "Dead: "+std::to_string(grid.CountDead()) + "\n"<< std::endl;
    auto grid1 = gol::Cells(5,5,5);
    auto grid2 = gol::Cells(5,5,5);
    std::cout << "Grid 1: \n" + grid1.PrintCells() + "\n"<< std::endl;
    std::cout << "Grid 2: \n" + grid2.PrintCells() + "\n"<< std::endl;
    std::cout << "Two grids are the same? " + std::to_string(grid1.PrintCells() == grid2.PrintCells()) << std::endl;
    auto grid3 = Cells(3,3,2);
    auto grid4 = grid3;
    std::cout << "Grid 3: \n" + grid3.PrintCells() + "\n"<< std::endl;
    std::cout << "Grid 4: \n" + grid4.PrintCells() + "\n"<< std::endl;
    std::cout << "Two grids are the same? " + std::to_string(grid3.PrintCells() == grid4.PrintCells()) << std::endl;
  }


  void GolTests::PrintFileCells(std::string filename)
  { 
    auto grid = gol::Cells(filename);
    std::cout << "Trial grid: \n" + grid.PrintCells() + "\nend"<< std::endl;
  }
  =============================================================================*/



  // initiating the cell grid from class Cells
  // note: The user index for cell grids starts from 1, rather than the traditional 0.
  GolActionGrid::GolActionGrid(int row, int col) : Cells(row, col)
  {
    current_grid = Cells::GetGrid();
  }


  GolActionGrid::GolActionGrid(int row, int col, int num_alive) : Cells(row, col, num_alive)
  {
    current_grid = Cells::GetGrid();
  }


  GolActionGrid::GolActionGrid(const std::string filename) : Cells(filename)
  {
    current_grid = Cells::GetGrid();
  }

  void GolActionGrid::GolSet(int row_index, int col_index, std::string status) 
  {
    if(status == "o" || status == "-")
    {
      current_grid[row_index-1][col_index-1] = status;
    }
    else
    {
      throw std::logic_error("Undefined cell status. Cell should either be dead (-) or alive (o).");
    }
  }

  std::vector<std::vector<std::string>> GolActionGrid::GetCurrentGrid()
  {
    return current_grid;
  }

  std::vector<std::vector<std::string>> GolActionGrid::GetNextGrid()
  {
    return next_grid;
  }

  // adding a delimiter for clarity
  std::string GolActionGrid::AddDelimiter()
  { 
    std::string delimiter = "";
    delimiter += "\n";
    for (int i = 0; i < NumCols()+5 ; i++) // delimiter is a line of "=", with a length 5 more than the number of columns of the cell grid
    {
      delimiter += "=";
    }
    delimiter += "\n";
    return delimiter;
  }

  // similar to PrintCells() function in class Cells
  // but adding a delimiter for clarity
  std::string GolActionGrid::PrintGrid() //called when new iteration occurs
  { 
    std::string return_string = "";
    return_string += AddDelimiter();

    auto grid_to_print = Cells(current_grid); // printing out the grid being updated, which is current_grid
    return_string += grid_to_print.PrintCells();

    return_string += AddDelimiter();

    return return_string;
  }

  // Taking steps on the cell grid according to the rules in Conway's Game of Life
  void GolActionGrid::TakeStep(bool show_debug) 
  { 
    std::string deadcell = "-";
    std::string alivecell = "o";
    std::vector<int> rulelist;
    int gen_count = 0;

    //Indication of starting iteration
    // std::cout << "START" << AddDelimiter() << std::endl;
    // std::cout << PrintCells() << AddDelimiter()<< std::endl;

    for(int row = 1; row < NumRows()+1; row++)
    {
      for(int col = 1; col < NumCols()+1; col++)
      {
        int num_nearest_alive = GetNearestAlive(row,col); // getting number of nearest neighbours which are alive

        //Rule One: A dead cell with exactly three live neighbours should become a live cell.

        if (GetCell(row,col) == deadcell) 
        { 
          if (GetNearestAlive(row,col) == 3)
          {
            GolSet(row, col, "o");
            if(show_debug)
            {
              rulelist.push_back(1);
              std::cout << "Rule 1 at " << row << "," << col;
              std::cout << PrintGrid() << std::endl;
            }
          }
        }

        else if (GetCell(row,col) == alivecell)
        { 
          //Rule Two: A live cell with two or three live neighbours should stay alive.
          if (GetNearestAlive(row,col) == 2 || GetNearestAlive(row,col) == 3)
          {
            GolSet(row, col, "o");
            if(show_debug)
            {
              rulelist.push_back(2);
              std::cout << "Rule 2 at " << row << "," << col;
              std::cout << PrintGrid() << std::endl;
            }
          }

          //Rule Three: A live cell with less than two or more than three live neighbours should die.
          else if (GetNearestAlive(row,col) < 2 || GetNearestAlive(row,col) > 3)
          {
            GolSet(row, col, "-");
            if(show_debug)
            {
              rulelist.push_back(3);
              std::cout << "Rule 3 at " << row << "," << col;
              std::cout << PrintGrid() << std::endl;
            }
          }
        }
      }
    }

    //list of rules applied if show_debug is true:
    if (show_debug)
    {
      for(auto &rule : rulelist)
      {
        std::cout << rule << " ";
      }
      std::cout << std::endl;
    }

    //Update grid: rewriting cell_grid by using current_grid
    auto grid_copy = GetGrid();
    for(int row = 1; row < NumRows()+1; row++)
    {
      for(int col = 1; col < NumCols()+1; col++)
      { 
        std::string status = current_grid[row-1][col-1]; // the updated grid
        // auto prev_grid = GetGrid();
        SetCell(row, col, status); //updates the original grid
        // next_grid = GetGrid(); 
      }
    }
    std::cout << PrintGrid() << std::endl;
  }

  // Sleeper function to delay the output on the screen so we can see the simulation in real time
  void Sleeper(int sec)
  {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
  }

  // Updates the cell grid for num_gen many times, according to rules in the Game of Life
  void GolActionGrid::Update(int num_gen)
  { 
    Sleeper(1);
    std::cout  << "Iteration START" << AddDelimiter();
    std::cout << PrintCells() << AddDelimiter()<< std::endl;
    for(int i = 0; i < num_gen; i++)
    { 
      Sleeper(1);
      std::cout << "Step " << i+1 << std::endl;
      TakeStep(false);
    }
    std::cout << "Iteration END" << std::endl;
  }
}// end namespace
