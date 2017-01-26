#include"newSequence.h"


Sequence::Sequence(int size)
{
	if (size < 0)
		exit(1);
	m_size = 0;
	m_array = new ItemType[size];
	m_max = size;
}

Sequence::Sequence(const Sequence &other)
{
	m_size = other.m_size;
	m_max = other.m_max;
	m_array = new ItemType[other.m_max];
	for (int i = 0; i < m_size; i++)
	{
		m_array[i] = other.m_array[i];
	}
}

Sequence& Sequence::operator=(const Sequence& other)
{
	if (this==&other)
	{
		return *this;
	}
	m_size = other.m_size;
	m_max = other.m_max;
	if (m_array!=nullptr)
	{
		delete[] m_array;

	}
	m_array = new ItemType[m_max];
	for (int i = 0; i < m_max; i++)
	{
		m_array[i] = other.m_array[i];
	}
	return	*this;
}

Sequence::~Sequence()
{
	delete[] m_array;
}

bool Sequence::empty() const
{
	return m_size == 0;
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos<0||pos>m_size||(m_size >= m_max))
	{
		return false;
	}
	if (pos==m_size)
	{
		m_array[m_size] = value;
		m_size++;
		return true;
	}
	for (int i = m_size; i > pos; i--)
	{
		m_array[i] = m_array[i - 1];
	}
	m_array[pos] = value;
	m_size++;
	return true;
}

int Sequence::insert(const ItemType& value)
{
	int p = 0;
	while (m_array[p]<value&& p <size())
	{
		p++;
	}
	if (!insert(p, value))
		return -1;
	else
		return p;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_size)
		return false;
	else
	{
		for (int i = pos; i < m_size-1; i++)
		{
			m_array[i] = m_array[i+1];
		}
		m_size--;
		return true;
	}
}

int Sequence::remove(const ItemType& value)
{
	int cnt = 0;
	for (int i = 0; i < size();)
	{
		if (m_array[i] == value)
		{
			erase(i);
			cnt++;
		}
		else
			i++;
	}
	return cnt;
}
bool Sequence::get(int pos, ItemType& value) const
{
	if (pos<0||pos>=m_size)
		return false;
	else
	{
		value = m_array[pos];
		return true;
	}
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos<0 || pos >= m_size)
		return false;
	else
	{
		m_array[pos] = value;
		return true;
	}
}

int Sequence::find(const ItemType& value) const
{
	int p = 0;
	while (value!=m_array[p]&&p<m_size)
	{
		p++;
	}
	if (p == m_size)
		return -1;
	else
		return p;
}

void Sequence::swap(Sequence& other)
{
	ItemType* temp = m_array;

	m_array = other.m_array;
	other.m_array = temp;

	m_max = m_max + other.m_max;
	other.m_max = m_max - other.m_max;
	m_max = m_max - other.m_max;

	m_size = m_size + other.m_size;
	other.m_size = m_size - other.m_size;
	m_size = m_size - other.m_size;
}

