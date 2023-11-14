#include <iostream>
#include <cmath>
#include <chrono>
#include <Eigen/Core>
#include <particle.hpp>


static void show_usage()
{ 
  // help message

  std::cout << "\nUsage: ./build/solarSystemSimulator [-h] [--help] [-t --len <len_time> <timesteps> <epsilon>] [-t --num <num_timesteps> <timesteps> <epsilon>]"
            << "\n\t\t\t\t    [-tel <len_time> <timesteps> <epsilon> <num_diff_times>] [-gel <len_time> <timesteps> <epsilon> <num_planets>]\n\n"
            << "Options:\n\n"
            << "Commands and Description\n\n"
            << "-h | --help \nShows this help message.\n\n"
            << "-t --len <len_time> <timesteps> <epsilon> \nControlling the timestep dt and the total length of time, with softening factor epsilon.\n\n"
            << "-t --num <num_timesteps> <timesteps> <epsilon> \nControlling the timestep dt and the total number of timesteps to simulate, with softening factor epsilon.\n\n"
            << "-tel <len_time> <max_timestep> <epsilon> <num_diff_times>\nShowing the energy before and after, as well as the energy loss for different timesteps, for the"
            << " evolution of controlling the timestep dt and the total number of timesteps to simulate, with softening factor epsilon."
            << " The maximum timestep is the maximum value of the timestep dt, and there will be "
            << "num_diff_times of such dt, each with decreasing orders of 10 starting from the maximum dt. "
            << "The time taken for the loop to run will also be printed in a summary table.\n\n"
            << "-gel <len_time> <timesteps> <epsilon> <num_planets>\nShowing the total energy loss for the simulation of a general solar system with num_planets many planets with softening factor epsilon."
            << " The general solar system will run for total time of len_time with timesteps dt. Positions and masses of bodies inside the syetem are always randomised. The time taken for the application to run will be printed on a summary table."
            << "\n\nArguments are separated by a single whitespace.\n\n"
            << std::endl;

  // command explanations
  std::cout << "Variable explanations:\n\n" 
            << "\t<len_time> \t\t The total length of time to simulate the evolution of the solar system. (type: float)\n"
            << "\t<num_timesteps> \t The total number of timesteps to simulate the evolution of the solar system. (type: integer)\n"
            << "\t<timesteps> \t\t The timestep dt to evolve the solar system (type: float)\n"
            << "\t<max_timestep> \t\t The maximum timestep to evolve the solar system (type: float)\n"
            << "\t<epsilon> \t\t Softening factor when calculating the acceleration. (type: float)\n"
            << "\t<num_diff_times> \t Number of different timesteps to calculate and compare the solar system energies for each of these timesteps. (type: int)\n"
            << "\t<num_planets> \t\t Number of planets in the general solar system. (type: int)\n"
            // << "\t<set_rand_seed> \t Toggling random conditions on or off. (type: bool: true / false)"
            << "\t\t\t\t\t\t\n\n"
            << "**NOTE**: Usage of π=3.14159265... , please input <constant>*PI or <constant>*pi, where <constant> is any number of type float or integer.\n\n "
            << std::endl;

  // example inputs
  std::cout << "Example inputs:\n\n"
            << "-h  \nShows this help message\n\n"
            << "-t --len 2.0*PI 0.0001 0.0 \nEvolving the solar system with a total time of 2π with timestep dt = 0.0001, with softening factor of epsilon = 0.0 .\n\n"
            << "-t --num 400 0.01 0.001 \nEvolving the solar system with 400 timesteps with timestep dt = 0.01, , with softening factor of epsilon = 0.001 .\n\n"
            << "-tel 200.0*PI 10.0 0.0 5 \nShowing the energy before and after, as well as the total energy loss for the evolution of the solar system with"
            << " a total time of 200π with timestep dt, with softening factor of epsilon = 0.0. There are 5 different timesteps for this run,"
            << " which are dt = 10, 1, 0.1, 0.01, 0.001\n\n"
            << "-gel 200.0*PI 0.001 0.1 64 \nShowing the total energy loss for the evolution of a general solar system with "
            << "a total time of 200π with timestep dt=0.001, with softening factor of epsilon = 0.1. There are 64 planets in this general solar system, "
            << "where their masses, distance from sun and orientation from the sun are randomised.\n\n"
            << std::endl;
  
  // scaling disclaimer
  std::cout << "**NOTE** Scaling of this application: \n"
            << "Units are all scaled so that the central star's (e.g. Sun in our solar system) mass is 1. For the solar system specifically, the distance between the Sun and the Earth is 1(AU).\n"
            << "Using this scaling, the Earth will travel around the Sun exactly once every 2π. (i.e. one year corresponds to a time of 2π)\n"
            << std::endl;
}

static void AddDelimiter()
{
  std::cout << "\n======================================================================\n" << std::endl;
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

  // shows help message when there are no arguments (just the argument for executing: ./build/solarSystemSimulator)
  if(argc == 0 || argc == 1)
  {
    std::cout << "Showing help message: \n" << std::endl;
    show_usage();
  }
  
  // if there are more than one arguments
  else if(argc > 1)
  {  
    // getting the first argument
    std::string mode = std::string(argv[1]);

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

    else if(mode == "-t")
    {
      switch (argc) 
      {
        case 2:
        {
          std::cout << "Please choose a mode: --len or --num; then please specify the parameters you would like to input. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }
          

        case 3:
        { 
          std::string mode_input = argv[2];

          // comparing strings: https://cplusplus.com/reference/string/string/compare/
          if(mode_input.compare("--len") == 0)
          {
            std::cout << "Please input the total length of time to simulate the evolution of the solar system, the timestep dt to evolve the solar system and the softening factor epsilon. \n" 
                      << "Check the help message below for more detail:\n"
                      << std::endl;
            show_usage();
            break;
          }

          else if(mode_input.compare("--num") == 0)
          {
            std::cout << "Please input the total number of timesteps to simulate the evolution of the solar system, the timestep dt to evolve the solar system and the softening factor epsilon. \n" 
                      << "Check the help message below for more detail:\n"
                      << std::endl;
            show_usage();
            break;
          }

          else
          {
            std::cout<< "Invalid mode input: "
                    << mode_input
                    << "\nShowing help message:"
                    << std::endl;
            show_usage();
            break;
          }
        }
          
        case 4:
        { 
          std::string mode_input = argv[2];

          if(mode_input.compare("--len") == 0 || mode_input.compare("--num") == 0)
          {
            std::cout << "Please input the timestep dt to evolve the solar system and the softening factor epsilon. \n" 
                      << "Check the help message below for more detail:\n"
                      << std::endl;
            show_usage();
            break;
          }

          else
          {
            std::cout<< "Invalid input: "
                    << input
                    << "\n Showing help message:"
                    << std::endl;
            show_usage();
            break;
          }
        }

        case 5:
        {
          std::cout << "Please input the softening factor epsilon. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }

        // do evolution in this case
        case 6:
        { 
          // input of mode
          std::string mode_input = argv[2];

          // when mode is --len
          if(mode_input.compare("--len") == 0)
          {
            auto len_time = std::string(argv[3]);
            double final_time;

            // processing and validating inputs

            // variable len_time

            // if the user uses π
            if (len_time.find("PI") != std::string::npos || len_time.find("pi") != std::string::npos)
            {
              std::string delimiter = "*";
              std::string constant = len_time.substr(0, len_time.find(delimiter)); // token is <constant>

              double num_constant = std::stod(constant);
              
              final_time = num_constant * M_PI;
            }

            // if the user does not use π or has invalid input
            else
            { 
              try 
              {
                final_time = std::stod(len_time);
              } 

              // catching exception if <constant> is of invalid data type when converting from string to double
              // check documentation for std::stod here: https://cplusplus.com/reference/string/stod/
              catch (const std::invalid_argument& err) 
              {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
              } 
              final_time = std::stod(len_time);
            }

            // variable dt
            auto timestep = std::string(argv[4]);
            try
            {
              double dt = std::stod(timestep);
            }

            // catching exception if <timestep> is of invalid data type when converting from string to double
            catch(const std::invalid_argument& err) 
            {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
            }

            double dt = std::stod(timestep);

            // variable epsilon
            auto epsilon = std::string(argv[5]);
            try
            {
              float eps = std::stof(epsilon);
            }

            // catching exception if <epsilon> is of invalid data type when converting from string to double
            catch(const std::invalid_argument& err) 
            {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
            }

            float eps = std::stof(epsilon);

            // evolution with controlled parameters: timestep dt and the total length of time final_time
            
            SolarSystemGenerator ssgen;
            auto system_gen = ssgen.GenerateInitialConditions();
            SolarSystem solar_system(system_gen);
            
            // positions before
            std::cout<< "Starting positions: \n" << std::endl;
            AddDelimiter();

            // solar_system.PrintEarthDetails();
            solar_system.PrintPositions();
            
            auto init_earth = solar_system.system[3].GetPosition();
            AddDelimiter();
            std::cout << "Earth's starting position:\n" << init_earth << std::endl;
            AddDelimiter();
            
            // evolve system
            std::cout<< "STARTING EVOLUTION" << std::endl;
            solar_system.TimeEvolve(final_time, dt, eps);
            // solar_system.EarthSunEvol(final_time, dt, eps);
            AddDelimiter();
            auto final_earth = solar_system.system[3].GetPosition();
            std::cout << "Earth's final position:\n" << final_earth << std::endl;
            
            // positions after
            std::cout<< "\nEnd positions: " << std::endl;
            AddDelimiter();

            // solar_system.PrintEarthDetails();
            solar_system.PrintPositions();

            return 0;
          }

          // when mode is --num
          else if(mode_input.compare("--num") == 0)
          {
            auto num_timesteps = argv[3];

            // processing and verifying inputs

            // variable num_timesteps
            try 
            {
              int num_times = std::stoi(num_timesteps);

              double num_time_double = std::stod(num_timesteps);

              // ensuring num_timesteps input is of type int
              try
              { 
                if( (num_times-num_time_double)!=0 )
                {
                  throw std::invalid_argument( "Input is of type float or double" );
                }                
              }
              
              catch(const std::invalid_argument& err)
              {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
              }
            } 

            // catching exception if <num_timesteps> is of invalid data type when converting from string to int
            catch (const std::invalid_argument& err) 
            {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
            } 

            int num_times = std::stoi(num_timesteps);

            // variable dt
            auto timestep = argv[4];
            try
            {
              double dt = std::stod(timestep);
            }

            // catching exception if <timestep> is of invalid data type when converting from string to double
            catch(const std::invalid_argument& err) 
            {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
            }
            
            double dt = std::stod(timestep);

            // variable epsilon
            auto epsilon = std::string(argv[5]);
            try
            {
              float eps = std::stof(epsilon);
            }

            // catching exception if <epsilon> is of invalid data type when converting from string to double
            catch(const std::invalid_argument& err) 
            {
                std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
                std::cerr << "Input valid data type and check the help message below" << std::endl;
                show_usage();
                break;
            }

            float eps = std::stof(epsilon);

            // evolution with controlled parameters: timestep dt and the total number of timesteps num_times         
            SolarSystemGenerator ssgen;
            auto system_gen = ssgen.GenerateInitialConditions();
            SolarSystem solar_system(system_gen);

            // positions before
            std::cout<< "Starting positions: \n" << std::endl;
            AddDelimiter();

            solar_system.PrintEarthDetails();
            solar_system.PrintPositions();
            
            auto init_earth = solar_system.system[3].GetPosition();
            AddDelimiter();
            std::cout << "Earth's starting position:\n" << init_earth << std::endl;
            AddDelimiter();
            
            // evolve system
            std::cout<< "STARTING EVOLUTION" << std::endl;
            solar_system.StepEvolve(num_times, dt, eps);

            AddDelimiter();
            auto final_earth = solar_system.system[3].GetPosition();
            std::cout << "Earth's final position:\n" << final_earth << std::endl;
            
            // positions after
            std::cout<< "\nEnd positions: " << std::endl;
            AddDelimiter();

            solar_system.PrintEarthDetails();
            solar_system.PrintPositions();

            return 0;
          }

          else
          {
            std::cout<< "Invalid input: "
                    << input
                    << "\nShowing help message:"
                    << std::endl;
            show_usage();
            break;
          }

        } 
        
        default:
        {
          std::cout << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        }
      }
    }
    
    else if(mode == "-tel")
    { 
      switch (argc) 
      {   
        case 2:
        { 
          // comparing strings: https://cplusplus.com/reference/string/string/compare/
          std::cout << "Please input the total length of time to simulate the evolution of the solar system, the timestep dt to evolve the solar system, the softening factor epsilon and the number of different timesteps.\n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }
          
        case 3:
        { 
          std::cout << "Please input the timestep dt to evolve the solar system, the softening factor epsilon and the number of different timesteps. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }

        case 4:
        {
          std::cout << "Please input the softening factor epsilon and the number of different timesteps. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }

        case 5:
        {
          std::cout << "Please input the number of different timesteps. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }

        // do evolution in this case, showing energy before and after and the energy loss
        case 6:
        { 
          // input of len_time
          auto len_time = std::string(argv[2]);

          // processing and validating inputs

          // variable len_time
          double final_time;

          // if the user uses π
          if (len_time.find("PI") != std::string::npos || len_time.find("pi") != std::string::npos)
          {
            std::string delimiter = "*";
            std::string constant = len_time.substr(0, len_time.find(delimiter)); // token is <constant>

            double num_constant = std::stod(constant);
            
            final_time = num_constant * M_PI;
          }

          // if the user does not use π or has invalid input
          else
          { 
            try 
            {
              final_time = std::stod(len_time);
            } 

            // catching exception if <constant> is of invalid data type when converting from string to double
            // check documentation for std::stod here: https://cplusplus.com/reference/string/stod/
            catch (const std::invalid_argument& err) 
            {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
            } 
            final_time = std::stod(len_time);
          }

          // variable dt
          auto max_timestep = std::string(argv[3]);
          try
          {
            double max_dt = std::stod(max_timestep);
          }

          // catching exception if <max_timestep> is of invalid data type when converting from string to double
          catch(const std::invalid_argument& err) 
          {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
          }

          double max_dt = std::stod(max_timestep);

          // variable epsilon
          auto epsilon = std::string(argv[4]);
          try
          {
            float eps = std::stof(epsilon);
          }

          // catching exception if <epsilon> is of invalid data type when converting from string to double
          catch(const std::invalid_argument& err) 
          {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
          }

          float eps = std::stof(epsilon);


          // variable num_diff_times
          auto num_diff_times = std::string(argv[5]);

          try 
          {
            int diff_timesteps = std::stoi(num_diff_times);

            double diff_timesteps_double = std::stod(num_diff_times);

            // ensuring num_diff_times input is of type int
            try
            { 
              if( (diff_timesteps-diff_timesteps_double)!=0 )
              {
                throw std::invalid_argument( "Input is of type float or double" );
              }                
            }
            
            catch(const std::invalid_argument& err)
            {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
            }
          } 

          // catching exception if <num_diff_times> is of invalid data type when converting from string to int
          catch (const std::invalid_argument& err) 
          {
            std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
            std::cerr << "Input valid data type and check the help message below" << std::endl;
            show_usage();
            break;
          } 

          int diff_times = std::stoi(num_diff_times);

          std::vector<double> timesteps_list;
          std::vector<double> energy_loss_list;
          std::vector<double> time_list; 
          double dt = max_dt;



          // evolution with controlled parameters: timestep dt and the total length of time final_time
          SolarSystemGenerator ssgen;
          auto system = ssgen.GenerateInitialConditions();
          SolarSystem solar_system(system);


          for(int n = 0; n < diff_times; n++)
          {
          
          // Marking the start time
          auto start_time = std::chrono::high_resolution_clock::now();

          std::cout << "Timestep dt: " << dt << std::endl;
          // Initial energies
          std::cout<< "Starting energies: \n" << std::endl;
          AddDelimiter();

          solar_system.ShowEnergies();

          double init_energy = solar_system.TotalSystemEnergy();
          
          AddDelimiter();
          
          // evolve system
          std::cout<< "STARTING EVOLUTION" << std::endl;
          solar_system.TimeEvolve(final_time, dt, eps);

          AddDelimiter();
          
          // Final energies
          std::cout<< "\nFinal energies: " << std::endl;
          AddDelimiter();

          solar_system.ShowEnergies();
          double final_energy = solar_system.TotalSystemEnergy();

          // total energy loss in solar system due to numerical errors
          double total_energy_loss = final_energy - init_energy;
          std::cout << "The total energy loss for dt=" << dt << " is " << total_energy_loss << std::endl;

          // Marking the end time
          auto end_time = std::chrono::high_resolution_clock::now();

          // Calculating the time taken for the program to run in milliseconds
          auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

          
          // adding total_energy_loss and dt to respective lists for summary message at the end
          timesteps_list.push_back(dt);
          energy_loss_list.push_back(total_energy_loss);
          time_list.push_back(time_taken);
          
          // restarting the loop with new dt value
          dt = dt/10.0;
          }

          AddDelimiter();

          // summary message
          std::cout << "In summary" << std::endl;
          AddDelimiter();
          std::cout << "Timestep" << "\t" << "Total energy loss\t" << "Time (microseconds)" << std::endl;
          for(int n = 0; n < diff_times; n++)
          {
            std::cout << "" << timesteps_list[n] << "\t\t" << energy_loss_list[n] << "\t\t" << time_list[n] << std::endl;
          }
      
          return 0;
        }

        default:
        {
          std::cout << "Too much arguments\n"
                    << "Invalid input: "
                    << input
                    << std::endl;
          show_usage();
          break;
        }
      }  
    }
    
    else if(mode == "-gel")
    {
      switch (argc) 
      {
        case 2:
        {
          std::cout << "Please input the total length of time to simulate the evolution of the general solar system, the timestep dt to evolve it, the softening factor epsilon and number of planets. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }
          
        case 3:
        { 

          std::cout << "Please input the timestep dt to evolve the general solar system, the softening factor epsilon and number of planets. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }
          
        case 4:
        { 
          std::string mode_input = argv[2];
          std::cout << "Please input the softening factor epsilon, the number of planets in the general solar system. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;

        }

        case 5:
        {
          std::cout << "Please input the number of planets in the general solar system. \n" 
                    << "Check the help message below for more detail:\n"
                    << std::endl;
          show_usage();
          break;
        }

        // do evolution in this case
        case 6:
        { 
          // input of mode
          std::string mode_input = argv[2];

          // processing and validating inputs   

          // input of len_time
          auto len_time = std::string(argv[2]);

          // processing and validating inputs

          // variable len_time
          double final_time;

          // if the user uses π
          if (len_time.find("PI") != std::string::npos || len_time.find("pi") != std::string::npos)
          {
            std::string delimiter = "*";
            std::string constant = len_time.substr(0, len_time.find(delimiter)); // token is <constant>

            double num_constant = std::stod(constant);
            
            final_time = num_constant * M_PI;
          }

          // if the user does not use π or has invalid input
          else
          { 
            try 
            {
              final_time = std::stod(len_time);
            } 

            // catching exception if <constant> is of invalid data type when converting from string to double
            // check documentation for std::stod here: https://cplusplus.com/reference/string/stod/
            catch (const std::invalid_argument& err) 
            {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
            } 
            final_time = std::stod(len_time);
          }

          // variable dt
          auto timestep = std::string(argv[3]);
          try
          {
            double dt = std::stod(timestep);
          }

          // catching exception if <max_timestep> is of invalid data type when converting from string to double
          catch(const std::invalid_argument& err) 
          {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
          }

          double dt = std::stod(timestep);

          // variable epsilon
          auto epsilon = std::string(argv[4]);
          try
          {
            float eps = std::stof(epsilon);
          }

          // catching exception if <epsilon> is of invalid data type when converting from string to double
          catch(const std::invalid_argument& err) 
          {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
          }

          float eps = std::stof(epsilon);       
          
          // variable num_diff_times
          auto num_planets = std::string(argv[5]);

          try 
          {
            int num_bodies = std::stoi(num_planets);

            double num_bodies_double = std::stod(num_planets);

            // ensuring num_diff_times input is of type int
            try
            { 
              if( (num_bodies-num_bodies_double)!=0 )
              {
                throw std::invalid_argument( "Input is of type float or double" );
              }                
            }
            
            catch(const std::invalid_argument& err)
            {
              std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
              std::cerr << "Input valid data type and check the help message below" << std::endl;
              show_usage();
              break;
            }
          } 

          // catching exception if <num_diff_times> is of invalid data type when converting from string to int
          catch (const std::invalid_argument& err) 
          {
            std::cerr << "Caught an invalid_argument exception. " << err.what() << std::endl;
            std::cerr << "Input valid data type and check the help message below," << std::endl;
            show_usage();
            break;
          } 

          int num_bodies = std::stoi(num_planets);

          // doing simulation here
          RandomInitialGenerator randgen;
          auto general_system_gen = randgen.GenerateInitialConditions(num_bodies);
          SolarSystem general_system(general_system_gen);
          
          
          // Marking the start time
          auto start_time = std::chrono::high_resolution_clock::now();

          double init_energy = general_system.TotalSystemEnergy();
          
          // evolve system
          std::cout<< "STARTING EVOLUTION" << std::endl;
          general_system.TimeEvolve(final_time, dt, eps);

          AddDelimiter();

          double final_energy = general_system.TotalSystemEnergy();

          // total energy loss in solar system due to numerical errors
          double total_energy_loss = final_energy - init_energy;

          // Marking the end time
          auto end_time = std::chrono::high_resolution_clock::now();

          // Calculating the time taken for the program to run in minutes
          auto time_taken = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

          
          // adding total_energy_loss and dt to respective lists for summary message at the end
          
          std::cout << "Number of planets\t" << num_bodies << "\n"
                    << "Timestep\t\t" << dt << "\n"
                    << "Total energy loss\t" << total_energy_loss << "\n"
                    << "Time (minutes)\t\t" << time_taken/60. << "\n"
                    << std::endl;
          return 0;
        }
        
        default:
        {
          std::cout << "Too much arguments\n"
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
      std::cout << "Invalid input: "
                << input
                << std::endl;
      show_usage();
      return 0;   
    }
    return 0;
  }
  return 0;
}
