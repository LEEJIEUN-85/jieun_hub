#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <math.h>

#define MAXSTAGE 3
#define BITMAP_W 40
#define BITMAP_H 40

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("Animal game_2");

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, BOOL isBlink); //��Ʈ�� �׸��� �Լ�
BOOL RemoveIcon(HWND hWnd); //�ٲ� ��ǥ �������� 3�� �̻� ������ �������ִ� �Լ�
BOOL RemoveWithDir(HWND hWnd); //�ٲ�� �� ��ǥ �������� 3�� �̻� ������ �������ִ� �Լ�
void function(HDC hdc, char a, int x, int y);
void InitStage(); //�������� ù ȭ�� �����ϴ� �Լ�
void DrawAllScreen(HDC hdc); //ù ȭ�� �׸��� �Լ�
BOOL TestEnd(); //������ �������� Ȯ���ϴ� �Լ�
void Move(HDC hdc, int dir); //�����̴� �Լ�

int nStage; //���������� Ȯ���ϴ� ����
char nArray[10][11];
static int nx, ny;
static int dir_num;
static BOOL blink; //��Ʈ���� �������� ���� ����
static BOOL nowChange; //�ܼ��̵�����, �����ܱ�ȯ����(=�����̽��� ���ȴ��� �ȴ��ȴ���)
static int score; //����
HBITMAP hBit[6]; //��Ʈ���� �ҷ��ͼ� ����

char ori_Array[MAXSTAGE][10][11] =
{
	{
		"@#@@%$#@#$",
		"##@%$@@&$@",
		"#@&&%$@&$%",
		"$%@#@@$%%#",
		"@@%&@##@$$",
		"&%%#$&&@##",
		"@&#%&$%&@$",
		"%%@@$@$@&#",
		"#$$%@%&&@%",
		"&$#@##$$@&",
	},
{
	"@&#%&$%&@$",
	"$%$#@@$%%#",
	"##@%$@@&$@",
	"@#$@%$#@#$",
	"%%@#$@$@&#",
	"&$#@##$$@&",
	"&%%#$&&@##",
	"#@&&%$@&$%",
	"@#@#$&&%%$",
	"##@%$@&@$&",
},
{
	"#@&&%$@&$%",
	"@#@#$&@%%$",
	"%%##@@$@&#",
	"##@%$@&&$@",
	"@&#%&$%&&$",
	"@#$@%$#@#$",
	"%%&#$@$@&#",
	"@#@#$&@%%$",
	"$#%%$@&&$@",
	"&$#@##$$@&",
}
};
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd; // hWnd ������ ���������� ����!

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	int i;
	TCHAR Message[256]; //ȭ�鿡 ���ڿ� ����� �� ���

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 800, BITMAP_H * 10 + 30);
		AdjustWindowRect(&crt, WS_CAPTION | WS_MINIMIZEBOX, FALSE);
		//crt ũ��� �ش� ������ ��Ÿ�Ͽ� �´� ������ ũ�⸦ ����Ͽ� crt�� �ٽ� ����
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, //������ ũ�� ����
			SWP_NOMOVE | SWP_NOZORDER);
		for (i = 0; i < 6; i++)
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BEAR + i)); //��Ʈ���� �ҷ���
		nStage = 0;
		dir_num = 0;
		score = 0;
		nowChange = FALSE;
		InitStage();
		SetTimer(hWnd, 1, 90000, NULL);
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			KillTimer(hWnd, 1);
			wsprintf(Message, _T("Stage %d is over!\n") _T("Your score is %d !\n")_T("Go to the next step?"), nStage + 1, score);
			if (MessageBox(hWnd, Message, _T("Game Over"), MB_OKCANCEL) == IDOK) {
				nStage++;
				InitStage();
				SetTimer(hWnd, 1, 90000, NULL);
			}
			else
				DestroyWindow(hWnd);
			break;
		case 2:
			RemoveIcon(hWnd);
			RemoveWithDir(hWnd);
			nowChange = FALSE;
			KillTimer(hWnd, 2);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawAllScreen(hdc); //ȭ���� �׸���
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			nowChange = TRUE;
			break;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN: //����Ű���� ������
			hdc = GetDC(hWnd);
			Move(hdc, wParam); //Move�Լ��� ȣ���ؼ� �������� �̵���Ŵ
			if (nowChange)
				SetTimer(hWnd, 2, 500, NULL);
			if (TestEnd()) { //TestEnd�Լ��� ������ �������� Ȯ���ϰ�, �����ٸ�
				if (nStage != MAXSTAGE) { //������ ���������� �ƴ϶�� ���� ���������� �̵�
					wsprintf(Message, _T("Stage %d clear!\n") _T("Your score is %d !\n")_T("Go to the next step?"), nStage + 1, score);
					if (MessageBox(hWnd, Message, _T("Clear"), MB_OKCANCEL) == IDOK) {
						KillTimer(hWnd, 1);
						nStage++;
						InitStage();
						SetTimer(hWnd, 1, 90000, NULL);
					}
				}
				else {
					KillTimer(hWnd, 1);
					wsprintf(Message, _T("Stage %d clear!\n") _T("Your score is %d !\n") _T("It was a last stage! !\n")_T("Bye Bye :-)"), nStage + 1, score);
					MessageBox(hWnd, Message, _T("End"), MB_OK);
					DestroyWindow(hWnd);
				}
			}
			ReleaseDC(hWnd, hdc);
			break;

		case 'Q': //Q�� �ԷµǸ�(Quit)
			wsprintf(Message, _T("Do you want to quit?\n") _T("Your score is %d !\n")_T("Go to the next step?"), nStage + 1, score);
			if (MessageBox(hWnd, _T("Do you want to quit?\n"), _T("Quit"), MB_OKCANCEL) == IDOK) {
				wsprintf(Message, _T("Your score is %d !\n")_T("Bye Bye :-)"), score);
				KillTimer(hWnd, 1);
				MessageBox(hWnd, Message, _T("Quit"), MB_OK);
				DestroyWindow(hWnd);
			}
			break;
		case 'R': //R�� �ԷµǸ�(Reset)
			if (MessageBox(hWnd, _T("Do you want to reset?"), _T("Reset?"), MB_OKCANCEL) == IDOK) {
				KillTimer(hWnd, 1);
				InitStage();
				SetTimer(hWnd, 1, 90000, NULL);
			}
		
			break;
		case 'D': //D�� �ԷµǸ�
			if (nStage < MAXSTAGE - 1) {
				KillTimer(hWnd, 1);
				nStage++; //�������� ��ȣ�� 1 ������Ű��
				InitStage(); //�ö� ���������� ù ȭ���� �׸���.
				SetTimer(hWnd, 1, 90000, NULL);
			}
			break;
		case 'A': //A�� �ԷµǸ�
			if (nStage > 0) {
				KillTimer(hWnd, 1);
				nStage--; //�������� ��ȣ�� 1 ���ҽ�Ű��
				InitStage(); //������ ���������� ù ȭ���� �׸���.
				SetTimer(hWnd, 1, 90000, NULL);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		for (i = 0; i < 6; i++)
			DeleteObject(hBit[i]); //��Ʈ���� �޸𸮸� �ϳ��� ������Ų��.
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void DrawAllScreen(HDC hdc)
{
	int x, y;
	TCHAR Message[256];

	for (y = 0; y < 10; y++) {
		for (x = 0; x < 10; x++) {
			function(hdc, nArray[y][x], x, y);
		}
	}

	SetTextAlign(hdc, TA_CENTER); //���ڿ��� �߾����Ľ�Ŵ
	SetBkMode(hdc, TRANSPARENT); //���ڿ� ������ ����

	wsprintf(Message, _T("<<Animal Game>>"));
	TextOut(hdc, 600, 90, Message, lstrlen(Message));
	wsprintf(Message, _T("���ѽð� : 90��"));
	TextOut(hdc, 600, 110, Message, lstrlen(Message));
	wsprintf(Message, _T("Q : ����, R : �ٽ� ����"));
	TextOut(hdc, 600, 130, Message, lstrlen(Message));
	wsprintf(Message, _T("A : ���� ��������, D : ���� ��������"));
	TextOut(hdc, 600, 150, Message, lstrlen(Message));
	wsprintf(Message, _T("�������� : %d"), nStage + 1);
	TextOut(hdc, 600, 190, Message, lstrlen(Message));
	wsprintf(Message, _T("���� : %d"), score);
	TextOut(hdc, 600, 210, Message, lstrlen(Message));
	wsprintf(Message, _T("������ �̵� ��� : �����̽��� �� ���ϴ� ����Ű"));
	TextOut(hdc, 600, 280, Message, lstrlen(Message));
}
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, BOOL isBlink)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	if (blink)
		BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, NOTSRCCOPY); //���� ��ġ�� �̹����� �������Ѽ� ǥ����.
	else
		BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
BOOL TestEnd() //������ �������� Ȯ���ϴ� �Լ�
{
	int x, y;
	for (y = 0; y < 10; y++) {
		for (x = 0; x < 10; x++) {
			if (nArray[y][x] != ' ')
				return FALSE;
		}
	}
	return TRUE;
}
void Move(HDC hdc, int dir)
{
	char tmp;
	int whatBit = 0;
	int old_x = nx, old_y = ny;

	if (nowChange) { //�����̽��ٰ� ������ ����Ű�� ���ȴٸ�(=������ ��ȯ)
		switch (dir) {
		case VK_LEFT:
			if (nx != 0) {
				tmp = nArray[ny][nx];
				nx -= 1;
				nArray[ny][nx + 1] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				function(hdc, nArray[ny][nx + 1], nx + 1, ny); //�����ڸ�
				dir_num = -1;
			}
			else {
				tmp = nArray[ny][nx];
				nx = 9;
				nArray[ny][0] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][0], 0, ny); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = -2;
			}
			break;
		case VK_RIGHT:
			if (nx != 9) {
				tmp = nArray[ny][nx];
				nx += 1;
				nArray[ny][nx - 1] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][nx - 1], nx - 1, ny); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = 1;
			}
			else {
				tmp = nArray[ny][nx];
				nx = 0;
				nArray[ny][9] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][9], 9, ny); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = 2;
			}
			break;
		case VK_UP:
			if (ny != 0) {
				tmp = nArray[ny][nx];
				ny -= 1;
				nArray[ny + 1][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny + 1][nx], nx, ny + 1); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = -10;
			}
			else {
				tmp = nArray[ny][nx];
				ny = 9;
				nArray[0][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[0][nx], nx, 0); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = -20;
			}
			break;
		case VK_DOWN:
			if (ny != 9) {
				tmp = nArray[ny][nx];
				ny += 1;
				nArray[ny - 1][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny - 1][nx], nx, ny - 1); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = 10;
			}
			else {
				tmp = nArray[ny][nx];
				ny = 0;
				nArray[9][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[9][nx], nx, 9); //�����ڸ�
				function(hdc, nArray[ny][nx], nx, ny); //�̵��ذ� �ڸ�
				dir_num = 20;
			}
			break;
		}
	}
	else { //�����̽��� �ȴ����� ����Ű ����(=������ǥ�̵�)
		switch (dir) {
		case VK_LEFT:
			if (nx != 0)
				nx -= 1;
			else
				nx = 9;
			break;
		case VK_RIGHT:
			if (nx != 9)
				nx += 1;
			else
				nx = 0;
			break;
		case VK_UP:
			if (ny != 0)
				ny -= 1;
			else
				ny = 9;
			break;
		case VK_DOWN:
			if (ny != 9)
				ny += 1;
			else
				ny = 0;
			break;
		}
		function(hdc, nArray[old_y][old_x], old_x, old_y);
		function(hdc, nArray[ny][nx], nx, ny);
	}
}
void InitStage() //�� ���������� ù ȭ���� �׸��� �Լ�
{
	memcpy(nArray, ori_Array[nStage], sizeof(nArray));

	nx = rand() % 10;
	ny = rand() % 10; //�������� ù ��ǥ ����
	score = 0; //������ 0���� �ʱ�ȭ�Ѵ�.
	InvalidateRect(hWndMain, NULL, TRUE); //ȭ���� ���� �׸���.
}
void function(HDC hdc, char a, int x, int y) {
	int whatBit = 0;
	switch (a) {
	case '#': //��
		whatBit = 0;
		break;
	case '$': //������
		whatBit = 1;
		break;
	case '%': //����
		whatBit = 2;
		break;
	case '&': //����
		whatBit = 3;
		break;
	case '@': //�䳢
		whatBit = 4;
		break;
	case ' ': //�������� �� �׸�
		whatBit = 5;
		break;
	}
	if ((x == nx) && (y == ny))
		blink = TRUE; //���� ��ġ�� ���� �׸�����
	else
		blink = FALSE;
	DrawBitmap(hdc, x * BITMAP_W + 3, y * BITMAP_H + 3, hBit[whatBit], blink); //iBit�� �ش��ϴ� ��Ʈ���� �׸���.
}
BOOL RemoveIcon(HWND hWnd)
{
	int l_count = 0; //���ʿ� ������ �� �� �ִ��� ����
	int r_count = 0; //������
	int u_count = 0; //����
	int d_count = 0; //�Ʒ���

	int i;
	char animal;

	HWND r_hwnd = hWnd;
	HDC r_hdc;

	animal = nArray[ny][nx]; //�ݺ��� �ȿ��� animal�� ���Ұ���

	if (animal == ' ')
		return FALSE;

	r_hdc = GetDC(r_hwnd);

	//nx - 1���� �����ؼ� �������� ���ҽ��Ѱ��鼭 count!
	for (i = nx - 1; i >= 0; i--) {
		if (animal != nArray[ny][i])
			break;
		else
			l_count++;
	}
	//nx + 1���� �����ؼ� ���������� �������Ѱ��鼭 count!
	for (i = nx + 1; i <= 9; i++) {
		if (animal != nArray[ny][i])
			break;
		else
			r_count++;
	}
	if (l_count + r_count >= 2) { //nx���� ���������� 3�� �̻� �����ƴٸ� �Ͷ߸�
		for (i = nx - l_count; i <= nx + r_count; i++) {
			nArray[ny][i] = ' ';
			function(r_hdc, nArray[ny][i], i, ny);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}

	//ny - 1���� �����ؼ� �������� ���ҽ�Ű�鼭 count!
	for (i = ny - 1; i >= 0; i--) {
		if (animal != nArray[i][nx])
			break;
		else
			u_count++;
	}
	//ny + 1���� �����ؼ� �Ʒ������� ������Ű�鼭 count!
	for (i = ny + 1; i <= 9; i++) {
		if (animal != nArray[i][nx])
			break;
		else
			d_count++;
	}
	if (u_count + d_count >= 2) {
		for (i = ny - u_count; i <= ny + d_count; i++) {
			nArray[i][nx] = ' ';
			function(r_hdc, nArray[i][nx], nx, i);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}
	ReleaseDC(r_hwnd, r_hdc);
	return TRUE;
}
BOOL RemoveWithDir(HWND hWnd) 
{
	int l_count = 0; //���ʿ� ������ �� �� �ִ��� ����
	int r_count = 0; //������
	int u_count = 0; //����
	int d_count = 0; //�Ʒ���

	int i;
	char d_animal;
	int r_x = nx, r_y = ny; //dir_num ����� ��ǥ

	HWND r_hwnd = hWnd;
	HDC r_hdc;

	switch (dir_num) {
	case -1:
		r_x = nx + 1;
		break;
	case -2:
		r_x = 0;
		break;
	case 1:
		r_x = nx - 1;
		break;
	case 2:
		r_x = 9;
		break;
	case -10:
		r_y = ny + 1;
		break;
	case -20:
		r_y = 0;
		break;
	case 10:
		r_y = ny - 1;
		break;
	case 20:
		r_y = 9;
		break;
	}

	d_animal = nArray[r_y][r_x]; //�ݺ��� �ȿ��� d_animal�� ���Ұ���

	if (d_animal == ' ')
		return FALSE;

	r_hdc = GetDC(r_hwnd);

	//nx - 1���� �����ؼ� �������� ���ҽ��Ѱ��鼭 count!
	for (i = r_x - 1; i >= 0; i--) {
		if (d_animal != nArray[r_y][i])
			break;
		else
			l_count++;
	}
	//nx + 1���� �����ؼ� ���������� �������Ѱ��鼭 count!
	for (i = r_x + 1; i <= 9; i++) {
		if (d_animal != nArray[r_y][i])
			break;
		else
			r_count++;
	}
	if (l_count + r_count >= 2) { //nx���� ���������� 3�� �̻� �����ƴٸ� �Ͷ߸�
		for (i = r_x - l_count; i <= r_x + r_count; i++) {
			nArray[r_y][i] = ' ';
			function(r_hdc, nArray[r_y][i], i, r_y);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}

	//ny - 1���� �����ؼ� �������� ���ҽ�Ű�鼭 count!
	for (i = r_y - 1; i >= 0; i--) {
		if (d_animal != nArray[i][r_x])
			break;
		else
			u_count++;
	}
	//ny + 1���� �����ؼ� �Ʒ������� ������Ű�鼭 count!
	for (i = r_y + 1; i <= 9; i++) {
		if (d_animal != nArray[i][r_x])
			break;
		else
			d_count++;
	}
	if (u_count + d_count >= 2) {
		for (i = r_y - u_count; i <= r_y + d_count; i++) {
			nArray[i][r_x] = ' ';
			function(r_hdc, nArray[i][r_x], r_x, i);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}
	ReleaseDC(r_hwnd, r_hdc);
	return TRUE;
}