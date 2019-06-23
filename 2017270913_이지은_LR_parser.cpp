#include<iostream>
#include<string>
#define STATE 12
#define SYMBOL 9
using namespace std;

/*
1. E -> E + T
2. E -> T
3. T -> T + F
4. T -> F
5. F -> (E)
6. F -> a  
*/

int parsing(string input); //파싱하는 함수
void reduce_1();
void reduce_2();
void reduce_3();
void reduce_4();
void reduce_5();
void reduce_6();

int stack_top = 0;
int index = 0; //input string index
int r_index = 0; //rightParse index
int parsing_state = 0; //-1 : error, 0: parsing중 , 1: accept
char stack[100] = { "0" };
int rightParse[50];
int pTable[STATE][SYMBOL] = {
	/*symbol :  a  +  *  (  )  $  E  T  F  */
	/*state 0*/ {5, 0, 0, 4, 0, 0, 1, 2, 3},
	/*state 1*/ {0, 6, 0, 0, 0, -7, 0, 0, 0}, //7번 생성규칙으로 reduce될 때, accept된다
	/*state 2*/ {0, -2, 7, 0, -2, -2, 0, 0, 0},
	/*state 3*/{0, -4, -4, 0, -4, -4, 0, 0, 0},
	/*state 4*/{5, 0, 0, 4, 0, 0, 8, 2, 3},
	/*state 5*/{0, -6, -6, 0, -6, -6, 0, 0, 0},
	/*state 6*/{5, 0, 0, 4, 0, 0, 0, 9, 3},
	/*state 7*/{5, 0, 0, 4, 0, 0, 0, 0, 10},
	/*state 8*/{0, 6, 0, 0, 11, 0, 0, 0, 0},
	/*state 9*/{0, -1, 7, 0, -1, -1, 0, 0, 0},
	/*state 10*/{0, -3, -3, 0, -3, -3, 0, 0, 0},
	/*state 11*/{0, -5, -5, 0, -5, -5, 0, 0, 0}
};

/* ############# MAIN #############*/

int main() {
	string input;
	cout << "Input string: ";
	cin >> input;

	parsing(input);
	if (parsing_state == -1) //error
		cout << "Error: Wrong sentence" << endl;
	else if (parsing_state == 1) { //accept
		cout << "Accept!!" << endl << "Right Parse: ";
		for (int i = 0; i < r_index; i++)
			cout << rightParse[i] << ' ';
		cout << endl;
	}
	return 0;
}

/*
1. E -> E + T
2. E -> T
3. T -> T + F
4. T -> F
5. F -> (E)
6. F -> a
*/
int parsing(string input) {
	char c; //지금 보고있는 input값
	int num, p_num;

	while (parsing_state == 0) {
		c = input.at(index);
		switch (c) {
		case 'a':
			num = 0;
			break;
		case '+':
			num = 1;
			break;
		case '*':
			num = 2;
			break;
		case '(':
			num = 3;
			break;
		case ')':
			num = 4;
			break;
		case '$':
			num = 5;
			break;
		default:
			parsing_state = -1;
			return 0;
		}
		int tmp = (char)stack[stack_top] - '0';
		p_num = pTable[tmp][num];

		if (p_num == 0) {
			parsing_state = -1;
			return 0;
		}
		else if (p_num < 0) { //reduce
			switch (p_num) {
			case -1:
				reduce_1();
				break;
			case -2:
				reduce_2();
				break;
			case -3:
				reduce_3();
				break;
			case -4:
				reduce_4();
				break;
			case -5:
				reduce_5();
				break;
			case -6:
				reduce_6();
				break;
			case -7: //accept
				parsing_state = 1;
				return 0;
			}
		}
		else { //shift
			stack[++stack_top] = c;
			stack[++stack_top] = '0' + p_num;
			index++;
		}
	}
	
	return 0; 
}
void reduce_1() {
	stack_top--;
	if (stack[stack_top] == 'T') {
		stack_top -= 2;
		stack[stack_top + 1] = NULL; //상태번호까지 pop
		if (stack[stack_top] == '+') {
			stack_top -= 2;
			stack[stack_top + 1] = NULL; //상태번호까지 pop
			if (stack[stack_top] == 'E') {
				stack[stack_top] = NULL;
				stack[stack_top] = 'E';
				int tmp = stack[stack_top - 1] - '0';
				stack[++stack_top] = '0' + pTable[tmp][6];
				rightParse[r_index++] = 1;
			}
		}
	}
}
void reduce_2() {
	stack_top--;
	if (stack[stack_top] == 'T') {
		stack[stack_top] = NULL;
		stack[stack_top] = 'E';
		int tmp = stack[stack_top - 1] - '0';
		stack[++stack_top] = '0' + pTable[tmp][6];
		rightParse[r_index++] = 2;
	}
}
void reduce_3() {
	stack_top--;
	if (stack[stack_top] == 'F') {
		stack_top -= 2;
		stack[stack_top + 1] = NULL; //상태번호까지 pop
		if (stack[stack_top] == '*') {
			stack_top -= 2;
			stack[stack_top + 1] = NULL; //상태번호까지 pop
			if (stack[stack_top] == 'T') {
				stack[stack_top] = NULL;
				stack[stack_top] = 'T';
				int tmp = stack[stack_top - 1] - '0';
				stack[++stack_top] = '0' + pTable[tmp][7];
				rightParse[r_index++] = 3;
			}
		}
	}
}
void reduce_4() {
	stack_top--;
	if (stack[stack_top] == 'F') {
		stack[stack_top] = NULL;
		stack[stack_top] = 'T';
		int tmp = stack[stack_top - 1] - '0';
		stack[++stack_top] = '0' + pTable[tmp][7];
		rightParse[r_index++] = 4;
	}
}
void reduce_5() {
	stack_top--;
	if (stack[stack_top] == ')') {
		stack_top -= 2;
		stack[stack_top + 1] = NULL; //상태번호까지 pop
		if (stack[stack_top] == 'E') {
			stack_top -= 2;
			stack[stack_top + 1] = NULL; //상태번호까지 pop
			if (stack[stack_top] == '(') {
				stack[stack_top] = NULL;
				stack[stack_top] = 'F';
				int tmp = stack[stack_top - 1] - '0';
				stack[++stack_top] = '0' + pTable[tmp][8];
				rightParse[r_index++] = 5;
			}
		}
	}
}
void reduce_6() {
	stack_top--;
	if (stack[stack_top] == 'a') {
		stack[stack_top] = NULL;
		stack[stack_top] = 'F';
		int tmp = stack[stack_top - 1] - '0';
		stack[++stack_top] = '0' + pTable[tmp][8];
		rightParse[r_index++] = 6;
	}
}