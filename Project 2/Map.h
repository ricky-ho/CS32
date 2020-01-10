#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
    
    // Constructor/Destructor
    Map();
    ~Map();
    
    // Copy Constructor/ Assignment Operator
    Map(const Map &other);
    Map& operator= (const Map &other);
    
    // Accessors
    bool empty() const;
    int size() const;
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    
    // Mutators
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    void swap(Map& other);

    //void dump() const;
    
private:
    
    struct mapNode
    {
        KeyType m_key;
        ValueType m_value;
        
        mapNode* next;
        mapNode* prev;
    };
    
    int m_size;
    mapNode* head;
    mapNode* tail;
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);


#endif
