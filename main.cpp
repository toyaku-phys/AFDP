#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <thread>
#include <boost/lexical_cast.hpp>
#include "Vector3D.hpp"
#include "constexpr_math.hpp"
std::mt19937_64 mt(456981365819651);

constexpr double Radius_of_Sphere = 10.0;

void projection(std::vector<Vector3D>& ps)
{
   for(size_t i=0,i_size=ps.size();i<i_size;++i)
   {
      ps.at(i)/=ps.at(i).norm();
      ps.at(i)*=Radius_of_Sphere;
   }
   return ;
}

Vector3D random_base_point()
{
   std::uniform_real_distribution<double> dist(-1.0, +1.0);
   Vector3D res;
   do
   {
      res.x=dist(mt); 
      res.y=dist(mt); 
      res.z=dist(mt); 
   }while(res.norm()>1.0);
   res/=res.norm();
   res*=Radius_of_Sphere;
   return res;
}

int main(int argc, char* argv[])
{
   std::vector<std::string> argments;
   for(int i=0;i<argc;++i){ argments.push_back(argv[i]); }
  
   return EXIT_SUCCESS;
}
