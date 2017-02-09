#include <iostream>
#include <queue>
#include <string>

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

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	Coord init(sr, sc);
	coordQueue.push(init);
	maze[sr][sc] = 'X';

	int row = sr;
	int col = sc;

	while (!coordQueue.empty())
	{
		init = coordQueue.front();
		row = init.r();
		col = init.c();
		coordQueue.pop();
		if (row == er && col == ec)
			return true;

		if (row > 0 && maze[row - 1][col] != 'X')
		{
			Coord temp(row - 1, col);
			coordQueue.push(temp);
			maze[row - 1][col] = 'X';
		}

		if (col < nCols - 1 && maze[row][col + 1] != 'X')
		{
			Coord temp(row, col + 1);
			coordQueue.push(temp);
			maze[row][col + 1] = 'X';
		}

		if (row < nRows - 1 && maze[row + 1][col] != 'X')
		{
			Coord temp(row + 1, col);
			coordQueue.push(temp);
			maze[row + 1][col] = 'X';
		}

		if (col > 0 && maze[row][col - 1] != 'X')
		{
			Coord temp(row, col - 1);
			coordQueue.push(temp);
			maze[row][col - 1] = 'X';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}
