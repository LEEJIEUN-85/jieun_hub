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
		if (cur_state == state[0]) { //���� ���°� p
			if (str[i] == '0') { //0�� ������
				cur_state = state[1]; //���� ���� q�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
			else { //1�� ������
				cur_state = state[0]; //���� ���� p�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else if (cur_state == state[1]) { //���� ���°� q
			if (str[i] == '0') { //0�� ������
				cur_state = state[2]; //���� ���� r�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input recognized !!" << endl;
					break;
				}
			}
			else { //1�� ������
				cur_state = state[0]; //���� ���� p�� ����
				if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
					cout << "input NOT recognized" << endl;
					break;
				}
			}
		}
		else { //���� ���°� r
			cur_state = state[2]; //0�̵� 1�̵� ���� ���� r�� ����
			if (str[i + 1] == NULL) { //��� �� ���ڰ� input�� �������̿��ٸ�
				cout << "input recognized !!" << endl;
				break;
			}
		}
	}
	
	
	return 0;
}