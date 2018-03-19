#ifndef POLINOMIO_H
#define POLINOMIO_H

#include "Monomio.h"

#include <iostream>
using std::ostream;

class Polinomio
{
 	  friend ostream& operator<< (ostream&, const Polinomio&);
	  public:
	  		 Polinomio(int = 10);
	  		 ~Polinomio();
	  		 Polinomio(const Polinomio&);
	  		 int get_num_monomi () const {return num_monomi;}
	  		 int get_size() const {return size;}
	  		 void add_Monomio (int = 0, int = 0);
	  		 Polinomio operator+(const Polinomio&);
	  		 Polinomio operator*(const Polinomio&);
	  		 Polinomio operator=(const Polinomio&);
	  		 Polinomio operator*=(const Polinomio&);
	  		 Polinomio operator+=(const Polinomio&);
      
	  private:
	  		  Monomio ** polinomio;
	  		  int num_monomi, size;
			  Polinomio semplifica(Polinomio&);
	  		  void ordina (Polinomio&);		  
};

#endif
