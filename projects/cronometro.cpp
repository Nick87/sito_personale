#include <cstdlib>
#include <iostream>
#include <ctime>
using std::cin;
using std::cout;
using std::endl;


void cronometro(int sec_max, int step = 1)
{
    int counter = 0;
    
    if (sec_max % step != 0)
    {
        cout << "Non ti assicuro una corretta visualizzazione!" << endl;
    	system("PAUSE");
    }
    system("cls");
    cout << counter;
    while (counter < sec_max)
    {
          int a = time(0);
          int b;
          do
          {
                   b = time(0);
          }
          while (b - a != step);
          counter += step;
          system("cls");
          cout << counter;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    int sec_max, step;
    
    do
    {
          cout << "Quanti secondi devo contare?: ";
          cin >> sec_max;
          cout << "Ad intervalli di?: ";
          cin >> step;
          
          if (step <= sec_max)
                   cronometro(sec_max, step);  
          else
                   cout << "Vuoi fare il furbo..." << endl << endl;
    }
    while (step > sec_max);
        
    system("PAUSE");
    return EXIT_SUCCESS;
}
