#include<iostream>
using namespace std;

class Set {
	char p[26];
	int size;
public:
	Set() { size = 0; }
	Set(const char *s);
	friend ostream& operator << (ostream &out, Set s); //Set ��ü ����� ������
	Set operator+(char newElement);//���� �߰�
	Set operator| (Set op2); //������
	Set operator&(Set op2); //������
	friend bool operator <=(Set op1, Set op2); //op1�� op2�� �κ��������� ��
	friend Set operator~(Set s); // ������(��ü ���� = �ҹ��� ��ü)
	friend Set operator-(Set op1, Set op2); //������
};
Set::Set(const char* s)
{
	int same = 0;
	int count = 0;
	int tmp = 0;
	for (int i = 0; s[i]; i++)
		tmp++;

	size = 0;

	p[size++] = s[0];
	for (int i = 1; i < tmp; i++) {
		same = 0;
		for (int j = 0; j < size; j++) {
			if (p[j] == s[i]) {
				same = 1;
				break;
			}
			else
				continue;
		}
		if (!same)
			p[size++] = s[i];
	}
	p[size] = NULL;
}
Set Set::operator+(char Element) //�����߰�
{
	int same = 0;
	for (int i = 0; i < size; i++) {
		if (p[i] == Element)
			return *this;
	}
	p[size++] = Element;
	p[size] = NULL;

	return *this;
}
Set Set::operator|(Set op2) //������
{
	Set tmp(p);
	int count = 0;

	for (int i = 0; i < op2.size; i++) {
		count = 0;
		for (int j = 0; j < tmp.size; j++) {
			if (op2.p[i] == tmp.p[j]) {
				count = 1;
				break;
			}
		}
		if (!count)
			tmp = tmp + op2.p[i];
	}
	return tmp;
}
Set Set::operator&(Set op2) //������
{
	Set tmp;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < op2.size; j++) {
			if (p[i] == op2.p[j]) {
				tmp = tmp + p[i];
				break;
			}
		}
	}
	return tmp;
}
bool operator <= (Set op1, Set op2) //op1�� op2�� �κ������ΰ�?
{
	for (int i = 0; i < op1.size; i++) {
		for (int j = 0; j < op2.size; j++) {
			if (op1.p[i] != op2.p[i])
				return false;
		}
	}
	return true;
}
Set operator~(Set s) //������
{
	Set tmp;
	int count = 0;
	for (int i = 0; i < 26; i++) {
		count = 0;
		for (int j = 0; j < s.size; j++) {
			if (s.p[j] == (char)('a' + i)) {
				count = 1;
				break;
			}
		}
		if (!count)
			tmp = tmp + (char)('a' + i);
	}
	return tmp;
}
Set operator-(Set op1, Set op2) //������
{
	Set tmp;
	char arr[26];
	for (int i = 0; i < op1.size; i++)
		arr[i] = op1.p[i];
	arr[op1.size] = 0;

	for (int i = 0; arr[i]; i++) {
		for (int j = 0; j < op2.size; j++) {
			if (arr[i] == op2.p[j]) {
				arr[i] = ' ';
				break;
			}
		}
	}

	for (int i = 0; arr[i]; i++) {
		if (arr[i] != ' ')
			tmp = tmp + arr[i];
	}
	return tmp;
}

ostream& operator << (ostream &out, Set s) {
	if (!s.size) {
		cout << "{ }";
		return out;
	}
	out << "{";
	for (int i = 0; i < s.size; i++)
		out << s.p[i] << ", ";
	out << "\b\b}";
	return out;
}
int main()
{
	Set s1, s2("mathematics"), s3, s4;

	s3 = s3 + 't' + 'e' + 's' + 't' + 'i' + 'n' + 'g';	//���� ����
	cout << "s1 = " << s1 << "\ns2 = " << s2 << "\ns3 = " << s3 << "\n\n";

	s1 = s2 & s3;		//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";

	s1 = s2 | s3;		//������
	cout << s2 << " �� " << s3 << " = " << s1 << "\n\n";

	s4 = s1 - s3;		//������
	cout << s1 << " - " << s3 << " = " << s4 << "\n\n";

	s4 = ~s1;			//������
	cout << "~" << s1 << " = " << s4 << "\n\n";

	if (s2 <= s1)
		cout << s2 << " �� " << s1 << "\n\n";

	if (!(s2 <= s3))
		cout << s2 << " is not susbset of " << s3 << "\n\n";

	return 0;
}