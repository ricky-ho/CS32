///////////////////////////////////////////////////////////////////////////
//  History Implementation
///////////////////////////////////////////////////////////////////////////

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
: m_nRows(nRows), m_nCols(nCols)
{
    // Creates the default grid with '.' at every coordinate of the city
    int r, c;
    for (r = 0; r < m_nRows; r++)
    {
        for (c = 0; c < m_nCols; c++)
            m_hGrid[r][c] = '.';
    }
}

bool History::record(int r, int c)
{
    if (r > m_nRows || c > m_nCols)
        return false;
    
    // Save the current char
    char x = m_hGrid[r][c];
    
    // Changes the char in m_hGrid accordingly
    switch (x)
    {
        case '.': m_hGrid[r][c] = 'A';
            break;
        case 'Z':
            break;
        default: m_hGrid[r][c] = ++x;
            break;
    }
    
    return true;
}

void History::display() const
{
    // Display the Grid
    clearScreen();
    
    int k, l;
    for(k = 0; k < m_nRows; k++)
    {
        for(l = 0; l < m_nCols; l++)
            cout << m_hGrid[k][l];
        cout << endl;
    }
    cout << endl;
    
    
}
