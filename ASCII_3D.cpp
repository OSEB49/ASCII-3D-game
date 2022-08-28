#include <iostream>
#include<chrono>
#include<vector>
#include<algorithm>
#include<stdio.h>
#include <Windows.h>
using namespace std;


int ScreenWidth = 120;
int ScreenHeight = 40;
//player's x,y and angle position
float FPX = 14.7f;
float FPY = 5.09f;
float FPA = 0.0f;
int MapWidth = 16;
int MapHeight = 16;

float ViewRange = 3.14159 / 4.0;
float MaxDepth = 16.0f;
float Speed = 5.0f;


int main() {


	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//map line by line
	wstring map;
	map += L"#######.........";
	map += L"#..............#";
	map += L"#.........######";
	map += L"#..............#";
	map += L"#.......##.....#";
	map += L"#.......###....#";
	map += L"#.#.#.#.#....###";
	map += L"#.....###......#";
	map += L"#..............#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#........##....#";
	map += L"#..............#";
	map += L"#....########..#";
	map += L"###............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (1) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float ElapsedTime = elapsedTime.count();


		//rotation and movement of character
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			FPA -= (Speed * 0.75f) * ElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			FPA += (Speed * 0.75f) * ElapsedTime;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			FPA += sinf(FPA) * Speed * ElapsedTime;
			FPY += cosf(FPA) * Speed * ElapsedTime;
			if (map.c_str()[(int)FPX * MapWidth + (int)FPY] == '#') {
				FPA -= sinf(FPA) * Speed * ElapsedTime;
				FPY -= cosf(FPA) * Speed * ElapsedTime;
			}
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			FPX -= sinf(FPA) * Speed * ElapsedTime;;
			FPY -= cosf(FPA) * Speed * ElapsedTime;;
			if (map.c_str()[(int)FPX * MapWidth + (int)FPY] == '#')
			{
				FPX -= sinf(FPA) * Speed * ElapsedTime;;
				FPY -= cosf(FPA) * Speed * ElapsedTime;;
			}
		}




		for (int x = 0; x < ScreenWidth; x++)
		{
			float RayAngle = (FPA - ViewRange / 2.0f) + ((float)x / (float)ScreenWidth) * ViewRange;


			float StepSize = 0.1f;
			float WallDistance = 0.0f;

			bool HitTheWall = false;
			bool Boundary = false;

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

						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
						vector<pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = (float)TestY + ty - FPY;
								float vx = (float)TestX + tx - FPX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (EyeX * vx / d) + (EyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) Boundary = true;
						if (acos(p.at(1).second) < fBound) Boundary = true;
						if (acos(p.at(2).second) < fBound) Boundary = true;
					}
				}

			}
			//calculation of celling and floor
			int Celling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)WallDistance);
			int Floor = ScreenHeight - Celling;
			short Shade = ' ';

			if (WallDistance <= MaxDepth / 4.0f)     Shade = 0x2588;
			else if (WallDistance < MaxDepth / 3.0f) Shade = 0x2593;
			else if (WallDistance < MaxDepth / 2.0f) Shade = 0x2592;
			else                                     Shade = ' ';

			if (Boundary) Shade = ' ';
			{
				for (int y = 0; y < ScreenHeight; y++) {
					if (y <= Celling)
						screen[y * ScreenWidth + x] = ' ';
					else if (y > Celling && y <= Floor)
						screen[y * ScreenWidth + x] = '#';
					else
					{
						// Shade floor based on distance
						float b = 1.0f - (((float)y - ScreenHeight / 2.0f) / ((float)ScreenHeight / 2.0f));
						if (b < 0.25)		Shade = '#';
						else if (b < 0.5)	Shade = 'x';
						else if (b < 0.75)	Shade = '.';
						else if (b < 0.9)	Shade = '-';
						else				Shade = ' ';
						screen[y * ScreenWidth + x] = Shade;
					}
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", FPX, FPY, FPA, 1.0f / ElapsedTime);

		// Display Map
		for (int nx = 0; nx < MapWidth; nx++)
			for (int ny = 0; ny < MapWidth; ny++)
			{
				screen[(ny + 1) * ScreenWidth + nx] = map[ny * MapWidth + nx];
			}
		screen[((int)FPX + 1) * ScreenWidth + (int)FPY] = 'P';


		screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}


	return 0;
}
