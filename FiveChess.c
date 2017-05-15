// FiveChess.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define WHITE 1
#define BLACK 2
#define ID_BUTTON 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
static int m_iStatus[15][15] = { 0 };
static RECT  m_rect[15][15];//∆Â◊”∑≈÷√Œª÷√
int WINS[15][15][572] = { 0 };
int COUNT = 0;
int M_WINS[572] = { 0 };
int C_WINS[572] = { 0 };
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void DrawWhite(HDC hdc, HBRUSH hBrush, RECT rect, POINT point);
void DrawBlack(HDC hdc, HBRUSH hBrush, RECT rect, POINT point);
void Restart(HWND hWnd);
//void Check(HWND hWnd);
//int WhoWin();
POINT GetPoint(POINT point);
POINT CpAI();
void Handle(HWND hWnd, HDC hDC, HBRUSH hBBrush, HBRUSH hWBrush, POINT pt);
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FIVECHESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FIVECHESS);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_FIVECHESS);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//(LPCSTR)IDC_FIVECHESS;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 11; j++) {
			for (int k = 0; k < 5; k++) {
				WINS[i][j + k][COUNT] = 1;
			}
			COUNT++;
		}
	}
	for (int i = 0; i < 15; i++) {  
		for (int j = 0; j < 11; j++) {
			for (int k = 0; k < 5; k++) {
				WINS[j + k][i][COUNT] = 1;
			}
			COUNT++;
		}
	}
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			for (int k = 0; k < 5; k++) {
				WINS[i + k][j + k][COUNT] = 1;
			}
			COUNT++;
		}
	}
	for (int i = 0; i < 11; i++) {
		for (int j = 14; j > 3; j--) {
			for (int k = 0; k < 5; k++) {
				WINS[i + k][j - k][COUNT] = 1;
			}
			COUNT++;
		}
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditRecv, hEditSend, hButton;
	static int cxClient, cyClient;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	static HBRUSH hWBrush, hBBrush;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	static HPEN hPen;
	int i, j;
	HDC hDC;
	POINT point;
	POINT pt;
	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_CREATE:
		MoveWindow(hWnd, 300, 100, 700, 700, TRUE);
		hPen = CreatePen(PS_SOLID, 1, 0);
		hWBrush = CreateSolidBrush(RGB(250, 250, 250));
		hBBrush = CreateSolidBrush(RGB(50, 50, 50));
		for (i = 0; i < 15; i++)
			for (j = 0; j < 15; j++)
			{
			m_rect[i][j].left = 30 + 40 * i;
			m_rect[i][j].top = 10 + 40 * j;
			m_rect[i][j].right = 70 + 40 * i;
			m_rect[i][j].bottom = 50 + 40 * j;
			m_rect[i][j].left = m_rect[i][j].left + 1;
			m_rect[i][j].top = m_rect[i][j].top + 1;
			m_rect[i][j].right = m_rect[i][j].right - 1;
			m_rect[i][j].bottom = m_rect[i][j].bottom - 1;
			}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hPen);
		for (i = 0; i < 15; i++)
		{
			MoveToEx(hdc, 50 + 40 * i, 30, NULL);
			LineTo(hdc, 50 + 40 * i, 30 + 15 * 40 - 40);
		}
		for (j = 0; j < 15; j++)
		{
			MoveToEx(hdc, 50, 30 + j * 40, NULL);
			LineTo(hdc, 50 + 40 * 15 - 40, 30 + j * 40);
		}
		for (i = 0; i < 15; i++)
			for (j = 0; j < 15; j++)
			{
			if (m_iStatus[i][j] == BLACK)
			{
				point.x = i; point.y = j;
				DrawBlack(hdc, hBBrush, m_rect[i][j], point);
			}
			if (m_iStatus[i][j] == WHITE)
			{
				point.x = i; point.y = j;
				DrawWhite(hdc, hWBrush, m_rect[i][j], point);
			}
			}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		hDC = GetDC(hWnd);
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		pt = GetPoint(point);
		Handle(hWnd, hDC, hBBrush, hWBrush, pt);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
POINT CpAI(){

	int m_score[15][15] = { 0 };
	int c_score[15][15] = { 0 };
	int max = 0;
	int u = 0, v = 0;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (m_iStatus[i][j] == 0) {
				for (int k = 0; k < COUNT; k++) {
					if (WINS[i][j][k]) {
						if (M_WINS[k] == 1) {
							m_score[i][j] += 200;
						}
						else if (M_WINS[k] == 2) {
							m_score[i][j] += 400;
						}
						else if (M_WINS[k] == 3) {
							m_score[i][j] += 2000;
						}
						else if (M_WINS[k] == 4) {
							m_score[i][j] += 10000;
						}

						if (C_WINS[k] == 1) {
							c_score[i][j] += 220;
						}
						else if (C_WINS[k] == 2) {
							c_score[i][j] += 420;
						}
						else if (C_WINS[k] == 3) {
							c_score[i][j] += 2100;
						}
						else if (C_WINS[k] == 4) {
							c_score[i][j] += 20000;
						}
					}
				}
				if (m_score[i][j] > max) {
					max = m_score[i][j];
					u = i;
					v = j;
				}
				else if (m_score[i][j] == max) {
					if (c_score[i][j] > c_score[u][v]) {
						u = i;
						v = j;
					}
				}
				if (c_score[i][j] > max) {
					max = c_score[i][j];
					u = i;
					v = j;
				}
				else if (c_score[i][j] == max) {
					if (m_score[i][j] > m_score[u][v]) {
						u = i;
						v = j;
					}
				}
			}
		}
	}
	POINT p;
	p.x = u;
	p.y = v;
	return p;
}
void Handle(HWND hWnd, HDC hDC, HBRUSH hBBrush, HBRUSH hWBrush, POINT pt){

	if (m_iStatus[pt.x][pt.y] != 0 || pt.x == -1 || pt.y == -1)
		return;
	DrawBlack(hDC, hBBrush, m_rect[pt.x][pt.y], pt);
	for (int k = 0; k < COUNT; k++){
		if (WINS[pt.x][pt.y][k]){
			M_WINS[k]++;
			C_WINS[k] = 6;
			if (M_WINS[k] == 5){
				if (MessageBox(hWnd, TEXT("ƒ„”Æ¡À£°"), TEXT(" ‰”Æ"), 1) == IDOK){
					Restart(hWnd);
					return;
				}
				else{
					DestroyWindow(hWnd);
				}
			}
		}
	}      
		POINT p = CpAI(); 
		for (int k = 0; k < COUNT; k++){
			if (WINS[p.x][p.y][k]){
				C_WINS[k]++;
				M_WINS[k] = 6;
				if (C_WINS[k] == 5){
					DrawWhite(hDC, hWBrush, m_rect[p.x][p.y], p);
					if (MessageBox(hWnd, TEXT("µÁƒ‘”Æ¡À£°"), TEXT(" ‰”Æ"), 1) == IDOK){
						Restart(hWnd);
						return;
					}
					else{
						DestroyWindow(hWnd);
					}
				}
			}
		}
		DrawWhite(hDC, hWBrush, m_rect[p.x][p.y], p);
	
}
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
void DrawWhite(HDC hdc, HBRUSH hBrush, RECT rect, POINT point)
{
	SelectObject(hdc, hBrush);
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	m_iStatus[point.x][point.y] = WHITE;
}
void DrawBlack(HDC hdc, HBRUSH hBrush, RECT rect, POINT point)
{
	SelectObject(hdc, hBrush);
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	m_iStatus[point.x][point.y] = BLACK;
}
POINT GetPoint(POINT point)
{
	int i, j;
	POINT Pt;
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
		{
		if (PtInRect(&m_rect[i][j], point))
		{
			Pt.x = i;
			Pt.y = j;
			return Pt;
		}
		}
	Pt.x = -1;
	Pt.y = -1;
	return Pt;
}
void Restart(HWND hWnd)
{
	int i, j;
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			m_iStatus[i][j] = 0;
	for (int k = 0; k < COUNT; k++){
		M_WINS[k] = 0;
		C_WINS[k] = 0;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}