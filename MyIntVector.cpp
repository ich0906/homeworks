//소프트웨어학부 
//2017203061 임찬형
#include <iostream>
#include <cstdlib>
#include "MyIntVector.h"
using namespace std;

MyIntVector::MyIntVector(size_t init_space=DEFAULT_SPACE)
{
	data = new int[init_space];
	this->space = init_space;
	used = 0;
}

MyIntVector::MyIntVector(const MyIntVector& v)
{
	data = new int[v.space];
	this->space = v.space;
	used = v.used;
	copy(v.data, v.data + used, data);
}

MyIntVector::~MyIntVector()
{
	delete[] data;
}

MyIntVector& MyIntVector::operator=(const MyIntVector& v)
{
	if (this == &v)
		return *this;
	if (this->space != v.space)
	{
		delete[] data;
		data = new int[v.space];
		this->space = v.space;
	}
	used = v.used;
	copy(v.data, v.data + used, data);
	return *this;
}

void MyIntVector::reserve(size_t n)
{
	int* larger_array;
	if (n == this->space)
		return;
	if (n < used)
		n = used;

	larger_array = new int[n];
	copy(data, data + used, larger_array);
	delete[] data;
	data = larger_array;
	this->space = n;
}

void MyIntVector::push_back(int x)
{
	if (used == this->space)
		reserve(used + 1);

	data[used] = x;
	used++;
}

void MyIntVector::operator+=(const MyIntVector& addend)
{
	if (used + addend.used > this->space)
		reserve(used + addend.used);
	copy(addend.data, addend.data + addend.used, data + used);
	used += addend.used;
}

int& MyIntVector::operator[](int pos) const
{
	if (pos >= used || pos<0)
	{
		cout << "Out of range" << endl;
		exit(-1);
	}
	return data[pos];
}

MyIntVector MyIntVector::operator +(const MyIntVector& v)
{
	if (this->used != v.size())
	{
		cout << "They're have different sizes" << endl;
		exit(1);
	}

	MyIntVector answer(used);
	for (int i = 0; i < answer.capacity(); i++)
	{
		answer.push_back(data[i] + v[i]);
	}

	return answer;
}

MyIntVector MyIntVector::operator -(const MyIntVector& v)
{
	if (this->used != v.size())
	{
		cout << "They're have different sizes" << endl;
		exit(1);
	}

	MyIntVector answer(used);
	for (int i = 0; i < answer.capacity(); i++)
	{
		answer.push_back(data[i] - v[i]);
	}

	return answer;
}

int MyIntVector::operator *(const MyIntVector& v)
{
	if (this->used != v.size())
	{
		cout << "They're have different sizes" << endl;
		exit(1);
	}

	int answer = 0;

	for (int i = 0; i <v.size(); i++)
	{
		answer += data[i] * v[i];
	}

	return answer;
}

MyIntVector MyIntVector::operator -()
{
	MyIntVector answer(space);

	for (int i = 0; i < space; i++)
	{
		answer.push_back(-data[i]);
	}

	return answer;
}

bool MyIntVector::operator==(const MyIntVector& v)
{
	if (used == v.size())
	{
		for (int i = 0; i < v.size(); i++)
		{
			if (data[i] != v[i])
			{
				//cout << "Two vectors are different" << endl;
				return false;
			}
		}
		return true;
	}
	else
	{
		//cout << "Two vectors have different sizes" << endl;
		return false;
	}
}

void MyIntVector::operator() (int value)
{
	for (int i = 0; i < used; i++)
	{
		data[i] = value;
	}

}

void MyIntVector::pop_back()
{
	if (used == 0) { cout << "There's no values in vector." << endl; }
	else
	{
		used--;
	}
}

bool MyIntVector::is_empty() const
{
	if (used == 0) { return true; }
	else { return false; }
}

void MyIntVector::clear()
{
	used = 0;
	space = 0;
}
