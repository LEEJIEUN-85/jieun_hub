#include<iostream>
#include<stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
	int max_num, index_row, last_num;
	char line[12],
		up_left[17], up_mid[16], up_right[16],
		mid_left[17], mid_mid[16], mid_right[16],
		down_left[17], down_mid[16], down_right[16];

	line[0] = 01; //┌
	line[1] = 02; //┐
	line[2] = 03;  //└
	line[3] = 04; //┘
	line[4] = 05; //│
	line[5] = 06; //─
	line[6] = 16; //┼
	line[7] = 21; //┴
	line[8] = 22; //┬
	line[9] = 23; //┤
	line[10] = 25; //├
	line[11] = 00; //공백

	up_left[0] = line[0];
	up_left[16] = up_mid[15] = line[8];
	up_right[15] = line[1];

	mid_left[0] = line[10];
	mid_left[16] = mid_mid[15] = line[6];
	mid_right[15] = line[9];

	down_left[0] = line[2];
	down_left[16] = down_mid[15] = line[7];
	down_right[15] = line[3];

	for (int i = 0; i < 15; i++)
		up_right[i] = up_mid[i] =mid_right[i] = mid_mid[i] = down_right[i] = down_mid[i] = line[5];
	for (int i = 1; i < 16; i++)
		up_left[i] = mid_left[i] = down_left[i] = line[5];

	max_num = atoi(argv[1]);
	
	cout <<"max multiplication number : " << max_num<< endl;

	index_row = max_num / 4;
	
	for (int k = 0; k <= index_row; k++) {
		if (k == index_row)
			last_num = max_num;
		else
			last_num = (k + 1) * 4;
		for (int j = 1; j <= last_num; j++) {
			if (k == 0 && j == 1) { //첫째 줄
				for (int p = 0; p < last_num; p++) {
					if (p == 0) {
						for (int q = 0; q < 17; q++)
							cout << up_left[q];
					}
					else if (p == last_num - 1) {
						for (int q = 0; q < 16; q++)
							cout << up_right[q];
					}
					else {
						for (int q = 0; q < 16; q++)
							cout << up_mid[q];
					}
				}
				cout << endl;
			}
			else if (k != 0 && j == 1) { //중간줄
				for (int p = 0; p < 4; p++) {
					if (p == 0) {
						for (int q = 0; q < 17; q++)
							cout << mid_left[q];
					}
					else if (p == 3) {
						for (int q = 0; q < 16; q++)
							cout << mid_right[q];
					}
					else {
						for (int q = 0; q < 16; q++)
							cout << mid_mid[q];
					}
				}
				cout << endl;
			}
			for (int i = k * 4 + 1; i <= last_num; i++) {
				if (i < j) {
					if (i == k * 4 + 1)
						cout << line[4] << "\t\t" << line[4];
					else
						cout << "\t\t" << line[4];
				}

				else {
					if (i == k * 4 + 1)
						cout << line[4] << line[11] << i << " x " << j << " = " << i * j << "\t" << line[4];
					else
						cout << line[11] << i << " x " << j << " = " << i * j << "\t" << line[4];

				}
			}
			cout << endl;
		}
		if (k == index_row) {
			for (int p = 0; p < max_num % 4; p++) {
				if (p == 0)
					for (int q = 0; q < 17; q++)
						cout << down_left[q];
				else if (p == (max_num % 4) - 1)
					for (int q = 0; q < 16; q++)
						cout << down_right[q];
				else
					for (int q = 0; q < 16; q++)
						cout << down_mid[q];
			}
		}
	}

	return 0;
}