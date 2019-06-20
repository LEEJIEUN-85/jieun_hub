#include<iostream>
#include<Windows.h>
#include<iomanip> //setw()함수 : 인자로 넘어가는 정수값만큼 공백으로 출력
#include<psapi.h>
#include<time.h>
using namespace std;

int main(int argc, char* argv[]) {
	SYSTEMTIME st;
	SYSTEM_INFO systemInfo;
	DWORD cpu_count, proc[1024], bytes, temp;
	MEMORYSTATUSEX mem_load;
	FILETIME idle, kernel, user;
	double pre_idle, cur_idle, pre_kernel, cur_kernel, pre_user, cur_user, cpu_load, arr[15] = { 0 };
	double avg_5 =0 , avg_10 = 0, avg_15 = 0;
	int index = 0, tmp_index, NumOfProc;
	time_t idle_time2;

	GetSystemInfo(&systemInfo); //cpu개수 받아옴
	
	cpu_count = systemInfo.dwNumberOfProcessors; //CPU의 개수
	cout << "Number of CPUs  :  " << cpu_count << endl;

	cout << "Index" << setw(17) << "Time" << setw(25) << "Processes" << setw(20) << "MemoryLoad" << setw(13) <<"CPULoad" << endl;

	GetSystemTimes(&idle, &kernel, &user);
	pre_idle =(double)( idle.dwHighDateTime + idle.dwLowDateTime);
	pre_kernel = (double)(kernel.dwHighDateTime + kernel.dwLowDateTime);
	pre_user = (double)(user.dwHighDateTime + user.dwLowDateTime);

	for (int i = 0;  ; i++) { 
		
		Sleep(1000);
		
		GetLocalTime(&st); //st에 현재시간 받아옴
		EnumProcesses(proc, sizeof(proc), &bytes);
		NumOfProc = bytes / sizeof(DWORD);
		mem_load.dwLength = sizeof(mem_load);
		GlobalMemoryStatusEx(&mem_load);

		GetSystemTimes(&idle, &kernel, &user);
		cur_idle = (double)(idle.dwHighDateTime + idle.dwLowDateTime);
		cur_kernel = (double)(kernel.dwHighDateTime + kernel.dwLowDateTime);
		cur_user = (double)(user.dwHighDateTime + user.dwLowDateTime);

		cpu_load = (((cur_kernel - pre_kernel) + (cur_user - pre_user) - (cur_idle - pre_idle)) / ((cur_kernel - pre_kernel) + (cur_user - pre_user))) * 100;
		arr[index % 15] = cpu_load;

		cout << setw(5) << index << setw(9);
		
		cout << st.wYear << "_" << st.wMonth << "_" << st.wDay << " " << st.wHour << "_" << st.wMinute << "_" << st.wSecond;
		cout.fill(' ');
		cout << "\t:\t";
		cout << NumOfProc << setw(20) << (mem_load.dwMemoryLoad) << "%";
		cout << fixed;
		cout.precision(2);
		cout << setw(20) << "[CPU Load:\t" <<  cpu_load << "] ";
		if ((index % 15) > 4 || index > 14) { //5sec_avg 계산하는 for문
			tmp_index = index - 5;
			for (int j = 0; j < 5; j++, tmp_index++)
				avg_5 += arr[tmp_index % 15];
			avg_5 /= 5.0;
			cout << "[5sec avg:\t" << avg_5 << "] ";
		}
		else {
			cout << endl;
			index++;
			continue;
		}
		if ((index % 15) > 9 || index > 14) { //10sec_avg 계산하는 for문
			tmp_index = index - 10;
			for (int j = 0; j < 10; j++, tmp_index++)
				avg_10 += arr[tmp_index % 15];
			avg_10 /= 10.0;
			cout << "[10sec avg:\t" << avg_10 << "] ";
		}
		else {
			cout << endl;
			index++;
			continue;
		}
		if (index > 14) { //15sec_avg 계산하는 for문
			for (int j = 0; j < 15; j++)
				avg_15 += arr[j];
			avg_15 /= 15.0;
			cout << "[15sec avg:\t" << avg_15 << "]";
		}
		else {
			cout << endl;
			index++;
			continue;
		}
		cout << endl;
		index++;
	}
	return 0;
}

