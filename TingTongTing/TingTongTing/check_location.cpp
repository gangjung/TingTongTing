#include <Windows.h>

// ��ư�� ���������� Ȯ���ϴ� ��.
bool Check_Location(LPARAM lParam, RECT ButtonRect)	
{
	if ((ButtonRect.left <= LOWORD(lParam)) && (LOWORD(lParam) <= ButtonRect.right))
	{
		if ((ButtonRect.top <= HIWORD(lParam)) && (HIWORD(lParam) <= ButtonRect.bottom))
			return true;
	}
	return false;
}