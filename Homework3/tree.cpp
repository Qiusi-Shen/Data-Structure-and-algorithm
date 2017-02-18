int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return 1;
	if (n1 <= 0)
		return 0;
	int counter = 0;
	if (a1[0] == a2[0])
		counter += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		countIncludes(a1 + 1, n1 - 1, a2, n2);
	return
		counter + countIncludes(a1 + 1, n1 - 1, a2, n2);
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

void separate(double a[], int n, double separator,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > separator
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == separator
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < separator

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1)
		return;
	int less = -1, great = n, mid = n/2;
	separate(a, n, a[mid], great, less);
	order(a, great);
	order(a+great+1, n-less);
}
