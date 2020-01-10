
#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
    
private:
    vector<char> m_separators;  // vector to hold each separator character
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int k = 0; k < separators.size(); k++)
    {
        m_separators.push_back(separators[k]);
    }
}

vector<string> TokenizerImpl::tokenize(const string& s) const
{
    vector<string> tokens;  // Create a vector to hold each token
    string temp;            // Create a temp string to build each token
    
    for (int k = 0; k < s.size(); k++)
    {
        bool isSeparator = false;
        char c = s[k];
        
        for (int l = 0; l < m_separators.size(); l++)
        {
            if (c == m_separators[l])   // Check if the string char is a separator
            {
                if (temp != "")         // If we have letters in our temp string i.e a token, add it to tokens vector
                    tokens.push_back(temp);
                
                temp = "";              // Reset temp to be empty
                isSeparator = true;     // Indicate that the character we are looking at is a separator
                break;
            }
        }
        
        if (! isSeparator)  // Only add characters that are not separators into our temp string
            temp += s[k];
    }
    
    if (temp != "")     // Add the last valid token into tokens vector
        tokens.push_back(temp);
    
    return tokens;  // Return our tokens vector
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
    return m_impl->tokenize(s);
}
