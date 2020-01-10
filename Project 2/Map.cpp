#include "Map.h"

#include <iostream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Map::Map()
{
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}


Map::~Map()
{
    if (! empty())  // Call destructor only if there are nodes in the map
    {
        if (head == tail)   // There is only one node in the map, delete it
            delete head;
        
        else    // There are more than one nodes, delete them
        {
            mapNode* temp = head;
            while (temp != nullptr)   // Iterate and delete the nodes
            {
                if (temp->next != nullptr)     // If we are not on the last node
                {
                    mapNode* p = temp->next;
                    delete temp;
                    temp = p;
                }
                else                           // Else we are on the last node, delete it and break
                {
                    delete temp;
                    break;
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// COPY CONSTRUCTOR / ASSIGNMENT OPERATOR //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Map::Map(const Map &other)
{
    m_size = 0;     // Initialize the new map's size to 0 and head/tail pointers to null
    head = nullptr;
    tail = nullptr;
    
    mapNode* temp = other.head;

    while (temp != nullptr)     // Iterate through other map's nodes
    {
        insert(temp->m_key, temp->m_value);     // Insert other map's values into the new map's
        temp = temp->next;
    }
}


Map& Map::operator= (const Map &other)
{
    if (this != &other)     // Check that we are not referencing the same map itself
    {
        mapNode* temp = head;
        while (temp != nullptr)     // While the map is not empty, delete each node
        {
            if (temp->next != nullptr)      // If we are not on the last node
            {
                mapNode* p = temp->next;
                delete temp;                // Delete node at temp and set temp to next node
                temp = p;
            }
            else                            // Else we are on the last node
            {
                delete temp;                // Delete the last node and set head/tail to nullptr and break
                head = nullptr;
                tail = nullptr;
                
                break;
            }
        }
        
        m_size = 0;         // Reset this map's size to 0
        
        temp = other.head;
        while (temp != nullptr)     // Iterate through other map's nodes
        {
            insert(temp->m_key, temp->m_value);     // Insert other map's nodes into this map's
            temp = temp->next;
        }
    }
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// ACCESSOR IMPLEMENTATIONS ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


bool Map::empty() const
{
    return (m_size == 0);
}


int Map::size() const
{
    return m_size;
}


bool Map::contains(const KeyType& key) const
{
    if (empty())    // Empty map has no nodes, thus no matches
        return false;
    
    mapNode* it = head;
    
    
    while (it != nullptr)   // Iterate through nodes and check m_key values
    {
        if (it->m_key == key)
            return true;    // Return true if there is a match
        
        it = it->next;
    }

    return false;    // No matches, thus return false
}


bool Map::get(const KeyType& key, ValueType& value) const
{
    if (empty())
        return false;
    
    mapNode* it = head;
    
    while (it != nullptr)   // Iterate through nodes
    {
        if (it->m_key == key)
        {
            value = it->m_value;    // Change the value to the matching key's value
            return true;            // and return true
        }
        
        it = it->next;
    }
    
    return false;   // No matches, thus do nothing and return false
}


bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (empty())
        return false;
    
    if (i >= 0 && i < size())
    {
        mapNode* temp = head;
        for (int k = 0; k < i; k++)     // Move temp to the targeted node
        {
            temp = temp->next;
        }
        
        key = temp->m_key;      // Once we reach targeted node, set key/value to the node's key/value
        value = temp->m_value;  // and return true
        return true;
    }
    
    return false;   // i is negative or i >= size(), thus we return false
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// MUTATOR IMPLEMENTATIONS /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (contains(key))
        return false;
    
    return (insertOrUpdate(key, value));
}


bool Map::update(const KeyType& key, const ValueType& value)
{
    if (! contains(key))
        return false;
    
    return (insertOrUpdate(key, value));
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    // Case of empty map: Insert first node
    if (empty())
    {
        m_size++;
        
        head = new mapNode;
        head->m_key = key;
        head->m_value = value;
        
        head->next = nullptr;    // There is only 1 node, thus next and prev point to nothing
        head->prev = nullptr;
        
        tail = head;     // head and tail point to the only node
        
        return true;
    }
    
    // Case non-empty, but matching: update value
    if (contains(key))
    {
        mapNode* temp = head;
        
        while (temp != nullptr)
        {
            if (temp->m_key == key)
            {
                temp->m_value = value;
            }
            
            temp = temp->next;
        }
        
        return true;
    }
    
    // Case non-empty & non-matching: insert new node at end
    m_size++;
    mapNode* temp = tail;

    temp->next = new mapNode;    // Create new node after tail and initialize it accordingly
    temp->next->m_key = key;
    temp->next->m_value = value;
    
    temp->next->prev = temp;    // Assign new node's next and prev pointers accordingly
    temp->next->next = nullptr; // and assign tail to the new node
    tail = temp->next;
    
    return true;
}


bool Map::erase(const KeyType& key)
{
    if (contains(key))
    {
        // Case of erasing the only node in the map
        if (m_size == 1)
        {
            mapNode* temp = head;
        
            if (temp->m_key == key)
            {
                head = nullptr;
                tail = nullptr;
            
                delete temp;
                m_size--;
                return true;
            }
        }
    
        mapNode* it = head;
        
        while (it != nullptr)
        {
            // Case of erasing the first node
            if (it == head && it != nullptr && it->m_key == key)
            {
                head = it->next;
                it->next->prev = nullptr;
                
                delete it;
                m_size--;
                return true;
            }
            
            // Case of erasing the last node
            else if (it == tail && tail != nullptr && it->m_key == key)
            {
                tail = it->prev;
                it->prev->next = nullptr;
                
                delete it;
                m_size--;
                return true;
            }
            
            // Case of erasing any node in between
            else if (it->m_key == key)
            {
                it->prev->next = it->next;
                it->next->prev = it->prev;
                
                delete it;
                m_size--;
                return true;
            }
            
            it = it->next;
        }
    }
    
    return false;
}


void Map::swap(Map& other)
{
    int tempSize = m_size;
    
    mapNode* tempH = head;      // Create pointers to this map's head and tail
    mapNode* tempT = tail;
    
    head = other.head;      // Set this map's head and tail to the other map's head and tail
    tail = other.tail;      // and change this map's size
    m_size = other.m_size;
    
    other.head = tempH;     // Set other map's head and tail to this map's original head and tail
    other.tail = tempT;     // and change this map's size
    other.m_size = tempSize;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// NON-MEMBER FUNCTIONS ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


bool combine(const Map& m1, const Map& m2, Map& result)
{
    Map temp_m1 = m1;   // Creating temp. copies of m1 and m2 to prevent unwanted behavior from aliasing
    Map temp_m2 = m2;
    
    if (! result.empty())   // If result is not empty
    {
        while (! result.empty())    // Erase all of result's nodes until empty
        {
            KeyType key;
            ValueType value;
        
            result.get(0, key, value);
            result.erase(key);
        }
    }

    bool no_matchedKey_diffValues = true;   // If a key appears in both m1 and m2, but w/ diff. values, set this to false
    
    for (int k = 0; k < temp_m1.size(); k++)     // Loop through m1
    {
        KeyType key;
        ValueType value;
        
        temp_m1.get(k, key, value);          // Get each of m1's pairs and check if m2 contains a matching key
        
        if (! temp_m2.contains(key))         // if no matching key, then insert to result
            result.insert(key, value);
        
        else                            // otherwise, check the matching keys' values
        {
            ValueType compareValue;
            temp_m2.get(key, compareValue);
            
            if (compareValue == value)      // If m2's pair matches m1's pair, then insert into result
                result.insert(key, value);
            else                            // Otherwise, set no_matchedKey_diffValues to false, indicating diff values w/ same key value
                no_matchedKey_diffValues = false;
        }
    }
    
    for (int k = 0; k < temp_m2.size(); k++)     // Loop through m2 to find the remaining non-matching pairs
    {
        KeyType key;
        ValueType value;
        
        temp_m2.get(k, key, value);
        
        if (! temp_m1.contains(key))             // Insert them into result
            result.insert(key, value);
    }
    
    return no_matchedKey_diffValues;
}


void subtract(const Map& m1, const Map& m2, Map& result)
{
    Map temp_m1 = m1;
    Map temp_m2 = m2;
    
    if (! result.empty())   // If result is not empty
    {
        while (! result.empty())    // Erase all of result's nodes until empty
        {
            KeyType key;
            ValueType value;
            
            result.get(0, key, value);
            result.erase(key);
        }
    }

    for (int k = 0; k < temp_m1.size(); k++)
    {
        KeyType key;
        ValueType value;
        
        temp_m1.get(k, key, value);
        
        if (! temp_m2.contains(key))
            result.insert(key, value);
    }
}


