#include"ScoreList.h"
#include<iostream>

ScoreList::ScoreList() :m_score()
{}

bool ScoreList::add(unsigned long score)
{
	if (score < 0 || score>100||m_score.size()>=DEFAULT_MAX_ITEMS)
		return false;
	else
	{
		m_score.insert(score);
		return true;
	}
}

bool ScoreList::remove(unsigned long score)
{
	int pos = m_score.find(score);
	if (pos == -1)
		return false;
	else
	{
		m_score.erase(pos);
		return true;
	}
}

int ScoreList::size() const
{
	return m_score.size();
}

unsigned long ScoreList::minimum() const
{
	if (m_score.size() == 0)
		return NO_SCORE;
	else
	{
		unsigned long min;
		m_score.get(0, min);
		for (int i = 1; i < m_score.size(); i++)
		{
			unsigned long temp;
			m_score.get(i, temp);
			if (temp < min)
			{
				min = temp;
			}
		}
		return min;
	}
}

unsigned long ScoreList::maximum() const
{
	if (m_score.size() == 0)
		return NO_SCORE;
	else
	{
		unsigned long max;
		m_score.get(0, max);
		for (int i = 1; i < m_score.size(); i++)
		{
			unsigned long temp;
			m_score.get(i, temp);
			if (temp > max)
			{
				max = temp;
			}
		}
		return max;
	}
	
}
