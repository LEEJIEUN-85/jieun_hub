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
		if (cur_state == state[0]) { //���� ���°� A
			if (str[i] == 'a') { //a ������
				cur_state = state[3]; //���� ���� D�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b ������
				cur_state = state[0]; //���� ���� A�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[1]) { //���� ���°� B
			if (str[i] == 'a') { //a ������
				cur_state = state[2]; //���� ���� C�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
			else { //b ������
				cout << "input NOT recognized" << endl;
				break;
			}
		}
		else if(cur_state == state[2]){ //���� ���°� C
			if (str[i] == 'a') { //a ������
				cur_state = state[4]; //���� ���� E�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b ������
				cur_state = state[0]; //���� ���� A�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[3]) { //���� ���°� D
			if (str[i] == 'a') { //a ������
				cur_state = state[4]; //���� ���� E�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b ������
				cur_state = state[1]; //���� ���� B�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else { //���� ���°� E
			if (str[i] == 'a') { //a ������
				cur_state = state[4]; //���� ���� E�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //b ������
				cur_state = state[2]; //���� ���� C�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
	}


	return 0;
}