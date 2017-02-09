#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

int p(char c);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	postfix = "";
	stack<char> operatorStack;
	for (unsigned int i = 0; i < infix.size(); i++)
	{
		char temp = infix[i];
		switch (temp)
		{
		case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			postfix += temp;
			break;
		case'(':
			if (i >= infix.size() - 1 || infix[i + 1] == '&' || infix[i + 1] == '|')
				return 1;
			operatorStack.push(temp);
			break;
		case')':
			while (operatorStack.top()!='(')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
			break;
		case'|': case'&':case'!':
			while (!operatorStack.empty()&&operatorStack.top()!='('&&p(temp)<=p(operatorStack.top()))
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(temp);
			break;
		default:
			break;
		}
	}

	while (!operatorStack.empty())
	{
		if (operatorStack.top() == '(')
			return 1;
		postfix += operatorStack.top();
		operatorStack.pop();
	}
	if (postfix.empty())
		return 1;
	stack<bool>resultStack;

	for (unsigned int i = 0; i < postfix.size(); i++)
	{
		char temp = postfix[i];
		if (temp - '0'>= 0 && temp - '0' <= 9)
		{
			resultStack.push(values[temp - '0']);
		}
		if (resultStack.empty())
			return 1;
		if (temp == '!')
		{
			bool ch = resultStack.top();
			resultStack.pop();
			resultStack.push(!ch);
		}
		if (temp =='|')
		{
			bool temp1 = resultStack.top();
			resultStack.pop();
			if (resultStack.empty())
				return 1;
			bool temp2 = resultStack.top();
			resultStack.pop();
			resultStack.push(temp1 || temp2);
		}
		if (temp == '&')
		{
			bool temp1 = resultStack.top();
			resultStack.pop();
			if (resultStack.empty())
				return 1;
			bool temp2 = resultStack.top();
			resultStack.pop();
			resultStack.push(temp1 && temp2);
		}
	}
	if (resultStack.size() != 1)
		return 1;
	result = resultStack.top();
	return 0;
}

int p(char c)
{
	switch (c)
	{
	case'!':
		return 2;
		break;
	case'&':
		return 1;
		break;
	case'|':
		return 0;
		break;
	default:
		return -1;
		break;
	}
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true, true, true, false, false, false, true, false, true, false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|"  &&  !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9"  &&  !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  !answer);
	cout << "Passed all tests" << endl;
}
