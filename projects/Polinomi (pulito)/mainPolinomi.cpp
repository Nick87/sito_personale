#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include "Polinomio.h"

/////////PROTOTIPI

Polinomio moltiplica(Polinomio**, int);
Polinomio somma(Polinomio**, int);

int main(int argc, char *argv[])
{
 	Polinomio ** polinomi;
	int num_polinomi = 0;
	cout << "Su quanti polinomio vuoi operare? (minimo 2): " << endl;
	cin >> num_polinomi;
	polinomi = new Polinomio * [num_polinomi];
	cout << "\t\t ### Prodotto e somma tra " << num_polinomi << " polinomi ###" << endl << endl;
	for (int i = 0; i < num_polinomi; i++)
	{
	 	int dim;
		cout << "Numero monomi del polinomio " << i + 1 << ": ";
		cin >> dim;
	 	polinomi[i] = new Polinomio(dim);
	 	cout << "--> Creazione polinomio " << i + 1 << endl;
	 	for (int j = 0; j < dim; j++)
		{
			 int coeff, esp;
			 cout << "Coefficiente monomio " << j + 1 << ": ";
			 cin >> coeff;
			 cout << "Esponente monomio " << j + 1 << ": ";
			 cin >> esp;
			 polinomi[i]->add_Monomio(coeff, esp);
			 cout << endl;
	    }
	    cout << "Polinomio creato:" << endl;
	    cout << *polinomi[i] << endl;
	    system("pause");
	    cout << "\r\n";
	}
	
	//Stampa polinomio prodotto (già semplificato)
	cout << "Polinomio prodotto:\n";
	for (int i = 0; i < num_polinomi; i++)
	{
		cout << "(" << *polinomi[i] << ")";
		if (i != num_polinomi - 1)
		   cout << " * ";
    }
    cout << " = " << endl;
	cout << moltiplica(polinomi, num_polinomi) << endl << endl;;
	
	//Stampa polinomio somma (già semplificato)
	cout << "Polinomio somma:\n";
	for (int i = 0; i < num_polinomi; i++)
	{
		cout << "(" << *polinomi[i] << ")";
		if (i != num_polinomi - 1)
		   cout << " + ";
    }
    cout << " = " << endl;
	cout << somma(polinomi, num_polinomi) << endl;
	
	system("PAUSE");
    return EXIT_SUCCESS;
}

/******************************************************************************/

Polinomio moltiplica (Polinomio ** p, int num_polinomi)
{
    Polinomio temp;
    temp.add_Monomio(1,0);
	for (int i = 0; i < num_polinomi; i++)
	 	temp *= (*p[i]);
	return temp; 	
}

Polinomio somma (Polinomio ** p, int num_polinomi)
{
    Polinomio temp;
    temp.add_Monomio(0,0);
	for (int i = 0; i < num_polinomi; i++)
	 	temp += (*p[i]);
	return temp;
}
