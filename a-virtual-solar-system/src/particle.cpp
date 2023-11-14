#include "particle.hpp"
#include <cmath>
#include <memory>
#include <omp.h>
#include <random>
#include <Eigen/Core>

// constructor of the particle
Particle::Particle(double mass):mass(mass)
{   
    // setting particles' positions, velocities and accelerations to all zeroes first
    Eigen::Vector3d init_vector = {0.0, 0.0, 0.0};
    position = init_vector;
    velocity = init_vector;
    acceleration = init_vector;

    if (mass < 0)
    {
        throw std::logic_error("Mass of a particle should be equal or greater than 0.");
    }
}

double Particle::GetMass() const
{
    return mass;
}

Eigen::Vector3d Particle::GetPosition() const
{
    return position;
}
        
Eigen::Vector3d Particle::GetVelocity() const
{
    return velocity;
}

Eigen::Vector3d Particle::GetAcceleration() const
{
    return acceleration;
}

void Particle::Update(double dt)
{   
    position += dt * velocity;
    velocity += dt * acceleration;
    
    SetPosition(position);
    SetVelocity(velocity);
}

void Particle::TotalCircularUpdate(double final_t, double dt)
{
    for(double t = 0; t <= final_t; t += dt)
    {   
        Update(dt);
        SetAcceleration(-position);
    }
}

void Particle::SetPosition(Eigen::Vector3d pos)
{
    position = pos;
}

void Particle::SetVelocity(Eigen::Vector3d vel)
{
    velocity = vel;
}

void Particle::SetAcceleration(Eigen::Vector3d acc)
{
    acceleration = acc;
}

bool Particle::IsMoving()
{   
    if (velocity.isZero())
    {
        return false;
    }
    return true;
}

bool Particle::IsAccelerating()
{
    if (acceleration.isZero())
    {
        return false;
    }
    return true;
}

Eigen::Vector3d Particle::CalcAcceleration(Particle particle_i, Particle particle_j, float epsilon = 0.0)
{
    Eigen::Vector3d acc_ji;
    auto mass_j = particle_j.GetMass();
    auto x_j = particle_j.GetPosition();
    auto x_i = particle_i.GetPosition();
    auto distance_ji = (x_i - x_j).norm();

    acc_ji = (mass_j * (x_j - x_i))/(pow (pow(distance_ji, 2) + pow(epsilon, 2), 1.5 ));

    return acc_ji;
}

// calculates the total acceleration of the target particle due to other particles
// sets the acceleration and returns the acceleration value of the particle

// to use this function, do the following:
// Particle target_particle{<mass of the target particle>}
// target_particle.CalculateTotalAcceleration(<particle_list>, <index of target_particle>, <epsilon>)
Eigen::Vector3d Particle::CalculateTotalAcceleration(std::vector<Particle> particles, int target_index, float epsilon) 
{   
    // looping over all pairs of particles    
    Eigen::Vector3d final_acc = {0.0, 0.0, 0.0};
    auto target_particle = particles[target_index];

    // help from: https://stackoverflow.com/questions/40495250/openmp-reduction-with-eigenvectorxd
    #pragma omp declare reduction(Vector3dSum: Eigen::Vector3d: \
    omp_out += omp_in) \
    initializer(omp_priv = Eigen::Vector3d::Zero())

    #pragma omp parallel for reduction(Vector3dSum:final_acc)
    for(auto j = 0; j < particles.size(); j++) 
    {   
        if( j != target_index )
        {   
            final_acc += CalcAcceleration(particles[target_index], particles[j], epsilon);
        }
    }

    SetAcceleration(final_acc);
    
    return final_acc;
}

double Particle::KineticEnergy()
{   
    double ke;  

    auto mass = GetMass();
    auto v_i = GetVelocity();
    ke = mass * pow(v_i.norm(), 2) / 2;

    return ke;
}

double Particle::PotentialEnergy(std::vector<Particle> particles, int target_index)
{   
    double summation_term = 0.;

    #pragma omp parallel for schedule(static)
    for(int j = 1; j < particles.size(); j++)
    {   
        if (j != target_index)
        {
            auto d_ij =  (particles[target_index].GetPosition() - particles[j].GetPosition()).norm();
            auto mass_i = particles[target_index].GetMass();
            auto mass_j = particles[j].GetMass();
            summation_term += mass_i * mass_j / d_ij;
        }
    }

    double total_pe = (-1/2) * summation_term;
    return total_pe;
}

double Particle::TotalEnergy(std::vector<Particle> particles, int target_index)
{
    double total_energy = KineticEnergy() + PotentialEnergy(particles, target_index);
    return total_energy;
}

// constructor for Solar System
SolarSystem::SolarSystem(std::vector<Particle> particles)
{
    system = particles;
}

// getting the masses
std::vector<double> SolarSystem::GetMasses()
{   
    for(int i = 0; i < system.size(); i++)
    {
        mass_list.push_back(system[i].GetMass());
    }
    return mass_list;
}

// getting the distances
std::vector<double> SolarSystem::GetDistances()
{
    for(int i = 0; i < system.size(); i++)
    {
        distance_list.push_back(system[i].GetPosition().norm());
    }
    return distance_list;
}

// getting the names, only applicable for the Solar System, not the general one
std::vector<std::string> SolarSystem::GetNames()
{   
    return bodies_list;
}


// initial condition generator
std::vector<Particle> SolarSystemGenerator::GenerateInitialConditions(int num_planets)
{
    std::vector<Particle> system_vector;

    if (num_planets == 0)
    {
        num_planets = 8;
    }

    // setting sun
    Particle sun{1.};

    // position and velocity of sun are zero
    sun.SetPosition(Eigen::Vector3d {0., 0., 0.});
    sun.SetVelocity(Eigen::Vector3d {0., 0., 0.});
    
    // adding sun to system of type vector
    system_vector.push_back(sun);

    for (int i = 1; i < mass_list.size(); i++) 
    {
        // adding the mass of the planet
        auto mass = mass_list[i];
        Particle planet{mass};

        // transforming coordinates from spherical polar to cartesian
        
        // angles \theta are random
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0, 2 * M_PI);
        double theta = dist(gen);
        
        // double theta = 0.0;
        auto r = distance_list[i];

        // individual components of positions 
        auto x_x = r * cos(theta);
        auto x_y = r * sin(theta);

        // individual components of velocities
        auto v_x = -(1/sqrt(r)) * sin(theta);
        auto v_y = (1/sqrt(r)) * cos(theta);

        // z components of the position and velocity are 0
        double x_z = 0.0;
        double v_z = 0.0;
        
        Eigen::Vector3d position = {x_x, x_y, x_z};
        Eigen::Vector3d velocity = {v_x, v_y, v_z};

        // setting position
        planet.SetPosition( position );
        
        // setting velocity
        planet.SetVelocity( velocity );

        // adding the planet into the system vector;
        system_vector.push_back(std::move(planet));
    }
    
    return system_vector;
}

// evolution of the solar system
void SolarSystem::TimeEvolve(double final_time, double dt, float epsilon)
{   
    // outer while loop to loop over all timesteps

    for(double t = 0.0; t <= final_time; t+=dt)
    {  
        std::vector<Eigen::Vector3d> acceleration_list;

        #pragma omp parallel for ordered
        for(auto i = 1 ; i < system.size() ;i++)
        #pragma omp ordered
        {   
            auto acc_planet = system[i].CalculateTotalAcceleration(system, i, epsilon);
            acceleration_list.push_back(acc_planet);
        }

        #pragma omp parallel for schedule(static)
        for(auto i = 1 ; i < system.size();i++) 
        {   
            system[i].SetAcceleration(acceleration_list[i-1]);
            system[i].Update(dt);
        }
    }
}

void SolarSystem::StepEvolve(int num_steps, double dt, float epsilon)
{
    double t = 0.0;
    int steps = 0;

    while (steps <= num_steps)
    {   
        std::vector<Eigen::Vector3d> acceleration_list;

        for(auto i = 1 ; i < system.size() ;i++)
        {   
            auto acc_planet = system[i].CalculateTotalAcceleration(system, i, epsilon);
            acceleration_list.push_back(acc_planet);
            std::cout << "acceleration calculated: " << acc_planet << std::endl;
        }

        for(auto i = 1 ; i < system.size();i++) 
        {   
            std::cout << "From acc_list: " << acceleration_list[i-1] << std::endl;

            system[i].SetAcceleration(acceleration_list[i]);
            system[i].Update(dt);
        }
        steps++;
    }
}

// used during debugging, not used in main.cpp
void SolarSystem::EarthSunEvol(double final_time, double dt, float epsilon)
{
    double t = 0.0;
    // std::vector<Eigen::Vector3d> final_pos_list;
    // outer while loop to loop over all timesteps
    Particle sun{1.};
    sun.SetVelocity(Eigen::Vector3d {0., 0., 0.});
    Particle mercury{GetMasses()[1]};
    Particle venus{GetMasses()[2]};
    Particle earth{GetMasses()[3]};


    auto merc_pos = system[1].GetPosition();
    auto merc_vel = system[1].GetVelocity();

    auto venus_pos = system[2].GetPosition();
    auto venus_vel = system[2].GetVelocity();

    auto earth_pos = system[3].GetPosition();
    auto earth_vel = system[3].GetVelocity();

    std::cout << "Earth Initial position: " << earth_pos << std::endl;

    // setting position
    earth.SetPosition( earth_pos );
    
    // setting velocity
    earth.SetVelocity( earth_vel );

    mercury.SetPosition(system[1].GetPosition());
    mercury.SetVelocity(system[1].GetVelocity());

    venus.SetPosition(system[2].GetPosition());
    venus.SetVelocity(system[2].GetVelocity());

    std::vector<Particle> se_system = {sun, mercury, venus, earth};

    std::cout << "Earth initial position: \n" << se_system[3].GetPosition() << std::endl;
    std::cout << "Earth initial velocity: \n" << se_system[3].GetVelocity() << std::endl;
    std::cout << "Earth initial acceleration: \n" << se_system[3].GetAcceleration() << std::endl;
    while (t <= final_time)
    {   
        std::vector<Eigen::Vector3d> acceleration_list;

        for(auto i = 1 ; i < se_system.size() ;i++)
        { 

            auto acc_planet = se_system[i].CalculateTotalAcceleration(se_system, 3, epsilon);
            acceleration_list.push_back(acc_planet);
        }

        for(auto i = 1 ; i < system.size();i++) //system.size()
        {   
            se_system[i].SetAcceleration(acceleration_list[i-1]);
            se_system[i].Update(dt);
        } 

        t+=dt;
    }
    std::cout << "radius final: " << se_system[3].GetPosition().norm() << std::endl;
    std::cout << "Earth final position: \n" << se_system[3].GetPosition() << std::endl;
    std::cout << "Earth final velocity: \n" << se_system[3].GetVelocity() << std::endl;
    std::cout << "Earth final acceleration: \n" << se_system[3].GetAcceleration() << std::endl;
}


// printing the positions of the bodies in the solar system
void SolarSystem::PrintPositions()
{   
    std::cout << "Printing positions of the Solar System bodies: \n" << std::endl;
    for (int i = 0; i <system.size(); i++) 
    {
        auto name = GetNames()[i];
        auto body = system[i];
        auto euclidean_distance = body.GetPosition();

        std::cout << name << ":\n"
                  << euclidean_distance << "\n" 
                  << "Distance from Sun: " << euclidean_distance.norm() << "\n"
                  << std::endl;
    }
}

void SolarSystem::PrintEarthDetails()
{
    auto earth = system[3];
    auto euclidean_distance = earth.GetPosition();
    auto vel = earth.GetVelocity();
    std::cout << "Details for Earth:\n\n"
              << "Position: \n" << euclidean_distance << "\n" 
              << "Distance from sun: " << euclidean_distance.norm() << "\n\n"
              << "Velocity: \n" << vel << "\n" 
              << "Acceleration: \n" << earth.GetAcceleration() << "\n\n"
              << std::endl;
}

double SolarSystem::TotalSystemEnergy()
{
    double total_system_energy = 0.;
    for(int i = 1; i < system.size(); i++)
    {
        total_system_energy += system[i].TotalEnergy(system, i);
    }
    return total_system_energy;
}

void SolarSystem::ShowEnergies()
{
    std::cout << "Printing energies of the Solar System bodies: \n" << std::endl;
    for (int i = 0; i <system.size(); i++) 
    {
        auto name = GetNames()[i];
        auto body = system[i];
        auto body_energy = body.TotalEnergy(system, i);

        std::cout << " Energy of " << name << ": " << body_energy << "\n" << std::endl;
    }
    std::cout << "Total energy of the bodies in the solar system: " << TotalSystemEnergy() << std::endl;
}

// random initial generator described in 2.3 
std::vector<Particle> RandomInitialGenerator::GenerateInitialConditions(int num_planets)
{   
    std::vector<Particle> final_system;

    // first particle should be a central star with mass 1 and zero velocity
    Particle star{1.};
    star.SetVelocity(Eigen::Vector3d {0., 0., 0.});
    
    final_system.push_back(star);

    // adding planets
    #pragma omp parallel for ordered
    for (int i = 1; i < num_planets + 1; i++) 
    #pragma omp ordered
    {   
        // adding the mass of the planet
        double mass;
        
        std::random_device rd_mass;
        std::mt19937 gen_mass(rd_mass());
        std::uniform_real_distribution<> dis_mass(1./6000000, 1./1000);
    
        mass = dis_mass(gen_mass);

        Particle planet{mass};

        // transforming coordinates from spherical polar to cartesian

        // angle theta
        double theta;

        // angles \theta are random
        std::random_device rd_angle;
        std::mt19937 gen_angle(rd_angle());
        std::uniform_real_distribution<> dist_angle(0, 2 * M_PI);
        theta = dist_angle(gen_angle);
        
        // distance from sun r
        double r;

        std::random_device rd_distance;
        std::mt19937 gen_distance(rd_distance());
        std::uniform_real_distribution<> dist_distance(0.4, 30.);
        r = dist_distance(gen_distance);
        

        // individual components of positions 
        auto x_x = r * cos(theta);
        auto x_y = r * sin(theta);

        // individual components of velocities
        auto v_x = -(1/sqrt(r)) * sin(theta);
        auto v_y = (1/sqrt(r)) * cos(theta);

        // z components of the position and velocity are 0
        double x_z = 0.0;
        double v_z = 0.0;
        
        Eigen::Vector3d position = {x_x, x_y, x_z};
        Eigen::Vector3d velocity = {v_x, v_y, v_z};

        // setting position
        planet.SetPosition( position );
        
        // setting velocity
        planet.SetVelocity( velocity );

        // adding the planet into the system vector;
        final_system.push_back(planet);
    }

    return final_system;
}
