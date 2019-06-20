#include<iostream>
using namespace std;

int Floor(double x);
int Ceiling(double x);

int main() {
	double neg = -9.8;
	double pos = 9.8;
	double intnum = 9.0;

	cout << neg << "의 floor 값은 " << Floor(neg) << "입니다." << endl;
	cout << pos << "의 floor 값은 " << Floor(pos) << "입니다." << endl;
	cout << intnum << "의 floor 값은 " << Floor(intnum) << "입니다." << endl;
	cout << "-----------------------------------" << endl;

	cout << neg << "의 ceiling 값은 " << Ceiling(neg) << "입니다." << endl;
	cout << pos << "의 ceiling 값은 " << Ceiling(pos) << "입니다." << endl;
	cout << intnum << "의 ceiling 값은 " << Ceiling(intnum) << "입니다." << endl;
	cout << "-----------------------------------" << endl;

	return 0;
}
int Floor(double x) {
	if (!(x - (int)x)) //x가 소수부를 갖지 않는다면
		return (int)x;
	else { //x가 소수부를 갖는다면
		if (x > 0) //x가 양수라면
			return (int)x;
		else if(x < 0) //x가 음수라면
			return ((int)x - 1);
	}
}
int Ceiling(double x) {
	if (!(x - (int)x)) //x가 소수부를 갖지 않는다면
		return (int)x;
	else { //x가 소수부를 갖는다면
		if (x > 0) //x가 양수라면
			return ((int)x + 1);
		else if (x < 0) //x가 음수라면
			return (int)x;
	}
}