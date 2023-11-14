/*=============================================================================

  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <golAction.h>
#include <golExceptionMacro.h>
#include <iostream>
#include <random>


// void Debug()
// {
//   // gol::GolTests initCells; 
//   // // initCells.PrintInitiateCells();
//   // // initCells.RandomiseCells();
//   // // initCells.PrintFileCells("test/data/testfiles/alldead.txt");

//   // auto allrulesgrid = gol::GolActionGrid("test/data/testfiles/allalive.txt");
//   // // std::cout<< allrulesgrid.GetNearestAlive(1,4) << std::endl;
//   // allrulesgrid.TakeStep(true);
//   auto rand_grid = gol::GolActionGrid(4,4,5);
//   rand_grid.RegenGrid(10);

// }

static void show_usage()
{ 
  // help message

  std::cerr << "\nUsage: ./build/bin/golSimulator [-h] [--help] [-f <filename> <num_gen>] [-r <row_num> <col_num> <num_alive> <num_gen>]\n"
            << " \t\t\t\t[-ssl  <row_num> <col_num> <num_alive> <num_gen> <num_init> <min_alive>]\n\n" 
            << "Options:\n"
            << "Commands \t\t\t\t\t\t\t\tDescription\n\n"
            << "-h | --help\t\t\t\t\t\t\t\tShows this help message\n"
            << "-f | --file <filename> <num_gen> \t\t\t\t\tChoosing the text file input and looping through a specified number of steps.\n"
            << "-r | --rand <row_num> <col_num> <num_alive> <num_gen>  \t\t\tInitialising a random cell grid of specified \n"
            << "\t\t\t\t\t\t\t\t\trows and columns, and a chosen number of alive cells generated at random positions, with required number of generations to simulate \n"
            << "-sls  <row_num> <col_num> <num_alive> <num_gen> <num_init> <min_alive>\tSearching for still lifes. A random cell grid is generated, with specified\n"
            << "\t\t\t\t\t\t\t\t\trows and columns and a chosen number of alive cells generated at random positions.\n"
            << "\t\t\t\t\t\t\t\t\tThe program will generate <num_init> times of the random cell grid. \n"
            << "\t\t\t\t\t\t\t\t\tThere will be a specified, minimum number of alive cells in the grid\n"
            << "\t\t\t\t\t\t\t\t\tWithin the required number of generations, the application will print out, for each random configuration:\n"
            << "\t\t\t\t\t\t\t\t\t2. The resulting still life pattern\n"
            << "\t\t\t\t\t\t\t\t\t3. The number of steps required to generate the final still life pattern from the initial cell grid\n"
            << "\nArguments are separated by a single whitespace.\n"
            << std::endl;

  // command explanations
  std::cout << "Variable explanations:\n\n" 
            << "\t<filename> \t name of the text file (e.g. \"test/data/glider.txt\") \n"
            << "\t<row_num> \t number of rows in the cell grid (e.g. 5)\n" 
            << "\t<col_num> \t number of columns in the cell grid (e.g. 6)\n" 
            << "\t<num_alive> \t number of live cells in the cell grid, with randomly generated positions (e.g. 7)\n" 
            << "\t<num_gen> \t number of generations, or steps of cellular automata required (e.g. 7)\n" 
            << "\t<min_alive> \t minumum number of alive cells present in the cell grid\n"
            << "\t<num_init> \t number of random cell grids to be tried to find still life patterns\n"
            << std::endl;

  // example inputs
  std::cout << "Example inputs:\n\n"

            << "\t-f test/data/input.txt 4 \t Reading from text file named \"test/data/input.txt\", with 4 steps of generation\n"
            << "\t-r 6 4 10 6 \t\t\t Initialising a random cell grid of 6 rows and 4 columns,\n"
            << "\t\t\t\t\t with 10 alive cells generated at random positions, and with 6 steps of generation\n"  
            << "\t-h \t\t\t\t Showing this help message\n"
            << "\t-sls 6 4 6 9 5\t\t\t Initialising a random cell grid of 6 rows and 4 columns,\n"
            << "\t\t\t\t\t with 6 alive cells generated at random positions, and with 9 steps of generation.\n"
            << "\t\t\t\t\t 5 different configurations of the cell grid will be run to find still life patterns.\n"
            << "\t\t\t\t\t Still lives will be found and printed on the screen.\n\n"
            << "If there are no inputs, the help message will be shown\n"
            << std::endl;
}


// main for the command-line application
int main(int argc, char* argv[])
{
  // getting the input string
  std::string input;
  for(int i = 0; i<argc; i++)
  {
    input = input + argv[i] + " ";
  }

  // shows help message when there are no arguments (just the argument for executing: ./golSimulator)
  if(argc == 0 || argc == 1)
  {
    show_usage();
  }
  
  // if there are more than one arguments
  else if(argc > 1)
  {  
    // getting the first argument
    std::string mode = std::string(argv[1]);
    
    // not included in the command-line application
    // if one wants to run the debugging, uncomment below, uncomment and edit the code in GolAction.cpp and GolAction.h
    // to run the command-line application for the debug, run: ./build/bin/golSimulator -d
    /*===================
    // if(mode == "-d")
    // {
    //   Debug();
    // }
    ====================*/

    //shows help message when run with -h or --help options
    if(mode == "-h" || mode == "--help")
    {
      switch (argc) 
      {
        case 2:
        {
          std::cout << "Showing help message: \n" << std::endl;
          show_usage();
          break;
        }

        default:
        {
          std::cerr << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        }
          
      }
    }

    // scanning file to do game of life actions
    else if(mode == "-f" || mode == "--file")
    { 
      switch (argc) 
      {
        case 2:
        {
          std::cout << "Specify filename and number of generations" << std::endl;
          show_usage();
          break;
        }

        case 3:
          {
            std::cout << "Specify number of generations" << std::endl;
            show_usage();
            break;
          }

        // do GOL in this case
        case 4:
        {
          auto textfile = std::string(argv[2]);
          auto gen_steps = atoi(argv[3]);
          
          auto file_grid = gol::GolActionGrid(textfile);
          file_grid.Update(gen_steps);
          break;
        }
        default:
        {
          std::cerr << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        } 
      }
    }

    // initiate cell grid with random configuration to do game of life actions
    else if(mode == "-r" || mode == "--rand")
    {
      switch (argc) 
      {
        case 3:
        {
          std::cout << "Specify number of columns, number of randomly generated alive cells and number of generations" << std::endl;
          show_usage();
          break;
        }
          

        case 4:
        {
          std::cout << "Specify number of randomly generated alive cells and number of generations" << std::endl;
          show_usage();
          break;
        }
          
        case 5:
        {
          std::cout << "Specify number of generations" << std::endl;
          show_usage();
          break;
        }

        // do Gol in this case
        case 6:
        {
          int rows = atoi(argv[2]);
          int cols = atoi(argv[3]);
          int num_alive = atoi(argv[4]);
          int gen_steps = atoi(argv[5]);

          auto rand_grid = gol::GolActionGrid(rows, cols, num_alive);
          rand_grid.Update(gen_steps);
          return EXIT_SUCCESS;
        } 
        
        default:
        {
          std::cerr << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        }
      }
    }

    // do a still life search in this case
    else if(mode == "-sls")
    {
      switch (argc) 
      {
        case 3:
        {
          std::cout << "Specify number of columns, number of randomly generated alive cells, number of generations, " 
                    <<" minimum number of alive cells present, and number of random initial cell grids to be run." << std::endl;
          show_usage();
          break;
        }
          

        case 4:
        {
          std::cout << "Specify number of randomly generated alive cells, number of generations, "
                    << "minimum number of alive cells present, and number of random initial cell grids to be run." << std::endl;
          show_usage();
          break;
        }
          
        case 5:
        {
          std::cout << "Specify number of generations, minimum number of alive cells present, and number of random initial cell grids to be run." << std::endl;
          show_usage();
          break;
        }

        case 6:
        {
          std::cout << "Specify minimum number of alive cells present and number of random initial cell grids to be run." << std::endl;
          show_usage();
          break;
        }

        case 7:
        {
          std::cout << "Specify number of random initial cell grids to be run" << std::endl;
          show_usage();
          break;
        }

        // do GOL and finding still lives in this case
        case 8:
        { 
          int rows = atoi(argv[2]);
          int cols = atoi(argv[3]);
          int num_alive = atoi(argv[4]);
          int gen_steps = atoi(argv[5]);
          int min_alive = atoi(argv[6]);
          int num_init = atoi(argv[7]);

          // ensuring number of alive cells requested in the initial cell grid is larger than 
          // the minimum number of alive cells whilst doing Game of Life actions
          if (num_alive < min_alive)
          {
            throw std::logic_error("Number of alive cells must be larger than the number of minimum alive cells.");
          }
          
          gol::Sleeper(1);
          int num_still_lives = 0;

          // vector to contain the still lifes
          std::vector<gol::GolActionGrid> still_lifes;
          
          for(int i = 0 ; i<num_init;i++) //looping requested number of initiations
          { 
            auto init_grid = gol::GolActionGrid(rows,cols,num_alive); //initiating random cell grid

            std::cout << "\nIteration START" << init_grid.AddDelimiter(); //prints the starting grid
            std::cout << init_grid.PrintCells() << init_grid.AddDelimiter()<< std::endl;
            
            int gridloop = 0;
            
            // looping number of generation steps
            while (gridloop < gen_steps)
            { 
              auto grid_copy = init_grid.GetGrid(); // storing the initial grid for comparison later
              gol::Sleeper(1);
              std::cout << "Step " << gridloop+1 << std::endl;
              init_grid.TakeStep(false);
              
              // see if number of total alive cells are greater the required minimum number of alive cells
              if(init_grid.CountAlive() < min_alive)
              {
                  std::cout << "Total number of alive cells is less than the required minimum number of alive cells\n" 
                            << "No still lives found."<<std::endl; 

                  //exiting while loop 
                  gridloop = gen_steps;
                  break;
              }
             
              else
              { 
                // case where a still life is found
                if (init_grid.IsSame(grid_copy)) //comparing initial grid and the grid after undergoing the TakeStep function
                { 
                  // message indicating that a still life is found
                  std::cout << "*********************\n"
                            << "**Still life found!**\n"
                            << "*********************\n";

                  still_lifes.push_back(init_grid);
                  std::cout << init_grid.PrintGrid() << std::endl;
                  std::cout << "\nIterations required: " << gridloop << std::endl;

                  //exiting while loop
                  gridloop = gen_steps;
                  break;
                }
                
                // if no still life is found, continue the loop
                else
                { 
                  gridloop++;
                  continue;
                }
              }
            }
            std::cout << "Iteration END\n" << std::endl;
          }  

          // number of elements in the vector containing still_lifes will be the number of elements in the vector
          num_still_lives = still_lifes.size();
          std::cout << "Number of still lives found: " << num_still_lives << std::endl;
          int counts = 1;
          for (auto &grid : still_lifes)
          { 
            // printing out still lives
            std::cout << "Number " << counts << std::endl;
            std::cout << grid.PrintGrid() << std::endl;
            counts++;
          }
          std::cout << "Exiting application..." << std::endl;
          return EXIT_SUCCESS;
        } 

        default:
        {
          std::cerr << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        }
      }
    }

    else
    {
      std::cerr << "Invalid input: "
                << input
                << std::endl;
      show_usage();
      return EXIT_SUCCESS;   
    }
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
  
  // catching exception below, unrelated to the command-line application
  /*================================================================================
  int returnStatus = EXIT_FAILURE;

  try
  {
    throw std::runtime_error("Something went wrong!");

    returnStatus = EXIT_SUCCESS;
  }
  catch (gol::Exception& e)
  {
    std::cerr << "Caught gol::Exception: " << e.GetDescription() << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Caught std::exception: " << e.what() << std::endl;
  }

  return returnStatus;
  ================================================================================*/
}
