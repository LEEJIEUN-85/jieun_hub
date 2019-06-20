#include<iostream>
using namespace std;

int Floor(double x);
int Ceiling(double x);

int main() {
	double neg = -9.8;
	double pos = 9.8;
	double intnum = 9.0;

	cout << neg << "�� floor ���� " << Floor(neg) << "�Դϴ�." << endl;
	cout << pos << "�� floor ���� " << Floor(pos) << "�Դϴ�." << endl;
	cout << intnum << "�� floor ���� " << Floor(intnum) << "�Դϴ�." << endl;
	cout << "-----------------------------------" << endl;

	cout << neg << "�� ceiling ���� " << Ceiling(neg) << "�Դϴ�." << endl;
	cout << pos << "�� ceiling ���� " << Ceiling(pos) << "�Դϴ�." << endl;
	cout << intnum << "�� ceiling ���� " << Ceiling(intnum) << "�Դϴ�." << endl;
	cout << "-----------------------------------" << endl;

	return 0;
}
int Floor(double x) {
	if (!(x - (int)x)) //x�� �Ҽ��θ� ���� �ʴ´ٸ�
		return (int)x;
	else { //x�� �Ҽ��θ� ���´ٸ�
		if (x > 0) //x�� ������
			return (int)x;
		else if(x < 0) //x�� �������
			return ((int)x - 1);
	}
}
int Ceiling(double x) {
	if (!(x - (int)x)) //x�� �Ҽ��θ� ���� �ʴ´ٸ�
		return (int)x;
	else { //x�� �Ҽ��θ� ���´ٸ�
		if (x > 0) //x�� ������
			return ((int)x + 1);
		else if (x < 0) //x�� �������
			return (int)x;
	}
}