/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/
// #define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "golCatchMain.h"
#include "golInitCells.h"
#include "golAction.h"
#include <iostream>
#include <vector>
#include <stdexcept>


// Tests for part 1.1

TEST_CASE( "Check print method", "[1.1_print_cells]" ) 
{
  // comparing initiation of cells with the result of PrintCells() with correct strings
  REQUIRE( gol::Cells(2,3).PrintCells() == "---\n---" );
  REQUIRE( gol::Cells(4,4).PrintCells() == "----\n----\n----\n----" );
  REQUIRE( gol::Cells(8,8).PrintCells() == "--------\n--------\n--------\n--------\n--------\n--------\n--------\n--------" );
  REQUIRE( gol::Cells(3,2).PrintCells() == "--\n--\n--" );
  REQUIRE( gol::Cells(5,4).PrintCells() == "----\n----\n----\n----\n----" );
  REQUIRE( gol::Cells(1,1).PrintCells() == "-");
}

TEST_CASE( "Check SetAll method", "[1.1_SetAll_cells]" )
{
  // comparing initiation of cells with the result of PrintCells() with correct strings after setting cells to all alive and then setting all cells to dead
  auto grid = gol::Cells(6,4);

  REQUIRE( grid.PrintCells() == "----\n----\n----\n----\n----\n----" );

  grid.SetAll("o");

  REQUIRE( grid.PrintCells() == "oooo\noooo\noooo\noooo\noooo\noooo" );

  grid.SetAll("-");

  REQUIRE( grid.PrintCells() == "----\n----\n----\n----\n----\n----" );

}

TEST_CASE( "Check exceptions of SetAll method", "[1.1_SetAll_cells]" ) 
{
  // require program to throw a logic error if the SetAll method has an invalid input
  auto grid = gol::Cells(5,5);
 
  REQUIRE_THROWS_AS(grid.SetAll("2"), std::logic_error);

}

TEST_CASE( "Check set method", "[1.1_set_cells]" ) 
{ 
  // comparing cells which are set by the SetCell function with the correct string results
  auto grid = gol::Cells(5,5);

  grid.SetCell(2, 4, "o");
  grid.SetCell(2, 3, "o");
  grid.SetCell(1, 3, "o");
  grid.SetCell(5, 5, "-");
  REQUIRE( grid.PrintCells() == "--o--\n--oo-\n-----\n-----\n-----" );

  grid.SetAll("o");
  grid.SetCell(4, 2, "-");
  grid.SetCell(3, 1, "-");
  grid.SetCell(2, 3, "-");
  grid.SetCell(4, 4, "o");
  REQUIRE( grid.PrintCells() == "ooooo\noo-oo\n-oooo\no-ooo\nooooo" );
}

TEST_CASE( "Check exceptions of set method ", "[1.1_set_cells]" ) 
{ 
  // require program to throw a logic error if the SetCell method has an invalid input
  auto grid = gol::Cells(6,6);

  REQUIRE_THROWS_AS(grid.SetCell(3,6, "8"), std::logic_error);
}

TEST_CASE( "Check get method", "[1.1_get_cells]" ) 
{
  // comparing cell status by the GetCell function with the correct string results
  auto grid = gol::Cells(4,6);
  REQUIRE( grid.GetCell(2,3) == "-" );
  REQUIRE( grid.GetCell(4,6) == "-" );
  
  grid.SetCell(2,6,"o");
  grid.SetCell(3,1,"o");
  grid.SetCell(2,4,"o");
  REQUIRE( grid.GetCell(2,6) == "o" );
  REQUIRE( grid.GetCell(3,1) == "o" );
  REQUIRE( grid.GetCell(2,4) == "o" );
  REQUIRE( grid.GetCell(1,1) == "-" );
  REQUIRE( grid.GetCell(4,5) == "-" );
  
  grid.SetAll("o");
  grid.SetCell(1,2,"-");
  grid.SetCell(2,1,"-");
  grid.SetCell(3,3,"-");
  REQUIRE( grid.GetCell(1,2) == "-" );
  REQUIRE( grid.GetCell(2,1) == "-" );
  REQUIRE( grid.GetCell(3,3) == "-" );
  REQUIRE( grid.GetCell(4,4) == "o" );
  REQUIRE( grid.GetCell(3,5) == "o" );
}



// Tests for part 1.2

TEST_CASE( "Check CountAlive method", "[1.2_CountAlive]" ) 
{
  // comparing the integer results of the CountAlive method with the correct values  
  auto grid_set = gol::Cells(3,5);
  grid_set.SetCell(2,4,"o");
  grid_set.SetCell(1,5,"o");
  REQUIRE( grid_set.CountAlive() == 2);
  
  auto dead_grid = gol::Cells(6,8);
  REQUIRE( dead_grid.CountAlive() == 0);

  auto alive_grid = gol::Cells(11,20);
  alive_grid.SetAll("o");
  REQUIRE (alive_grid.CountAlive() == 220 );
  
  auto grid_set_two = gol::Cells(3,3);
  grid_set_two.SetCell(1,1,"o");
  grid_set_two.SetCell(2,2,"o");
  grid_set_two.SetCell(3,3,"o");
  REQUIRE( grid_set_two.CountAlive() == 3);
}
  
TEST_CASE( "Check CountDead method", "[1.2_CountDead]" ) 
{
  // comparing the integer results of the CountDead method with the correct values   
  auto grid_set = gol::Cells(6,7);
  grid_set.SetCell(6,6,"o");
  grid_set.SetCell(4,2,"o");
  grid_set.SetCell(2,1,"o");
  grid_set.SetCell(1,5,"o");
  REQUIRE( grid_set.CountDead() == 38);
  
  auto dead_grid = gol::Cells(6,8);
  REQUIRE( dead_grid.CountDead() == 48);

  auto alive_grid = gol::Cells(8,7);
  alive_grid.SetAll("o");
  REQUIRE (alive_grid.CountDead() == 0 );
  
  auto grid_set_two = gol::Cells(2,4);
  grid_set_two.SetCell(2,4,"o");
  grid_set_two.SetCell(1,3,"o");
  grid_set_two.SetCell(1,2,"o");
  REQUIRE( grid_set_two.CountDead() == 5);
}

TEST_CASE( "Check initiation of randomised alive cells", "[1.2_randomised_cells]" ) {

  // checking if cells with requested number of alive cells are initiated correctly 
  // by counting the number of alive cells in the grid and comparing it to the correct integer values

  // CountAlive case
  REQUIRE( gol::Cells(2,3,4).CountAlive() == 4 );
  REQUIRE( gol::Cells(5,7,0).CountAlive() == 0 );
  REQUIRE( gol::Cells(3,4,1).CountAlive() == 1 );
  REQUIRE( gol::Cells(7,7,49).CountAlive() == 49 );
  REQUIRE( gol::Cells(2,2,3).CountAlive() == 3 );
  
  // CountDead case
  REQUIRE( gol::Cells(5,7,2).CountDead() == 33 );
  REQUIRE( gol::Cells(2,2,4).CountDead() == 0 );
  REQUIRE( gol::Cells(3,5,0).CountDead() == 15 );
  REQUIRE( gol::Cells(2,4,4).CountDead() == 4 );
  REQUIRE( gol::Cells(4,3,10).CountDead() == 2 );
}

TEST_CASE( "Check exception thrown in the initiation of randomised alive cells", "[1.2_randomised_cells_exception]" )
{
  // require the function to throw a logic error if number of alive cells requested is larger than the total number of cells in the cell grid
  REQUIRE_THROWS_AS(gol::Cells(5,5,100), std::logic_error);
}

TEST_CASE( "Check the IsSame function", "[1.2_IsSame]" )
{  
  // checking if two cell grids are the same by initiating different instances but setting the cell contents in the same way

  // case one
  auto grid_set_one = gol::Cells(4,4);
  grid_set_one.SetCell(4,4,"o");
  grid_set_one.SetCell(1,1,"o");

  auto grid_set_two = gol::Cells(4,4);
  grid_set_two.SetCell(4,4,"o");
  grid_set_two.SetCell(1,1,"o");

  REQUIRE (grid_set_one.IsSame(grid_set_two) == true);
  REQUIRE (grid_set_two.IsSame(grid_set_one) == true);

  // case two
  auto grid_dead = gol::Cells(5,3);
  auto grid_other_dead = gol::Cells(5,3);

  REQUIRE (grid_dead.IsSame(grid_other_dead) == true);
  REQUIRE (grid_other_dead.IsSame(grid_dead) == true);
  
  // case three
  auto grid_alive = gol::Cells(4,6);
  grid_alive.SetAll("o");
  auto grid_other_alive = gol::Cells(4,6);
  grid_other_alive.SetAll("o");

  REQUIRE (grid_alive.IsSame(grid_other_alive) == true );
  REQUIRE (grid_other_alive.IsSame(grid_alive) == true);
}

TEST_CASE( "Randomised cell grids are not generated differently in different instances" , "[1.2_different_random_cells]")
 {
  // requiring randomised cell grids to be generated differently in different instances
  // case 1
  auto grid_one = gol::Cells(5,5,5);
  auto grid_one_other = gol::Cells(5,5,5);
  REQUIRE (grid_one.IsSame(grid_one_other) == false);

  // case 2
  auto grid_two = gol::Cells(6,4,12);
  auto grid_two_other = gol::Cells(6,4,12);
  REQUIRE (grid_two.IsSame(grid_two_other) == false);

  // case 3
  auto grid_three = gol::Cells(8,6,2);
  auto grid_three_other = gol::Cells(8,6,2);
  REQUIRE (grid_three.IsSame(grid_three_other) == false);
  
}

// test cases for section 1.3

TEST_CASE( "Check functionality of reading file", "[1.3_read_file]")
{

  // setting cell contents manually to match the expected outcome from input files
  
  // glider grid example
  auto glider_grid = gol::Cells("test/data/glider.txt"); // reading from file
  
  auto glider_compare = gol::Cells(10,10);
  glider_compare.SetCell(2,3,"o");
  glider_compare.SetCell(3,1,"o");
  glider_compare.SetCell(3,3,"o");
  glider_compare.SetCell(4,2,"o");
  glider_compare.SetCell(4,3,"o");
  REQUIRE(glider_compare.IsSame(glider_grid) == true);

  // case when cells are all dead
  auto alldead_grid = gol::Cells("test/data/testfiles/alldead.txt");
  auto alldead_grid_compare = gol::Cells(6,9);
  REQUIRE(alldead_grid .PrintCells() == alldead_grid_compare.PrintCells());

  // case when cells are all alive
  auto allalive_grid = gol::Cells("test/data/testfiles/allalive.txt");
  auto allalive_grid_compare = gol::Cells(4,3);
  allalive_grid_compare.SetAll("o");
  REQUIRE(allalive_grid.PrintCells() == allalive_grid_compare.PrintCells());

  // case of oscillators.txt
  auto oscillators_grid = gol::Cells("test/data/oscillators.txt").PrintCells();
  std::string oscillators_grid_compare = "------------\n--o-----ooo-\n--o---------\n--o---------\n------------\n-------ooo--\n--------ooo-\n-oo---------\n-oo---------\n---oo----o--\n---oo----o--\n---------o--";
  REQUIRE(oscillators_grid == oscillators_grid_compare);

}

TEST_CASE( "Check exception of reading non-exsistent file", "[1.3_read_non_existent_file]")
{
  // requires function to throw a runtime error if the file does not exist
  REQUIRE_THROWS_AS(gol::Cells("test/data/nonexsistent.txt"), std::runtime_error);

}

TEST_CASE( "Check exception of reading file with invalid contents", "[1.3_read_invalid_file]")
{
  // requires function to throw a logic error if the file has invalid contents
  REQUIRE_THROWS_AS(gol::Cells("test/data/testfiles/invalid.txt"), std::logic_error);
}

TEST_CASE( "Check exception of reading file with no contents", "[1.3_empty_file]")
{
  // requires function to throw a runtime error if the file is empty
  REQUIRE_THROWS_AS(gol::Cells("test/data/testfiles/empty.txt"), std::runtime_error);
}

TEST_CASE( "Check exception of reading file with uneven number of column elements", "[1.3_uneven_cols]")
{ 
  // requires function to throw a logic error if the cell grid in the file has uneven number of columns
  REQUIRE_THROWS_AS(gol::Cells("test/data/testfiles/uneven_cols.txt"), std::logic_error);
}

TEST_CASE( "Check exception of reading file with uneven number of row elements", "[1.3_uneven_rows]")
{
  // requires function to throw a logic error if the cell grid in the file has uneven number of rows
  REQUIRE_THROWS_AS(gol::Cells("test/data/testfiles/uneven_rows.txt"), std::logic_error);
}

// test cases for section 1.4

TEST_CASE( "Check exception of GetNearestNeighbour function when row and column number is less or equal than zero", "[1.4_NN_exception_le_zero]")
{ 
  // requires function to throw a logic error if the input row and column number is less than or equal to zero
  auto grid = gol::Cells("test/data/glider.txt");
  REQUIRE_THROWS_AS(grid.GetNearestAlive(0,-1), std::logic_error);
}

TEST_CASE( "Check exception of GetNearestNeighbour function when row and column number is greater than row and column number", "[1.4_NN_exception_greater_rowcol]")
{ 
  // requires function to throw a logic error if the input row and column number is greater than that of the cell grid
  auto grid = gol::Cells("test/data/testfiles/allalive.txt");
  REQUIRE_THROWS_AS(grid.GetNearestAlive(5,5), std::logic_error);
}

TEST_CASE( " Check GetNearestNeighbour function", "[1.4_NN]" )
{
  // requiring the GetNearestNeighbour function to output the correct number of nearest neighbours
  // the output of the function is compared to correct number of nearest neighbours, which are manually counted

  // glider case
  auto glider_grid = gol::Cells("test/data/glider.txt");
  REQUIRE(glider_grid.GetNearestAlive(3,2) == 5);
  REQUIRE(glider_grid.GetNearestAlive(3,1) == 1);
  REQUIRE(glider_grid.GetNearestAlive(3,3) == 3);

  // oscillator case
  auto oscillator_grid = gol::Cells("test/data/oscillators.txt");
  REQUIRE(oscillator_grid.GetNearestAlive(2,3) == 1);
  REQUIRE(oscillator_grid.GetNearestAlive(12,10) == 1);
  REQUIRE(oscillator_grid.GetNearestAlive(7,10) == 4);

  // all dead case
  auto alldead_grid = gol::Cells("test/data/testfiles/alldead.txt");
  REQUIRE(alldead_grid.GetNearestAlive(4,5) == 0);

  // all alive case
  auto allalive_grid = gol::Cells("test/data/testfiles/allalive.txt");
  REQUIRE(alldead_grid.GetNearestAlive(2,1) == 0);

  // edge cases
  auto edge_grid = gol::Cells("test/data/testfiles/edge.txt");
  REQUIRE(edge_grid.GetNearestAlive(1,1) == 2);
  REQUIRE(edge_grid.GetNearestAlive(1,6) == 1);
  REQUIRE(edge_grid.GetNearestAlive(4,1) == 0);
  REQUIRE(edge_grid.GetNearestAlive(4,6) == 1);
}

// tests for section 2.1

TEST_CASE( " Check initiation of cell grids from the GolActionGrid", "[2.1_init]" )
{
  // checking if cells grids initiated from GolActionGrid are the same as that of in class Cells

  // all dead cells
  REQUIRE(gol::Cells(4,4).PrintCells() == gol::GolActionGrid(4,4).PrintCells());
  REQUIRE(gol::Cells(5,6).PrintCells() == gol::GolActionGrid(5,6).PrintCells());
  REQUIRE(gol::Cells(1,3).PrintCells() == gol::GolActionGrid(1,3).PrintCells());
  
  // cases from files
  // glider case
  std::string glider_grid_file = "test/data/glider.txt";
  REQUIRE(gol::Cells(glider_grid_file).PrintCells() == gol::GolActionGrid(glider_grid_file).PrintCells());

  // oscillator case
  std::string oscillators_file = "test/data/oscillators.txt";
  REQUIRE(gol::Cells(oscillators_file).PrintCells() == gol::GolActionGrid(oscillators_file).PrintCells());

  // edge case
  std::string edge_file = "test/data/testfiles/edge.txt";
  REQUIRE(gol::Cells(edge_file).PrintCells() == gol::GolActionGrid(edge_file).PrintCells());
}


TEST_CASE( " Check initiation of implementation of TakeStep function ", "[2.1_TakeStep]" )
{
  // In this section, all *Result.txt files are self-derived to compare with results from the TakeStep function

  // test cases when rules one and two are implemented
  auto init_rules_onetwo = gol::GolActionGrid("test/data/testrules/RulesOneTwo.txt");
  auto rules_onetwo_result = gol::GolActionGrid("test/data/testrules/RulesOneTwoResult.txt");
  init_rules_onetwo.TakeStep(true);
  REQUIRE(init_rules_onetwo.PrintCells() == rules_onetwo_result.PrintCells());

  auto init_rules_two_one = gol::GolActionGrid("test/data/testrules/RulesTwoOne.txt");
  auto rules_two_one_result = gol::GolActionGrid("test/data/testrules/RulesTwoOneResult.txt");
  init_rules_two_one.TakeStep(true);
  REQUIRE(init_rules_two_one.PrintCells() == rules_two_one_result.PrintCells());

  // test cases when all rules one, two and three are implemented
  auto init_rules_all = gol::GolActionGrid("test/data/testrules/RulesTwoOne.txt");
  auto rules_all_result= gol::GolActionGrid("test/data/testrules/RulesTwoOneResult.txt");
  init_rules_all.TakeStep(true);
  REQUIRE(init_rules_all.PrintCells() == rules_all_result.PrintCells());

  // test case when no rules are implemented
  auto init_deadfile = gol::GolActionGrid("test/data/testfiles/alldead.txt");
  std::string alldead_result = gol::Cells(6,9).PrintCells();
  init_deadfile.TakeStep(true);
  REQUIRE(init_deadfile.PrintCells() == alldead_result);
}







