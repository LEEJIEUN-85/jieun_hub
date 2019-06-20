/*---------------precidtive parser--------------------*/
//S -> aS
//S -> bA
//A -> d
//A -> ccA

#include<iostream>
#include<string>
using namespace std;

class pTable;
class stack;
void parsing_function(string *str, int leftParse[]);
void getNextSymbol(string *str);

class pTable {
public: 
	int expand(); // stack top이 Vn인경우 생성규칙 적용하여 확장
	int S_a(); // 1
	int S_b(); // 2
	int A_d(); // 3
	int A_c(); // 4
};
class stack {
public:
	string s_str;
	stack() { s_str = "$S"; } //오른쪽이 TOP
	string seek(); // 스택의 top을 확인하는 함수
	void push(int len, string s);
	void pop();
};
//전역변수로 선언
pTable pt;
stack stk;
int parsing_state = 1; // 0이 되면 parsing이 종료된 것을 의미
string nextSymbol;
int index = 0;
int p_index = 0;

int pTable::expand() {
	string stack_top = stk.seek();
	if (stack_top.compare("S") == 0) {
		if (nextSymbol.compare("a") == 0)
			return S_a();
		else if (nextSymbol.compare("b") == 0)
			return S_b();
		else
			parsing_state = 0;
	}
	else if (stack_top.compare("A") == 0) {
		if (nextSymbol.compare("d") == 0)
			return A_d();
		else if (nextSymbol.compare("c") == 0)
			return A_c();
		else
			parsing_state = 0;
	}
	else 
		return 0;
	return 1;
}
int pTable::S_a() {
	stk.push((stk.s_str).length(), "Sa");
	return 1;
}
int pTable::S_b() {
	stk.push((stk.s_str).length(), "Ab");
	return 2;
}
int pTable::A_d() {
	stk.push((stk.s_str).length(), "d");
	return 3;
}
int pTable::A_c() {
	stk.push((stk.s_str).length(), "Acc");
	return 4;
}
string stack::seek() {
	return s_str.substr(s_str.length() - 1, 1);
}
void stack::push(int len, string s) {
	(stk.s_str).replace(len - 1, 1, s);
}
void stack::pop() {
	(stk.s_str).erase((stk.s_str).length() - 1, 1);
}

int main() {
	string input;
	int leftParse[50] = { 0 };
	cout << "Enter a sentence: ";
	cin >> input;

	nextSymbol = input.substr(0, 1);

	while (parsing_state) {
		parsing_function(&input, leftParse);

		if ((nextSymbol == "$") && ((stk.seek()).compare("$") == 0)) {
			cout << "Accept !!" << endl;
			cout << "LeftParse: ";
			for (int i = 0; leftParse[i]; i++)
				cout << leftParse[i];
			cout << endl;
			break;
		}
		else
			continue;
	}
	if (parsing_state)
		return 0;
	else{
		cout << "error !! " << endl;
	return 0; //parsing 실패 시 여기서 종료
	}
}
void getNextSymbol(string *str) {
	nextSymbol = str->substr(++index, 1);
}
void parsing_function(string *str, int leftParse[]) {
	string top = stk.seek();
	int result = pt.expand();

	if (result == 0) {
		if (nextSymbol.compare(top) == 0) {
			stk.pop();
			getNextSymbol(str);
		}
		else
			parsing_state = 0;
	}
	else
		leftParse[p_index++] = result;
}