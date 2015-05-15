/*
 * main.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: lukesovell
 */
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include "mancala.h"

#define MAX_DEPTH 7

using namespace std;

Move Minimax(int, Board, int);
Move alphaBeta(int,Board, float, float, int);
Move min(float* choices);
Move max(float* choices);

int main(int argc, char** argv)
{
	srand(time(NULL));
	int mode;
	printf("Choose game mode!\n1: Human vs Human\n2: Human vs Computer\n3:Computer vs Computer\n\n");
	cin >> mode;
	Board board = Board();
	if (mode == 1)
		board.play();
	else if (mode == 2)
		board.play2();
	else
		board.play3();
	return 0;
}

Board::Board()
{
	gameOver = false;
	players[0].store = &stores[0];
	players[0].id = 1;
	players[1].store = &stores[1];
	players[1].id = 2;
	stores[0].stones = 0;
	stores[1].stones = 0;
	for (int i = 0; i < 12; i++)
	{
		houses[i].stones = 4;

		if (i == 5 || i == 11)
			houses[i].isLast = true;
		else
			houses[i].isLast = false;
		if (i < 6)
		{
			houses[i].id = 1;
			players[0].housePtrs[i] = &houses[i];
			houses[i].opposite = &houses[11-i];
		}
		else
		{
			houses[i].id = 2;
			players[1].housePtrs[i-6] = &houses[i];
			houses[i].opposite = &houses[11-i];
		}

		if (i < 11)
			houses[i].next = &houses[i+1];
		else
			houses[i].next = &houses[0];
	}
}

Board::Board(const Board& old)
{
	gameOver = false;
	players[0].store = &stores[0];
	players[0].id = 1;
	players[1].store = &stores[1];
	players[1].id = 2;
	stores[0].stones = old.stores[0].stones;
	stores[1].stones = old.stores[1].stones;
	for (int i = 0; i < 12; i++)
	{
		houses[i].stones = old.houses[i].stones;

		if (i == 5 || i == 11)
			houses[i].isLast = true;
		else
			houses[i].isLast = false;
		if (i < 6)
		{
			houses[i].id = 1;
			players[0].housePtrs[i] = &houses[i];
			houses[i].opposite = &houses[11-i];
		}
		else
		{
			houses[i].id = 2;
			players[1].housePtrs[i-6] = &houses[i];
			houses[i].opposite = &houses[11-i];
		}

		if (i < 11)
			houses[i].next = &houses[i+1];
		else
			houses[i].next = &houses[0];
	}
}

void Board::print()
{
	printf("\n\t ");
	for (int i=0; i<=70;i++)
		printf("-");
	printf("\n\t|\t\t");
	for (int i=11;i>5;i--)
		printf("%.2d\t", houses[i].stones);
	printf("\t|\n\t|\t");
	printf("%.2d\t\t\t\t\t\t\t%.2d\t|\n", stores[1].stones, stores[0].stones);
	printf("\t|\t\t");
	for(int i=0;i<6;i++)
		printf("%.2d\t", houses[i].stones);
	printf("\t|\n");
	printf("\t ");
		for (int i=0; i<=70;i++)
			printf("-");
	printf("\n\n");
}

void Board::play()
{
	print();
	int counter = 0;
	bool goAgain = false;
	while (!gameOver)
	{
		int index;
		int playerIndex = counter%2;
		cout << "Player " << playerIndex+1 << " Enter index of house\n";
		cin >> index;
		players[playerIndex].sow(playerIndex, goAgain, index);
		print();
		checkGameOver(playerIndex);
		if (!goAgain)
			counter++;
		else goAgain = false;
	}
	if (stores[0].stones > stores[1].stones)
		printf("\nPLAYER 1 WINS!\n\n");
	else
		printf("\nPLAYER 2 WINS!\n\n");
}

void Board::play2()
{
	int counter = 1;
	float totalTime = 0;
	int turns = 0;
	char goFirst = 'y';
	printf("Go first? (y/n)");
	cin >> goFirst;
	if (goFirst == 'y' || goFirst == 'Y')
	{
		counter = 0;
	}
	print();
	bool goAgain = false;
	while (!gameOver)
	{
		int index;
		int playerIndex = counter%2;
		if (playerIndex == 0)
		{
			cout << "Player 1 Enter index of house\n";
			cin >> index;
			players[playerIndex].sow(playerIndex, goAgain, index);
		}
		else
		{
			printf("Player 2\n");
			struct timeval  tv1, tv2;
			gettimeofday(&tv1, NULL);
			//index = bestMove(playerIndex, goAgain).index;
			index = Minimax(0, *this, playerIndex).index;
			//index = alphaBeta(0, *this, -1000000.0, 1000000.0, playerIndex).index;
			gettimeofday(&tv2, NULL);
			printf("Total execution time: %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
			         (double) (tv2.tv_sec - tv1.tv_sec));
			totalTime += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
			         (double) (tv2.tv_sec - tv1.tv_sec);
			turns +=1;
			if (index == -1 || houses[5+index].stones == 0)
			{
				printf("RANDOM MOVE?\n");
					index = (rand()%6)+1;
			}
			players[playerIndex].sow(playerIndex, goAgain, index);
		}
		print();
		checkGameOver(playerIndex);
		if (!goAgain)
			counter++;
		else goAgain = false;
	}
	printf("Avg. Time = %f\n", (float) totalTime/turns);
}

void Board::play3()
{
	print();
	int counter = 0;
	bool goAgain = false;
	while (!gameOver)
	{
		int index;
		int playerIndex = counter%2;
		printf("Player %d\n", playerIndex);
		//index = bestMove(playerIndex, goAgain).index;
		index = Minimax(0, *this, playerIndex).index;
		if (index == -1) // || players[playerIndex].housePtrs[index]->stones == 0)
		{
			printf("RANDOM MOVE?\n");
				index = (rand()%6)+1;
		}
		players[playerIndex].sow(playerIndex, goAgain, index);
		print();
		checkGameOver(playerIndex);
		if (!goAgain)
			counter++;
		else goAgain = false;
	}
	if (stores[0].stones > stores[1].stones)
		printf("\nPLAYER 1 WINS!\n\n");
	else
		printf("\nPLAYER 2 WINS!\n\n");
}

void Board::checkGameOver(int player)
{
	gameOver = true;
	if (player == 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (houses[i].stones > 0)
			{
				gameOver = false;
				return;
			}
		}
	}
	else
	{
		for (int i = 6; i < 12; i++)
		{
			if (houses[i].stones > 0)
			{
				gameOver = false;
				return;
			}
		}
	}
	if (gameOver)
	{
		if (player == 0)
		{
			for (int i = 6; i < 12; i++)
			{
				if (houses[i].stones > 0)
				{
					int stones = houses[i].stones;
					houses[i].stones = 0;
					players[1].store->stones += stones;
				}
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				if (houses[i].stones > 0)
				{
					int stones = houses[i].stones;
					houses[i].stones = 0;
					players[0].store->stones += stones;
				}
			}
		}
		printf("\n GAME OVER \n");
		print();
		if (stores[0].stones > stores[1].stones)
			printf("\nPLAYER 1 WINS!\n\n");
		else if (stores[0].stones == stores[1].stones)
			printf("\nDRAW GAME!\n\n");
		else
			printf("\nPLAYER 2 WINS!\n\n");
	}
}

void Board::checkGameOver2(int player)
{
	gameOver = true;
	if (player == 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (houses[i].stones > 0)
			{
				gameOver = false;
				return;
			}
		}
	}
	else
	{
		for (int i = 6; i < 12; i++)
		{
			if (houses[i].stones > 0)
			{
				gameOver = false;
				return;
			}
		}
	}
	if (gameOver)
	{
		if (player == 0)
		{
			for (int i = 6; i < 12; i++)
			{
				if (houses[i].stones > 0)
				{
					int stones = houses[i].stones;
					houses[i].stones = 0;
					players[1].store->stones += stones;
				}
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				if (houses[i].stones > 0)
				{
					int stones = houses[i].stones;
					houses[i].stones = 0;
					players[0].store->stones += stones;
				}
			}
		}
	}
}

void Player::sow(int playerIndex, bool& goAgain, int index)
{
	goAgain = false;
	if (index > 6 || index < 1 || housePtrs[index-1]->stones == 0)
	{
		printf("Invalid option\n");
		printf("Index: %d\n", index);
		goAgain = true;
		return;
	}
	House* house = housePtrs[index-1];
	int stones = house->stones;
	house->stones = 0;
	for (int i = 0; i < stones; i++)
	{
		if (house->isLast && house == housePtrs[5])
		{
			store->stones++;
			i++;
			if (i >= stones)
			{
				goAgain = true;
				break;
			}
		}
		house = house->next;
		if (i >= stones-1 && house->stones == 0 && house->id == id)
		{
			int capture = 1 + house->opposite->stones;
			if (capture > 1)
			{
				house->opposite->stones = 0;
				house->stones -= 1;
				store->stones += capture;
			}
		}
		house->stones++;
	}
}


Move Board::bestMove(int playerIndex, bool goAgain)
{
	Move move;
	//printf("In best move\n");
	move.index = -1;
	move.value = -100;
	const Board copy = *this;
	for (int i=0; i<6; i++)
	{
		Board newBoard = copy;
		//newBoard.print();
		if (players[playerIndex].housePtrs[i]->stones == 0) continue;
		newBoard.players[playerIndex].sow(playerIndex, goAgain, i+1);
		int value = newBoard.heuristic1(playerIndex);
		if (goAgain) value = 2*value + newBoard.bestMove(playerIndex, goAgain).value;
		//newBoard.print();
		//printf("%d : %d\n", i, value);
		if (value >= move.value)
		{
			move.value = value;
			move.index = i+1;
		}
	}
	//printf("Best Move is: %d\n", move.index);
	return move;
}

float Board::heuristic1(int playerIndex)
{
	//Picks move which will maximize stones in the player's Mancala
	int opponentIndex = (playerIndex+1)%2;
	return (stores[playerIndex].stones) - (stores[opponentIndex].stones);
}

//heuristic which is how close you are to winning
float Board::heuristic2(int playerIndex)
{
	return 25 - (25 - stores[playerIndex].stones);
}

//linear combination of h1 and h2
float Board::heuristic3(int playerIndex)
{
	return 1.1*this->heuristic1(playerIndex) + this->heuristic2(playerIndex);
}

float Board::heuristic4(int playerIndex)
{
	return players[playerIndex].housePtrs[0]->stones +
			players[playerIndex].housePtrs[1]->stones +
			players[playerIndex].housePtrs[2]->stones +
			players[playerIndex].housePtrs[3]->stones +
			players[playerIndex].housePtrs[4]->stones +
			players[playerIndex].housePtrs[5]->stones +
			players[playerIndex].store->stones;
}

float Board::randomHeuristic(int playerIndex)
{
	return rand()%48;
}

float Board::heuristic5(int playerIndex)
{
	int stoneDifference;
	int opponentIndex = (playerIndex+1)%2;
	stoneDifference = players[playerIndex].housePtrs[0]->stones +
			players[playerIndex].housePtrs[1]->stones +
			players[playerIndex].housePtrs[2]->stones +
			players[playerIndex].housePtrs[3]->stones +
			players[playerIndex].housePtrs[4]->stones +
			players[playerIndex].housePtrs[5]->stones;

	stoneDifference -=  players[opponentIndex].housePtrs[0]->stones +
			players[opponentIndex].housePtrs[1]->stones +
			players[opponentIndex].housePtrs[2]->stones +
			players[opponentIndex].housePtrs[3]->stones +
			players[opponentIndex].housePtrs[4]->stones +
			players[opponentIndex].housePtrs[5]->stones;

	return heuristic1(playerIndex) + stoneDifference;
}

float Board::playerHeuristic(int playerIndex)
{
	return this->heuristic1(playerIndex);
}

float Board::opponentHeuristic(int playerIndex)
{
	return this->heuristic1(playerIndex);
}

Move Minimax(int depth, Board board, int playerIndex)
{
//	int max_depth;
//	if (playerIndex == 0)
//		max_depth = 2;
//	else
//		max_depth = 8;
	Move move;
	int depthCopy = depth;
	if (depth == MAX_DEPTH) //MAX_DEPTH
	{
		if (playerIndex == 0)
			move.value = board.playerHeuristic(playerIndex);
		else
					move.value = board.opponentHeuristic(playerIndex);
		return move;
	}
	else if (depth % 2 == 1)//min
	{
		float choices[] = {100,100,100,100,100,100};
		for (int i = 0; i < 6; i++)
		{
			depthCopy = depth;
			Board newBoard = board;
			int opponentIndex = (playerIndex+1)%2;
			bool goAgain = false;
			if (newBoard.players[opponentIndex].housePtrs[i]->stones == 0) continue;
			newBoard.players[opponentIndex].sow(opponentIndex, goAgain, i+1);
			newBoard.checkGameOver2(opponentIndex);
			if (newBoard.gameOver)
			{
				choices[i] = newBoard.opponentHeuristic(opponentIndex);
				continue;
			}
			if (!goAgain)
				choices[i] = Minimax(++depthCopy, newBoard, playerIndex).value;
			else
				choices[i] = Minimax(depthCopy, newBoard, playerIndex).value;
		}
		move = min(choices);
	}
	else //max
	{
		float choices[] = {-100,-100,-100,-100,-100,-100};
		for (int i = 0; i < 6; i++)
		{
			depthCopy = depth;
			Board newBoard = board;
			bool goAgain = false;
			if (newBoard.players[playerIndex].housePtrs[i]->stones == 0) continue;
			newBoard.players[playerIndex].sow(playerIndex, goAgain, i+1);
			newBoard.checkGameOver2(playerIndex);
			if (newBoard.gameOver)
			{
				choices[i] = newBoard.playerHeuristic(playerIndex);
				continue;
			}
			if (!goAgain)
			{
				choices[i] = Minimax(++depthCopy, newBoard, playerIndex).value;
			}
			else
			{
				choices[i] = Minimax(depthCopy, newBoard, playerIndex).value;
			}
		}
		move = max(choices);
	}
	return move;
}

Move alphaBeta(int depth, Board board, float alpha, float beta, int playerIndex)
{
	int max_depth;
//	if (playerIndex == 0)
//		max_depth = 8;
//	else
//		max_depth = 2;
	Move move;
	int depthCopy = depth;
	if (depth == MAX_DEPTH) //MAX_DEPTH
	{
		if (playerIndex == 0)
			move.value = board.playerHeuristic(playerIndex);
		else
					move.value = board.opponentHeuristic(playerIndex);
		return move;
	}
	else if (depth % 2 == 1)//min
	{
		float choices[] = {100,100,100,100,100,100};
		for (int i = 0; i < 6; i++)
		{
			depthCopy = depth;
			Board newBoard = board;
			int opponentIndex = (playerIndex+1)%2;
			bool goAgain = false;
			if (newBoard.players[opponentIndex].housePtrs[i]->stones == 0) continue;
			newBoard.players[opponentIndex].sow(opponentIndex, goAgain, i+1);
			newBoard.checkGameOver2(opponentIndex);
			if (newBoard.gameOver)
			{
				choices[i] = newBoard.opponentHeuristic(opponentIndex);
				continue;
			}
			if (!goAgain)
				choices[i] = alphaBeta(++depthCopy, newBoard, alpha, beta, playerIndex).value;
			else
				choices[i] = alphaBeta(depthCopy, newBoard, alpha, beta, playerIndex).value;
			beta = fmin(beta, choices[i]);
			if (beta <= alpha)
					break;
		}
		move = min(choices);
	}
	else //max
	{
		float choices[] = {-100,-100,-100,-100,-100,-100};
		for (int i = 0; i < 6; i++)
		{
			depthCopy = depth;
			Board newBoard = board;
			bool goAgain = false;
			if (newBoard.players[playerIndex].housePtrs[i]->stones == 0) continue;
			newBoard.players[playerIndex].sow(playerIndex, goAgain, i+1);
			newBoard.checkGameOver2(playerIndex);
			if (newBoard.gameOver)
			{
				choices[i] = newBoard.playerHeuristic(playerIndex);
				continue;
			}
			if (!goAgain)
			{
				choices[i] = alphaBeta(++depthCopy, newBoard, alpha, beta, playerIndex).value;
			}
			else
			{
				choices[i] = alphaBeta(depthCopy, newBoard, alpha, beta, playerIndex).value;
			}
			alpha = fmax(alpha, choices[i]);
			if (beta <= alpha)
					break;
		}
		move = max(choices);
	}
	return move;
}

Move min(float* choices)
{
	//printf("Min\n");
	Move min;
	min.value = 10000;
	for (int i = 0; i < 6; i++)
	{
		//printf("choice %d: %f\n", i, choices[i]);
		if (min.value >= choices[i])
		{
			min.value = choices[i];
			min.index = i+1;
		}
	}
	//printf("\n");
	return min;
}

Move max(float* choices)
{
	//printf("MAx\n");
	Move max;
	max.value = -10000;
	for (int i = 0; i < 6; i++)
	{
		//printf("choice %d: %f\n", i, choices[i]);
		if (max.value <= choices[i])
		{
			max.value = choices[i];
			max.index = i+1;
		}
	}
	//printf("\n");
	return max;
}

/*Board layout:
 *
 *	 -----------------------------------
 * 	|		4	4	4	4	4	4		|
 * 	|	0							0	|
 * 	|		4	4	4	4	4	4		|
 * 	 -----------------------------------
 *
 */









