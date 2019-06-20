#include<iostream>
#include<string>
#define NUM_STATE 5
using namespace std;

int main() {
	string cur_state;
	string str;
	string state[NUM_STATE] = { "A", "B", "C", "D", "E" };
	string final_state[2] = { state[3], state[4] };

	cout << "Input a String : ";
	cin >> str;

	cur_state = state[0];
	for (int i = 0; str[i]; i++) {
		if (cur_state == state[0]) { //현재 상태가 A
			if (str[i] == 'a') { //a 들어오면
				cur_state = state[3]; //현재 상태 D로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b 들어오면
				cur_state = state[0]; //현재 상태 A로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[1]) { //현재 상태가 B
			if (str[i] == 'a') { //a 들어오면
				cur_state = state[2]; //현재 상태 C로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
			else { //b 들어오면
				cout << "input NOT recognized" << endl;
				break;
			}
		}
		else if(cur_state == state[2]){ //현재 상태가 C
			if (str[i] == 'a') { //a 들어오면
				cur_state = state[4]; //현재 상태 E로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b 들어오면
				cur_state = state[0]; //현재 상태 A로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[3]) { //현재 상태가 D
			if (str[i] == 'a') { //a 들어오면
				cur_state = state[4]; //현재 상태 E로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b 들어오면
				cur_state = state[1]; //현재 상태 B로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else { //현재 상태가 E
			if (str[i] == 'a') { //a 들어오면
				cur_state = state[4]; //현재 상태 E로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b 들어오면
				cur_state = state[2]; //현재 상태 C로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
	}


	return 0;
}