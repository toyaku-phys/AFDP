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
inline double dS(const double& dtheta, const double& Radius);
inline double density(const Vector3D& center, const double dtheta, const std::vector<Vector3D>& ps);

int main(int argc, char* argv[])
{
   std::vector<std::string> argments;
   for(int i=0;i<argc;++i){ argments.push_back(argv[i]); }
   std::string input_file;
   std::string output_file="def.dat";
   int begin_step= 1000;
   int delta_step= 1000;
   double dtheta = 0.15;
   int num_sampling_loop = 10;

   for(size_t i=0,i_size=argments.size();i<i_size;++i)
   {
      std::vector<std::string> vs;
      boost::algorithm::split(vs,argments.at(i),boost::is_any_of("="));
      if("in" ==vs.at(0)){input_file =vs.at(1);}
      if("out"==vs.at(0)){output_file=vs.at(1);}
      if("begin_step"==vs.at(0)){begin_step=boost::lexical_cast<int>(vs.at(1));}
      if("delta_step"==vs.at(0)){delta_step=boost::lexical_cast<int>(vs.at(1));}
      if("delta_theta"==vs.at(0)){dtheta=boost::lexical_cast<double>(vs.at(1));}
      if("num_sampling_loop"==vs.at(0)){num_sampling_loop=boost::lexical_cast<int>(vs.at(1));}
   }
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

   std::ofstream ofs(output_file,std::ios::trunc);
   std::tuple<Kahan,Kahan> s2_stock;
   for(int sl=0;sl<num_sampling_loop;++sl)
   {
      std::vector<std::tuple<Kahan,Kahan> > sum_sum2;
      for(int step=begin_step; ;step+=delta_step)
      {
         std::vector<Vector3D> particles = load(gl,step,Sampling_R_Threthold);
         if(particles.empty()){break;}
         if(sum_sum2.empty()){sum_sum2.resize(particles.size());}

         projection(particles);
         for(int sp=0,sp_size=sampling_points.size();sp<sp_size;++sp)
         {
            const double x_res = density(sampling_points.at(sp),dtheta,particles);
            std::get<0>(sum_sum2.at(sp))+=x_res;
            std::get<1>(sum_sum2.at(sp))+=x_res*x_res;
         }
      }
      //output variance
      const auto s2 = 
         [](const std::tuple<Kahan,Kahan>& s_s2)->double
         {
            const double N = std::get<0>(s_s2).counter.convert_to<double>();
            return (std::get<1>(s_s2).sum/N)-(((std::get<0>(s_s2).sum)*(std::get<0>(s_s2).sum))/(N*N));
         };
      //結果の平均をプロット
      for(size_t i=0,i_size=sum_sum2.size();i<i_size;++i)
      {
         const auto res=s2(sum_sum2.at(i));
         std::get<0>(s2_stock) += res;
         std::get<1>(s2_stock) += res*res;
      }
      ofs<<sl<<" "<<(std::get<0>(s2_stock)).get_av()<<" "<<s2(s2_stock)<<std::endl;
   }
   ofs.close();
  
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

double dS(const double& dtheta, const double& Radius)
{
   return 2.0*M_PI*(1.0-std::cos(dtheta))*Radius*Radius;
}

double density(const Vector3D& center, const double dtheta, const std::vector<Vector3D>& ps)
{  //Rerative Number of
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
   const int Np = ps.size();
   return (4*M_PI*Radius_of_Sphere*Radius_of_Sphere*res)/(Np*dS(dtheta,Radius_of_Sphere));
}
