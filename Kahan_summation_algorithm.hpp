#ifndef KSA_H
#define KSA_H

class Kahan
{
   public:
      double sum;
      double c;
      Kahan();
      Kahan(const double& val);
      Kahan& operator+=(const double& val);
};

Kahan::Kahan()
{
   sum = 0.0;
   c   = 0.0;
}

Kahan::Kahan(const double& val)
{
   sum = val;
   c   = 0.0;
}

#pragma clang optimize off
inline Kahan& Kahan::operator+=(const double& val)
{
   const auto y = val - c;
   const auto t = (this->sum) + y;
   this->c      = (t-(this->sum))-y;
   this->sum    = t;
   return *this;
}
#pragma clang optimize on

#endif
