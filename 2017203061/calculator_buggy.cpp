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
	bool full;										//private 추가! (필수x)
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer=t; full=true; }  //false -> true 수정! (Requirement 1)

	void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqrt_ = 'R';							//수학 관련 함수들 기호 추가
const char sin_ = 'S';
const char cos_ = 'C';
const char tan_ = 'T';
const char log_ = 'l';
const char exp_ = 'E';
const char pactorial = 'P';

const double pi = 3.14159;						//pi,e 추가
const double e = 2.71828;

Token Token_stream::get()						//Token get() -> Token Token_stream::get() 수정! (Requirement 1)
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
	case '^':										//^추가
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

vector<Variable> names = { {"pi",pi}, {"e",e} };			//names에 pi,e를 미리 넣어둠.

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

double cal_expo(vector<double> v)						//vector에 넣어둔 연속 지수형 계산하는 함수
{
	double ans = v[v.size()-1];							//벡터 마지막 항 값을 넣어둠
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
		if (t.kind != ')') error("')' expected"); //error("'(' expected"); -> error("')' expected"); 수정!(Requirement 1)
		return d;								  //return d; 추가! (Requirement 1)
	}
	case '-':
		return - primary();
	case '+':									  //case '+' 추가하고 primary()리턴
		return primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);				  //get_value(t.value) -> get_value(t.name) 수정! (Requirement 1)
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
			temp -= (pi / 2);						//tan 속 값이 pi/2의 배수인지 확인하기 위해 양수범위에서 계속 뺀다
			if (temp > -0.0001 && temp < 0.0001) error("tan() can't calculate it");  //double범위이므로 0이 나왔을 경우를 표현
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

double exponentiation()								//exponentiation()추가
{
	double left = primary();
	double right;
	vector<double> expo;

	expo.push_back(left);							//left의 primary값을 벡터 expo의 첫 칸에 넣어둠.
	while (true) {
		Token t = ts.get();							//left 오른쪽 토큰 확인.
		switch (t.kind) {	
		case '^':
			while (true) {							//^가 몇개 등장할 지 모르므로 반복문 사용
				right = primary();					//^ 오른쪽의 수를 right에 넣음.
				t = ts.get();						//right 다음 토큰 확인
				if (t.kind == '^') {
					expo.push_back(right);			//right 옆의 토큰도 ^라면 right를 expo에 넣고 반복.
				}
				else {
					expo.push_back(right);			//반복이 끝나면 마지막 ^ 옆의 primary 까지 expo에 넣음.
					left = cal_expo(expo);			//cal_expo로 expo의 토큰(수)들을 오른쪽부터 계산해 left에 넣고 반복 종료
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
	double left = exponentiation();						//exponentiation으로 변경

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
		case '%':									//case '%' 추가. a%b => b가 커질 때까지 계속해서 b를 뺌
		{
			double d = exponentiation();
			if (d == 0) error("divide by zero");
			while (left >= d)
				left -= d;
			break;
		}
		default:
			ts.putback(t);							//ts.putback(t) 추가! (Requirement 1)
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
			ts.putback(t);							//ts.putback(t) 추가! (Requirement 1)
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
