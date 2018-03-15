#include "WM_Command.h"
#include "resource.h"
#include "Information.h"
#include "ball_move.h"

// 다른값들도 래퍼런스로 값을 가져와야하는지?
void WM_Command(HWND hwnd, WPARAM wParam, BALL &ball, OBSTACLE &obstacle, POINT start_point, HINSTANCE hInstance, int &stage, int &spendTime, bool &obstacleReset)
{
	switch (LOWORD(wParam))
	{
	case ID_NEWGAME:	// 공의 정보 초기화.
		init_ball(ball, start_point);
		obstacle.point[0] = { 0, 0 };
		obstacle.point[1] = { 0, 0 };
		obstacleReset = true;
		spendTime = 0;
		stage = 1;
		ball.collisionCount = 0;
		KillTimer(hwnd, 1);	// 공(투사체)를 멈춤.
		InvalidateRgn(hwnd, NULL, false);
		break;

	case ID_SCORE:	// 조작법을 출력하기위한 함수.
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Information);
		break;

	case ID_END:	// 끝내기
		if (MessageBox(hwnd, "\t정말로 끝낼거야?", "끝낼라고?", MB_OKCANCEL) == IDOK)
			if (MessageBox(hwnd, "\t레알? really?", "설마 진짜야?", MB_OKCANCEL) == IDOK)
				PostQuitMessage(0);
		break;

	case ID_INFORMATION:	// 프로그램 정보 출력.
		MessageBox(hwnd, "작성자 : 박예훈\n학번 : 2011270340\n버전 : 1. 0. 3", "윈도우즈프로그래밍 프로젝트", MB_ICONINFORMATION | MB_OK);
		break;
	}
}