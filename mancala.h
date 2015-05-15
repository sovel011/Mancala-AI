/*
 * mancala.h
 *
 *  Created on: Mar 22, 2015
 *      Author: lukesovell
 */

#ifndef MANCALA_H_
#define MANCALA_H_

class Node
{
public:
	int stones;
};

class Move
{
public:
	int index;
	float value;
};

class House : public Node
{
public:
	House* opposite;
	House* next;
	bool isLast;
	int id;
};

class Store : public Node
{

};

class Player
{
public:
	House* housePtrs[6];
	Store* store;
	int id;
	void sow(int, bool&, int);
};


class Board
{
public:
	bool gameOver;
	House houses[12];
	Store stores[2];
	Player players[2];

	void play();
	void play2();
	void play3();

	float heuristic1(int);
	float heuristic2(int);
	float heuristic3(int);
	float heuristic4(int);
	float heuristic5(int);
	float randomHeuristic(int);
	float playerHeuristic(int);
	float opponentHeuristic(int);
	Move bestMove(int, bool);


	void print();
	void checkGameOver(int player);
	void checkGameOver2(int player);
	Board();
	Board(const Board&);
};




#endif /* MANCALA_H_ */
