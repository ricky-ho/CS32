#include "MyHash.h"
#include <fstream>
#include <cctype>

#include "provided.h"
#include <functional>
#include <string>
#include <vector>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
    
private:
    string generateWordPattern(string word) const;
    MyHash<string, vector<string>> m_hashTable;
};

bool WordListImpl::loadWordList(string filename)  
{
    // Everytime loadWordList is called, it must discard all of its old contents
    m_hashTable.reset();
    
    ifstream infile(filename);
    if (! infile)
        return false;
    
    vector<string> checkPatterns;
    // Examine every word in wordlist.txt
    string word;
    while (getline(infile, word))
    {
        // Want case-insensitivity so just make all lowercase
        for (int k = 0; k < word.size(); k++)
            word[k] = tolower(word[k]);
        
        // Generate a word pattern for each word
        string wordPattern = generateWordPattern(word);
        
        // Ignore the word that contains a char that is not a letter/apostrophe
        if (wordPattern == "")
            continue;
        
        // Insert all unique patterns into a vector
        bool existingPattern = false;
        for (int k = 0; k < checkPatterns.size(); k++)
        {
            if (wordPattern == checkPatterns[k])
            {
                existingPattern = true;
                break;
            }
        }
        if (existingPattern == false)
            checkPatterns.push_back(wordPattern);
    }
    
    // For each pattern, create a vector of words with matching patterns
    // and insert it into m_hashTable
    for (int k = 0; k < checkPatterns.size(); k++)
    {
        vector<string> wordsV;
        
        ifstream infile1(filename);
        while (getline(infile1, word))
        {
            // Want case-insensitivity so just make all lowercase
            for (int k = 0; k < word.size(); k++)
                word[k] = tolower(word[k]);
            
            string wordPattern = generateWordPattern(word);
            if (wordPattern == checkPatterns[k])
                wordsV.push_back(word);
        }
        
        // Insert each pattern with all the words matching its pattern into m_hashTable
        m_hashTable.associate(checkPatterns[k], wordsV);
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    // Convert the word to all lower-case
    for (int k = 0; k < word.size(); k++)
        word[k] = tolower(word[k]);
    
    // Find the vector containing matching patterns of the word
    string wordPattern = generateWordPattern(word);
    const vector<string>* matchedPattern = m_hashTable.find(wordPattern);
    
    if (matchedPattern == nullptr)
        return false;
    else
    {
        // Look through the vector to see if it contains our word
        for(int k = 0; k < matchedPattern->size(); k++)
        {
            if ((*matchedPattern)[k] == word)
                return true;
        }
    }
    // At this point, we did not find our word so return false
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    // Want case-insensitivity so just make all lowercase
    for (int k = 0; k < cipherWord.size(); k++)
    {
        cipherWord[k] = tolower(cipherWord[k]);
        currTranslation[k] = tolower(currTranslation[k]);
    }
    
    vector<string> v;
    // Check if cipherWord and currTranslation have the same length
    // return empty vector if they do not
    if (cipherWord.size() != currTranslation.size())
        return v;
    
    // Check cipherWord has only letters and apostrophes chars and
    // currTranslation has only letters, apostrophes, and ? chars
    for (int k = 0; k < cipherWord.size(); k++)
    {
        if (! isalpha(cipherWord[k]) && cipherWord[k] != 39)
            return v;
        
        if (! isalpha(currTranslation[k]) && currTranslation[k] != 39 && currTranslation[k] != '?')
            return v;
    }
    
    // Generate word pattern for cipherWord and find vector of words containing the matching pattern
    string cipherWordPattern = generateWordPattern(cipherWord);
    const vector<string>* w = m_hashTable.find(cipherWordPattern);
    
    if (w != nullptr)
    {
        // For each word with a matching pattern as cipherWord
        for (int k = 0; k < w->size(); k++)
        {
            bool match = true;
            // For each position of currTranslation
            for (int j = 0; j < currTranslation.size(); j++)
            {
                // currTranslation[j] is a letter, w[j] & cipherWord[j] must be that letter
                if (isalpha(currTranslation[j]))
                {
                    if ((*w)[k][j] != currTranslation[j])
                    {
                        match = false;
                        break;
                    }
                    if (! isalpha(cipherWord[j]))
                        return v;
                }
                // currTranslation[j] is ?, don't know plaintext translation of cipherWord & w[j] must be a letter
                else if (currTranslation[j] == '?')
                {
                    if (! isalpha((*w)[k][j]))
                    {
                        match = false;
                        break;
                    }
                    if (! isalpha(cipherWord[j]))
                        return v;
                }
                // currTranslation[j] is an apostrophe, w[j] & cipherWord[j] must be apostrophe
                else if (currTranslation[j] == 39)
                {
                    if ((*w)[k][j] != 39)
                    {
                        match = false;
                        break;
                    }
                    if ((cipherWord[j] != 39))
                        return v;
                }
            }
            
            if (match == true)
                v.push_back((*w)[k]);
        }
    }
    
    return v;
}


string WordListImpl::generateWordPattern(string word) const
{
    string patternCode = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int count = 0;

    string wordPattern = word;
    string uniqueChars = "";
   
    // Find the unique letter or apostrophe characters
    for (int k = 0; k < word.size(); k++)
    {
        if (word[k] == '~' )
            continue;
        else if (! isalpha(word[k]) && word[k] != 39)
        {
            wordPattern = "";
            break;
        }
        else
            uniqueChars += word[k];
        
        for(int j = k + 1; j < word.size(); j++)
        {
            if (word[j] == word[k])
                word[j] = '~';
        }
    }
    
    // Encode the wordPattern
    while (count < uniqueChars.size())
    {
        for (int l = 0; l < wordPattern.size(); l++)
        {
            if (wordPattern[l] == uniqueChars[count])
                wordPattern[l] = patternCode[count];
        }
        count++;
    }
    
    return wordPattern;
}


//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
