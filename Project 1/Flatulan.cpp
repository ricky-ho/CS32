///////////////////////////////////////////////////////////////////////////
//  Flatulan Implementations
///////////////////////////////////////////////////////////////////////////

#include "Flatulan.h"
#include "City.h"
#include "Player.h"
#include "globals.h"

#include <iostream>
using namespace std;

Flatulan::Flatulan(City* cp, int r, int c)
: m_city(cp), m_row(r), m_col(c)
{
    if (cp == nullptr)
    {
        cout << "***** A Flatulan must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > cp->rows()  ||  c < 1  ||  c > cp->cols())
    {
        cout << "***** Flatulan created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
}

int Flatulan::row() const
{
    return m_row;
}

int Flatulan::col() const
{
    return m_col;
}

void Flatulan::move()
{
    // Attempt to move in a random direction; if we can't move, don't move.
    // If the player is there, don't move.
    int dir = randInt(0, NUMDIRS-1);  // dir is now UP, DOWN, LEFT, or RIGHT
    int r = m_row;
    int c = m_col;
    m_city->determineNewPosition(r, c, dir);
    Player* p = m_city->player();
    if (r != p->row()  ||  c != p->col())
    {
        m_row = r;
        m_col = c;
    }
}

bool Flatulan::possiblyGetConverted()  // return true if converted
{
    // Be converted with 2/3 probability
    return randInt(0, 2) < 2;
}
