#include "Information.h"

// ���۹��� ����ϱ����� ��ȭ����.
BOOL CALLBACK Information(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}