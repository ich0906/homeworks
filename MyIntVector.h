//소프트웨어학부
//2017203061 임찬형

#define DEFAULT_SPACE 10

class MyIntVector
{
public:
	MyIntVector(size_t init_space=DEFAULT_SPACE);
	//Precondition:size_t type input must be required(not negative number)
	//Postcondition:constructor operate

	MyIntVector(const MyIntVector& v);
	//Precondition:MyIntVector type input must be required
	//Postcondition:copy constructor operate

	~MyIntVector();

	void pop_back();
	//Precondition:call when array size > 0
	//Postcondition:erase a last value in data array(also reducing size by one)

	void push_back(int x);
	//Precondition:integer type input must be required
	//Postcondition:increase the size of data array and add x in the end of it

	size_t capacity() const { return this->space; }
	size_t size() const { return this->used; }

	void reserve(size_t n);
	//Precondition:size_t type input must be required
	//Postcondition:extend data array with dynamic allocation

	bool is_empty() const;
	//Precondition:use when you want to know if this vector is empty.
	//Postcondition:return true if the vector size is 0(else return false)

	void clear();
	//Precondition:use when you want to reset the vector
	//Postcondition:reset the vector(make size and capacity 0)

	void operator+=(const MyIntVector& v);
	//Precondition:input MyIntVector type to add(connected to)
	//Postcondition:connect two data arrays 

	int& operator[](int pos) const;
	//Precondition:input pos must satisfy the condition (0<=pos<=used) 
	//Postcondition:bring the value in data array of [pos] position

	MyIntVector& operator=(const MyIntVector& v);
	//Precondition:use this between MyIntVector types
	//Postcondition:make the left-side MyIntVector equal to right-side MyIntVector

	MyIntVector operator +(const MyIntVector& v);
	//Precondition:use this between MyIntVector types that have same size.
	//Postcondition:Return a MyIntVector that adds the value of each location of the two MyIntVectors.

	MyIntVector operator -(const MyIntVector& v);
	//Precondition:use this between MyIntVector types that have same size.
	//Postcondition:Return a MyIntVector that subtracts the value of each location of the two MyIntVectors.

	MyIntVector operator -();
	//Precondition:put - before a MyIntVector
	//Postcondition:invert the sign of numbers in data array.

	void operator() (int value);
	//Precondition:input a integer type number in ()
	//Postcondition:change all values in data array into input value

	bool operator==(const MyIntVector& v);
	//Precondition:use this between MyIntVector types
	//Postcondition:return true if two Vectors are same(else return false)

	int operator *(const MyIntVector& v);
	//Precondition:use this between MyIntVector types that have same size.
	//Postcondition:return a vector internal of two MyIntVectors.

private:
	int* data;
	size_t used;
	size_t space;
};