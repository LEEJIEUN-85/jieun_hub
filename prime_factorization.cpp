#include<iostream>
using namespace std;

int change_prime(int *n, int *end);
int main()
{
	int n = 2;

	while (n > 1) {
		int arr[10] = { 0 };
		cout << "1���� ū ������ �Է�(1 ������ �� �Է�: ����) :";
		cin >> n;
		
		if (n <= 1) {
			cout << "���α׷��� �����մϴ�." << endl;
			return 0;
		}	
		cout << endl << "\t" << n << " = ";

		int end = (int)sqrt(n);

		int prime = -1;
		int i = 0;
		while ((prime = change_prime(&n, &end)) != 0) 
			arr[i++] = prime;
		
		if (prime == 0)
			arr[i] = n;

		int len = 0;
		while (arr[len] != 0)
			len++;

		
		for (i = 0; i < len - 1; i++)
			cout << arr[i] << " * ";
		cout << arr[i] << endl << endl;
		}
	return 0;
}
int change_prime(int *n, int *end)
{
	for (int i = 2; i <= *end; i++) {
		if (((*n) % i) == 0) {
			(*n) /= i;
			(*end) = (int)sqrt(*n);
			return i;
		}
	}
	return 0;
}