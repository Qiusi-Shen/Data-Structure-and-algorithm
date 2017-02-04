#include"Sequence.h"
#include<iostream>

Sequence::Sequence() :m_size(0)												// constrcutor
{
	head = new Node;														// using a circular Linkedlist
	head->m_next = head;
	head->m_prev = head;
}

Sequence::~Sequence()
{
	Node *temp = head;
	Node *p = head->m_next;
	while (p!=head)														//		destructor
	{
		delete temp;
		temp = p;
		p = p->m_next;
	}
	delete temp;
}

bool Sequence::empty() const								// check if it is an empty set
{
	return m_size==0;
}


int Sequence::size() const									// return the size of the sequence
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos<0 || pos>size())
		return false;

	Node*p = head->m_next;
	for (int i = 0; i < pos; i++)													
		p = p->m_next;								// Insert value into the sequence so that it becomes the item at
	Node *temp = new Node;							// position pos.
	temp->m_next = p;
	temp->m_prev = p->m_prev;
	p->m_prev->m_next = temp;
	p->m_prev = temp;
	temp->m_value = value;
	m_size++;
	return true;
}

int Sequence::insert(const ItemType& value)
{																			// Let p be the smallest integer such that value <= the item at
	Node*p = head->m_next;													// position p in the sequence;
	int pos = 0;
	while (p!=head)
	{
		if (value <= p->m_value) break;
		p = p->m_next;
		pos++;
	}
		insert(pos, value);
		return pos;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size())
		return false;										 // If 0 <= pos < size(), remove the item at position pos from
	Node *p = head->m_next;									// the sequence 
	for (int i = 0; i < pos; i++)
		p = p->m_next;
	p->m_next->m_prev = p->m_prev;
	p->m_prev->m_next = p->m_next;
	m_size--;
	delete p;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	Node *p = head->m_next;
	int cnt = 0;
	while (p!=head)
	{
		if (value==p->m_value)
		{															 // Erase all items from the sequence that == value.  Return the
			Node *temp = p;										    // number of items removed (which will be 0 if no item == value).
			temp->m_next->m_prev = temp->m_prev;
			temp->m_prev->m_next = temp->m_next;
			p = p->m_next;
			m_size--;
			cnt++;
			delete temp;
		}
		else
		{
			p = p->m_next;
		}
	}
	return cnt;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size())
		return false;											 // If 0 <= pos < size(), copy into value the item at position pos
	else
	{															 // in the sequence and return true. 
		Node *p = head->m_next;
		for (int i = 0; i < pos; i++)
		{
			p = p->m_next;
		}
		value = p->m_value;
		return true;
	}
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size())
		return false;
	else
	{																// If 0 <= pos < size(), replace the item at position pos in the
		Node *p = head->m_next;										// sequence with value and return true.
		for (int i = 0; i < pos; i++)					
		{
			p = p->m_next;
		}
		p->m_value = value;
		return true;
	}
}

int Sequence::find(const ItemType& value) const
{
	Node *p = head->m_next;
	int pos = 0;
	while (p!=head)													// Let p be the smallest integer such that value == the item at
	{																// position p in the sequence
		if (value == p->m_value)			
			return pos;
		pos++;
		p = p->m_next;
	}
	return -1;
}

void Sequence::swap(Sequence& other)
{																// Exchange the contents of this sequence with the other one.
	Node *temp = head;
	head = other.head;
	other.head = temp;

	int t = m_size;
	m_size = other.m_size;
	other.m_size = t;
}

Sequence::Sequence(const Sequence&other)
{																		// copy constuctor
	m_size = other.m_size;
	head = new Node;
	head->m_prev = head;

	for (Node* p = other.head->m_next; p!=other.head;
		p = p->m_next)
	{
		Node *temp = new Node;
		temp->m_value = p->m_value;
		temp->m_prev = head->m_prev;
		head->m_prev = temp;
		temp->m_prev->m_next = temp;
	}
	head->m_prev->m_next = head;

}

Sequence& Sequence::operator=(const Sequence& other)
{																				// assignmeent operator
	if (this!= &other)
	{
		Sequence temp(other);
		swap(temp);
	}
	return *this;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence tempResult(result);
	while (!tempResult.empty())
		tempResult.erase(0);
	if (seq1.empty())
	{
		tempResult = seq2;																
		return;
	}
	if (seq2.empty())
	{
		tempResult = seq1;
		return;
	}
	int pos1=0, pos2=0, pos=0;
	ItemType t1, t2;

	while (pos1<seq1.size()||pos<seq2.size())
	{
		if (seq1.get(pos1,t1))
		{
			tempResult.insert(pos, t1);
			pos1++;
			pos++;
		}
		if (seq2.get(pos2, t2))
		{
			tempResult.insert(pos, t2);
			pos2++;
			pos++;
		}
	}
	result = tempResult;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq2.empty()||seq2.size()>seq1.size())
		return -1;
	bool check = true;
	int pos = -1;
	ItemType t1, t2;
	for (int i = 0; i < seq1.size(); i++)
	{
		seq1.get(i, t1);
		seq2.get(0, t2);
		if (t1==t2)
		{
			check = true;
			pos = i;
			for (int j = 0; j < seq2.size(); j++)
			{
				if (!seq1.get(i+j,t1)||!seq2.get(j,t2)||t1!=t2)
				{
					check = false;
					break;
				}
			}
			if (check==true)
				return pos;
		}
	}
	return -1;
	
}
