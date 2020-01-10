#include "CarMap.h"

#include <iostream>
#include <string>


CarMap::CarMap()
{
    Map m_carMap;
}


bool CarMap::addCar(std::string license)
{
    // Returns true or false based on whether carMap was able to insert a new car with a given license in the map
    return (m_carMap.insert(license, 0));
}


double CarMap::gas(std::string license) const
{
    double gallons;
    
    if (m_carMap.contains(license))
    {
        m_carMap.get(license, gallons);
        return gallons;
    }
    
    return -1;
}


bool CarMap::addGas(std::string license, double gallons)
{
    if (! m_carMap.contains(license) || gallons < 0)
        return false;
    
    double totalGas = gas(license) + gallons;
    m_carMap.update(license, totalGas);
    
    return true;
}


bool CarMap::useGas(std::string license, double gallons)
{
    if (! m_carMap.contains(license) || gallons < 0 || gallons > gas(license) )
        return false;
    
    double gasLeft = gas(license) - gallons;
    m_carMap.update(license, gasLeft);
    
    return true;
}


int CarMap::fleetSize() const
{
    return m_carMap.size();
}


void CarMap::print() const
{
    double gas;
    std::string license;
    
    for (int k = 0; k < m_carMap.size(); k++)
    {
        m_carMap.get(k, license, gas);
        std::cout << license << " " << gas << std::endl;
    }
}

