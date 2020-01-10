#include "Map.h"

#include <iostream>


Map::Map()
{
    // Setting m_lastKey to -1 means there are no key/value pairs, as map will rely on m_lastKey to determine number of data pairs
    m_lastKey = -1;
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
    if (contains(key) || size() == DEFAULT_MAX_ITEMS)
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
    dataPair temp[DEFAULT_MAX_ITEMS];
    int originalSize = size();
    int otherSize = other.size();
    
    // Copy the other map's DataPairs into the temp array
    for (int k = 0; k < otherSize; k++)
    {
        temp[k].m_key = other.m_DataPairs[k].m_key;
        temp[k].m_value = other.m_DataPairs[k].m_value;
    }
    
    // Erase all of other map's DataPairs
    while (! other.empty())
    {
        KeyType tempKey;
        ValueType tempValue;
        
        other.get(0, tempKey, tempValue);
        other.erase(tempKey);
    }
    
    // Insert original map's DataPairs into other map's
    for (int k = 0; k < originalSize; k++)
    {
        other.insert(m_DataPairs[k].m_key, m_DataPairs[k].m_value);
    }
    
    // Erase all of the original map's DataPairs
    while (! empty())
    {
        KeyType tempKey;
        ValueType tempValue;
        
        get(0, tempKey, tempValue);
        erase(tempKey);
    }
    
    // Insert other map's DataPairs, which were stored in temp, into original map's
    for (int k = 0; k < otherSize; k++)
    {
        insert(temp[k].m_key, temp[k].m_value);
    }
}


