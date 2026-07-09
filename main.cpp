#include <iostream>
#include <Windows.h> 
#include <thread>
#include <chrono>
#include <vector>
using namespace std;

uint16_t tetrimonos[7]; // tetris figures
int PlayfieldWidth = 12; // width of the arena
int PlayfieldHeight = 18; // height of the area
int ScreenWidth = 80; // width of the console
int ScreenHeight = 30; // height of the console
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r) {
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


bool DoesPieceFit(int Tetromino, int rotation, int posX, int posY) {
	for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				int position_index = Rotate(px, py, rotation);
				int field_index = (posY + py) * PlayfieldWidth + (posX + px);

				if (posX + px >= 0 && posX + px < PlayfieldWidth) {
					if (posY + py >= 0 && posY + py < PlayfieldHeight) {
						if ((tetrimonos[Tetromino] >> position_index) & 1 && pField[field_index] != 0)
							return false;
					}
				}
		}
		
	}
	return true;
}

int main() {

	// creates a screen buffer
	wchar_t *screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD dwBytesWritten = 0;

	// Forces the console window to be a specific size
	SMALL_RECT rect = { 0, 0, (SHORT)(ScreenWidth - 1), (SHORT)(ScreenHeight - 1) };
	SetConsoleWindowInfo(Console, TRUE, &rect);          
	COORD bufferSize = { (SHORT)ScreenWidth, (SHORT)ScreenHeight };
	SetConsoleScreenBufferSize(Console, bufferSize); 
    
    // Assets
	
	tetrimonos[0] = 0x4444; 
	tetrimonos[1] = 0x0264; 
    tetrimonos[2] = 0x0462;
	tetrimonos[3] = 0x0660; 
	tetrimonos[4] = 0x0464; 
	tetrimonos[5] = 0x4460;
	tetrimonos[6] = 0x2260;

	pField = new unsigned char[PlayfieldWidth * PlayfieldHeight]; // Create play field buffer
	for (int i = 0; i < PlayfieldWidth; i++) {
		for (int j = 0; j < PlayfieldHeight; j++) {
			pField[j * PlayfieldWidth + i] = (i == 0 || i == PlayfieldWidth - 1 || j == PlayfieldHeight - 1) ? 9 : 0;
		}
	}

	bool GameOver = false;
	int currentPiece = 0;
	int currentRotation = 0;
	int currentX = PlayfieldWidth / 2;
	int currentY = 0;
	bool bKey[4];
	bool rotateHold = false;

	int speed = 20;
	int speedCounter = 0;

	bool forceDown = false;
	int pieceCount = 0;
	int score = 0;

	vector<int> Lines;

	while (!GameOver) {

		//timing
		this_thread::sleep_for(50ms);
		speedCounter++;
		forceDown = (speedCounter == speed);
		
		// input
		for (int i = 0; i < 4; i++) {
			bKey[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[i]))) != 0;
		}
		// game logic 
		if (bKey[1]) {
			if (DoesPieceFit(currentPiece, currentRotation, currentX - 1, currentY)) {
				currentX = currentX - 1; 
			}
		}

		if (bKey[0]) {
			if (DoesPieceFit(currentPiece, currentRotation, currentX + 1, currentY)) {
				currentX = currentX + 1; 
			}
		}

		if (bKey[2]) {
			if (DoesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) {
				currentY = currentY + 1; 
			}
		}

		if (bKey[3]) {
			currentRotation += (rotateHold && DoesPieceFit(currentPiece, currentRotation + 1, currentX, currentY)) ? 1 : 0;
			rotateHold = false;
		}
			else {
				rotateHold = true;
			}
		
			if (forceDown) {
				if (DoesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) {
					currentY++;
				}

				else {
					
					//locking the piece in the field when it cannot go down anymore
					for (int px = 0; px < 4; px++) {
						for (int py = 0; py < 4; py++) {
							if ((tetrimonos[currentPiece] >> Rotate(px, py, currentRotation)) & 1) {
								pField[(currentY + py) * PlayfieldWidth + (currentX + px)] = currentPiece + 1;
							}
						}
					}

					pieceCount++;
					if (pieceCount % 10 == 0) {
						if (speed >= 10) speed--;
					}

					for (int py = 0; py < 4; py++) {
						if (currentY + py < PlayfieldHeight - 1) {
							bool line = true;
							for (int px = 1; px < PlayfieldWidth - 1; px++) {
								line &= (pField[(currentY + py) * PlayfieldWidth + px]) != 0;
							}
							
							// 8 represents the equal symbol
							if (line) {
								for (int px = 1; px < PlayfieldWidth - 1; px++) {
									pField[(currentY + py) * PlayfieldWidth + px] = 8;

									Lines.push_back(currentY + py);
								}
							}
						}
					}

					score += 25;
					if (!Lines.empty()) score += (1 << Lines.size()) * 100;
					//choosing the next piece in random
					currentX = PlayfieldWidth / 2;
					currentY = 0;
					currentRotation = 0;
					currentPiece = rand() % 7;
					
					//if the piece does not fit
					GameOver = !DoesPieceFit(currentPiece, currentRotation, currentX, currentY);
				}

				speedCounter = 0;
			}

		//drawing the field
		for (int i = 0; i < PlayfieldWidth; i++) {
			for (int j = 0; j < PlayfieldHeight; j++) {
				screen[(j + 2) * ScreenWidth + (i + 2)] = L" ABCDEFG=#"[pField[j * PlayfieldWidth + i]];
			}
		}

		//drawing the current piece
		for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				if ((tetrimonos[currentPiece] >> Rotate(px, py, currentRotation)) & 1) {
					screen[(currentY + py + 2) * ScreenWidth + (currentX + px + 2)] = currentPiece + 65;
				}
			}
		}

		//draw score
		swprintf(&screen[2 * ScreenWidth + PlayfieldWidth + 6], L"SCORE: %8d", score);

		if (!Lines.empty()) {
			WriteConsoleOutputCharacterW(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);

			for (auto &v :Lines) {
				for (int px = 1; px < PlayfieldWidth - 1; px++) {
					for (int py = v; py > 0; py--) {
						pField[py * PlayfieldWidth + px] = pField[(py - 1) * PlayfieldWidth + px];
						pField[px] = 0;
					}
				}
				
				Lines.clear();
			}

			
		}


		// display frame
		WriteConsoleOutputCharacterW(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// GAME OVER !!!
	CloseHandle(Console);
	cout << "Game over!! Score: " << score << endl;
	system("pause");
	

	return 0;
}