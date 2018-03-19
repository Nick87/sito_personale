#include "Polinomio.h"
#include <iostream>
using std::cout;
using std::endl;

Polinomio::Polinomio(int dim)
{
	 polinomio = new Monomio*[size = dim];
     for (int i = 0; i < size; i++)
     	 polinomio[i] = NULL;
     num_monomi = 0;
}

Polinomio::~Polinomio()
{
     for (int i = 0; i < num_monomi; i++)
     	 delete polinomio[i];
	 delete[] polinomio;
}

Polinomio::Polinomio(const Polinomio& p)
{
     num_monomi = p.num_monomi;
	 polinomio = new Monomio*[num_monomi];
	 for (int i = 0; i < num_monomi; i++)
	 	 polinomio[i] = new Monomio (p.polinomio[i]->get_coeff(),
		                             p.polinomio[i]->get_esp());
}

Polinomio Polinomio::operator=(const Polinomio& p)
{
 	for (int i = 0; i < num_monomi; i++)
     	delete polinomio[i];
	delete[] polinomio;
	polinomio = new Monomio*[size = p.num_monomi];
    for (int i = 0; i < size; i++)
     	polinomio[i] = NULL;
    num_monomi = p.num_monomi;
    for (int i = 0; i < size; i++)
		polinomio[i] = new Monomio (p.polinomio[i]->get_coeff(),
		                            p.polinomio[i]->get_esp());
	return *this;	                            
}

Polinomio Polinomio::operator*= (const Polinomio& p)
{
 	(*this) = (*this) * p;
	 return *this;	  
}

Polinomio Polinomio::operator+= (const Polinomio& p)
{
 	(*this)	= (*this) + p;
 	return *this;
}

void Polinomio::add_Monomio(int coef, int esp)
{
 	 if (num_monomi < size)
 	 	 polinomio[num_monomi++] = new Monomio (coef, esp);
 	 else
 	 {	 
	     Polinomio temp(*this);
 	     for (int i = 0; i < num_monomi; i++)
     	     delete polinomio[i];
	     delete[] polinomio;
	     polinomio = new Monomio * [num_monomi + 1];
	     for (int i = 0; i < num_monomi; i++)
         {
	  	     int coefficiente = temp.polinomio[i]->get_coeff();
	  	     int esponente = temp.polinomio[i]->get_esp();
	  	     polinomio[i] = new Monomio (coefficiente, esponente);
	     }
	     polinomio[num_monomi] = new Monomio (coef, esp);
	     num_monomi++;
	     size++;
     }
}

/* Creo un polinomio costituito da i monomi dei due polinomi in sequenza.
 * Poi semplifico tale polinomio
 */

Polinomio Polinomio::operator+(const Polinomio& p)
{
 	 int count1 = 0, count2 = 0, new_size = num_monomi + p.get_num_monomi();
	 Polinomio temp (new_size);
	 for (int i = 0; i < new_size; i++)
	 {
	  	 if (i < num_monomi)
	  	 {
			 int coefficiente = polinomio[count1]->get_coeff();
	  	     int esponente = polinomio[count1]->get_esp();
	  	     temp.add_Monomio(coefficiente, esponente);
	  	     count1++;
		 }
		 else
		 {
			 int coefficiente = p.polinomio[count2]->get_coeff();
	  	     int esponente = p.polinomio[count2]->get_esp();
	  	     temp.add_Monomio(coefficiente, esponente);
	  	     count2++;
		 }   
	 }
	 ordina(temp);
     return semplifica(temp);
}

Polinomio Polinomio::operator*(const Polinomio& p)
{
	 //Gestire caso di un polinomio fattore con un solo monomio
	 int new_size = num_monomi * p.num_monomi;
	 Polinomio temp (new_size);
     for (int i = 0; i < num_monomi; i++)
     {
     	 for (int j = 0; j < p.num_monomi; j++)
     	 {
			 Monomio m_temp = (*polinomio[i]) * (*p.polinomio[j]); //utilizzo operatore con overload
			 temp.add_Monomio(m_temp.get_coeff(), m_temp.get_esp());
	     }
	 }
	 if (new_size == 1)
         return temp;
     else
         return semplifica(temp);
}

/* Semplifica il polinomio sommando i monomi simili.
 * Viene utilizzato l'operatore == ridefinito per confrontare l'esponente
 * di due monomi*/

Polinomio Polinomio::semplifica(Polinomio& p)
{
	 ordina(p);
 	 int grado_max = p.polinomio[0]->get_esp();
 	 int grado_min = p.polinomio[num_monomi - 1]->get_esp();
 	 int new_size = grado_max - grado_min + 1;
 	 Polinomio temp(new_size);
 	 int somma = p.polinomio[0]->get_coeff();
 	 for (int i = 0; i < p.get_num_monomi(); i++)
 	 {
	  	 int actual_esp = p.polinomio[i]->get_esp();
		 if (i == p.get_num_monomi() - 1 && p.polinomio[i]->get_esp() != p.polinomio[i-1]->get_esp())
		  	 temp.add_Monomio(somma, actual_esp);
		 else if(*(p.polinomio[i]) == *(p.polinomio[i+1]))
		 {
			 somma += p.polinomio[i+1]->get_coeff();
		  	 if (i == p.get_num_monomi() - 2)
		  	 {
		  	 	temp.add_Monomio(somma, actual_esp);
		  	  	break;
			 }	
		 }  
		 else
		 {
			 temp.add_Monomio(somma, actual_esp);
			 somma = p.polinomio[i+1]->get_coeff();
		 }
	 }
	 /* Se il polinomio p non contiene i monomi di tutti i gradi compresi tra quello di grado minimo
	  * e quello di grado massimo, il polinomio temp risulterà "incompleto".
      * Ciò non crea alcun problema pratico perchè i monomi vengono aggiunti correttamente "da sinistra a
      * destra, si avrà solo della memoria inutilizzata, delle celle del polinomio vuoto, pari al numero
	  * di monomi mancanti*/
	 return temp;
}

/* Ordinamento decrescente del polinomio con l'algoritmo Bubble Sort*/
void Polinomio::ordina(Polinomio& p)
{
     for (int i = p.num_monomi - 1; i > 0 ; i--)
     {
         for(int j = 0; j < i; j++)
         {
             if(p.polinomio[j]->get_esp() < p.polinomio[j+1]->get_esp())
             {
				 Monomio * temp = p.polinomio[j];
				 p.polinomio[j] = p.polinomio[j+1];
				 p.polinomio[j+1] = temp;
	         }
	     }
     }
}

ostream& operator<< (ostream& out, const Polinomio& p)
{
	 bool check = true;   //Suppongo sia nulli tutti i coefficienti
	 if (p.num_monomi == 0)
	 {
	     out << "Polinomio nullo!" << endl;
		 return out;
	 }
			
     //Un polinomio è nullo anche quande tutti i suoi coefficienti sono 0
	 for (int i = 0; i < p.num_monomi; i++)
		if (p.polinomio[i]->get_coeff() != 0)
			check = false;
		
     if (check)
	 {
		out << "Polinomio nullo!" << endl;
		return out;
	 }
	 else
	 {
	    for (int i = 0; i < p.num_monomi; i++)
	    {
	  	    if (i == 0 && p.polinomio[i]->get_coeff() < 0)
	  	       out << "- ";
			else if (i == 0 && p.polinomio[i]->get_coeff() > 0)
			   out << "";
	  	    else if (p.polinomio[i]->get_coeff() == 0)
			   continue;
	  	    else if (p.polinomio[i]->get_coeff() > 0 && i != 0)
	  	       out << " + ";
	        else
	           out << " - ";	
			out << *p.polinomio[i];
        }
        return out;
     }   
}
