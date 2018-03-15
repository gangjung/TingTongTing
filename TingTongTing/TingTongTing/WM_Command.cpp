#include "WM_Command.h"
#include "resource.h"
#include "Information.h"
#include "ball_move.h"

// �ٸ����鵵 ���۷����� ���� �����;��ϴ���?
void WM_Command(HWND hwnd, WPARAM wParam, BALL &ball, OBSTACLE &obstacle, POINT start_point, HINSTANCE hInstance, int &stage, int &spendTime, bool &obstacleReset)
{
	switch (LOWORD(wParam))
	{
	case ID_NEWGAME:	// ���� ���� �ʱ�ȭ.
		init_ball(ball, start_point);
		obstacle.point[0] = { 0, 0 };
		obstacle.point[1] = { 0, 0 };
		obstacleReset = true;
		spendTime = 0;
		stage = 1;
		ball.collisionCount = 0;
		KillTimer(hwnd, 1);	// ��(����ü)�� ����.
		InvalidateRgn(hwnd, NULL, false);
		break;

	case ID_SCORE:	// ���۹��� ����ϱ����� �Լ�.
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Information);
		break;

	case ID_END:	// ������
		if (MessageBox(hwnd, "\t������ �����ž�?", "�������?", MB_OKCANCEL) == IDOK)
			if (MessageBox(hwnd, "\t����? really?", "���� ��¥��?", MB_OKCANCEL) == IDOK)
				PostQuitMessage(0);
		break;

	case ID_INFORMATION:	// ���α׷� ���� ���.
		MessageBox(hwnd, "�ۼ��� : �ڿ���\n�й� : 2011270340\n���� : 1. 0. 3", "�����������α׷��� ������Ʈ", MB_ICONINFORMATION | MB_OK);
		break;
	}
}