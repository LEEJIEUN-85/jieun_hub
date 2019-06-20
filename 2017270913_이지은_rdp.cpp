/*---------------recursive descent parser--------------------*/
//2019-05-30 완성

//S -> aS
//S -> bA
//A -> d
//A -> ccA

#include<iostream>
#include<string>
using namespace std;

int pS(string *nextSymbol, string *str, int *index, int *leftParse, int *p_index);
int pA(string *nextSymbol, string *str, int *index, int *leftParse, int *p_index);
void getNextSymbol(string *nextSymbol, string *str, int *index);

int main() {
	string str;
	int leftParse[50] = { 0 };
	cout << "Enter a sentence: ";
	cin >> str;

	int len = str.length();
	string nextSymbol;
	int index = 0;
	int p_index = 0;
	nextSymbol = str.substr(0, 1);

	pS(&nextSymbol, &str, &index, leftParse, &p_index);
	if (nextSymbol == "$") {
		cout << "Accept !!" << endl;
		cout << "LeftParse: ";
		for (int i = 0; leftParse[i]; i++)
			cout << leftParse[i];
		cout << endl;
		return 0;
	}
	else {
		cout << "error !!" << endl;
		return 0;
	}
}
int pS(string *nextSymbol, string *str, int *index, int *leftParse, int *p_index) {
	if (*nextSymbol == "a") { // 생성규칙 1번
		leftParse[(*p_index)++] = 1;
		getNextSymbol(nextSymbol, str, index);
		pS(nextSymbol, str, index, leftParse, p_index);
	}
	else if (*nextSymbol == "b") { //생성규칙 2번
		leftParse[(*p_index)++] = 2;
		getNextSymbol(nextSymbol, str, index);
		pA(nextSymbol, str, index, leftParse, p_index);
	}
	else
		return 0;

	return 0;
}
int pA(string *nextSymbol, string *str, int *index, int *leftParse, int *p_index) {
	if (*nextSymbol == "d") { //생성규칙 3번
		leftParse[(*p_index)++] = 3;
		getNextSymbol(nextSymbol, str, index);
	}
	else if (*nextSymbol == "c") { //생성규칙 4번
		getNextSymbol(nextSymbol, str, index);
		if (*nextSymbol == "c") {
			leftParse[(*p_index)++] = 4;
			getNextSymbol(nextSymbol, str, index);
			pA(nextSymbol, str, index, leftParse, p_index);
		}
		else
			return 0;
	}
	else
		return 0;

	return 0;
}
void getNextSymbol(string *nextSymbol, string *str, int *index) {
	(*nextSymbol) = str->substr(++(*index), 1);
}