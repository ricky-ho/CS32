
#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    
    stack<Coord> coordStack;
    
    coordStack.push(Coord(sr, sc));     // Push starting coordinate into the coord stack
    maze[sr][sc] = '#';                 // Update maze to indicate we have encountered it
    
    while(! coordStack.empty())
    {
        Coord currentCoord = coordStack.top();
        coordStack.pop();               // Pop top coordinate off stack
        
        int row = currentCoord.r();
        int col = currentCoord.c();
        
        //cerr << row << "," << col << endl;
        
        if (row == er && col == ec)     // If current coordinate is equal to end coordinate, we are done and return true
            return true;
        
        if (maze[row][col+1] != 'X' && maze[row][col+1] != '#' && maze[row][col+1] != nCols-1)    // If we can move EAST and haven't encountered that cell yet
        {
            coordStack.push(Coord(row, col+1));        // Push the coordinate onto the stack
            maze[row][col+1] = '#';             // And indicate the algorithm has encountered it
        }
        
        if (maze[row+1][col] != 'X' && maze[row+1][col] != '#' && maze[row+1][col] != nRows-1)    // If we can move SOUTH...
        {
            coordStack.push(Coord(row+1, col));
            maze[row+1][col] = '#';
        }
        
        if (maze[row][col-1] != 'X' && maze[row][col-1] != '#')    // If we can move WEST...
        {
            coordStack.push(Coord(row, col-1));
            maze[row][col-1] = '#';
        }
        
        if (maze[row-1][col] != 'X' && maze[row-1][col] != '#')    // If we can move NORTH...
        {
            coordStack.push(Coord(row-1, col));
            maze[row-1][col] = '#';
        }
    }
    
    return false;       // There was no solution, thus return false
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X..XX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 4,3, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    
    //for (int k = 0; k < 10; k++)
    //   cerr << maze[k] << endl;
}

