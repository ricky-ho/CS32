/*
#include <iostream>
#include <string>
#include <queue>
using namespace std;
*/

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

    //cerr << sr << "," << sc << endl;
    
    if (sr == er && sc == ec)   // If start location is equal to ending location
        return true;            // then we are finished and return true
    
    maze[sr][sc] = '#';         // Mark start location as visited
    
    if (maze[sr][sc+1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))   // If we can move EAST
            return true;
    
    if (maze[sr+1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))   // If we can move SOUTH
            return true;
    
    if (maze[sr][sc-1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))   // If we can move WEST
            return true;
    
    if (maze[sr-1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))   // If we can move NORTH
            return true;
    
    return false;       // There was no solution, thus return false
}

/*
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
*/

