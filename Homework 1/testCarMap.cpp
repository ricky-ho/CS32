#include <iostream>
#include <cassert>
#include "CarMap.h"
using namespace std;

int main()
{
    CarMap m1;
    m1.addCar("12345");
    m1.addCar("24689");
    m1.addCar("CAREY");
    m1.addCar("CAREY");
    
    m1.addGas("12345", 10.5);
    m1.addGas("24689", 3.14);
    m1.addGas("CAREY", 1);
    
    m1.useGas("12345", 5.5);
    m1.useGas("24689", .14);
    m1.useGas("CAREY", 0);
    
    CarMap m2;
    m2.addCar("Smallberg");
    
    m1.print(); cout << endl;
    m2.print();

}
