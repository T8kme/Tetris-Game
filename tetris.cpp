////////////////////////////////////////////////////////////////////////////////
// 																			  //
//																			  //
// 							   Tetris  Game v1.0                              //
// 								Rafa³ Olszewski                               //
// 									E4Y2S1                                    //
//                                                                            //
// 																		      //
////////////////////////////////////////////////////////////////////////////////
#include  <iostream>
#include  <ctime>
#include  <conio.h>
#include  <cmath>

#define BORDER_COLOR 1 // change border color

using namespace std;

// Variable declarations
const int g_high = 16; // max. 40
const int g_width = 14; // max. 30
const int g_size = 4; // max. 4
int level = 1;
int lines = 0;
int points = 0;
int combo = 0;
int board[g_width][g_high];
int block[g_size][2];
bool if_combo;

// Functions
void Begin();
void Refresh();
void Draw();
void Start();
void To_Left();
void To_Right();
void Remove(int);
void Rotate();
void Center();
void Change_Center(int);
bool Create();
bool Load();
bool Fall();
bool Line(int);
bool Restart();
// double round(double);  // to win32

///////////////////////////////////////////////////////////////////////////////

int main() {
	Begin();
	Start();
	Draw();
	for (; Create();) {
		Refresh();
		bool next = true;
		for (int a = 0; next; a++) {
			int limit = 50000 / level;
			if (a > limit) {
				a = 0;
				next = Fall();
				Refresh();
			}
			if (kbhit()) {
				next = Load();
				Refresh();
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void Draw() // Drawborder
{
	clrscr(); // clear clonsole
	for (int a = 0; a < g_width + 2; a++) {
		textcolor(BORDER_COLOR);
		putch(219);
	}
	for (int a = 1; a <= g_high; a++) {
		gotoxy(1, a + 1);
		textcolor(BORDER_COLOR);
		putch(219);
		gotoxy(g_width + 2, a + 1);
		putch(219);
	}
	cout << endl;
	for (int a = 0; a < g_width + 2; a++) {
		textcolor(BORDER_COLOR);
		putch(219);
	}
}

bool Create() {
	bool next = true;
	for (int a = g_width / 2 - g_size / 2;
	a <= g_width / 2 + g_size / 2; a++) // check if line is clear
	{
		if (board[a][0] != 0) {
			next = false;
		}
	}
	if (next) {
		srand(time(0));
		int x = g_width / 2;
		int y = 0;
		block[0][0] = x;
		block[0][1] = y;
		board[x][y] = 1;
		for (int a = 1; a < g_size;) // Create block
		{
			int element = rand() % a;
			int direction = rand() % 4;
			y = block[element][1];
			x = block[element][0];
			switch (direction) {
			case 0:
				if (y != 0 && board[x][y - 1] == 0) {
					y--;
					block[a][0] = x;
					block[a][1] = y;
					board[x][y] = 1;
					a++;
				}
				break;
			case 1:
				if (board[x][y + 1] == 0) {
					y++;
					block[a][0] = x;
					block[a][1] = y;
					board[x][y] = 1;
					a++;
				}
				break;
			case 2:
				if (board[x - 1][y] == 0) {
					x--;
					block[a][0] = x;
					block[a][1] = y;
					board[x][y] = 1;
					a++;
				}
				break;
			case 3:
				if (board[x + 1][y] == 0) {
					x++;
					block[a][0] = x;
					block[a][1] = y;
					board[x][y] = 1;
					a++;
				}
				break;
			}
			srand(rand());
		}
		Center();
		return true;
	}
	else // stop
	{
		gotoxy(1, g_high + 3);
		cout << "Game over!" << endl;
		cout << "Do you want start game again? T/N" << endl;
		if (Restart() == 1) {
			Start();
			Draw();
			return true;
		}
		else
			return false;
	}
}

void Start() // clear table
{
	for (int a = 0; a < g_width; a++) {
		for (int b = 0; b < g_high; b++) {
			board[a][b] = 0;
		}
	}
}

void Refresh() {
	for (int a = 0; a < g_high; a++) // draw blocks
	{
		gotoxy(2, a + 2);
		for (int b = 0; b < g_width; b++) {
			if (board[b][a] == 1) { // moving
				textcolor(10);
				putch(177);
			}
			else if (board[b][a] == 2) { // laying
				textcolor(2);
				putch(178);
			}
			else
				cout << " ";
		}
	}
	gotoxy(g_width + 4, 1); // stats
	cout << "Points: " << points << endl;
	gotoxy(g_width + 4, 2);
	cout << "Combo: " << combo << endl;
	gotoxy(g_width + 4, 3);
	cout << "Level: " << level << endl;
}

bool Load() // keys
{
	bool correct = true;
	char znak = getch();
	switch (znak) {
	case 72:
		Rotate();
		break;
	case 80:
		correct = Fall();
		break;
	case 75:
		To_Left();
		break;
	case 77:
		To_Right();
		break;
	}
	return correct;
}

bool Fall() // move block down
{
	bool correct = true;
	for (int a = 0; a < g_size; a++) {
		if (block[a][1] + 1 >= g_high || board[block[a][0]][block[a][1] + 1] == 2) // stop
		{ // falling
			correct = false;
		}
	}
	if (correct) {
		for (int a = 0; a < g_size; a++) // przesuwanie
		{
			board[block[a][0]][block[a][1]] = 0; // delete old position
			block[a][1]++;
		}
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 1; // do new
		}
		return true;
	}
	else {
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 2;
		}
		if_combo = false;
		for (int a = 0; a < g_high; a++) {
			if (Line(a)) // if in one line
			{
				Remove(a);
				combo++;
				if_combo = true;
				lines++;
			}
		}
		if (!if_combo) // points multiplication - combo
		{
			points += combo * combo;
			combo = 0;
		}
		if (lines >= 5) // increase level
		{
			lines %= 5;
			level++;
		}
		return false;
	}
}

void To_Left() {
	bool correct = true;
	for (int a = 0; a < g_size; a++) {
		if (block[a][0] - 1 < 0 || board[block[a][0] - 1][block[a][1]] == 2) {
			correct = false;
		}
	}
	if (correct) {
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 0;
			block[a][0]--;
		}
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 1;
		}
	}
}

void To_Right() {
	bool correct = true;
	for (int a = 0; a < g_size; a++) {
		if (block[a][0] + 1 >= g_width ||
			board[block[a][0] + 1][block[a][1]] == 2) {
			correct = false;
		}
	}
	if (correct) {
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 0;
			block[a][0]++;
		}
		for (int a = 0; a < g_size; a++) {
			board[block[a][0]][block[a][1]] = 1;
		}
	}
}

bool Line(int a) // filled line
{
	bool correct = true;
	for (int b = 0; b < g_width; b++) {
		if (board[b][a] != 2) {
			correct = false;
		}
	}
	return correct;
}

void Remove(int a) // remove line and move down
{
	for (int b = a; b > 0; b--) {
		for (int c = 0; c < g_width; c++) {
			board[c][b] = board[c][b - 1];
		}
	}
	for (int b = 0; b < g_width; b++) {
		board[b][0] = 0;
	}
}

void Rotate() {
	bool correct = true;

	for (int a = 1; a < g_size; a++) {
		int x = block[a][0] - block[0][0];
		int y = block[a][1] - block[0][1];
		if (board[block[0][0] - y][block[0][1] +
			x] == 2 // if rotate possible
			|| block[0][1] + x >= g_high || block[0][1] + x < 0
			|| block[0][0] - y >= g_width || block[0][0] - y < 0) {
			correct = false;
		}
	}
	if (correct) {
		for (int a = 1; a < g_size; a++) // rotate
		{
			int x = block[a][0] - block[0][0];
			int y = block[a][1] - block[0][1];
			board[block[a][0]][block[a][1]] = 0;
			block[a][0] = block[0][0] - y;
			block[a][1] = block[0][1] + x;
		}
	}
	for (int a = 0; a < g_size; a++) {
		board[block[a][0]][block[a][1]] = 1; // fill table
	}
}

void Center() // set center of block
{
	double x = 0;
	double y = 0;
	for (int a = 0; a < g_size; a++) {
		x += block[a][0];
		y += block[a][1];
	}
	x /= 4;
	y /= 4;
	bool correct = true;
	for (int a = 1; a < g_size && correct; a++) {
		if ((block[a][0] == round(x)) && (block[a][1] == round(y))) {
			Change_Center(a);
			correct = false;
		}
	}
}

void Change_Center(int a) // change block center
{
	int tym = block[0][0];
	block[0][0] = block[a][0];
	block[a][0] = tym;
	tym = block[0][1];
	block[0][1] = block[a][1];
	block[a][1] = tym;
}

double round(double x) // round function
{
	return floor(x + 0.5);
}

bool Restart() // Game restart
{
	char znak = getch();
	switch (znak) {
	case 116: {
			combo = 0;
			points = 0;
			level = 1;
			for (int a = g_width / 2 - g_size / 2;
			a <= g_width / 2 + g_size / 2; a++) {
				board[a][0] = 0;
			}
			return true;
		}
	case 110:
		return false;
	default:
		Restart();
	}
}

void Begin() // signature
{
	printf("#######                              \n");
	printf("   #    ###### ##### #####  #  ####  \n");
	printf("   #    #        #   #    # # #      \n");
	printf("   #    #####    #   #    # #  ####  \n");
	printf("   #    #        #   #####  #      # \n");
	printf("   #    #        #   #   #  # #    # \n");
	printf("   #    ######   #   #    # #  ####  Ver 1.0\n\n");

	printf("Author: Rafal Olszewski E4Y2S1\n\nTo start please press any key..."
		);
	getch();
}
