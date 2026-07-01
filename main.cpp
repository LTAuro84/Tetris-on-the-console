#include <iostream>
#include <Windows.h> 
using namespace std;

wstring tetrimonos[7]; //tetris figures
int PlayfieldWidth = 12; // width of the arena
int PlayfieldHeight = 18; // height of the area
int ScreenWidth = 80; // width of the console
int ScreenHeight = 30; // height of the console
unsigned char *pField = nullptr;

int Rotation(int px, int py, int r) {
    switch (r % 4) {
        case 0:
            return py * 4 + px;
        case 1:
            return 12 + py - (px * 4);
        case 2:
            return 15 - (py * 4) - px;
        case 3:
            return 3 - py + (px * 4);
        
    }
    return 0;
}

int main() {
    
    // Assets
	tetrimonos[0].append(L"..X.");
	tetrimonos[0].append(L"..X.");
	tetrimonos[0].append(L"..X.");
	tetrimonos[0].append(L"..X.");

    tetrimonos[1].append(L"..X.");
	tetrimonos[1].append(L".XX.");
	tetrimonos[1].append(L".X..");
	tetrimonos[1].append(L"....");

    tetrimonos[2].append(L".X..");
	tetrimonos[2].append(L".XX.");
	tetrimonos[2].append(L"..X.");
	tetrimonos[2].append(L"....");

    tetrimonos[3].append(L"....");
	tetrimonos[3].append(L".XX.");
	tetrimonos[3].append(L".XX.");
	tetrimonos[3].append(L"....");
    
    tetrimonos[4].append(L"..X.");
	tetrimonos[4].append(L".XX.");
	tetrimonos[4].append(L"..X.");
	tetrimonos[4].append(L"....");

    tetrimonos[5].append(L"....");
	tetrimonos[5].append(L".XX.");
	tetrimonos[5].append(L"..X.");
	tetrimonos[5].append(L"..X.");

    tetrimonos[6].append(L"....");
	tetrimonos[6].append(L".XX.");
	tetrimonos[6].append(L".X..");
	tetrimonos[6].append(L".X..");

	pField = new unsigned char[PlayfieldWidth * PlayfieldHeight]; // Create play field buffer
	for (int i = 0; i < PlayfieldWidth; i++) {
		for (int j = 0; j < PlayfieldHeight; j++) {
			pField[j * PlayfieldWidth + i] = (i == 0 || i == PlayfieldWidth - 1 || j == PlayfieldHeight - 1) ? 9 : 0;
		}
	}

	// creates a screen buffer
	wchar_t *screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD dwBytesWritten = 0;

	bool GameOver = false;
	while (!GameOver) {
		//drawing the field
		


		// display frame
		WriteConsoleOutputCharacter(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	

	return 0;
}