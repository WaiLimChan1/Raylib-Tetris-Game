#include"raylib.h"
#include<vector>
#include<cstdlib>
#include<time.h>
#include<string>
#include <iostream>

using namespace std;

//KeyBinds
//-------------------------------------
// W = Rotate Right
// S = Rotate Left
// A = Left
// D = Right
// F = Instantly Drop
// Space = Hold Piece
// Space = Play Aagain
//-------------------------------------

struct Point {
	int x;
	int y;
};

enum eShape { Tee = 0, LeftKink, RightKink, Square, LeftElbow, RightElbow, Bar };

struct Shape {
	Point block[4];
	eShape currentShape;
	int shapeRotation;
	int possibleRotations;
	bool shapeNeeded;

	void swapShape(Shape& obj)
	{
		eShape currentShapeTemp = obj.currentShape;
		int shapeRotationTemp = obj.shapeRotation;
		int possibleRotationsTemp = obj.possibleRotations;

		obj.currentShape = currentShape;
		obj.shapeRotation = shapeRotation;
		obj.possibleRotations = possibleRotations;

		currentShape = currentShapeTemp;
		shapeRotation = shapeRotationTemp;
		possibleRotations = possibleRotationsTemp;
	}
};

//Global Const Variables
//-------------------------------------
const int screenWidth = 1200;
const int screenHeight = 900;

const int boardWidth = 10;
const int boardHeight = 24;
const int displayBoardHeight = 20;
char board[boardHeight][boardWidth];

const int boxWidth = 30;
const int paddingWidth = 3;
const int borderWidth = 10;

const int rightBoardWidth = 4;
const int rightBoardHeight = 4;
//-------------------------------------


//Global Variables
//-------------------------------------
int score;

Shape shape, nextShape, heldShape;

enum eInput { Left = 0, Right, RotateRight, RotateLeft, Down, InstaDrop, HoldShape, DoNothing };
eInput input;

int speed;
int speedCounter;

bool gameOver;
//-------------------------------------

void ranNextShapeAndRotation()
{
	nextShape.currentShape = eShape(rand() % 7);
	switch (nextShape.currentShape)
	{
	case Tee:
		nextShape.possibleRotations = 4;
		break;
	case LeftKink:
		nextShape.possibleRotations = 2;
		break;
	case RightKink:
		nextShape.possibleRotations = 2;
		break;
	case Square:
		nextShape.possibleRotations = 1;
		break;
	case LeftElbow:
		nextShape.possibleRotations = 4;
		break;
	case RightElbow:
		nextShape.possibleRotations = 4;
		break;
	case Bar:
		nextShape.possibleRotations = 2;
		break;
	}
	nextShape.shapeRotation = rand() % nextShape.possibleRotations;
}

void setShape(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Shape& obj)
{
	obj.block[0].x = x0;
	obj.block[0].y = y0;
	obj.block[1].x = x1;
	obj.block[1].y = y1;
	obj.block[2].x = x2;
	obj.block[2].y = y2;
	obj.block[3].x = x3;
	obj.block[3].y = y3;
}

void getShape(Shape& obj)
{
	switch (obj.currentShape)
	{
	case Tee:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(4, 3, 3, 3, 4, 2, 5, 3, obj);
			break;
		case 1:
			setShape(4, 2, 4, 3, 5, 2, 4, 1, obj);
			break;
		case 2:
			setShape(4, 2, 5, 2, 4, 3, 3, 2, obj);
			break;
		case 3:
			setShape(4, 2, 4, 3, 3, 2, 4, 1, obj);
			break;
		}
		break;
	case LeftKink:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(5, 2, 4, 1, 4, 2, 5, 3, obj);
			break;
		case 1:
			setShape(5, 2, 4, 3, 5, 3, 6, 2, obj);
			break;
		}
		break;
	case RightKink:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(4, 2, 5, 3, 4, 3, 3, 2, obj);
			break;
		case 1:
			setShape(4, 2, 5, 1, 5, 2, 4, 3, obj);
			break;
		}
		break;
	case Square:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(4, 3, 5, 3, 4, 2, 5, 2, obj);
			break;
		}
		break;
	case LeftElbow:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(5, 3, 3, 3, 4, 3, 5, 2, obj);
			break;
		case 1:
			setShape(4, 3, 4, 1, 4, 2, 5, 3, obj);
			break;
		case 2:
			setShape(4, 2, 6, 2, 5, 2, 4, 3, obj);
			break;
		case 3:
			setShape(4, 1, 4, 3, 4, 2, 3, 1, obj);
			break;
		}
		break;
	case RightElbow:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(3, 3, 5, 3, 4, 3, 3, 2, obj);
			break;
		case 1:
			setShape(4, 1, 4, 3, 4, 2, 5, 1, obj);
			break;
		case 2:
			setShape(5, 2, 3, 2, 4, 2, 5, 3, obj);
			break;
		case 3:
			setShape(5, 3, 5, 1, 5, 2, 4, 3, obj);
			break;
		}
		break;
	case Bar:
		switch (obj.shapeRotation)
		{
		case 0:
			setShape(4, 2, 4, 0, 4, 1, 4, 3, obj);
			break;
		case 1:
			setShape(4, 3, 6, 3, 5, 3, 3, 3, obj);
			break;
		}
		break;
	}
}

void SetUp()
{
	score = 0;
	shape.shapeNeeded = true;
	nextShape.shapeNeeded = true;
	heldShape.shapeNeeded = true;
	input = DoNothing;
	speed = 5;
	speedCounter = 0;
	gameOver = false;
	for (int row = 0; row < boardHeight; row++)
		for (int column = 0; column < boardWidth; column++)
			board[row][column] = ' ';

	//Fill nextShape
	ranNextShapeAndRotation();
	getShape(nextShape);
	nextShape.shapeNeeded = false;
}

void Store()
{
	if (shape.shapeNeeded == false)
		for (int i = 0; i < 4; i++)
			board[shape.block[i].y][shape.block[i].x] = 'x';
}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
	if (!gameOver)
	{
		//Game Board
		int offSetX = (GetScreenWidth() - ((boxWidth + paddingWidth) * boardWidth)) / 2;
		int offSetY = (GetScreenHeight() - ((boxWidth + paddingWidth) * displayBoardHeight)) / 2;
		DrawRectangle(offSetX - paddingWidth - boardWidth, offSetY - paddingWidth - borderWidth, borderWidth, ((boxWidth + paddingWidth) * displayBoardHeight) + borderWidth * 2, WHITE);
		DrawRectangle(offSetX - boardWidth - paddingWidth, offSetY - paddingWidth - borderWidth, ((boxWidth + paddingWidth) * boardWidth) + paddingWidth + boardWidth * 2, borderWidth, WHITE);
		for (int row = 4; row < boardHeight; row++)
		{
			for (int column = 0; column < boardWidth; column++)
			{
				if (board[row][column] == ' ')
					DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GRAY);
				else if (board[row][column] == 'X')
					DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, DARKGREEN);
				else if (board[row][column] == 'x')
					DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GREEN);
				offSetX += boxWidth + paddingWidth;
			}
			if (row == 4)
				DrawRectangle(offSetX, offSetY - paddingWidth - borderWidth, borderWidth, ((boxWidth + paddingWidth) * displayBoardHeight) + borderWidth * 2, WHITE);
			offSetX = (GetScreenWidth() - ((boxWidth + paddingWidth) * boardWidth)) / 2;
			offSetY += boxWidth + paddingWidth;
		}
		DrawRectangle(offSetX - boardWidth - paddingWidth, offSetY, ((boxWidth + paddingWidth) * boardWidth) + paddingWidth + boardWidth * 2, borderWidth, WHITE);


		//Right Side Information
		//-------------------------------------
		
		//Display Score
		offSetX += ((boxWidth + paddingWidth) * boardWidth) + boxWidth * 2;
		offSetY = (GetScreenHeight() - ((boxWidth + paddingWidth) * displayBoardHeight)) / 2;
		char message[20];
		strcpy_s(message, "Your Score: ");
		strcat_s(message, to_string(score).c_str());
		int textWidth = MeasureText(message, 30);
		DrawText(message, offSetX, offSetY, 30, YELLOW);

		//Display Next Shape Message
		offSetY += boxWidth * 3;
		textWidth = MeasureText("Next Shape", 30);
		DrawText("Next Shape", offSetX, offSetY, 30, YELLOW);

		//Display Next Shape
		offSetY += boxWidth + 30;
		for (int row = 0; row < rightBoardHeight; row++)
		{
			for (int column = 0; column < rightBoardWidth; column++)
			{
				bool contain = false;
				for (int i = 0; i < 4; i++)
				{
					if (nextShape.block[i].y == row && nextShape.block[i].x - 3 == column)
						contain = true;
				}
				if (contain) DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GREEN);
				else DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GRAY);
				offSetX += boxWidth + paddingWidth;
			}
			offSetX = (GetScreenWidth() - ((boxWidth + paddingWidth) * boardWidth)) / 2 + ((boxWidth + paddingWidth) * boardWidth) + boxWidth * 2;
			offSetY += boxWidth + paddingWidth;
		}

		//Display Held Shape Message
		offSetY += boxWidth * 3;
		textWidth = MeasureText("Held Shape", 30);
		DrawText("Held Shape", offSetX, offSetY, 30, YELLOW);

		//Display Held Shape
		offSetY += boxWidth + 30;
		for (int row = 0; row < rightBoardHeight; row++)
		{
			for (int column = 0; column < rightBoardWidth; column++)
			{
				bool contain = false;
				for (int i = 0; i < 4; i++)
				{
					if (!heldShape.shapeNeeded && heldShape.block[i].y == row && heldShape.block[i].x - 3 == column)
						contain = true;
				}
				if (contain) DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GREEN);
				else DrawRectangle(offSetX, offSetY, boxWidth, boxWidth, GRAY);
				offSetX += boxWidth + paddingWidth;
			}
			offSetX = (GetScreenWidth() - ((boxWidth + paddingWidth) * boardWidth)) / 2 + ((boxWidth + paddingWidth) * boardWidth) + boxWidth * 2;
			offSetY += boxWidth + paddingWidth;
		}
		//--------------------------------------------------------


		//Left Side Information
		//--------------------------------------------------------
		offSetX = (GetScreenWidth() - ((boxWidth + paddingWidth) * boardWidth)) / 2 - boardWidth * (boxWidth + paddingWidth);
		offSetY = (GetScreenHeight() - ((boxWidth + paddingWidth) * displayBoardHeight)) / 2;

		textWidth = MeasureText("Directions", 30);
		DrawText("Directions", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("W = Rotate Right", 30);
		DrawText("W = Rotate Right", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("S = Down", 30);
		DrawText("S = Down", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("A = Left", 30);
		DrawText("A = Left", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("D = Right", 30);
		DrawText("D = Right", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("F = Instantly Drop", 30);
		DrawText("F = Instantly Drop", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;

		textWidth = MeasureText("Space = Hold Piece", 30);
		DrawText("Space = Hold Piece", offSetX, offSetY, 30, YELLOW);
		offSetY += 40;
	}
	else
	{
		int textWidth = MeasureText("Game over", 60);
		DrawText("Game over", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 60, 60, YELLOW);

		char message[20];
		strcpy_s(message, "Your Score: ");
		strcat_s(message, to_string(score).c_str());
		textWidth = MeasureText(message, 60);
		DrawText(message, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2, 60, YELLOW);

		if (IsKeyDown(KEY_SPACE))
		{
			SetUp();
		}
	}
	EndDrawing();
}

void Input()
{
	if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) input = Left;
	else if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) input = Right;
	else if (IsKeyPressed(KEY_W)) input = RotateRight;
	else if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) input = Down; //input = RotateLeft;
	else if (IsKeyPressed(KEY_F) || IsKeyDown(KEY_F)) input = InstaDrop;
	else if (IsKeyPressed(KEY_SPACE) || IsKeyDown(KEY_SPACE)) input = HoldShape;
}

bool breakingLeftBound()
{
	bool outLeftBound = false;
	for (int i = 0; i < 4; i++)
	{
		if (shape.block[i].x < 0)
			outLeftBound = true;
	}
	return outLeftBound;
}

bool breakingRightBound()
{
	bool outRightBound = false;
	for (int i = 0; i < 4; i++)
	{
		if (shape.block[i].x > 9)
			outRightBound = true;
	}
	return outRightBound;
}

bool breakingBottomBound()
{
	bool outBottomBound = false;
	for (int i = 0; i < 4; i++)
	{
		if (shape.block[i].y > 23 || board[shape.block[i].y + 1][shape.block[i].x] == 'X')
			outBottomBound = true;
	}
	return outBottomBound;
}

void rotateShape()
{
	int oldX = shape.block[0].x;
	int oldY = shape.block[0].y;
	getShape(shape);
	int difInX = oldX - shape.block[0].x;
	int difInY = oldY - shape.block[0].y;
	for (int i = 0; i < 4; i++)
	{
		shape.block[i].x += difInX;
		shape.block[i].y += difInY;
	}
	while (breakingLeftBound())
		for (int i = 0; i < 4; i++)
			shape.block[i].x++;
	while (breakingRightBound())
		for (int i = 0; i < 4; i++)
			shape.block[i].x--;
	while (breakingBottomBound())
		for (int i = 0; i < 4; i++)
			shape.block[i].y--;
}

void shapeFalls()
{
	bool touchingBoundary = false;
	for (int i = 0; i < 4; i++)
		if (shape.block[i].y == 23 || board[shape.block[i].y + 1][shape.block[i].x] == 'X')
			touchingBoundary = true;
	if (touchingBoundary)
	{
		shape.shapeNeeded = true;
		for (int i = 0; i < 4; i++)
			board[shape.block[i].y][shape.block[i].x] = 'X';

		//Delete finished row
		int mul = 0;
		for (int row = boardHeight - 1; row >= 0; row--)
		{
			bool rowFinished = true;
			for (int column = 0; column < boardWidth; column++)
			{
				if (board[row][column] != 'X')
					rowFinished = false;
			}
			if (rowFinished)
			{
				for (int column = 0; column < boardWidth; column++)
					board[row][column] = ' ';

				for (int rowsAbove = row - 1; rowsAbove >= 0; rowsAbove--)
				{
					for (int column = 0; column < boardWidth; column++)
					{
						if (board[rowsAbove][column] == 'X')
						{
							board[rowsAbove][column] = ' ';
							board[rowsAbove + 1][column] = 'X';
						}
					}
				}
				row++;
				mul++;
				score += 100;
			}
		}	
		if (mul == 2) score += 50;
		else if (mul == 3) score += 200;
		else if (mul == 4) score += 400;
	}
	else
		for (int i = 0; i < 4; i++)
			shape.block[i].y += 1;
}

void Logic()
{
	speedCounter++;

	//Get shape from nextShape
	if (shape.shapeNeeded)
	{
		shape = nextShape;
		ranNextShapeAndRotation();
		getShape(nextShape);
		shape.shapeNeeded = false;
	}
	else
	{
		bool touchingBoundary = false;
		int oldRotation = shape.shapeRotation;
		switch (input)
		{
		case Left:
			for (int i = 0; i < 4; i++)
				if (shape.block[i].x == 0 || board[shape.block[i].y][shape.block[i].x - 1] == 'X')
					touchingBoundary = true;
			if (!touchingBoundary)
				for (int i = 0; i < 4; i++)
					shape.block[i].x -= 1;
			break;
		case Right:
			for (int i = 0; i < 4; i++)
				if (shape.block[i].x == 9 || board[shape.block[i].y][shape.block[i].x + 1] == 'X')
					touchingBoundary = true;
			if (!touchingBoundary)
				for (int i = 0; i < 4; i++)
					shape.block[i].x += 1;
			break;
		case RotateRight:
			shape.shapeRotation++;
			if (shape.shapeRotation == shape.possibleRotations)
				shape.shapeRotation = 0;
			break;
		case RotateLeft:
			shape.shapeRotation--;
			if (shape.shapeRotation == -1)
				shape.shapeRotation += shape.possibleRotations;
			break;
		case Down:
			shapeFalls();
			break;
		case HoldShape:
			if (input == HoldShape)
			{
				if (heldShape.shapeNeeded)
				{
					heldShape.swapShape(nextShape);
					ranNextShapeAndRotation();
					getShape(nextShape);
				}
				shape.swapShape(heldShape);
				heldShape.shapeNeeded = false;
				getShape(heldShape);
			}
			rotateShape();
			break;
		}

		if (oldRotation != shape.shapeRotation && HoldShape != input)
			rotateShape();

		//Shape falls down
		if (speedCounter % speed == 0)
			shapeFalls();

		while (input == InstaDrop && !shape.shapeNeeded)
			shapeFalls();
	}

	//Remove old shape
	for (int row = 0; row < boardHeight; row++)
		for (int column = 0; column < boardWidth; column++)
			if (board[row][column] == 'x')
				board[row][column] = ' ';


	//Add shape into board
	if (!shape.shapeNeeded)
		for (int i = 0; i < 4; i++)
			board[shape.block[i].y][shape.block[i].x] = 'x';

	//Check if game is over
	for (int i = 0; i < boardWidth; i++)
		if (board[3][i] == 'X')
			gameOver = true;

	input = DoNothing;
}

int main()
{
	InitWindow(screenWidth, screenHeight, "Snake Raylib");
	SetTargetFPS(10);
	srand(time(0));

	SetUp();
	while (!WindowShouldClose())
	{
		Store();
		Draw();
		if (!gameOver)
		{
			Input();
			Logic();
		}
	}
	CloseWindow();
	return 0;
}
