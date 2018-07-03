#include "ReadFile.hpp"
#include "Vector3D.hpp"

int main(int argc, char* argv[])
{
   std::vector<std::string> argments;
   for(int i=1;i<argc;++i){ argments.push_back(argv[i]); }
   std::vector<std::tuple<std::string,double,int> > contents;
   const auto tag_picker = [](const std::string name)->std::string
   {
      const char A = 'b';
      const char B = 's';
      std::string res;
      bool f = true;
      for(size_t s=0,s_size=name.size();s<s_size;++s)
      {
         const char c = name.at(s); 
         if(f)
         {
            if(A==c)
            {
               f = false;
            }
         }
         else
         {
            if(B==c)
            {
               return res;
            }
            res+=name.at(s);
         }
      }
      return res;
   };

   const auto insert = [&](const std::string tag, const double val)->void
      {
         for(size_t i=0,i_size=contents.size();i<i_size;++i)
         {
            if(tag==std::get<std::string>(contents.at(i)))
            {
               std::get<double>(contents.at(i))+=val;
               std::get<int>(contents.at(i))   +=1;
               return ;
            }
         }
         contents.push_back
            (
               std::tuple<std::string,double,int>(tag,val,1) 
            );
         return ;
      };
   for(size_t i=0,i_size=argments.size();i<i_size;++i)
   {
      const std::string tag = tag_picker(argments.at(i));
      Getline gl(argments.at(i));
      std::string tmp;
      while(gl.is_open())
      {
         try{tmp=gl.get();}catch(...){break;}
         if(""==tmp){break;}
         std::vector<std::string> vs;
         boost::algorithm::split(vs,tmp,boost::is_any_of(" "));
         const double val = boost::lexical_cast<double>(vs.at(1));
         insert(tag,val);
      }
   }
   for(size_t i=0,i_size=contents.size();i<i_size;++i)
   {
      std::cout<<std::get<std::string>(contents.at(i))<<" "<<(std::get<double>(contents.at(i))/std::get<int>(contents.at(i)))<<std::endl;
   }
   return EXIT_SUCCESS;
}
