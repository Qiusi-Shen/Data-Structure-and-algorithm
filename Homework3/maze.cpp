bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	if (sr==er&&sc==ec)
		return true;
	maze[sr][sc] = 'X';

	if (sr>0 && maze[sr-1][sc]!='X')
	{
		if (pathExists(maze,nRows,nCols, sr - 1, sc, er, ec))
			return true;
	}
	if (sc<nCols-1 && maze[sr][sc+1] != 'X')
	{
		if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
			return true;
	}
	if (sr<nRows-1 && maze[sr + 1][sc] != 'X')
	{
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	}
	if (sc>0 && maze[sr][sc-1] != 'X')
	{
		if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
			return true;
	}
	return false;
}

