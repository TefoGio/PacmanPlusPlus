#include <iostream>
#include <windows.h>
#include <conio.h> 

using namespace std;

// Keyboard ASCII codes
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27

// Window handle
static HANDLE hStdout;
static CONSOLE_SCREEN_BUFFER_INFO csbi;
DWORD dummy;

// For main game loop to quit
bool escKeyPressed = false;

// To draw grid
WORD color;
char character;

// Pacman
struct pacman {
	int location;
	bool mouthOpen = false;
	bool godMode = false;
	int godModeMaxMoves = 20;
	int godModeMovesLeft = 20;
	int lives = 3;
	int score = 0;
} pacman;

// Game variables
struct game {
	int dotsRemaining = 0;
	int teleporterLeft = 0;
	int teleporterRight = 0;
	int status = 0;
	int themeMusicPlayed = 0;
} game;

#define NOTHING 0
#define WALL 1
#define WHITE_DOT 2
#define YELLOW_DOT 3
#define BLINKY 4
#define INKY 5
#define PINKY 6
#define CLYDE 7
#define PACMAN 8
#define TELEPORT 9

unsigned short int originalGrid[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 3, 1,
	1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	9, 0, 0, 0, 0, 2, 0, 0, 1, 4, 5, 0, 6, 7, 1, 0, 0, 2, 0, 0, 0, 0, 9,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
	1, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 8, 2, 2, 2, 2, 2, 2, 1, 2, 2, 3, 1,
	1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

unsigned short int grid[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 3, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 3, 1,
	1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	9, 0, 0, 0, 0, 2, 0, 0, 1, 4, 5, 0, 6, 7, 1, 0, 0, 2, 0, 0, 0, 0, 9,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
	1, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 8, 2, 2, 2, 2, 2, 2, 1, 2, 2, 3, 1,
	1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void setUpGame() {
	// Get pacman's location
	// Count number of white dots
	for (int i = 0; i < (sizeof(grid) / sizeof(*grid)); i++) {
		switch (grid[i]) {
			case PACMAN:
				pacman.location = i;
				break;
			case WHITE_DOT:
				game.dotsRemaining++;
				break;
			case YELLOW_DOT:
				game.dotsRemaining++;
				break;
			case TELEPORT:
				if (game.teleporterLeft == 0) {
					game.teleporterLeft = i;
				} else {
					game.teleporterRight = i;
				}
				break;
			default:
				break;
		}
	}
}

void drawGrid() {
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 23; j++) {
			switch (grid[(i * 23) + j]) {
			case NOTHING:
				character = { ' ' }; color = 0; break;
			case WALL:
				character = { '#' }; color = 1; break;
			case WHITE_DOT:
				character = { '.' }; color = 15; break;
			case YELLOW_DOT:
				character = { '.' }; color = 14; break;
			case BLINKY:
				character = { 'o' }; color = 4; break;
			case INKY:
				character = { 'o' }; color = 5; break;
			case PINKY:
				character = { 'o' }; color = 6; break;
			case CLYDE:
				character = { 'o' }; color = 7; break;
			case PACMAN:
				// Make him chomp
				if (pacman.mouthOpen) {
					character = { 'C' };
				}
				else {
					character = { 'O' };
				}

				// For the last 10 moves have pacman alternate color
				// to warn the godmode is going to turn off
				if (pacman.godMode) {
					if ((pacman.godModeMovesLeft <= 10) && (pacman.godModeMovesLeft % 2 == 1)) {
						color = 15;
					}
					else {
						color = 13;
					}

				}
				else {
					color = 14;
				}

				break;
			case TELEPORT:
				character = { ' ' }; color = 0; break;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
			SetConsoleCursorPosition(hStdout, { j, i });
			cout << character;
		}
	}

	// Print score and lives
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(hStdout, { 0, 26 });

	int spacing;

	if (pacman.score > 9999) {
		spacing = 3;
	}
	else if (pacman.score > 999) {
		spacing = 4;
	}
	else if (pacman.score > 99) {
		spacing = 5;
	}
	else if (pacman.score > 9) {
		spacing = 6;
	}
	else {
		spacing = 7;
	}

	cout << "Score: " << pacman.score;

	for (int i = 0; i < spacing; i++) {
		cout << " ";
	}

	cout << "Lives: " << pacman.lives;
}

void flashScreen() {
	int alternate = 1;
	int color = 0;

	for (int k = 0; k < 10; k++) {

		if (alternate == 0) {
			color = 1;
			alternate = 1;
		}
		else {
			color = 15;
			alternate = 0;
		}

		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 23; j++) {
				switch (grid[(i * 23) + j]) {
				case WALL:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
					SetConsoleCursorPosition(hStdout, { j, i });
					cout << "#";
					break;
				default:
					break;
				}
			}
		}
		Sleep(200);
	}
}

void movePacman(int oldLocation, int newLocation) {
	// Process godmode
	if (pacman.godMode) {
		if (pacman.godModeMovesLeft - 1 == 0) {
			pacman.godMode = false;
			pacman.godModeMovesLeft = pacman.godModeMaxMoves;
		}
		else {
			pacman.godModeMovesLeft--;
		}
	}

	// Teleporter special case
	if (newLocation == game.teleporterLeft) {
		grid[oldLocation] = NOTHING;
		pacman.location = game.teleporterRight;
		
		grid[game.teleporterRight] = PACMAN;
	} else if (newLocation == game.teleporterRight) {
		grid[oldLocation] = NOTHING;
		pacman.location = game.teleporterLeft;
		
		grid[game.teleporterLeft] = PACMAN;
	} else {
	// All other cases
		switch (grid[newLocation]) {
		case NOTHING:
			grid[oldLocation] = NOTHING;
			pacman.location = newLocation;
			grid[newLocation] = PACMAN;
			break;
		case WALL:
			break;
		case WHITE_DOT:
			pacman.score++;
			game.dotsRemaining--;
			grid[oldLocation] = NOTHING;
			pacman.location = newLocation;
			grid[newLocation] = PACMAN;
			
			if (game.dotsRemaining == 0) {
				drawGrid();
				flashScreen();
				copy(originalGrid, originalGrid + 575, grid);
				setUpGame();
			}

			break;
		case YELLOW_DOT:
			Beep(261.63 * 2, 200); // C
			Beep(329.63 * 2, 200); // E
			game.dotsRemaining--;
			grid[oldLocation] = NOTHING;
			pacman.location = newLocation;
			grid[newLocation] = PACMAN;
			pacman.godMode = true;
			pacman.godModeMovesLeft = pacman.godModeMaxMoves;

			if (game.dotsRemaining == 0) {
				drawGrid();
				flashScreen();
				copy(originalGrid, originalGrid + 575, grid);
				setUpGame();
			}

			break;
		case BLINKY:
			break;
		case INKY:
			break;
		case PINKY:
			break;
		case CLYDE:
			break;
		}
	}
}

void moveLeft() {
	movePacman(pacman.location, pacman.location - 1);
}
void moveRight() {
	movePacman(pacman.location, pacman.location + 1);
}
void moveUp() {
	movePacman(pacman.location, pacman.location - 23);
}
void moveDown() {
	movePacman(pacman.location, pacman.location + 23);
}

void getInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case KB_ESCAPE:
			escKeyPressed = true;
			break;
		case KB_LEFT:
			pacman.mouthOpen = !pacman.mouthOpen;
			moveLeft();
			break;
		case KB_RIGHT:
			pacman.mouthOpen = !pacman.mouthOpen;
			moveRight();
			break;
		case KB_UP:
			pacman.mouthOpen = !pacman.mouthOpen;
			moveUp();
			break;
		case KB_DOWN:
			pacman.mouthOpen = !pacman.mouthOpen;
			moveDown();
			break;
		}
	}
}

void themeMusic() {
	Beep(245.94, 200); // B
	Beep(245.94 * 2, 200); // B
	Beep(185.00 * 2, 200); // F#
	Beep(155.56 * 2, 200); // Eb
	Beep(245.94 * 2, 100); // B
	Beep(185.00 * 2, 200); // F#
	Beep(155.56 * 2, 400); // Eb
	Sleep(100);

	Beep(261.63, 200); // C 
	Beep(261.63 * 2, 200); // C
	Beep(391.00, 200); // G
	Beep(329.63, 200); // E
	Beep(261.63 * 2, 100); // C
	Beep(391.00, 200); // G
	Beep(329.63, 400); // E
	Sleep(100);

	Beep(245.94, 200); // B
	Beep(245.94 * 2, 200); // B
	Beep(185.00 * 2, 200); // F#
	Beep(155.56 * 2, 200); // Eb
	Beep(245.94 * 2, 100); // B
	Beep(185.00 * 2, 200); // F#
	Beep(155.56 * 2, 400); // Eb
	Sleep(100);

	Beep(155.56 * 2, 100); // Eb
	Beep(329.63, 100); // E
	Beep(349.23, 200); // F

	Beep(349.23, 100); // F
	Beep(369.99, 100); // F#
	Beep(391.00, 200); // G

	Beep(391.00, 100); // G
	Beep(415.30, 100); // Ab
	Beep(440.00, 200); // A
	Beep(245.00 * 2, 400); //B
}

int main() {
	// Center the console to the center of the screen
	HWND console = GetConsoleWindow();
	int consoleWidth = 800;
	int consoleHeight = 500;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(console, (screenWidth - consoleWidth) / 2, (screenHeight -consoleHeight) / 2, consoleWidth, consoleHeight, TRUE);

	// Get stdout handle
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Hide cursor
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(hStdout, &CURSOR);
	
	// Set up game variables
	setUpGame();

	// Main game loop
	while ( ! escKeyPressed) {
		if ( ! game.themeMusicPlayed) {
			drawGrid();
			themeMusic();
			game.themeMusicPlayed = 1;
		}
		else {
			getInput();
			drawGrid();
		}
	}

	return 0;
}