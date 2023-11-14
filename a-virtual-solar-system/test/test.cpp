#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"
#include <algorithm>
#include <math.h>

// documentation for floating point matchers: 
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md
using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

// tests for mass
TEST_CASE( "Particle sets mass incorrectly", "[particle_mass]" ) 
{   
    Particle p1{3.14};
    REQUIRE_THAT( p1.GetMass(), WithinRel(3.14, 0.01) );

    Particle p2{0.00};
    REQUIRE_THAT( p2.GetMass(), WithinRel(0.00, 0.01) );

    Particle p3{123456789.9876};
    REQUIRE_THAT( p3.GetMass(), WithinRel(123456789.9875, 0.0001) );
}

TEST_CASE( "Particle cannot allow for negative mass. Check thrown exception", "[particle_mass_exception]" )
{   
    double mass = -10.69;
    REQUIRE_THROWS_AS( Particle(mass), std::logic_error);
}

// tests for position
TEST_CASE( "Particle sets position incorrectly", "[particle_position]" ) 
{   

    Particle p_init{89};
    REQUIRE(p_init.GetPosition().isZero());

    Particle p1{500.0};
    Eigen::Vector3d p1_position = {1.0, 0.0, 0.0};
    p1.SetPosition(p1_position);
    REQUIRE( p1.GetPosition().isApprox(p1_position,0.01));


    Particle p2{45.0};
    Eigen::Vector3d p2_position = {87.5, 100.0, 78.99};
    p2.SetPosition(p2_position);
    REQUIRE( p2.GetPosition().isApprox(p2_position,0.01));

    Particle p3{1.0};
    Eigen::Vector3d p3_position = {5.5, 5.05, 4.41};
    p3.SetPosition(p3_position);
    REQUIRE( p3.GetPosition().isApprox(p3_position,0.01));
}

// // tests for velocity
TEST_CASE( "Particle sets velocity incorrectly", "[particle_velocity]" ) 
{   

    Particle p_init{75.6};
    REQUIRE(p_init.GetVelocity().isZero());

    Particle p1{500.0};
    
    Eigen::Vector3d p1_position = {5.5, 4.4, 6.6};
    p1.SetPosition(p1_position);
    
    Eigen::Vector3d p1_velocity = {1.0, 0.0, 0.0};
    p1.SetVelocity(p1_velocity);
    REQUIRE( p1.GetVelocity().isApprox(p1_velocity, 0.001));

    
    Particle p2{45.0};

    Eigen::Vector3d p2_position = {44.5, 88.7, 100000.5};
    p2.SetPosition(p2_position);

    Eigen::Vector3d p2_velocity = {87.5, 100.0, 78.99};
    p2.SetVelocity(p2_velocity);
    REQUIRE( p2.GetVelocity().isApprox(p2_velocity, 0.001));

    Particle p3{1.0};
    Eigen::Vector3d p3_position = {4.55, 8.79, 10.5};
    p3.SetPosition(p3_position);
    Eigen::Vector3d p3_velocity = {5.5, 5.05, 4.41};
    p3.SetVelocity(p3_velocity);
    REQUIRE( p3.GetVelocity().isApprox(p3_velocity, 0.001));
}

// tests for IsMoving
TEST_CASE( "IsMoving function states incorrectly whether particle is moving", "[particle_moving]")
{    
    Particle p_moving{50.0};
    Eigen::Vector3d moving_vel = {33.3, 33.3, 33.3};
    p_moving.SetVelocity(moving_vel);
    REQUIRE( p_moving.IsMoving() == true );

    Particle p_freeze{9.0};
    Eigen::Vector3d zero_vel = {0.0, 0.0, 0.0};
    p_freeze.SetVelocity(zero_vel);
    REQUIRE( p_freeze.IsMoving() == false);

    Particle p_slow{1.1};
    Eigen::Vector3d slow_vel = {0.0002, 0.0004, 0.0006};
    p_slow.SetVelocity(slow_vel);
    REQUIRE( p_slow.IsMoving() == true);
}

// tests for acceleration
TEST_CASE( "Particle sets acceleration incorrectly", "[particle_acceleration]" ) 
{   

    Particle p_init{44.44};
    REQUIRE(p_init.GetAcceleration().isZero());

    Particle p1{500.0};
    Eigen::Vector3d p1_position = {7.8, 9.9, 6.4};
    p1.SetPosition(p1_position);
    Eigen::Vector3d p1_velocity = {1.2, 2.4, 7.8};
    p1.SetVelocity(p1_velocity);
    Eigen::Vector3d p1_acc = {7.8,9.7,4.4};
    p1.SetAcceleration(p1_acc);
    REQUIRE( p1.GetAcceleration().isApprox(p1_acc, 0.001));

    Particle p2{1000.1};
    Eigen::Vector3d p2_position = {99.99, 88.88, 77.77};
    p2.SetPosition(p2_position);
    Eigen::Vector3d p2_velocity = {87.5, 100.0, 78.99};
    p2.SetAcceleration(p2_velocity);
    Eigen::Vector3d p2_acc = {1.1,7.56,10.4};
    p2.SetAcceleration(p2_acc);
    REQUIRE( p2.GetAcceleration().isApprox(p2_acc, 0.001));

    Particle p3{0.01};
    Eigen::Vector3d p3_position = {20.3, 20.3, 20.3};
    p3.SetPosition(p3_position);
    Eigen::Vector3d p3_velocity = {20.3, 20.3, 20.3};
    p3.SetVelocity(p3_velocity);
    Eigen::Vector3d p3_acc = {20.3, 20.3, 20.3};
    p3.SetAcceleration(p3_acc);
    REQUIRE( p3.GetAcceleration().isApprox(p3_acc, 0.001));
}

// tests for IsAccelerating
TEST_CASE( "IsAccelerating function states incorrectly whether particle is accelerating", "[particle_accelerating]")
{    
    Particle p_no_acc{50.0};
    REQUIRE( p_no_acc.IsAccelerating() == false);

    Particle p_slow_acc{78.0};
    Eigen::Vector3d vel = {7.5, 8.9, 4.7};
    p_slow_acc.SetVelocity(vel);
    Eigen::Vector3d slow_acc = {0.003, 0.009, 0.006};
    p_slow_acc.SetAcceleration(slow_acc);
    REQUIRE( p_slow_acc.IsAccelerating());

    Particle p_some_acc{189.71};
    Eigen::Vector3d p_acc = {78.3, 559.65, 5.6};
    p_some_acc.SetAcceleration(p_acc);
    REQUIRE( p_some_acc.IsAccelerating());
}

// testing Particle class, confirming that a particle moves as it should using the Update() function


// 1. There is no acceleration \vec{a} = \vec{0}.
TEST_CASE( "Particle moves incorrectly when there is no acceleration", "[particle_update_no_acc]" ) 
{   
    double dt = 2.0;

    // case when particle has no velocity, no acceleration
    Particle p_init{789.45};
    Eigen::Vector3d p_init_pos= {10.5, 56.87, 44.4};
    p_init.SetPosition(p_init_pos);
    // initially, there should be no velocity no acceleration
    REQUIRE(p_init.IsMoving()==false);
    REQUIRE(p_init.IsAccelerating()==false);

    // position should not change
    REQUIRE( p_init.GetPosition().isApprox(p_init_pos,0.001));
    
    // the update function should not have any affect
    p_init.Update(dt);
    REQUIRE(p_init.IsMoving()==false);
    REQUIRE(p_init.IsAccelerating()==false);

    REQUIRE(p_init.GetPosition().isApprox(p_init_pos,0.001));
    


    // case when particle has velocity, no acceleration
    Particle p1{59.65};
    Eigen::Vector3d position_const_vel = {10.0, 8.0, 5};
    p1.SetPosition(position_const_vel);
    Eigen::Vector3d p1_vel(2.3, 0.2, 10.4);
    p1.SetVelocity(p1_vel);

    // expected, correct values of updated position and velocity
    Eigen::Vector3d expected_position_p1 = p1.GetPosition() + dt*p1.GetVelocity();
    Eigen::Vector3d expected_velocity_p1 = p1.GetVelocity() + dt*p1.GetAcceleration();
    
    // Update() will change the position but not velocity, and has no acceleration
    // std::cout << "initial pos: " << p1.GetPosition() << std::endl;
    p1.Update(dt);
    REQUIRE(p1.GetPosition().isApprox(expected_position_p1, 0.001));
    REQUIRE(p1.GetVelocity().isApprox(expected_velocity_p1, 0.001));
    REQUIRE(p1.GetVelocity().isApprox(p1_vel, 0.001));
    REQUIRE(p_init.IsAccelerating()==false);
    // std::cout << "final pos: " << p1.GetPosition() << std::endl;

}

// 2. There is a constant acceleration.


TEST_CASE( "Particle moves incorrectly when there is constant acceleration", "[particle_update_const_acc]" ) 
{   
    double dt = 0.001;

    // case when particle has no velocity but constant acceleration
    Particle p_init{789.45};
    Eigen::Vector3d p_init_pos= {10.5, 56.87, 44.4};
    p_init.SetPosition(p_init_pos);
    // initially, there should be no velocity no acceleration

    REQUIRE(p_init.IsAccelerating()==false);
    // position should not change
    REQUIRE( p_init.GetPosition().isApprox(p_init_pos,0.001));
    
    // the update function should not have any affect
    p_init.Update(dt);
    REQUIRE(p_init.IsMoving()==false);
    REQUIRE(p_init.IsAccelerating()==false);
    REQUIRE(p_init.GetPosition().isApprox(p_init_pos,0.001));


    // case when particle has velocity and constant acceleration
    Particle p1{59.65};
    Eigen::Vector3d position_const_vel = {10.0, 8.0, 5};
    p1.SetPosition(position_const_vel);
    Eigen::Vector3d p1_vel(2.3, 0.2, 10.4);
    p1.SetVelocity(p1_vel);

    // expected, correct values of updated position and velocity
    Eigen::Vector3d expected_position_p1 = p1.GetPosition() + dt*p1.GetVelocity();
    Eigen::Vector3d expected_velocity_p1 = p1.GetVelocity() + dt*p1.GetAcceleration();
    
    // Update() will change the position but not velocity, and has constant acceleration
    p1.Update(dt);
    REQUIRE(p1.GetPosition().isApprox(expected_position_p1, 0.001));
    REQUIRE(p1.GetVelocity().isApprox(expected_velocity_p1, 0.001));
    REQUIRE(p1.GetVelocity().isApprox(p1_vel, 0.001));
    REQUIRE(p_init.IsAccelerating()==false);
}

// 3. When an artificial acceleration \vec{a} = −\vec{x} is applied. 
// updating the acceleration as \vec{x} changes during the simulation
TEST_CASE( "Particle moves incorrectly when there is artificial acceleration", "[particle_update_artificial_acc]" ) 
{   
    double dt = 0.0001;
    Particle target_particle{40.5};

    //setting \vec{x} = {1,0,0}
    target_particle.SetPosition(Eigen::Vector3d {1,0,0});
    Eigen::Vector3d initial_position_target = target_particle.GetPosition();
    //setting \vec{v} = {0,1,0}
    target_particle.SetVelocity(Eigen::Vector3d {0,1,0});

    //setting \vec{a} = {0,-1,0} = -\vec{x}
    target_particle.SetAcceleration(Eigen::Vector3d {0,-1,0});

    // integrating system from 0 to 2\pi in steps of dt
    target_particle.TotalCircularUpdate( 2 * M_PI, dt);

    // particle's final position should be approximately be equal to its initial position
    REQUIRE(target_particle.GetPosition().isApprox(initial_position_target, 0.001));
}

// 
// tests for CalcAcceleration

// Testing the acceleration code

// 1. The gravitational force between two particles is as expected. 
// You should calculate a few test cases by hand then encode them in a unit test.
TEST_CASE( "Wrong calculation for gravitational force between two particles", "[particle_acceleration_grav_force]" ) 
{ 
    Particle p1{1544.7};
    Particle p2{6857.4};

    Eigen::Vector3d p1_position = {67,89,15};
    Eigen::Vector3d p2_position = {5,4,1};

    p1.SetPosition(p1_position);
    p2.SetPosition(p2_position);

    // values from CalcAcceleration function
    auto a_21 = p1.CalcAcceleration(p1, p2, 0.00001);
    auto a_12 = p2.CalcAcceleration(p2, p1, 0.00001);

    // expected values from formula
    auto expected_a_21 = ( p2.GetMass() * (p2_position - p1_position) ) / (pow( pow((p1_position-p2_position).norm(), 2) + pow(0.00001, 2) , 1.5)) ;
    auto expected_a_12 = ( p1.GetMass() * (p1_position - p2_position) ) / (pow( pow((p2_position-p1_position).norm(), 2) + pow(0.00001, 2) , 1.5)) ;

    REQUIRE(p1.GetMass() * a_21 == p1.GetMass() * a_21);
    REQUIRE(p2.GetMass() * a_12 == p2.GetMass() * a_12);
}

// 2. Providing a list of particles that includes itself to a particle will not add any acceleration.
TEST_CASE( "The list of particles that includes itself to a particle incorrectly adds acceleration", "[particle_acceleration_not_include]" ) 
{ 
    Particle init_p{1.6};
    Particle p1{40.1};
    Particle p2{60.7};
    Particle p3{982.03};
    Particle p4{34.65};

    double epsilon = 0.0001;

    p1.SetPosition(Eigen::Vector3d {1,1,6});
    p2.SetPosition(Eigen::Vector3d {5,10,3});
    p3.SetPosition(Eigen::Vector3d {3,5,7});
    p4.SetPosition(Eigen::Vector3d {7,9,5});

    std::vector<Particle> particle_list = { p1, p2, p3, p4 };

    auto a0 = p1.CalculateTotalAcceleration(particle_list, 0, epsilon);
    REQUIRE( a0.isApprox(p1.GetAcceleration(),0.0001) );

    auto a3 = p4.CalculateTotalAcceleration(particle_list, 3, epsilon);
    REQUIRE( a3 == p4.GetAcceleration() );


}


// 3. If a particle has two equally sized particles an equal distance either side of it 
// (e.g. at \vec{x1} = (1, 0, 0) and \vec{x2} = (−1, 0, 0)) your code correctly calculates zero acceleration.

TEST_CASE( "Particle does not compute zero acceleration when two equally sized particle are on the either side of a particle", "[particle_acceleration_equal_size]" ) 
{   
    auto epsilon = 0.;

    Particle p_centre{2023.4};
    Particle p1{108.8};
    Particle p2{108.8};

    p1.SetPosition(Eigen::Vector3d {5,0,0});
    p2.SetPosition(Eigen::Vector3d {-5,0,0});

    std::vector<Particle> particle_list_12 = { p_centre, p1, p2 };

    auto a_centre_12 = p_centre.CalculateTotalAcceleration(particle_list_12, 0, epsilon);
    
    REQUIRE(a_centre_12.isApprox(Eigen::Vector3d {0,0,0}, 0.01));

    Particle p3{10.12};
    Particle p4{10.12};
    
    p3.SetPosition(Eigen::Vector3d {44,55,66});
    p4.SetPosition(Eigen::Vector3d {-44,-55,-66});

    std::vector<Particle> particle_list_34 = { p_centre, p3, p4 };

    auto a_centre_34 = p_centre.CalculateTotalAcceleration(particle_list_34, 0, epsilon);

    // std::cout << a_centre_34 << "acceleration unit test" << std::endl;
    REQUIRE(a_centre_34.isApprox(Eigen::Vector3d {0,0,0}, 0.001));
}

// testing GetMasses()
TEST_CASE( "SolarSystem class does not get the masses correctly", "[SS_get_masses]" ) 
{   
    std::vector<double> expected_mass_list = {1., 1./6023600, 1./408524, 1./332946.038, 1./3098710, 1./1047.55, 1./3499, 1./22962, 1./19352};

    SolarSystemGenerator ssgen;
    auto system = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system);

    auto mass_list = solar_system.GetMasses();
    REQUIRE(mass_list == expected_mass_list);
}

// testing GetDistances()
TEST_CASE( "SolarSystem class does not get the distances correctly", "[SS_get_distances]" ) 
{   
    std::vector<double> expected_distance_list = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    
    SolarSystemGenerator ssgen;
    auto system = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system);

    std::vector<double> distance_list = solar_system.GetDistances();
    for (int i = 0; i < distance_list.size(); i++) 
    {
        REQUIRE_THAT(distance_list[i], WithinRel(expected_distance_list[i], 0.001));
    }
}

// testing GetDistances()
TEST_CASE( "SolarSystem class does not get the names of the solar system bodies correctly", "[SS_get_names]" ) 
{   
    std::vector<std::string> expected_bodies_list = {"Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Pluto"};
    
    SolarSystemGenerator ssgen;
    auto system = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system);

    auto bodies_list = solar_system.GetNames();
    REQUIRE(bodies_list == expected_bodies_list);
}

// testing the SolarSystem initial condition generator

// the sun is not moving in the centre
TEST_CASE( "SolarSystem class does not set the Sun correctly", "[SS_setting_sun]" ) 
{   

    SolarSystemGenerator ssgen;
    auto system_list = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system_list);

    auto sun_position = system_list[0].GetPosition();
    REQUIRE(sun_position.isApprox(Eigen::Vector3d {0,0,0}, 0.0001));

    auto sun_velocity = system_list[0].GetVelocity();
    REQUIRE(sun_velocity.isApprox(Eigen::Vector3d {0,0,0}, 0.0001));

}

// can get the mass of specified planets
TEST_CASE( "SolarSystem class cannot get the mass of specific planets correctly", "[SS_get_mass_planets]" ) 
{   
    SolarSystemGenerator ssgen;
    auto system_list = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system_list);

    std::vector<double> expected_mass_list = {1., 1./6023600, 1./408524, 1./332946.038, 1./3098710, 1./1047.55, 1./3499, 1./22962, 1./19352};

    double venus_mass = system_list[2].GetMass();
    REQUIRE(venus_mass == expected_mass_list[2]);

    double pluto_mass = system_list[8].GetMass();
    REQUIRE(pluto_mass == expected_mass_list[8]);

    double saturn_mass = system_list[6].GetMass();
    REQUIRE(saturn_mass == expected_mass_list[6]);

}

// can get the distances of specified planets
TEST_CASE( "SolarSystem class cannot get the distance of specific planets correctly", "[SS_get_distance_planets]" ) 
{   
    float margin = 0.01;

    SolarSystemGenerator ssgen;
    auto system_list = ssgen.GenerateInitialConditions();
    SolarSystem solar_system(system_list);

    std::vector<double> expected_distance_list = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};

    float mars_position = system_list[4].GetPosition().norm();
    REQUIRE_THAT(mars_position, WithinAbs(expected_distance_list[4], margin));

    double earth_position = system_list[3].GetPosition().norm();
    REQUIRE_THAT(earth_position, WithinAbs(expected_distance_list[3], margin));

    double uranus_position = system_list[6].GetPosition().norm();
    REQUIRE_THAT(uranus_position, WithinAbs(expected_distance_list[6], margin));
}

// different angles for different instances

TEST_CASE( "SolarSystem class does not generate different polar angles in different instances", "[SS_different_angles_planets]" ) 
{   
    float margin = 0.00001;    

    SolarSystemGenerator ssgen_1;
    auto system_list_1 = ssgen_1.GenerateInitialConditions();
    SolarSystem solar_system_1(system_list_1);

    SolarSystemGenerator ssgen_2;
    auto system_list_2 = ssgen_2.GenerateInitialConditions();
    SolarSystem solar_system_2(system_list_2);

    auto jupiter_position_1 = system_list_1[5].GetPosition();
    auto jupiter_position_2 = system_list_2[5].GetPosition();
    REQUIRE(jupiter_position_1.isApprox(jupiter_position_2, 0.0001) == false);
    REQUIRE_THAT(jupiter_position_1.norm(), WithinAbs(jupiter_position_2.norm(), margin));

    SolarSystemGenerator ssgen_3;
    auto system_list_3 = ssgen_3.GenerateInitialConditions();
    SolarSystem solar_system_3(system_list_3);
    
    SolarSystemGenerator ssgen_4;
    auto system_list_4 = ssgen_4.GenerateInitialConditions();
    SolarSystem solar_system(system_list_4);

    auto mercury_position_3 = system_list_3[1].GetPosition();
    auto mercury_position_4 = system_list_4[1].GetPosition();
    REQUIRE(mercury_position_3.isApprox(mercury_position_4, 0.0001) == false);
    REQUIRE_THAT(mercury_position_3.norm(), WithinAbs(mercury_position_4.norm(), margin));
}

// 