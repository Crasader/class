#ifndef orderalgo_h__
#define orderalgo_h__

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

class FullCardInfo {
private:
	int index;
	int number;			// 
	int suit;			// chat: 1.CO, 2.RO, 3.TEP, 4.BICH	
	bool bTaken;

public:
	FullCardInfo();
	FullCardInfo(int idx, int num, int s);

	void setTaken(bool bVal);
	bool isTaken();

	int getIndex();

	int getNumber();

	int getSuit();

};

class CardOrder {
public:
	int index;
	bool bTaken;

	CardOrder();
};

class CardDeck {
private:
	FullCardInfo ** cardlst;
	static CardDeck * instance_;

public:
	CardDeck();

	~CardDeck();
	static CardDeck * instance();

	static void release();

	FullCardInfo * getCard(int idx);

	FullCardInfo ** getListFromIdxList(CardOrder *pCards, int count);
};


class Argument {
public:
	FullCardInfo ** lst;
	int count;

	Argument(FullCardInfo ** pLst, int c);
};

class CardOrderAlgo {
private:
	bool isMinimum(FullCardInfo **lst, int count);

	bool isMaximum(Argument * seq);

	bool resolve(vector<Argument> * lst, int * mark, Argument * seq);

	void work(vector<Argument> * tl, int * c1, int * d1,
		int * rets, int * retm, int upper, int sequence,
		int * outc, int * outs);

public:
	void order(CardOrder *pCards, int count);
};


#endif // orderalgo_h__