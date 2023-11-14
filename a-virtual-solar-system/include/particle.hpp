#ifndef particle_h
#define particle_h

#include <string>
#include <iostream>
#include <memory>
#include <Eigen/Core>

class Particle {

    public:
        Particle(double mass);

        double GetMass() const;
        
        Eigen::Vector3d GetPosition() const;
        
        Eigen::Vector3d GetVelocity() const;
        
        Eigen::Vector3d GetAcceleration() const;
        
        void Update(double dt);

        void TotalCircularUpdate(double final_t, double dt);

        Eigen::Vector3d CalcAcceleration(Particle particle_i, Particle particle_j, float epsilon);

        void SetPosition(Eigen::Vector3d pos);
        
        void SetVelocity(Eigen::Vector3d vel);

        void SetAcceleration(Eigen::Vector3d acc);

        bool IsMoving();

        bool IsAccelerating();

        Eigen::Vector3d CalculateTotalAcceleration(std::vector<Particle> particles, int target_index, float epsilon); 
        
        double KineticEnergy();

        double PotentialEnergy(std::vector<Particle> particles, int target_index);

        double TotalEnergy(std::vector<Particle> particles, int target_index);

    private:
        
        double mass;
        
        Eigen::Vector3d position;
        
        Eigen::Vector3d velocity;
        
        Eigen::Vector3d acceleration;

};

// abstract class for 2.3
class InitialConditionGenerator
{
    public:
    virtual std::vector<Particle> GenerateInitialConditions(int num_planets) = 0;
};

class RandomInitialGenerator : public InitialConditionGenerator
{   
    public:

    std::vector<Particle> system_vector;
    
    std::vector<Particle> GenerateInitialConditions(int num_planets);
};

class SolarSystemGenerator : public InitialConditionGenerator
{   
    private:
    // Masses in order Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Pluto
    const std::vector<double> mass_list = {1., 1./6023600, 1./408524, 1./332946.038, 1./3098710, 1./1047.55, 1./3499, 1./22962, 1./19352};

    // Distances from Sun
    const std::vector<double> distance_list = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};

    // Names of solar system bodies in order
    const std::vector<std::string> bodies_list = {"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Pluto"};

    // Global vector to store particles
    std::vector<Particle> system;
    
    public:
    // initial condition generator
    std::vector<Particle> GenerateInitialConditions(int num_planets = 8);
};

class SolarSystem
{
    private:
        // Masses in order Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Pluto
        std::vector<double> mass_list;

        // Distances from Sun
        std::vector<double> distance_list;

        // Names of solar system bodies in order (only used in Solar System simulation, but not for a general solar system)
        const std::vector<std::string> bodies_list = {"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Pluto"};
    
    public:

        // Global vector to store particles
        std::vector<Particle> system;

        // constructor for SolarSystem
        SolarSystem(std::vector<Particle> particles);

        //constructor for a general SolarSystem with random masses and positions 
        // SolarSystem(std::vector<std::unique_ptr> particles, int num_planets);

        // getting the masses
        std::vector<double> GetMasses();

        // getting the distances
        std::vector<double> GetDistances();

        // getting the names
        std::vector<std::string> GetNames();

        void TimeEvolve(double final_time, double dt, float epsilon);
        
        void StepEvolve(int num_steps, double dt, float epsilon);

        void PrintPositions();

        void PrintEarthDetails();

        void EarthSunEvol(double final_time, double dt, float epsilon);

        double TotalSystemEnergy();

        void ShowEnergies();

};
#endif