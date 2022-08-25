#include <iostream>
#include<chrono>
using namespace std;
#include <Windows.h>

int ScreenWidth = 120;
int ScreenHeight = 40;
//player's x,y and angle position
float FPX = 8.0f;
float FPY = 8.0f;
float FPA = 0.0f;
int MapWidth = 16;
int MapHeight = 16;

float ViewRange = 3.14159 / 4.0;
float MaxDepth = 16.0f;

int main() {


	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//map line by line
	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();



	

	while (1) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float ElapsedTime = elapsedTime.count();



		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			FPA -= (0.1f) * ElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			FPA += (0.1f) + ElapsedTime;

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
					if (map.c_str()[TestY * MapWidth + TestX] == '#') {
						HitTheWall = true;
					}
				}

			}
			//calculation of celling and floor
			int Celling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)WallDistance);
			int Floor = ScreenHeight - Celling;

			for (int y = 0; y < ScreenHeight; y++) {
				if (y < Celling)
					screen[y * ScreenWidth + x] = ' ';
				else if (y > Celling && y <= Floor)
					screen[y * ScreenWidth + x] = '#';
				else
					screen[y * ScreenWidth + x] = ' ';
			}


		}

		screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}


	return 0;
}
