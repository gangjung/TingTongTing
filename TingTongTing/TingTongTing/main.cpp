#pragma comment(lib, "winmm.lib")

#pragma warning(disable : 4996)

#include <windows.h> 
#include <mmsystem.h>
#include <stdio.h>
#include <math.h>
#include "ball.h"
#include "collision.h"
#include "ball_move.h"
#include "WM_Command.h"
#include "check_location.h"
#include "resource.h"
#include "Information.h"
#include "Sound.h"
#include "collision_obstacle.h"
#include "StageClear.h"

#define ID_STARTBUTTON 100

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClassName = TEXT("TingTangTing");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;
	WNDCLASS wc;
	MSG msg;

	hInst = hInstance;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = lpszClassName;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	hwnd = CreateWindow(lpszClassName, lpszClassName, WS_OVERLAPPEDWINDOW, 100, 50, 1000, 600, 0, 0, hInstance, 0);

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	
	HDC	hdc;
	HDC memdc1, memdc2;	// ������۸�.
	PAINTSTRUCT ps;
	static RECT rect;
	
	// ��Ʈ���� ���� ������.
	HBITMAP backBitmap;	
	static HBITMAP hBitmap;
	static HBITMAP startBitmap1, startBitmap2, startBitmap3;
	static HBITMAP howtoBitmap1, howtoBitmap2, howtoBitmap3;
	static HBITMAP closeBitmap1, closeBitmap2, closeBitmap3;
	static HBITMAP gameBitmap;
	static HBITMAP resultBitmap;
	HBRUSH hBrush, oldBrush;
	HFONT hFont, oldFont;

	// �ð����� ����. �ҿ�ð� ����.
	SYSTEMTIME sTime;	
	static int saveTime;
	static int nowTime;
	static int spendTime;

	// ���� �� ���Ǵ� ������Ʈ��.
	static BALL ball;	
	static OBSTACLE obstacle;

	// ���� ����� ���� ������
	static char score[50];	
	static char stageInfo[20];
	static SIZE size;
	static int stage;	// ���� ��������.

	// �������� ���¸� ��Ÿ���� ������.
	static bool StartScreen;
	static bool defeat;
	static bool click;	// ó��ȭ���� ��ư�� �������� �Ǻ�
	static bool clear;	// ��ü ���������� Ŭ�����ߴ��� Ȯ��
	static bool obstacleReset;
	static bool gameStart;	// ���� �����Ӱ� ����.
	static bool	stageStart;	// ������ ���������� Ȯ��.

	// �� �ʿ� ��ġ���ִ� ������ ����.
	static double gradients_stage1[10];	
	static double gradients_stage2[17];
	static double gradients_stage3[26];

	// ������ ���Ǵ� ���� ��ġ������.
	static POINT start_point[3];	
	static POINT end_point[3];
	static POINT mouse_point;
	static RECT button_start_rect;	// ����ȭ�鿡 �ִ� ��ư�� ũ������.
	static RECT button_howto_rect;
	static RECT button_close_rect;

	// ����ȭ�鿡 �ִ� ��ư�� ��������.
	static bool onStartButton = false;	
	static bool onHowtoButton = false;
	static bool onCloseButton = false;

	// ���� ��Ÿ���� �����͵�.
	static POINT point_stage1[10] = { { 30, 250 },{ 330, 30 },{ 630, 30 },{ 930, 250 },{ 930, 500 },{ 630, 500 },{ 630, 250 },{ 330,250 },{ 330,500 },{ 30,500 } };
	static POINT point_stage2[17] = { { 30, 300 },{ 350, 300 },{ 400, 200 },{ 330, 50 },{ 630, 50 },{ 550, 200 },{ 610,300 },{ 700, 300 },{ 610, 200 },{ 710, 100 },
	{ 850, 100 },{ 800, 300 },{ 930, 300 },{ 730, 500 },{ 580, 400 },{ 380, 400 },{ 230, 500 }, };
	static POINT point_stage3[26] = { { 30, 400 },{ 200, 400 },{ 200, 300 },{ 50, 280 },{ 30, 200 },{ 330, 50 },{ 750, 50 },{ 850, 150 },{ 930, 50 },{ 930, 350 },
	{ 700, 500 },{ 700, 400 },{ 630, 400 },{ 630, 500 },{ 430, 500 },{ 430, 300 },{ 800, 300 },{ 800, 250 },{ 630, 250 },{ 630, 150 },
	{ 430, 150 },{ 430, 200 },{ 200, 200 },{ 330, 250 },{ 330, 500 },{ 30, 500 } };

	switch (msg)
	{
	case WM_CREATE:
		Sound_->generate_system("sound\\Background.mp3", SOUND_LOOP_PLAY);	// ������� ���
		GetClientRect(hwnd, &rect);

		button_start_rect = { rect.right - 150, rect.bottom - 200, rect.right - 50, rect.bottom - 150 };
		button_howto_rect = { rect.right - 150, rect.bottom - 140, rect.right - 50, rect.bottom - 90 };
		button_close_rect = { rect.right - 150, rect.bottom - 80, rect.right - 50, rect.bottom - 30 };

		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

		// ����� ��Ʈ�ʵ� �̸� LOAD.
		hBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		startBitmap1 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_NOSELECT));
		startBitmap2 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_SELECT_BEFORE));
		startBitmap3 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_SELECT_AFTER));

		howtoBitmap1 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_NOHOWTO));
		howtoBitmap2 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_HOWTO_BEFORE));
		howtoBitmap3 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_HOWTO_AFTER));

		closeBitmap1 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_NOCLOSE));
		closeBitmap2 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_CLOSE_BEFORE));
		closeBitmap3 = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_CLOSE_AFTER));

		gameBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE1));

		resultBitmap = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_RESULT));

		// ������ġ �� ������ġ �ʱ�ȭ.
		start_point[0] = { 180, 485 };		
		start_point[1] = { 230, 450 };
		start_point[2] = { 180,485 };
		end_point[0] = { 780, 485 };
		end_point[1] = { 800, 150 };
		end_point[2] = { 530, 485 };

		// �� ��ġ �ʱ�ȭ.
		ball.x = start_point[0].x;
		ball.y = start_point[0].y;

		// ���� ���¸� ��Ÿ���� ������ �ʱ�ȭ.
		stage = 1;
		spendTime = 0;
		obstacleReset = true;
		StartScreen = true;
		gameStart = false;
		stageStart = false;
		defeat = false;
		clear = false;
		click = false;

		get_gradients(point_stage1, gradients_stage1, 10);	// �� ���������� ��ġ���ִ� ������ ���⸦ �����ִ� �Լ�.
		get_gradients(point_stage2, gradients_stage2, 17);
		get_gradients(point_stage3, gradients_stage3, 26);
		break;

	case WM_SIZE:	// ������ ����� ������ �� ���.
		GetClientRect(hwnd, &rect);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);	// ������۸� ���.
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(memdc1, backBitmap);
		memdc2 = CreateCompatibleDC(hdc);

		if (StartScreen) // ����ȭ�� ���.
		{
			SelectObject(memdc2, hBitmap);

			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 663, 500, SRCCOPY);

			hFont = CreateFont(40, 0, 0, 0, 100, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "����");
			oldFont = (HFONT)SelectObject(memdc1, hFont);
			SetBkMode(memdc1, TRANSPARENT);

			TextOut(memdc1, rect.left + 70, rect.bottom - 200, "Win API Programming", strlen("Win API Programming"));
			TextOut(memdc1, rect.left + 70, rect.bottom - 160, "2011270340", strlen("2011270340"));
			TextOut(memdc1, rect.left + 70, rect.bottom - 120, "�� �� ��", strlen("�� �� ��"));

			if (onStartButton)	// ���۹�ư�� �����ٸ�.
			{
				if (!click)
				{
					SelectObject(memdc2, startBitmap2);
					StretchBlt(memdc1, button_start_rect.left, button_start_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
				else
				{
					SelectObject(memdc2, startBitmap3);
					StretchBlt(memdc1, button_start_rect.left, button_start_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
			}
			else
			{
				SelectObject(memdc2, startBitmap1);
				StretchBlt(memdc1, button_start_rect.left, button_start_rect.top, 100, 50,
					memdc2, 0, 0, 123, 65, SRCCOPY);
			}

			if (onHowtoButton)	// ���ھ��ư�� �����ٸ�.
			{
				if (!click)
				{
					SelectObject(memdc2, howtoBitmap2);
					StretchBlt(memdc1, button_howto_rect.left, button_howto_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
				else
				{
					SelectObject(memdc2, howtoBitmap3);
					StretchBlt(memdc1, button_howto_rect.left, button_howto_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
			}
			else
			{
				SelectObject(memdc2, howtoBitmap1);
				StretchBlt(memdc1, button_howto_rect.left, button_howto_rect.top, 100, 50,
					memdc2, 0, 0, 123, 65, SRCCOPY);
			}

			if (onCloseButton)	// �������ư�� �����ٸ�.
			{
				if (!click)
				{
					SelectObject(memdc2, closeBitmap2);
					StretchBlt(memdc1, button_close_rect.left, button_close_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
				else
				{
					SelectObject(memdc2, closeBitmap3);
					StretchBlt(memdc1, button_close_rect.left, button_close_rect.top, 100, 50,
						memdc2, 0, 0, 123, 65, SRCCOPY);
				}
			}
			else
			{
				SelectObject(memdc2, closeBitmap1);
				StretchBlt(memdc1, button_close_rect.left, button_close_rect.top, 100, 50,
					memdc2, 0, 0, 123, 65, SRCCOPY);
			}
			DeleteDC(memdc2);
		}
		else if (defeat)	// ���â ȭ��
		{
			SelectObject(memdc2, resultBitmap);
			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 666, 500, SRCCOPY);

			hFont = CreateFont(80, 0, 0, 0, 50, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "����");

			if (stage != 4)	// ��� ���������� Ŭ�������� �ʾ��� ��.
			{
				oldFont = (HFONT)SelectObject(memdc1, hFont);
				SetBkMode(memdc1, TRANSPARENT);
				TextOut(memdc1, 400, 250, "�й�", strlen("�й�"));
			}
			else	// ��� ���������� Ŭ�������� ��.
			{
				oldFont = (HFONT)SelectObject(memdc1, hFont);
				SetBkMode(memdc1, TRANSPARENT);
				TextOut(memdc1, 380, 230, "Ŭ����", strlen("Ŭ����"));
			}

			hFont = CreateFont(40, 0, 0, 0, 50, 0, 0, 0, HANGEUL_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "����");

			oldFont = (HFONT)SelectObject(memdc1, hFont);
			wsprintf(score, "�浹 Ƚ�� : %d��, �ҿ�ð� %d��", ball.collisionCount, spendTime);	// ��������� ��Ÿ����.
			TextOut(memdc1, 210, 350, score, strlen(score));

			SelectObject(memdc1, oldFont);
			DeleteDC(memdc2);

			Sound_->generate_system("", SOUND_LOOP_PLAY);
			Sound_->generate_system("sound\\close.mp3", SOUND_LOOP_PLAY);
		}

		if (gameStart)	// ����ȭ�� ������ ����ȭ�� ���.
		{
			SelectObject(memdc2, gameBitmap);
			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 666, 500, SRCCOPY);
			DeleteDC(memdc2);

			hBrush = CreateSolidBrush(RGB(255, 255, 230));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			switch (stage)	// ���������� ���� �� ��ȯ.
			{
			case 1:
				Polygon(memdc1, point_stage1, 10);	// ��.
				break;
			case 2:
				Polygon(memdc1, point_stage2, 17);	// ��.
				break;
			case 3:
				Polygon(memdc1, point_stage3, 26);	// ��.
				break;
			}
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			Ellipse(memdc1, start_point[stage - 1].x - 15, start_point[stage - 1].y - 15, start_point[stage - 1].x + 15, start_point[stage - 1].y + 15);	// ������.
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			Ellipse(memdc1, end_point[stage - 1].x - 15, end_point[stage - 1].y - 15, end_point[stage - 1].x + 15, end_point[stage - 1].y + 15);	// ������.
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			if (obstacle.point[0].x && obstacle.point[1].x)	// ���ع��� ��ġ������ �Ѵ� 0�� ��ġ�ϸ� ������� �ʴ´�.
			{
				MoveToEx(memdc1, obstacle.point[0].x, obstacle.point[0].y, NULL);
				LineTo(memdc1, obstacle.point[1].x, obstacle.point[1].y);
			}

			if (ball.draw)	// ���� ���� ������ش�.
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
				Ellipse(memdc1, ball.x - ball.size, ball.y - ball.size, ball.x + ball.size, ball.y + ball.size);	// ��(����ü)�����ֱ�.
				SelectObject(memdc1, oldBrush);
				DeleteObject(hBrush);
			}

			SetBkMode(memdc1, TRANSPARENT);
			wsprintf(stageInfo, "STAGE : %d", stage);	// �������� ���.
			TextOut(memdc1, 50, 20, stageInfo, strlen(stageInfo));

			wsprintf(score, "�浹 Ƚ�� : %d��, ���ѽð� %d��", ball.collisionCount, 60 - spendTime);	// ���� ������������ ���
			TextOut(memdc1, 50, rect.bottom - 30, score, strlen(score));
		}
		DeleteDC(memdc2);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, memdc1, 0, 0, rect.right, rect.bottom, SRCCOPY);
		DeleteObject(backBitmap);
		DeleteDC(memdc1);
		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		if (StartScreen)	// ����ȭ���� �� ��ư�� ��ġ Ȯ��.
		{
			if (Check_Location(lParam, button_start_rect))
				onStartButton = true;
			else
				onStartButton = false;

			if (Check_Location(lParam, button_howto_rect))
				onHowtoButton = true;
			else
				onHowtoButton = false;

			if (Check_Location(lParam, button_close_rect))
				onCloseButton = true;
			else
				onCloseButton = false;
			InvalidateRgn(hwnd, NULL, false);
		}
		if (gameStart)	// ���ӽ����� ���, ���ع� ������ ������ ��ģ��.
		{
			if (!obstacle.draw)
			{
				obstacle.point[1].x = LOWORD(lParam);
				obstacle.point[1].y = HIWORD(lParam);
			}
		}
		break;

	case WM_LBUTTONDOWN:
		if (!StartScreen)	// ����ȭ���� ��� ��ư Ŭ��
		{
			if (!ball.draw)		// ��(����ü)�� �����̰����� ���� ������ ������.
			{
				mouse_point.x = LOWORD(lParam);
				mouse_point.y = HIWORD(lParam);

				shoot_ball(ball, start_point[stage - 1], mouse_point, ball.speed);	// ��(����ü)������ ������ �Է�.

				ball.draw = true;
				stageStart = true;	// ���ӽ����� ��Ÿ����.

				GetLocalTime(&sTime);
				saveTime = sTime.wSecond;	// �ɸ��ð��� �����ϱ� ���� �ð�����.

				SetTimer(hwnd, 1, 10, NULL);	// �����̰� ����.

			}
		}
		else
		{
			if (Check_Location(lParam, button_start_rect))	// ���۹�ư�� �������� Ȯ��.
				click = true;

			if (Check_Location(lParam, button_howto_rect)) // ���ھ��ư�� �������� Ȯ��.
				click = true;

			if (Check_Location(lParam, button_close_rect))	// ������ ��ư�� �������� Ȯ��
				click = true;
		}

		if (defeat)	// ���â�� ��� ó������ ���ư��� ����.
		{
			if (MessageBox(hwnd, "ó��ȭ������ ���ư��ðڽ��ϱ�?", "���ư�����", MB_OKCANCEL) == IDOK)
			{
				init_ball(ball, start_point[0]);
				StartScreen = true;
				ball.draw = false;
				defeat = false;
				gameStart = false;
				spendTime = 0;
				stage = 1;
				obstacle.point[0] = { 0, 0 };
				obstacle.point[1] = { 0, 0 };
				obstacleReset = true;
				ball.collisionCount = 0;
				Sound_->generate_system("", SOUND_LOOP_PLAY);
				Sound_->generate_system("sound\\Background.mp3", SOUND_LOOP_PLAY);
			}
			else
				PostQuitMessage(0);
		}
		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_LBUTTONUP:
		if (StartScreen)
		{
			if (Check_Location(lParam, button_start_rect))	// ���۹�ư�� �������� Ȯ��.
			{
				StartScreen = false;
				gameStart = true;
			}

			if (Check_Location(lParam, button_howto_rect)) // ���ھ��ư�� �������� Ȯ��.
			{
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Information);
			}

			if (Check_Location(lParam, button_close_rect))	// ������ ��ư�� �������� Ȯ��
			{
				if (MessageBox(hwnd, "\t������ �����ž�?", "�������?", MB_OKCANCEL) == IDOK)
					if (MessageBox(hwnd, "\t����? really?", "���� ��¥��?", MB_OKCANCEL) == IDOK)
						PostQuitMessage(0);
			}
			click = false;
		}
		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_RBUTTONDOWN:
		if (gameStart)	// ������ �����ϸ� ������ ��ư���� ���ع� ����.
		{
			if (!obstacleReset)
			{
				obstacle.point[0] = { 0, 0 };
				obstacle.point[1] = { 0, 0 };
				obstacleReset = true;
				break;
			}

			obstacle.point[0].x = LOWORD(lParam);
			obstacle.point[0].y = HIWORD(lParam);

			obstacle.draw = false;
			obstacleReset = false;
		}
		break;

	case WM_RBUTTONUP:
		if (gameStart)	// ������ ��ư�� ���� ���ع� ����.
		{
			get_gradient_obstacle(obstacle);
			obstacle.draw = true;
		}
		break;

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 1:	// ��(����ü)�� ��� ����.
			if (stage == 4)	// ��� ���������� Ŭ����.
			{
				KillTimer(hwnd, 1);
				clear = true;
				defeat = true;
				gameStart = false;
				InvalidateRgn(hwnd, NULL, false);
			}
			if (stageStart)	// ������ ���۵Ǹ� ���� �����δ�.
			{
				ball.x += ball.speed * cos(ball.radian);
				ball.y += ball.speed * sin(ball.radian);
			}

			if (checkClear(end_point[stage - 1], ball, 15))	// �������� �����ߴ��� Ȯ��.
			{
				stage++;
				init_ball(ball, start_point[stage - 1]);
				obstacle.point[0] = { 0, 0 };
				obstacle.point[1] = { 0, 0 };
				stageStart = false;
				obstacleReset = true;
			}
			switch (stage)	// ���������� ���� �浹 Ȯ��.
			{
			case 1:
				check_collision(point_stage1, gradients_stage1, 10, ball);
				break;

			case 2:
				check_collision(point_stage2, gradients_stage2, 17, ball);
				break;

			case 3:
				check_collision(point_stage3, gradients_stage3, 26, ball);
				break;
			}

			if (obstacle.draw)	// ���ع��� �����Ǹ� �浹 Ȯ��.
				check_collision_obstacle(obstacle, ball);

			GetLocalTime(&sTime);	// �ҿ�ð��� �����ϱ����ؼ� �ð������� �����´�.
			nowTime = sTime.wSecond;

			if (nowTime == 0)	// now�� 0�̸� save�� 59�̱� ������ 60�� �����༭ 1���̰� �ֵ��� ���ش�.
				nowTime += 60;

			if ((nowTime - saveTime) == 1)	// now�� 1�� ������ �ҿ�ð� 1 ����!
			{
				spendTime++;
				saveTime = nowTime;
				if (saveTime == 60)		// save�� 60�̸� 0���� ����.
					saveTime = 0;
			}

			if (spendTime == 60)	// ���ѽð� ����. Ŭ���� ����
			{
				InvalidateRgn(hwnd, NULL, false);

				KillTimer(hwnd, 1);
				Sound_->generate_system("", SOUND_LOOP_PLAY);

				if (MessageBox(hwnd, "          ���ѽð��� �ٵǾ����ϴ�.\n\t�ٽ��Ͻðڽ��ϱ�?", "�����ȴ粲", MB_OKCANCEL) == IDOK)
				{
					init_ball(ball, start_point[stage - 1]);//�ٽý���.
					obstacle.point[0] = { 0, 0 };
					obstacle.point[1] = { 0, 0 };
					obstacleReset = true;
					spendTime = 0;
					stage = 1;
					ball.collisionCount = 0;
					Sound_->generate_system("sound\\Background.mp3", SOUND_LOOP_PLAY);
				}
				else
				{
					defeat = true;
					gameStart = false;
				}
			}
			InvalidateRgn(hwnd, NULL, false);
			break;
		}
		break;

	case WM_COMMAND:
		WM_Command(hwnd, wParam, ball, obstacle, start_point[stage - 1], hInstance, stage, spendTime, obstacleReset);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
