/*
	calculator_buggy.cpp
*/

#include "std_lib_facilities.h"

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n) { }
};

class Token_stream {
private:
	bool full;										//private �߰�! (�ʼ�x)
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer=t; full=true; }  //false -> true ����! (Requirement 1)

	void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqrt_ = 'R';							//���� ���� �Լ��� ��ȣ �߰�
const char sin_ = 'S';
const char cos_ = 'C';
const char tan_ = 'T';
const char log_ = 'l';
const char exp_ = 'E';
const char pactorial = 'P';

const double pi = 3.14159;						//pi,e �߰�
const double e = 2.71828;

Token Token_stream::get()						//Token get() -> Token Token_stream::get() ����! (Requirement 1)
{
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case '^':										//^�߰�
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	
		cin.unget();
		double val;
		cin >> val;
		return Token(number,val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);	
			if (s == "quit") return Token(quit);
			if (s == "sqrt") return Token(sqrt_);
			if (s == "sin") return Token(sin_);
			if (s == "cos") return Token(cos_);
			if (s == "tan") return Token(tan_);
			if (s == "log") return Token(log_);
			if (s == "exp") return Token(exp_);
			if (s == "pactorial") return Token(pactorial);
			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names = { {"pi",pi}, {"e",e} };			//names�� pi,e�� �̸� �־��.

double get_value(string s)
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ",s);
}

void set_value(string s, double d)
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}

bool is_declared(string s)
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();

double cal_expo(vector<double> v)						//vector�� �־�� ���� ������ ����ϴ� �Լ�
{
	double ans = v[v.size()-1];							//���� ������ �� ���� �־��
	for (int i = v.size()-1; i>0; i--)
	{
		ans = pow(v[i - 1], ans);
	}
	return ans;
}

int cal_pactorial(int n)
{
	if (n == 1) return n;
	else return n*cal_pactorial(n - 1);
}

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected"); //error("'(' expected"); -> error("')' expected"); ����!(Requirement 1)
		return d;								  //return d; �߰�! (Requirement 1)
	}
	case '-':
		return - primary();
	case '+':									  //case '+' �߰��ϰ� primary()����
		return primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);				  //get_value(t.value) -> get_value(t.name) ����! (Requirement 1)
	case sin_:
		return sin(primary());
	case cos_:
		return cos(primary());
	case tan_:
	{
		double temp = primary();
		double d = temp;
		while (temp > 0)
		{
			temp -= (pi / 2);						//tan �� ���� pi/2�� ������� Ȯ���ϱ� ���� ����������� ��� ����
			if (temp > -0.0001 && temp < 0.0001) error("tan() can't calculate it");  //double�����̹Ƿ� 0�� ������ ��츦 ǥ��
		}
		return tan(d);
	}
		return tan(primary());
	case sqrt_:
	{
		double d = primary();
		if (d < 0) error("negative number in sqrt!");
		else return sqrt(d);
	}
	case log_:
	{
		double d = primary();
		if (d <= 0) error("0 or negative num in log!");
		else return log(d);
	}
	case exp_:
		return exp(primary());
	case pactorial:
	{
		double temp = primary();
		double d = temp;

		if (temp < 0) error("number in pactorial cannot be minus");
		if (temp == 0)return 1;										//0!=1(definition)

		while (temp > 0)
		{
			temp -= 1;
			if (temp<0.0001 && temp>-0.0001) return cal_pactorial(d);
		}
		error("type of number in pactorial is not integer!");
	}
	default:
		error("primary expected");
	}
}

double exponentiation()								//exponentiation()�߰�
{
	double left = primary();
	double right;
	vector<double> expo;

	expo.push_back(left);							//left�� primary���� ���� expo�� ù ĭ�� �־��.
	while (true) {
		Token t = ts.get();							//left ������ ��ū Ȯ��.
		switch (t.kind) {	
		case '^':
			while (true) {							//^�� � ������ �� �𸣹Ƿ� �ݺ��� ���
				right = primary();					//^ �������� ���� right�� ����.
				t = ts.get();						//right ���� ��ū Ȯ��
				if (t.kind == '^') {
					expo.push_back(right);			//right ���� ��ū�� ^��� right�� expo�� �ְ� �ݺ�.
				}
				else {
					expo.push_back(right);			//�ݺ��� ������ ������ ^ ���� primary ���� expo�� ����.
					left = cal_expo(expo);			//cal_expo�� expo�� ��ū(��)���� �����ʺ��� ����� left�� �ְ� �ݺ� ����
					break;
				}
			}
		default:
			ts.putback(t);
			return left;
		}
	}

}

double term()
{
	double left = exponentiation();						//exponentiation���� ����

	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '*':
			left *= exponentiation();
			break;
		case '/':
		{	double d = exponentiation();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		case '%':									//case '%' �߰�. a%b => b�� Ŀ�� ������ ����ؼ� b�� ��
		{
			double d = exponentiation();
			if (d == 0) error("divide by zero");
			while (left >= d)
				left -= d;
			break;
		}
		default:
			ts.putback(t);							//ts.putback(t) �߰�! (Requirement 1)
			return left;
		}
	}
}

double expression()
{
	double left = term();

	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.putback(t);							//ts.putback(t) �߰�! (Requirement 1)
			return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
	double d = expression();
	names.push_back(Variable(name,d));
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while(true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.putback(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
try {
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	keep_window_open();
	return 1;
}
catch (...) {
	cerr << "exception\n";
	keep_window_open();
	return 2;
}
