#include"History.h"
#include"globals.h"

#include<iostream>

using namespace std;

History::History(int nRows, int nCols)
{
	m_nRows = nRows;
	m_nCols = nCols;
	for (int r = 1; r <= m_nRows; r++)
	{
		for (int c = 1; c <= m_nCols; c++)
		{
			m_history[r - 1][c - 1]=0;
		}
	}
}

bool History::record(int r, int c)
{
	if (r>m_nRows||c>m_nCols)
	{
		return false;
	}
		m_history[r - 1][c - 1] += 1;
		return true;	
}

void History::display() const
{
	char displayGrid[MAXROWS][MAXCOLS];

	for (int r = 1; r <= m_nRows; r++)
	{
		for (int c = 1; c <= m_nCols; c++)
		{
			int n = m_history[r - 1][c - 1];
			if (n == 0)
				displayGrid[r - 1][c - 1] = '.';
			else if (n > 26)
				displayGrid[r - 1][c - 1] = 'Z';
			else
			{
				displayGrid[r - 1][c - 1] = 'A' + n - 1;
			}
		}
	}

	clearScreen();

	for (int r = 1; r <= m_nRows; r++)
	{
		for (int c = 1; c <= m_nCols; c++)
		{
			cout << displayGrid[r - 1][c - 1];
		}
		cout << endl;
	}
	cout << endl;
}
