// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP_H
#define MYMAP_H
#include <iostream>

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		Node(KeyType key, ValueType value) : m_key(key), m_value(value), left(nullptr), right(nullptr)
		{
		}
		KeyType m_key;
		ValueType m_value;
		Node *left;
		Node *right;
	};

	Node* m_root;
	int m_size;
	
	Node* insert(const KeyType& key, const ValueType& value);
	void deleteAll(Node* root);
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
	clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	deleteAll(m_root);
	m_size = 0;
	m_root = nullptr;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
	return m_size;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	if (m_root == nullptr)
		m_root = insert(key, value);

	else
	{
		Node* current = m_root;
		for(;;)
		{
			if (key == current->m_key)
			{
				current->m_value = value;
				return;
			}

			else if (key < current->m_key)
			{
				if (current->left == nullptr)
				{
					current->left = insert(key, value);
					return;
				}
				else
				{
					current = current->left;
				}
			}

			else
			{
				if (current->right == nullptr)
				{
					current->right = insert(key, value);
					return;
				}
				else
				{
					current = current->right;
				}
			}
		}
	}
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	Node* currentPtr = m_root;

	if (m_root == nullptr)
		return nullptr;
	for (;;)
	{
		if (key == currentPtr->m_key)
		{
			ValueType* valuePtr = &(currentPtr->m_value);
			return valuePtr;
		}

		if (key < currentPtr->m_key)
			currentPtr = currentPtr->left;

		else currentPtr = currentPtr->right;

		if (currentPtr == nullptr)
			return nullptr;
	}
}

template<typename KeyType, typename ValueType>
typename MyMap<KeyType, ValueType>::Node* MyMap<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
	Node* insert = new Node(key, value);
	m_size++;
	return insert;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::deleteAll(Node* root)
{
	if (root == nullptr)
		return;

	deleteAll(root->left);
	deleteAll(root->right);

	delete root;
	m_size--;
}

#endif	// MYMAP_H