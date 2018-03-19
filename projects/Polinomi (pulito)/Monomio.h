#ifndef MONOMIO_H
#define MONOMIO_H

#include <iostream>
using std::ostream;

class Monomio
{
 	  friend ostream& operator<< (ostream&, const Monomio&);
	  public:
	  		 Monomio (int = 0, int = 0);
	  		 int get_coeff() const {return coeff;}
	  		 int get_esp() const {return esp;}
	  		 Monomio operator*(const Monomio&);
	  		 Monomio operator+(const Monomio&);
	  		 bool operator>(const Monomio&);
	  		 bool operator<(const Monomio&);
	  		 bool operator==(const Monomio&);

      private:
	  		  int coeff;
	  		  int esp;
};

#endif
