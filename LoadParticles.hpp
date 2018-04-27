#ifndef LOAD_PARTICLES_H
#define LOAD_PARTICLES_H
#include "ReadFile.hpp"
#include "Vector3D.hpp"

std::vector<Vector3D> load(Getline& gl, const int step, const double r_threthold)
{
   std::vector<Vector3D> res;
   const std::string str_step=boost::lexical_cast<std::string> (step);
   while(gl.is_open())
   {
      std::string tmp;
      try{tmp=gl.get();}catch(...){/*eof*/break;}
      std::vector<std::string> vs;
      boost::algorithm::split(vs,tmp,boost::is_any_of(" "));
      if("#"==vs.at(0) && "Bead"==vs.at(1) && str_step==vs.at(2))
      {
         while(gl.is_open())
         {
            try{tmp=gl.get();}catch(...){/*eof*/break;}
            if(""==tmp){break;}
            std::vector<std::string> vs_;
            boost::algorithm::split(vs_,tmp,boost::is_any_of(" "));
            Vector3D vtmp(boost::lexical_cast<double> (vs_.at(1)),boost::lexical_cast<double> (vs_.at(2)),boost::lexical_cast<double> (vs_.at(3)));
            if(vtmp.norm()<=r_threthold){res.push_back(vtmp);}
         }
         break;
      }
   }
   return res;
}


#endif
