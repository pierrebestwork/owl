#include <windows.h>
int STDCALL WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{
 MessageBox (NULL, "Test message", "Test", MB_OK);
 return 0;
}