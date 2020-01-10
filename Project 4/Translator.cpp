#include "MyHash.h"
#include "provided.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl(); // Constructor
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    MyHash<char, char> charMap;
    vector<MyHash<char, char>*> mapStack;
    int pushMapTrue;
    int popMapTrue;
};

TranslatorImpl::TranslatorImpl()
: pushMapTrue(0), popMapTrue(0)
{
    // Create the initial map with all plaintext letters as ?
    string map = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int k = 0; k < map.size(); k++)
        charMap.associate(map[k], '?');
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    // If ciphertext and plaintext are not the same length, return false
    if (ciphertext.size() != plaintext.size())
        return false;
    
    // If either contains a non-letter, return false
    for (int k = 0; k < ciphertext.size(); k++)
    {
        if (! isalpha(ciphertext[k]) || ! isalpha(plaintext[k]))
            return false;
        
        // Make ciphertext and plaintext case-insensitive
        ciphertext[k] = toupper(ciphertext[k]);
        plaintext[k] = toupper(plaintext[k]);
    }
    
    // Check for inconsistencies
    for (int k = 0; k < ciphertext.size(); k++)
    {
        char* ptr = charMap.find(ciphertext[k]);
        if ((*ptr) == '?')
        {
            for (char letter = 'A'; letter <= 'Z'; letter++)
            {
                char* ptr2 = charMap.find(letter);
                if (ptr2 != nullptr && *ptr2 == plaintext[k])
                    return false;
            }
        }
        else if (*ptr != plaintext[k])
            return false;
    }
    
    // Save the current map into mapStack
    MyHash<char, char> *temp = new MyHash<char,char>;
    for (char letter = 'A'; letter <= 'Z'; letter++)
    {
        char* ptr = charMap.find(letter);
        temp->associate(letter, *ptr);
    }
    mapStack.push_back(temp);
    
    // Update the current mapping table to incorporate mappings
    for (int k = 0; k < ciphertext.size(); k++)
        charMap.associate(ciphertext[k], plaintext[k]);
    
    pushMapTrue++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    // If there are no maps, return false
    if (mapStack.empty() || pushMapTrue == popMapTrue)
        return false;
    
    MyHash<char, char>* temp = mapStack.back();
    
    for (char letter = 'A'; letter <= 'Z'; letter++)
    {
        // Make the most recent map from stack to the current map
        char* ptr = temp->find(letter);
        charMap.associate(letter, *ptr);
    }
    
    // Delete the most recent map from the stack and pop it
    delete temp;
    mapStack.pop_back();
    
    popMapTrue++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const   // DONE
{
    string temp = ciphertext;
    for (int k = 0; k < temp.size(); k++)
    {
        if (isalpha(ciphertext[k]))
        {
            const char* ptr = charMap.find(toupper(temp[k]));
            if (ptr != nullptr)
            {
                if (islower(ciphertext[k]))
                    temp[k] = tolower(*ptr);
                else
                    temp[k] = *ptr;
            }
        }
    }
    return temp;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
