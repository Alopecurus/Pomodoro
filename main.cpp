/*
* @file    main.cpp
* @brief   Implement main
* @auther  Alopecurus
*/

#include "common.hpp"
#include "resource.hpp"
#include "pomodoro.hpp"

const char* NAME = "Pomodoro";

static HINSTANCE hInst;
static HWND hWnd, hButton, hDisp;

static HDC         hdc;
static PAINTSTRUCT ps;
static RECT        rect;

static HFONT hFont = CreateFont(
	CLIENT_SIZE.y / 2,
	0,
	0,
	0,
	FW_BOLD,
	FALSE,
	FALSE,
	FALSE,
	SHIFTJIS_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	PROOF_QUALITY,
	FIXED_PITCH | FF_MODERN,
	"メイリオ"
);

static char		text[64];
static char		time[64];
static char		caption[64];
static int		textHeight;

static vector2 windowSize;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RED[0]);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			LPCSTR("RegisterClassExの処理に失敗しました"),
			LPCSTR("Error"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	hWnd = CreateWindow(
		NAME,
		NAME,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CLIENT_SIZE.x, CLIENT_SIZE.y,
		NULL,
		NULL,
		hInst,
		NULL
	);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, (255 * 80) / 100, LWA_ALPHA);

	if (!hWnd)
	{
		MessageBox(NULL,
			LPCSTR("ウィンドウ生成に失敗しました!"),
			LPCSTR("Error"),
			NULL);
		return 1;
	}

	RECT rWnd, rClt, rDisp;

	hDisp = GetDesktopWindow();
	GetWindowRect(hDisp, &rDisp);
	GetWindowRect(hWnd, &rWnd);
	GetClientRect(hWnd, &rClt);

	windowSize.x  = (rWnd.right - rWnd.left) - (rClt.right - rClt.left) + CLIENT_SIZE.x;
	windowSize.y = (rWnd.bottom - rWnd.top) - (rClt.bottom - rClt.top) + CLIENT_SIZE.y;

	SetWindowPos(hWnd, NULL, (rDisp.right - windowSize.x) / 2, (rDisp.bottom - windowSize.y) / 2, windowSize.x, windowSize.y, SWP_NOZORDER);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

//  ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
	int msgResult;

	switch (uMsg)
	{
	case WM_CREATE:
		// 生成時
		hButton = CreateWindow("Button", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, 0, CLIENT_SIZE.x, CLIENT_SIZE.y, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
		pomodoro::init(hWnd, hInst);
		wsprintf(text, TEXT("START"));
		break;
	case WM_PAINT:
		// 描画
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SelectObject(hdc, hFont);
		SetTextColor(hdc, WHITE);
		SetBkColor(hdc, RED[0]);
		wsprintf(time, TEXT("%02d:%02d"), pomodoro::getRemainingTime() / 60, pomodoro::getRemainingTime() % 60);
		textHeight = DrawText(hdc, time, 5, &rect, DT_WORDBREAK | DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		// タイマー更新
		pomodoro::update();
		wsprintf(caption, TEXT("%s : %03d"), NAME, pomodoro::getCountPomodoro());
		SetWindowText(hWnd, caption);
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_COMMAND:
		// メニュー
		switch (wParam)
		{
		case ID_BUTTON:
		pomodoro::pauseSwitch(hWnd, hInst);
			DestroyWindow(hButton);
			
			if (!pomodoro::getEnablePause()) wsprintf(text, TEXT("PAUSE"));
			else wsprintf(text, TEXT("RESUME"));

			hButton = CreateWindow("Button", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
				0, textHeight, CLIENT_SIZE.x, CLIENT_SIZE.y - textHeight, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
			break;
		case ID_40001:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		pomodoro::fin(hWnd);
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM:
		if (wParam == ID_BUTTON)
		{
			if (lpDIS->itemState & ODS_SELECTED)
				SetDCBrushColor(lpDIS->hDC, GREEN[1]);
			else
				SetDCBrushColor(lpDIS->hDC, GREEN[0]);
			
			SelectObject(lpDIS->hDC, GetStockObject(DC_BRUSH));

			RoundRect(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
				lpDIS->rcItem.right, lpDIS->rcItem.bottom, 5, 5);

			SelectObject(lpDIS->hDC, hFont);
			SetTextColor(lpDIS->hDC, WHITE);
			SetBkMode(lpDIS->hDC, TRANSPARENT);
			DrawText(lpDIS->hDC, text, -1, &lpDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			return TRUE;
		}
		break;
	case WM_CLOSE:
		msgResult = MessageBox(
			hWnd, TEXT("ウィンドウを閉じます。\nよろしいですか？"), TEXT("確認"),
			MB_OKCANCEL | MB_ICONWARNING
		);

		if (msgResult != IDOK) return 0;
		else DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}