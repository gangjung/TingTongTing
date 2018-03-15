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
	HDC memdc1, memdc2;	// 더블버퍼링.
	PAINTSTRUCT ps;
	static RECT rect;
	
	// 비트맵을 위한 변수들.
	HBITMAP backBitmap;	
	static HBITMAP hBitmap;
	static HBITMAP startBitmap1, startBitmap2, startBitmap3;
	static HBITMAP howtoBitmap1, howtoBitmap2, howtoBitmap3;
	static HBITMAP closeBitmap1, closeBitmap2, closeBitmap3;
	static HBITMAP gameBitmap;
	static HBITMAP resultBitmap;
	HBRUSH hBrush, oldBrush;
	HFONT hFont, oldFont;

	// 시간관련 변수. 소요시간 측정.
	SYSTEMTIME sTime;	
	static int saveTime;
	static int nowTime;
	static int spendTime;

	// 게임 중 사용되는 오브젝트들.
	static BALL ball;	
	static OBSTACLE obstacle;

	// 글자 출력을 위한 변수들
	static char score[50];	
	static char stageInfo[20];
	static SIZE size;
	static int stage;	// 현재 스테이지.

	// 여러가지 상태를 나타내는 변수들.
	static bool StartScreen;
	static bool defeat;
	static bool click;	// 처음화면의 버튼을 눌렀는지 판별
	static bool clear;	// 전체 스테이지를 클리어했는지 확인
	static bool obstacleReset;
	static bool gameStart;	// 공의 움직임과 관련.
	static bool	stageStart;	// 게임이 진행중인지 확인.

	// 각 맵에 위치해있는 벽들의 기울기.
	static double gradients_stage1[10];	
	static double gradients_stage2[17];
	static double gradients_stage3[26];

	// 게임중 사용되는 각종 위치변수들.
	static POINT start_point[3];	
	static POINT end_point[3];
	static POINT mouse_point;
	static RECT button_start_rect;	// 시작화면에 있는 버튼의 크기정보.
	static RECT button_howto_rect;
	static RECT button_close_rect;

	// 시작화면에 있는 버튼의 상태정보.
	static bool onStartButton = false;	
	static bool onHowtoButton = false;
	static bool onCloseButton = false;

	// 벽을 나타내는 포인터들.
	static POINT point_stage1[10] = { { 30, 250 },{ 330, 30 },{ 630, 30 },{ 930, 250 },{ 930, 500 },{ 630, 500 },{ 630, 250 },{ 330,250 },{ 330,500 },{ 30,500 } };
	static POINT point_stage2[17] = { { 30, 300 },{ 350, 300 },{ 400, 200 },{ 330, 50 },{ 630, 50 },{ 550, 200 },{ 610,300 },{ 700, 300 },{ 610, 200 },{ 710, 100 },
	{ 850, 100 },{ 800, 300 },{ 930, 300 },{ 730, 500 },{ 580, 400 },{ 380, 400 },{ 230, 500 }, };
	static POINT point_stage3[26] = { { 30, 400 },{ 200, 400 },{ 200, 300 },{ 50, 280 },{ 30, 200 },{ 330, 50 },{ 750, 50 },{ 850, 150 },{ 930, 50 },{ 930, 350 },
	{ 700, 500 },{ 700, 400 },{ 630, 400 },{ 630, 500 },{ 430, 500 },{ 430, 300 },{ 800, 300 },{ 800, 250 },{ 630, 250 },{ 630, 150 },
	{ 430, 150 },{ 430, 200 },{ 200, 200 },{ 330, 250 },{ 330, 500 },{ 30, 500 } };

	switch (msg)
	{
	case WM_CREATE:
		Sound_->generate_system("sound\\Background.mp3", SOUND_LOOP_PLAY);	// 배경음악 재생
		GetClientRect(hwnd, &rect);

		button_start_rect = { rect.right - 150, rect.bottom - 200, rect.right - 50, rect.bottom - 150 };
		button_howto_rect = { rect.right - 150, rect.bottom - 140, rect.right - 50, rect.bottom - 90 };
		button_close_rect = { rect.right - 150, rect.bottom - 80, rect.right - 50, rect.bottom - 30 };

		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

		// 사용할 비트맵들 미리 LOAD.
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

		// 시작위치 및 도착위치 초기화.
		start_point[0] = { 180, 485 };		
		start_point[1] = { 230, 450 };
		start_point[2] = { 180,485 };
		end_point[0] = { 780, 485 };
		end_point[1] = { 800, 150 };
		end_point[2] = { 530, 485 };

		// 공 위치 초기화.
		ball.x = start_point[0].x;
		ball.y = start_point[0].y;

		// 각종 상태를 나타내는 변수들 초기화.
		stage = 1;
		spendTime = 0;
		obstacleReset = true;
		StartScreen = true;
		gameStart = false;
		stageStart = false;
		defeat = false;
		clear = false;
		click = false;

		get_gradients(point_stage1, gradients_stage1, 10);	// 각 스태이지에 위치해있는 벽들의 기울기를 구해주는 함수.
		get_gradients(point_stage2, gradients_stage2, 17);
		get_gradients(point_stage3, gradients_stage3, 26);
		break;

	case WM_SIZE:	// 윈도우 사이즈가 조정될 때 사용.
		GetClientRect(hwnd, &rect);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);	// 더블버퍼링 사용.
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(memdc1, backBitmap);
		memdc2 = CreateCompatibleDC(hdc);

		if (StartScreen) // 시작화면 출력.
		{
			SelectObject(memdc2, hBitmap);

			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 663, 500, SRCCOPY);

			hFont = CreateFont(40, 0, 0, 0, 100, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "돋움");
			oldFont = (HFONT)SelectObject(memdc1, hFont);
			SetBkMode(memdc1, TRANSPARENT);

			TextOut(memdc1, rect.left + 70, rect.bottom - 200, "Win API Programming", strlen("Win API Programming"));
			TextOut(memdc1, rect.left + 70, rect.bottom - 160, "2011270340", strlen("2011270340"));
			TextOut(memdc1, rect.left + 70, rect.bottom - 120, "박 예 훈", strlen("박 예 훈"));

			if (onStartButton)	// 시작버튼을 눌렀다면.
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

			if (onHowtoButton)	// 스코어버튼을 눌렀다면.
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

			if (onCloseButton)	// 끝내기버튼을 눌렀다면.
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
		else if (defeat)	// 결과창 화면
		{
			SelectObject(memdc2, resultBitmap);
			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 666, 500, SRCCOPY);

			hFont = CreateFont(80, 0, 0, 0, 50, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "돋움");

			if (stage != 4)	// 모든 스태이지를 클리어하지 않았을 때.
			{
				oldFont = (HFONT)SelectObject(memdc1, hFont);
				SetBkMode(memdc1, TRANSPARENT);
				TextOut(memdc1, 400, 250, "패배", strlen("패배"));
			}
			else	// 모든 스태이지를 클리어했을 때.
			{
				oldFont = (HFONT)SelectObject(memdc1, hFont);
				SetBkMode(memdc1, TRANSPARENT);
				TextOut(memdc1, 380, 230, "클리어", strlen("클리어"));
			}

			hFont = CreateFont(40, 0, 0, 0, 50, 0, 0, 0, HANGEUL_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "돋움");

			oldFont = (HFONT)SelectObject(memdc1, hFont);
			wsprintf(score, "충돌 횟수 : %d번, 소요시간 %d초", ball.collisionCount, spendTime);	// 결과값들을 나타낸다.
			TextOut(memdc1, 210, 350, score, strlen(score));

			SelectObject(memdc1, oldFont);
			DeleteDC(memdc2);

			Sound_->generate_system("", SOUND_LOOP_PLAY);
			Sound_->generate_system("sound\\close.mp3", SOUND_LOOP_PLAY);
		}

		if (gameStart)	// 시작화면 다음의 게임화면 출력.
		{
			SelectObject(memdc2, gameBitmap);
			StretchBlt(memdc1, 0, 0, rect.right, rect.bottom, memdc2, 0, 0, 666, 500, SRCCOPY);
			DeleteDC(memdc2);

			hBrush = CreateSolidBrush(RGB(255, 255, 230));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			switch (stage)	// 스태이지에 따른 맵 변환.
			{
			case 1:
				Polygon(memdc1, point_stage1, 10);	// 맵.
				break;
			case 2:
				Polygon(memdc1, point_stage2, 17);	// 맵.
				break;
			case 3:
				Polygon(memdc1, point_stage3, 26);	// 맵.
				break;
			}
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			Ellipse(memdc1, start_point[stage - 1].x - 15, start_point[stage - 1].y - 15, start_point[stage - 1].x + 15, start_point[stage - 1].y + 15);	// 시작점.
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
			Ellipse(memdc1, end_point[stage - 1].x - 15, end_point[stage - 1].y - 15, end_point[stage - 1].x + 15, end_point[stage - 1].y + 15);	// 도착점.
			SelectObject(memdc1, oldBrush);
			DeleteObject(hBrush);

			if (obstacle.point[0].x && obstacle.point[1].x)	// 방해물의 위치변수가 둘다 0에 위치하면 출력하지 않는다.
			{
				MoveToEx(memdc1, obstacle.point[0].x, obstacle.point[0].y, NULL);
				LineTo(memdc1, obstacle.point[1].x, obstacle.point[1].y);
			}

			if (ball.draw)	// 작은 공을 출력해준다.
			{
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(memdc1, hBrush);
				Ellipse(memdc1, ball.x - ball.size, ball.y - ball.size, ball.x + ball.size, ball.y + ball.size);	// 공(투사체)보여주기.
				SelectObject(memdc1, oldBrush);
				DeleteObject(hBrush);
			}

			SetBkMode(memdc1, TRANSPARENT);
			wsprintf(stageInfo, "STAGE : %d", stage);	// 스태이지 출력.
			TextOut(memdc1, 50, 20, stageInfo, strlen(stageInfo));

			wsprintf(score, "충돌 횟수 : %d번, 제한시간 %d초", ball.collisionCount, 60 - spendTime);	// 현재 게임정보들을 출력
			TextOut(memdc1, 50, rect.bottom - 30, score, strlen(score));
		}
		DeleteDC(memdc2);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, memdc1, 0, 0, rect.right, rect.bottom, SRCCOPY);
		DeleteObject(backBitmap);
		DeleteDC(memdc1);
		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		if (StartScreen)	// 시작화면일 때 버튼의 위치 확인.
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
		if (gameStart)	// 게임시작일 경우, 방해물 생성에 영향을 미친다.
		{
			if (!obstacle.draw)
			{
				obstacle.point[1].x = LOWORD(lParam);
				obstacle.point[1].y = HIWORD(lParam);
			}
		}
		break;

	case WM_LBUTTONDOWN:
		if (!StartScreen)	// 게임화면일 경우 버튼 클릭
		{
			if (!ball.draw)		// 공(투사체)이 움직이고있을 때는 반응이 없도록.
			{
				mouse_point.x = LOWORD(lParam);
				mouse_point.y = HIWORD(lParam);

				shoot_ball(ball, start_point[stage - 1], mouse_point, ball.speed);	// 공(투사체)에대한 정보를 입력.

				ball.draw = true;
				stageStart = true;	// 게임시작을 나타낸다.

				GetLocalTime(&sTime);
				saveTime = sTime.wSecond;	// 걸린시간을 측정하기 위한 시간저장.

				SetTimer(hwnd, 1, 10, NULL);	// 움직이게 해줌.

			}
		}
		else
		{
			if (Check_Location(lParam, button_start_rect))	// 시작버튼을 눌렀는지 확인.
				click = true;

			if (Check_Location(lParam, button_howto_rect)) // 스코어버튼을 눌렀는지 확인.
				click = true;

			if (Check_Location(lParam, button_close_rect))	// 끝내가 버튼을 눌렀는지 확인
				click = true;
		}

		if (defeat)	// 결과창일 경우 처음으로 돌아갈지 결정.
		{
			if (MessageBox(hwnd, "처음화면으로 돌아가시겠습니까?", "돌아갈낑가", MB_OKCANCEL) == IDOK)
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
			if (Check_Location(lParam, button_start_rect))	// 시작버튼을 눌렀는지 확인.
			{
				StartScreen = false;
				gameStart = true;
			}

			if (Check_Location(lParam, button_howto_rect)) // 스코어버튼을 눌렀는지 확인.
			{
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Information);
			}

			if (Check_Location(lParam, button_close_rect))	// 끝내가 버튼을 눌렀는지 확인
			{
				if (MessageBox(hwnd, "\t정말로 끝낼거야?", "끝낼라고?", MB_OKCANCEL) == IDOK)
					if (MessageBox(hwnd, "\t레알? really?", "설마 진짜야?", MB_OKCANCEL) == IDOK)
						PostQuitMessage(0);
			}
			click = false;
		}
		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_RBUTTONDOWN:
		if (gameStart)	// 게임이 시작하면 오른쪽 버튼으로 방해물 생성.
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
		if (gameStart)	// 오른쪽 버튼을 때면 방해물 생성.
		{
			get_gradient_obstacle(obstacle);
			obstacle.draw = true;
		}
		break;

	case WM_TIMER:
		switch (LOWORD(wParam))
		{
		case 1:	// 공(투사체)의 운동을 조절.
			if (stage == 4)	// 모든 스태이지를 클리어.
			{
				KillTimer(hwnd, 1);
				clear = true;
				defeat = true;
				gameStart = false;
				InvalidateRgn(hwnd, NULL, false);
			}
			if (stageStart)	// 게임이 시작되면 공이 움직인다.
			{
				ball.x += ball.speed * cos(ball.radian);
				ball.y += ball.speed * sin(ball.radian);
			}

			if (checkClear(end_point[stage - 1], ball, 15))	// 목적지에 도달했는지 확인.
			{
				stage++;
				init_ball(ball, start_point[stage - 1]);
				obstacle.point[0] = { 0, 0 };
				obstacle.point[1] = { 0, 0 };
				stageStart = false;
				obstacleReset = true;
			}
			switch (stage)	// 스태이지에 따른 충돌 확인.
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

			if (obstacle.draw)	// 방해물이 생성되면 충돌 확인.
				check_collision_obstacle(obstacle, ball);

			GetLocalTime(&sTime);	// 소요시간을 측정하기위해서 시간정보를 가져온다.
			nowTime = sTime.wSecond;

			if (nowTime == 0)	// now가 0이면 save는 59이기 때문에 60을 더해줘서 1차이가 있도록 해준다.
				nowTime += 60;

			if ((nowTime - saveTime) == 1)	// now가 1초 지나면 소요시간 1 증가!
			{
				spendTime++;
				saveTime = nowTime;
				if (saveTime == 60)		// save가 60이면 0으로 생각.
					saveTime = 0;
			}

			if (spendTime == 60)	// 제한시간 조절. 클리어 실패
			{
				InvalidateRgn(hwnd, NULL, false);

				KillTimer(hwnd, 1);
				Sound_->generate_system("", SOUND_LOOP_PLAY);

				if (MessageBox(hwnd, "          제한시간이 다되었습니다.\n\t다시하시겠습니까?", "져버렸당께", MB_OKCANCEL) == IDOK)
				{
					init_ball(ball, start_point[stage - 1]);//다시시작.
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
