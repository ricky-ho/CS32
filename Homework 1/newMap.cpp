#include "newMap.h"

#include <iostream>
#include <cstdlib>
using namespace std;

//
void Map::dump()
{
    for (int k = 0; k < size(); k++)
    {
        cout << m_DataPairs[k].m_key << " " << m_DataPairs[k].m_value << endl;
        
    }
    cout << "---- " << m_size << " " << m_lastKey << endl << endl;
}
//

Map::Map()
{
     // Setting m_lastKey to -1 means there are no key/value pairs, as map will rely on m_lastKey to determine number of data pairs
    m_size = DEFAULT_MAX_ITEMS;
    m_lastKey = -1;
    
    m_DataPairs = new dataPair[DEFAULT_MAX_ITEMS];
   
}

Map::Map(int size)
{
    if (size < 0)
    {
        std::cout << "INVALID: A map cannot be created with a negative size!" << std::endl;
        exit(1);
    }
    
    m_size = size;
    m_lastKey = -1;
    
    m_DataPairs = new dataPair[m_size];
}


Map::Map(const Map& other)
{
    m_size = other.m_size;
    m_lastKey = other.m_lastKey;
    
    m_DataPairs = new dataPair[m_size];
    
    for (int k = 0; k < size(); k++)
    {
        m_DataPairs[k].m_key = other.m_DataPairs[k].m_key;
        m_DataPairs[k].m_value = other.m_DataPairs[k].m_value;
    }
}


Map& Map::operator= (const Map &other)
{
    if (this != &other)
    {
        delete [] m_DataPairs;
        m_size = other.m_size;
        m_lastKey = other.m_lastKey;
    
        m_DataPairs = new dataPair[m_size];
    
        for (int k = 0; k < size(); k++)
        {
            m_DataPairs[k].m_key = other.m_DataPairs[k].m_key;
            m_DataPairs[k].m_value = other.m_DataPairs[k].m_value;
        }
    }
    
    return (*this);
}


Map::~Map()
{
    delete [] m_DataPairs;
}


bool Map::empty() const
{
    return (size() == 0);
}


int Map::size() const
{
    return (m_lastKey + 1);
}


bool Map::insert(const KeyType& key, const ValueType& value)
{
    // If the map already contains the key or map is full, do nothing and return false
    if (contains(key) || size() == m_size)
        return false;
    
    m_lastKey++;
    m_DataPairs[m_lastKey].m_key = key;
    m_DataPairs[m_lastKey].m_value = value;
    
    return true;
}


bool Map::update(const KeyType& key, const ValueType& value)
{
    for (int k = 0; k < size(); k++)
    {
        if (m_DataPairs[k].m_key == key)
        {
            m_DataPairs[k].m_value = value;
            return true;
        }
    }
    return false;
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    // If there are no matching keys, then we try to insert the key/value pair
    if (! update(key, value))
    {
        // If cannot insert, we return false
        if (! insert(key,value))
            return false;
    }
    
    // If compiler reaches this point, then either update or insert worked and we return true
    return true;
}


bool Map::erase(const KeyType& key)
{
    // If the map is empty, then there is nothing to erase and we return false
    if (empty())
        return false;
    
    for (int k = 0; k < size(); k++)
    {
        if (m_DataPairs[k].m_key == key)
        {
            // If the matching key is the last key in the map, then we remove access to that data by decrementing m_lastKey
            if (k == size()-1)
            {
                m_lastKey--;
                return true;
            }
            
            // Otherwise, we erase the matching key by shifting the entire array of dataPairs left and decrement m_lastKey
            for (int x = k; x < size()-1; x++)
            {
                m_DataPairs[x].m_key = m_DataPairs[x+1].m_key;
                m_DataPairs[x].m_value = m_DataPairs[x+1].m_value;
            }
            
            m_lastKey--;
            return true;
        }
    }
    
    // At this point, there are no matching keys so we do nothing and return false
    return false;
}


bool Map::contains(const KeyType& key) const
{
    // Increment to find matching key
    for (int k = 0 ; k < size() ; k++)
    {
        if (m_DataPairs[k].m_key == key)
            return true;
    }
    return false;
}


bool Map::get(const KeyType& key, ValueType& value) const
{
    // Increment through map's DataPairs to find matching key
    for (int k = 0; k < size(); k++)
    {
        if (m_DataPairs[k].m_key == key)
        {
            // Set value parameter to the map's DataPair value
            value = m_DataPairs[k].m_value;
            return true;
        }
    }
    
    return false;
}


bool Map::get(int i, KeyType& key, ValueType& value) const
{
    // Set key and value parameter to map's ith DataPair key and value
    if (i >= 0 && i < size())
    {
        key = m_DataPairs[i].m_key;
        value = m_DataPairs[i].m_value;
        return true;
    }
    return false;
}


void Map::swap(Map& other)
{
    Map temp = *this;
    
    *this = other;
    other = temp;
}
