bool allTrue(const double a[], int n)
{
	if (n == 0)
		return true;
	if (n == 1)
		return somePredicate(a[0]);

	return (somePredicate(a[0])&&allTrue(a+1,n-1)); 
}

int countFalse(const double a[], int n)
{
	if (n <= 0) 
		return 0;
	int count;
	if (somePredicate(a[0]))
		count = 0;
	else
		count = 1;
	if (n <= 1)
		return count;
	return count + countFalse(a + 1, n - 1);

}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
	if (n <= 0) 
		return -1;
	if (somePredicate(a[0]) == false) 
		return 0;
	int pos = firstFalse(a + 1, n - 1);
	if (pos != -1)
	{
		return 1 + pos;
	}
	return -1;
}

int indexOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 0)
		return 0;
	int pos = 1 + indexOfMin(a + 1, n - 1);
	if (a[0] <= a[pos])
		return 0;
	else
		return pos;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 == 0)
		return true;
	if (n1 == 0)
		return false;
	if (a1[0] == a2[0])
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		return	includes(a1 + 1, n1 - 1, a2, n2);
	
}
