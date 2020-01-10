#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

const int INITIAL_NUM_BUCKETS = 100;

template <typename KeyType, typename ValueType>
class MyHash
{
public:
    
    MyHash(double maxLoadFactor = 0.5)
    : m_items(0), m_buckets(INITIAL_NUM_BUCKETS)
    {
        if (maxLoadFactor <= 0)     // If user passes in zero or negative value for maxLoadFactor, use 0.5
            m_maxLF = 0.5;
        else if (maxLoadFactor > 2) // Else if user passes in load factor greater than 2, use 2.0
            m_maxLF = 2.0;
        else                        // Else set load factor to the positive value the user entered
            m_maxLF = maxLoadFactor;
        
        hashTable = new Node*[INITIAL_NUM_BUCKETS];
        
        // Initialize the hashTable to be empty i.e. all buckets are nullptrs
        for (int k = 0; k < m_buckets; k++)
            hashTable[k] = nullptr;
    }
    
    ~MyHash()
    {
        for (int k = 0; k < m_buckets; k++)
        {
            Node* temp = hashTable[k];
            
            while (temp != nullptr)
            {
                Node* tempNext = temp->next;
                delete temp;
                temp = tempNext;
            }
        }
        
        delete [] hashTable;
    }
    
    void reset()
    {
        // Free all memory associated with the current hash table
        for (int k = 0; k < m_buckets; k++)
        {
            Node* temp = hashTable[k];
            
            while (temp != nullptr)
            {
                Node* tempNext = temp->next;
                delete temp;
                temp = tempNext;
            }
            
            hashTable[k] = nullptr;
        }
        delete [] hashTable;
        
        // Allocate a new empty hash table
        m_buckets = INITIAL_NUM_BUCKETS;
        hashTable = new Node*[m_buckets];
        for (int k = 0; k < m_buckets; k++)
            hashTable[k] = nullptr;
        m_items = 0;
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        unsigned int bucketNum = generateBucketNum(key);     // Hash to a specific bucket number

        if (hashTable[bucketNum] == nullptr)    // If the bucket is empty
        {
            hashTable[bucketNum] = new Node(key, value);    // Simply add the new node into the bucket
            m_items++;      // Increment the item count
        }
        else    // Else the bucket was not empty
        {
            Node* temp = hashTable[bucketNum];
            
            // Search through the bucket to find either a matching key or the last node
            while (temp->m_key != key && temp->next != nullptr)
                temp = temp->next;

            if (temp->m_key != key && temp->next == nullptr)     // If we did not find an existing key
            {
                Node* newNode = new Node(key, value);   // Create a new node and add it to the end
                temp->next = newNode;
                m_items++;      // Increment item count
            }
            
            else if (temp->m_key == key)   // If we found an existing key
                temp->m_value = value;
        }
        
        // Check to see if we exceeded our maximum load factor
        if (getLoadFactor() > m_maxLF)
        {
            // Keep pointer to old hash table and keep its old number of buckets
            Node** temp = hashTable;
            int oldNumBuckets = m_buckets;

            // Allocate a new hash table with double the buckets
            m_buckets *= 2;
            m_items = 0;
            hashTable = new Node*[m_buckets];
            for (int k = 0; k < m_buckets; k++)
                hashTable[k] = nullptr;
            
            // Move old hashTable's items to new hashTable
            newHashTable(temp, oldNumBuckets);
            
            // Delete the old hash table
            for (int k = 0; k < oldNumBuckets; k++)
            {
                Node* old = temp[k];
                
                while (old != nullptr)
                {
                    Node* tempNext = old->next;
                    delete old;
                    old = tempNext;
                }
            }
            delete [] temp;
        }
    }
    
    int getNumItems() const
    {
        return m_items;
    }
    
    double getLoadFactor() const
    {
        return (m_items / (m_buckets / 1.0));
    }

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const
    {
        unsigned int bucketNum = generateBucketNum(key);     // Hash to a specific bucket number
        
        Node* temp = hashTable[bucketNum];
        if (temp == nullptr)
            return nullptr;
        else
        {
            while (temp->m_key != key)  // If the key does not match
            {
                if (temp->next != nullptr)  // Move to the next node's key if possible
                    temp = temp->next;
                else                        // Otherwise, there are no more nodes thus no such key exists
                    return nullptr;
            }
            
            return &(temp->m_value);    // Otherwise, we found the matching key and return pointer to its value
        }
    }

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
    struct Node
    {
        Node(KeyType key, ValueType value)
        : m_key(key), m_value(value)
        {
            next = nullptr;
        }

        KeyType m_key;
        ValueType m_value;
        Node* next;
    };
    
    unsigned int generateBucketNum(const KeyType& k) const
    {
        unsigned int hash(const KeyType& l);    // Prototype
        unsigned int hashCode = hash(k);
        unsigned int bucketNum = hashCode % m_buckets;
        return bucketNum;
    }
    
    void newHashTable(Node** &oldHT, int oldNumB)
    {
        // Get all of current hashtable's associations and copy them into the new hashtable
        for (int k = 0; k < oldNumB; k++)
        {
            if (oldHT[k] == nullptr)
                continue;
            else
            {
                Node* temp = oldHT[k];
                while (temp != nullptr)
                {
                    associate(temp->m_key, temp->m_value);
                    temp = temp->next;
                }
            }
        }
    }
    
    int m_items;
    int m_buckets;
    double m_maxLF;
    Node** hashTable;   // Hashtable is a pointer to Node pointers
};

#endif  // MYHASH_INCLUDED
