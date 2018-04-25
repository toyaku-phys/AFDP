#ifndef LOAD_PARTICLES_H
#define LOAD_PARTICLES_H
#include "ReadFile.hpp"
#include "Vector3D.hpp"

std::vector<Vector3D> load(Getline& gl, const int step, const double r_threthold)
{
   std::vector<Vector3D> res;
   const std::string str_step = boost::lexical_cast<std::string>(step);

   std::string tmp;
   std::vector<std::string> vs; 
   while(gl.is_open())
   {
      tmp=gl.get();
      if('#'==tmp.at(0))
      {
         boost::algorithm::split(vs,tmp,boost::is_any_of(" "));
         if(str_step==vs.at(2))//0# 1Bead 2step
         {
            //load and cast
            while(gl.is_open())
            {
               tmp=gl.get();
               if(""==tmp)
               {
                  break;
               }
               boost::algorithm::split(vs,tmp,boost::is_any_of(" "));//index x y z
               Vector3D vtmp(boost::lexical_cast<double> (vs.at(1)),boost::lexical_cast<double> (vs.at(2)),boost::lexical_cast<double> (vs.at(3)));
               if(vtmp.norm()>=r_threthold){res.push_back(vtmp);}
            }
         }
         break;
      }
   }
   return res;
}


#endif
