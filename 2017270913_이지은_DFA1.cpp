#include<iostream>
#include<string>
#define NUM_STATE 3
using namespace std;

int main() {
	string cur_state;
	string str;
	string state[NUM_STATE] = { "p", "q", "r" };
	string final_state = state[2];

	cout << "Input a String : ";
	cin >> str;

	cur_state = state[0];
	for (int i = 0; str[i]; i++) {
		if (cur_state == state[0]) { //현재 상태가 p
			if (str[i] == '0') { //0이 들어오면
				cur_state = state[1]; //현재 상태 q로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
			else { //1이 들어오면
				cur_state = state[0]; //현재 상태 p로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[1]) { //현재 상태가 q
			if (str[i] == '0') { //0이 들어오면
				cur_state = state[2]; //현재 상태 r로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //1이 들어오면
				cur_state = state[0]; //현재 상태 p로 변경
				if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else { //현재 상태가 r
			cur_state = state[2]; //0이든 1이든 현재 상태 r로 변경
			if (str[i + 1] == NULL) { //방금 본 문자가 input의 마지막이였다면
				cout << "input recognized !!" << endl;
				break;
			}
		}
	}
	
	
	return 0;
}