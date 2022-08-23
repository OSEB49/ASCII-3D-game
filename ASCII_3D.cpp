#include <iostream>
using namespace std;
#include <Windows.h>

int ScreenWidth = 120;
int ScreenHeight = 40;

float FPX = 8.0f;
float FPY = 8.0f;
float FPA = 0.0f;
int MapWidth = 16;
int MapHeight = 16;

float ViewRange = 3.14159 / 4.0;
float MaxDepth = 16.0f;

int main() {
	
	wstring map;
	map += L"||||||||||||||||||||";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"|..................|";
	map += L"||||||||||||||||||||";

	wchar_t * screen = new wchar_t[ScreenWidth*ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	screen[ScreenWidth * ScreenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);

	while(1)

		for (int x = 0; x < ScreenWidth; x++)
		{
			float RayAngle = (FPA - ViewRange / 2.0f) + ((float)x / (float)ScreenWidth) * ViewRange;

			float WallDistance = 0;
			bool HitTheWall = false;

			float EyeX = sinf(RayAngle);
			float EyeY = cosf(RayAngle);

			while (!HitTheWall && WallDistance < MaxDepth) {
				WallDistance += 0.1f;

				int TestX = (int)(FPX + EyeX * WallDistance);
				int TestY = (int)(FPY + EyeY * WallDistance);
		
				if (TestX < 0 || TestX >= MapWidth || TestY < 0 || TestY >= MapHeight)
				{
					HitTheWall = true;
					WallDistance = MaxDepth;
			}
				else {
					if (map[TestY * MapWidth + TestX] == '#') {
						HitTheWall = true;
					}
				}
			
			}
			int Celling = (float)(ScreenHeight / 2.0) - ScreenHeight / (float)WallDistance;
			int Floor = ScreenHeight - Celling;

			for (int y=0; y < ScreenHeight; y++) {
				if (y < Celling)
					screen[y * ScreenWidth + x] = ' ';
				else if(y > Celling && y <= Floor)
					screen[y*ScreenWidth + x] = '#';
				else
					screen[y * ScreenWidth + x] = ' ';
			}


		}









	return 0;
}