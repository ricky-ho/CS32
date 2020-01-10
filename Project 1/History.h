///////////////////////////////////////////////////////////////////////////
//  History Interface
///////////////////////////////////////////////////////////////////////////

#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    // Constructor
    History(int nRows, int nCols);
    
    // Accessors
    bool record(int r, int c);
    void display() const;
    
private:
    int m_nRows;
    int m_nCols;
    char m_hGrid[MAXROWS][MAXCOLS];
};

#endif  //HISTORY_H
