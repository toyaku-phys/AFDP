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
#include "ReadFile.hpp"
#include "LoadParticles.hpp"
#include "Kahan_summation_algorithm.hpp"
std::mt19937_64 mt(456981365819651);

constexpr double Radius_of_Sphere     = 10.0;
constexpr int    Num_Sampling_Points  = 10;
constexpr double Sampling_R_Threthold = 6.0;

void projection(std::vector<Vector3D>& ps);
Vector3D random_base_point();
inline int c(const Vector3D& center, const double dtheta, const std::vector<Vector3D>& ps);

int main(int argc, char* argv[])
{
   std::vector<std::string> argments;
   for(int i=0;i<argc;++i){ argments.push_back(argv[i]); }
   std::string input_file;//TODO set using argments
   int begin_step=1000;//   TODO set using argments
   int delta_step=1000;//   TODO set using argments
   double dtheta = 0.15;//  TODO set using argments
   Getline gl(input_file);
   const std::vector<Vector3D> sampling_points = 
      [&]()
      {
         std::vector<Vector3D> res(Num_Sampling_Points);
         for(int i=0;i<Num_Sampling_Points;++i)
         {
            res.at(i)=random_base_point();
         }
         return res;
      }();

   for(int step = begin_step; ;step+=delta_step)
   {
      std::vector<Vector3D> particles = load(gl,step,Sampling_R_Threthold);
      if(particles.empty()){break;}
      projection(particles);
      for(int sp=0,sp_size=sampling_points.size();sp<sp_size;++sp)
      {
         const int c_res = c(sampling_points.at(sp),dtheta,particles);
      }
   }
  
   return EXIT_SUCCESS;
}

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

int c(const Vector3D& center, const double dtheta, const std::vector<Vector3D>& ps)
{
   const auto is_on_range =
      [&](const Vector3D& p)-> bool
      {
         return angle(center,p)<=dtheta;
      };
   int res=0;
   for(size_t i=0,i_size=ps.size();i<i_size;++i)
   {
      res+=(is_on_range(ps.at(i)))?1:0;
   }
   return res;
}
