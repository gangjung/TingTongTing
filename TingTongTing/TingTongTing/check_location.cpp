#include <Windows.h>

// 버튼이 눌러졌는지 확인하는 것.
bool Check_Location(LPARAM lParam, RECT ButtonRect)	
{
	if ((ButtonRect.left <= LOWORD(lParam)) && (LOWORD(lParam) <= ButtonRect.right))
	{
		if ((ButtonRect.top <= HIWORD(lParam)) && (HIWORD(lParam) <= ButtonRect.bottom))
			return true;
	}
	return false;
}