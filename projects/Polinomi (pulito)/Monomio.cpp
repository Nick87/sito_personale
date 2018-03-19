#include <iostream>
using std::cout;

#include "Monomio.h"

Monomio::Monomio (int coef, int exp)
{
     coeff = coef;
     esp = exp;
}

Monomio Monomio::operator* (const Monomio& m)
{
	 return Monomio (coeff * m.coeff, esp + m.esp);	
}

Monomio Monomio::operator+ (const Monomio& m)
{
 	 return Monomio (coeff + m.coeff, esp);
}

bool Monomio::operator> (const Monomio& m)
{
 	 return esp > m.esp;
}

bool Monomio::operator< (const Monomio& m)
{
 	 return esp < m.esp;
}

bool Monomio::operator== (const Monomio& m)
{
 	 return esp == m.esp;
}

ostream& operator<< (ostream& out, const Monomio& m)
{
 	 if (m.coeff == m.esp && m.coeff == 1)
 	 	out << "x";
     else if (m.coeff == -1 && m.esp == 1)
     	out << "x"; 
     else if (m.coeff == 1 && m.esp == 0)
	 	out << "1"; 	
 	 else	
 	 {	
	    if (m.coeff != 1 && m.coeff != -1)
		   out << abs(m.coeff);
 	    if (m.esp == 1)
     	   out << "x";  
	    else if (m.esp != 0)
	       out << "x^" << m.esp;
     }
	 return out; 
}
