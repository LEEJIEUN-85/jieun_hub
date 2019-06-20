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

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, BOOL isBlink); //비트맵 그리는 함수
BOOL RemoveIcon(HWND hWnd); //바뀐 좌표 기준으로 3개 이상 나열시 삭제해주는 함수
BOOL RemoveWithDir(HWND hWnd); //바뀌기 전 좌표 기준으로 3개 이상 나열시 삭제해주는 함수
void function(HDC hdc, char a, int x, int y);
void InitStage(); //스테이지 첫 화면 설정하는 함수
void DrawAllScreen(HDC hdc); //첫 화면 그리는 함수
BOOL TestEnd(); //게임이 끝났는지 확인하는 함수
void Move(HDC hdc, int dir); //움직이는 함수

int nStage; //스테이지를 확인하는 변수
char nArray[10][11];
static int nx, ny;
static int dir_num;
static BOOL blink; //비트맵이 반전될지 여부 저장
static BOOL nowChange; //단순이동인지, 아이콘교환인지(=스페이스바 눌렸는지 안눌렸는지)
static int score; //점수
HBITMAP hBit[6]; //비트맵을 불러와서 저장

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
	hWndMain = hWnd; // hWnd 정보도 전역변수에 저장!

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
	TCHAR Message[256]; //화면에 문자열 출력할 때 사용

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 800, BITMAP_H * 10 + 30);
		AdjustWindowRect(&crt, WS_CAPTION | WS_MINIMIZEBOX, FALSE);
		//crt 크기와 해당 윈도우 스타일에 맞는 윈도우 크기를 계산하여 crt에 다시 대입
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, //윈도우 크기 조절
			SWP_NOMOVE | SWP_NOZORDER);
		for (i = 0; i < 6; i++)
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BEAR + i)); //비트맵을 불러옴
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
		DrawAllScreen(hdc); //화면을 그린다
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
		case VK_DOWN: //방향키들이 눌리면
			hdc = GetDC(hWnd);
			Move(hdc, wParam); //Move함수를 호출해서 아이콘을 이동시킴
			if (nowChange)
				SetTimer(hWnd, 2, 500, NULL);
			if (TestEnd()) { //TestEnd함수로 게임이 끝났는지 확인하고, 끝났다면
				if (nStage != MAXSTAGE) { //마지막 스테이지가 아니라면 다음 스테이지로 이동
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

		case 'Q': //Q가 입력되면(Quit)
			wsprintf(Message, _T("Do you want to quit?\n") _T("Your score is %d !\n")_T("Go to the next step?"), nStage + 1, score);
			if (MessageBox(hWnd, _T("Do you want to quit?\n"), _T("Quit"), MB_OKCANCEL) == IDOK) {
				wsprintf(Message, _T("Your score is %d !\n")_T("Bye Bye :-)"), score);
				KillTimer(hWnd, 1);
				MessageBox(hWnd, Message, _T("Quit"), MB_OK);
				DestroyWindow(hWnd);
			}
			break;
		case 'R': //R이 입력되면(Reset)
			if (MessageBox(hWnd, _T("Do you want to reset?"), _T("Reset?"), MB_OKCANCEL) == IDOK) {
				KillTimer(hWnd, 1);
				InitStage();
				SetTimer(hWnd, 1, 90000, NULL);
			}
		
			break;
		case 'D': //D가 입력되면
			if (nStage < MAXSTAGE - 1) {
				KillTimer(hWnd, 1);
				nStage++; //스테이지 번호를 1 증가시키고
				InitStage(); //올라간 스테이지의 첫 화면을 그린다.
				SetTimer(hWnd, 1, 90000, NULL);
			}
			break;
		case 'A': //A가 입력되면
			if (nStage > 0) {
				KillTimer(hWnd, 1);
				nStage--; //스테이지 번호를 1 감소시키고
				InitStage(); //내려간 스테이지의 첫 화면을 그린다.
				SetTimer(hWnd, 1, 90000, NULL);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		for (i = 0; i < 6; i++)
			DeleteObject(hBit[i]); //비트맵의 메모리를 하나씩 해제시킨다.
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

	SetTextAlign(hdc, TA_CENTER); //문자열을 중앙정렬시킴
	SetBkMode(hdc, TRANSPARENT); //문자열 배경색을 없앰

	wsprintf(Message, _T("<<Animal Game>>"));
	TextOut(hdc, 600, 90, Message, lstrlen(Message));
	wsprintf(Message, _T("제한시간 : 90초"));
	TextOut(hdc, 600, 110, Message, lstrlen(Message));
	wsprintf(Message, _T("Q : 종료, R : 다시 시작"));
	TextOut(hdc, 600, 130, Message, lstrlen(Message));
	wsprintf(Message, _T("A : 이전 스테이지, D : 다음 스테이지"));
	TextOut(hdc, 600, 150, Message, lstrlen(Message));
	wsprintf(Message, _T("스테이지 : %d"), nStage + 1);
	TextOut(hdc, 600, 190, Message, lstrlen(Message));
	wsprintf(Message, _T("점수 : %d"), score);
	TextOut(hdc, 600, 210, Message, lstrlen(Message));
	wsprintf(Message, _T("아이콘 이동 방법 : 스페이스바 → 원하는 방향키"));
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
		BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, NOTSRCCOPY); //현재 위치는 이미지를 반전시켜서 표현함.
	else
		BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
BOOL TestEnd() //게임이 끝났는지 확인하는 함수
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

	if (nowChange) { //스페이스바가 눌리고 방향키가 눌렸다면(=아이콘 교환)
		switch (dir) {
		case VK_LEFT:
			if (nx != 0) {
				tmp = nArray[ny][nx];
				nx -= 1;
				nArray[ny][nx + 1] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				function(hdc, nArray[ny][nx + 1], nx + 1, ny); //원래자리
				dir_num = -1;
			}
			else {
				tmp = nArray[ny][nx];
				nx = 9;
				nArray[ny][0] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][0], 0, ny); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = -2;
			}
			break;
		case VK_RIGHT:
			if (nx != 9) {
				tmp = nArray[ny][nx];
				nx += 1;
				nArray[ny][nx - 1] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][nx - 1], nx - 1, ny); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = 1;
			}
			else {
				tmp = nArray[ny][nx];
				nx = 0;
				nArray[ny][9] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny][9], 9, ny); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = 2;
			}
			break;
		case VK_UP:
			if (ny != 0) {
				tmp = nArray[ny][nx];
				ny -= 1;
				nArray[ny + 1][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny + 1][nx], nx, ny + 1); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = -10;
			}
			else {
				tmp = nArray[ny][nx];
				ny = 9;
				nArray[0][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[0][nx], nx, 0); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = -20;
			}
			break;
		case VK_DOWN:
			if (ny != 9) {
				tmp = nArray[ny][nx];
				ny += 1;
				nArray[ny - 1][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[ny - 1][nx], nx, ny - 1); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = 10;
			}
			else {
				tmp = nArray[ny][nx];
				ny = 0;
				nArray[9][nx] = nArray[ny][nx];
				nArray[ny][nx] = tmp;
				function(hdc, nArray[9][nx], nx, 9); //원래자리
				function(hdc, nArray[ny][nx], nx, ny); //이동해간 자리
				dir_num = 20;
			}
			break;
		}
	}
	else { //스페이스바 안눌리고 방향키 눌림(=현재좌표이동)
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
void InitStage() //각 스테이지의 첫 화면을 그리는 함수
{
	memcpy(nArray, ori_Array[nStage], sizeof(nArray));

	nx = rand() % 10;
	ny = rand() % 10; //랜덤으로 첫 좌표 설정
	score = 0; //점수를 0으로 초기화한다.
	InvalidateRect(hWndMain, NULL, TRUE); //화면을 새로 그린다.
}
void function(HDC hdc, char a, int x, int y) {
	int whatBit = 0;
	switch (a) {
	case '#': //곰
		whatBit = 0;
		break;
	case '$': //강아지
		whatBit = 1;
		break;
	case '%': //사자
		whatBit = 2;
		break;
	case '&': //생쥐
		whatBit = 3;
		break;
	case '@': //토끼
		whatBit = 4;
		break;
	case ' ': //없어져야 될 그림
		whatBit = 5;
		break;
	}
	if ((x == nx) && (y == ny))
		blink = TRUE; //현재 위치인 곳을 그릴거임
	else
		blink = FALSE;
	DrawBitmap(hdc, x * BITMAP_W + 3, y * BITMAP_H + 3, hBit[whatBit], blink); //iBit에 해당하는 비트맵을 그린다.
}
BOOL RemoveIcon(HWND hWnd)
{
	int l_count = 0; //왼쪽에 같은게 몇 개 있는지 저장
	int r_count = 0; //오른쪽
	int u_count = 0; //위쪽
	int d_count = 0; //아래쪽

	int i;
	char animal;

	HWND r_hwnd = hWnd;
	HDC r_hdc;

	animal = nArray[ny][nx]; //반복문 안에서 animal로 비교할거임

	if (animal == ' ')
		return FALSE;

	r_hdc = GetDC(r_hwnd);

	//nx - 1부터 시작해서 왼쪽으로 감소시켜가면서 count!
	for (i = nx - 1; i >= 0; i--) {
		if (animal != nArray[ny][i])
			break;
		else
			l_count++;
	}
	//nx + 1부터 시작해서 오른쪽으로 증가시켜가면서 count!
	for (i = nx + 1; i <= 9; i++) {
		if (animal != nArray[ny][i])
			break;
		else
			r_count++;
	}
	if (l_count + r_count >= 2) { //nx포함 오른쪽으로 3개 이상 나열됐다면 터뜨림
		for (i = nx - l_count; i <= nx + r_count; i++) {
			nArray[ny][i] = ' ';
			function(r_hdc, nArray[ny][i], i, ny);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}

	//ny - 1부터 시작해서 위쪽으로 감소시키면서 count!
	for (i = ny - 1; i >= 0; i--) {
		if (animal != nArray[i][nx])
			break;
		else
			u_count++;
	}
	//ny + 1부터 시작해서 아래쪽으로 증가시키면서 count!
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
	int l_count = 0; //왼쪽에 같은게 몇 개 있는지 저장
	int r_count = 0; //오른쪽
	int u_count = 0; //위쪽
	int d_count = 0; //아래쪽

	int i;
	char d_animal;
	int r_x = nx, r_y = ny; //dir_num 적용된 좌표

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

	d_animal = nArray[r_y][r_x]; //반복문 안에서 d_animal로 비교할거임

	if (d_animal == ' ')
		return FALSE;

	r_hdc = GetDC(r_hwnd);

	//nx - 1부터 시작해서 왼쪽으로 감소시켜가면서 count!
	for (i = r_x - 1; i >= 0; i--) {
		if (d_animal != nArray[r_y][i])
			break;
		else
			l_count++;
	}
	//nx + 1부터 시작해서 오른쪽으로 증가시켜가면서 count!
	for (i = r_x + 1; i <= 9; i++) {
		if (d_animal != nArray[r_y][i])
			break;
		else
			r_count++;
	}
	if (l_count + r_count >= 2) { //nx포함 오른쪽으로 3개 이상 나열됐다면 터뜨림
		for (i = r_x - l_count; i <= r_x + r_count; i++) {
			nArray[r_y][i] = ' ';
			function(r_hdc, nArray[r_y][i], i, r_y);
			score++;
			InvalidateRect(r_hwnd, NULL, TRUE);
		}
	}

	//ny - 1부터 시작해서 위쪽으로 감소시키면서 count!
	for (i = r_y - 1; i >= 0; i--) {
		if (d_animal != nArray[i][r_x])
			break;
		else
			u_count++;
	}
	//ny + 1부터 시작해서 아래쪽으로 증가시키면서 count!
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