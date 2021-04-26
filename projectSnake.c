/*********************************************************
				Nadav Porat 
**********************************************************/
/*********************************************************
This project is the game snake.
The bord is 75 cols on 25 rows
the playr play with the snake inside the bord.
evre 5 steeps the snake grow and move faster.
if the playr tech the limit bord or tach himself he lose.
**********************************************************/

//---------------INCLUDE---------------
#define _CRT_SESURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
//---------------DEFINE---------------
#define ROWS 25
#define COLS 75
#define Right 'd'
#define Left 'a'
#define  UP 'w'
#define Down 's'
#define BodySnake '@'
//---------------STRUCTURES---------------

typedef struct node {
	int x;
	int y;
	struct node* next;
} NODE;

//---------------FUNCTIONS DECLARATION---------------

// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulation).


//add a node to the head of the snake 
void addToSnake(NODE** tail, int* newX, int* newY);

// Control the movement of the snake. the function has a switch and 4 cases, every case is a direction and
//each of them has a yDirection and a xDirection depends (right left up or down).
//Every 5 paces the function ask from addToSnake function to increase the size of the snake by one
//and every 5 paces the speed of the snake is increasing with the funcrion sleep and "delay" as a speed counter.
void movement(NODE** tail, NODE** head);

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the position.
//     y: the column of the position.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);

// free the list form mallic
void freeList(NODE** head);

//update the snake NODES every turn to the next NODE every pace.
void updateSnake(NODE** head, int* xDirection, int* yDirection);

//print  Game over if a player collide in a wall or himself. 
void printGameOver();

//check if the snake crashed (collide himself).
//the function check if the head+the x and y direction of the snake is the same to any NODE of the snake
bool checkCrash(NODE** tail, NODE** head, int* xDirection, int* yDirection);

// Prints the borders of the game.
void DrawBoard();

//---------------MAIN---------------
void main()
 {
	init(); // clear board
	DrawBoard(); // print the board
	
    // DEFINE THE SNAKE
	NODE* head = NULL;
	head = (NODE*)malloc(sizeof(NODE));
	head->x = ROWS / 2;
	head->y = COLS / 2;
	head->next = NULL;
	NODE* tail = head;

	movement(&tail, &head); // run the move of snake
    gotoxy(26,0);
	printGameOver(); 
	freeList(&head); //free the list head
}

// Turn escape codes ON.
void init(void)
{
	system("cls");
}

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs * CLOCKS_PER_SEC)
		;
}

//prints the border
void DrawBoard()
{
	int y, z;

	for (y = 1; y <= COLS; y++)
	{
		gotoxy(1, y);
		printf("#");
	}
	for (z = 1; z <= ROWS; z++)
	{
		gotoxy(z, 1);
		printf("#");
		gotoxy(z, COLS);
		printf("#");
		printf("\n");
	}
	for (y = 1; y <= COLS; y++)
	{
		gotoxy(ROWS, y);
		printf("#");
	}
}
void movement(NODE** tail, NODE** head)
{
	float delay = 1;
	bool contGame = true;
	int yDirection = 0;
	int xDirection = 0;
	char direction = Right;
	int count = 1;

    
	while (contGame)
	{
		while (_kbhit())// keyboard hit
		{
			direction = _getch();
		}
		switch (direction)
		{
		case Right: // press 'd'.
			yDirection = 1;
			xDirection = 0;
			if (((*head)->y != COLS - 1) && (checkCrash(tail, head, xDirection, yDirection)))
			{
				count++;
				sleep(delay);
			}
			else contGame = false;
			break;

		case Left: // press 'a'.
			yDirection = -1;
			xDirection = 0;
			if (((*head)->y != 2) && (checkCrash(tail, head, xDirection, yDirection)))
			{
				count++;
				sleep(delay);
			}
			else contGame = false;
			break;

		case Down: // press 's'.
			yDirection = 0;
			xDirection = 1;
			if (((*head)->x != ROWS - 1) && (checkCrash(tail, head, xDirection, yDirection)))
			{
				count++;
				sleep(delay);
			}
			else contGame = false;
			break;
		

		case UP: // press 'w'.
			yDirection = 0;
			xDirection = -1;
			if (((*head)->x != 2) && (checkCrash(tail, head, xDirection, yDirection)))
			{
				count++;
				sleep(delay);
			}
			else contGame = false;
			break;
			
		} //  end Switch
			
		if ((count) % 6 != 0)
		{

			gotoxy((*tail)->x, (*tail)->y);
			printf(" ");
			gotoxy(26, 0);
			updateSnake(head, xDirection, yDirection);
			gotoxy((*head)->x, (*head)->y);
			printf("%c",BodySnake);
			gotoxy(26, 0);
		}
		else
		{
			if (count != 0)
				addToSnake(head, xDirection, yDirection);
			gotoxy((*head)->x, (*head)->y);
			printf("%c",BodySnake);
			gotoxy(26, 0);
		}
		if (delay > 0.1) // Increase the snake speed. (up to 0.1 millisecond).
			delay -= 0.03;
	}
}

//add a node to the head of the snake 
void addToSnake(NODE** head, int newX, int newY)
{
	NODE* temp;
	temp = *head;
	*head = (NODE*)malloc(sizeof(NODE)); // Dynamic size to *head 
	if (*head != NULL)
	{
		(*head)->x = (((temp)->x) + newX);
		(*head)->y = (((temp)->y) + newY);
		(*head)->next = temp;
	}
}


// free the list form mallic
void freeList(NODE** head)
{
	NODE* temp;
	while (*head != NULL)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}

////update the snake NODES every turn to the next NODE every pace.
void updateSnake(NODE** head, int xDirection, int yDirection)
{
	int xTemp = (*head)->x;
	int yTemp = (*head)->y;
	int xNext;
	int yNext;
	NODE* tempHead = (*head);

	while (tempHead->next != NULL)
	{
		xNext = tempHead->next->x;
		yNext = tempHead->next->y;
		tempHead->next->x = xTemp;
		tempHead->next->y = yTemp;
		xTemp = xNext;
		yTemp = yNext;
		tempHead = tempHead->next; // move the tempHead to next adrss.
	}

	(*head)->x = (*head)->x + xDirection;   // move the head to correct x
	(*head)->y = (*head)->y + yDirection;  // move the head to correct y
}

//check if the snake crashed (collide himself).
//the function check if the head+the x and y direction of the snake is the same to any NODE of the snake
bool checkCrash(NODE** tail, NODE** head, int xDirection, int yDirection)
{
	NODE* currentNode = *head;
	while (currentNode != NULL)
	{
		if ((((*head)->x) + xDirection == currentNode->x) && (((*head)->y) + yDirection == currentNode->y))
			return false;
		else
			currentNode = currentNode->next;
	}
	return true;
}

//print  Game over if a player collide in a wall or himself. 
void printGameOver()
{
	init();
	DrawBoard();
	gotoxy(ROWS / 2 + 1, COLS / 2 - 2);
	printf("GAME OVER");
	gotoxy(27, 0);
}

