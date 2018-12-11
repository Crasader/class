//
//  LogicInChess.h
//  iCasinov2
//
//  Created by Thanhhv on 6/5/14.
//
//

#ifndef __iCasinov2__LogicInChess__
#define __iCasinov2__LogicInChess__

#include <iostream>
//#include <vector.h>
#include "mUtils.h"
using namespace std;

class ChessLogic {
public:
	ChessLogic();
	ChessLogic(bool isCoUp_);
	~ChessLogic();

	void setChessIsFold(bool isCoUp_){ isCoUp = isCoUp_; }

	void printTable();
	void loadNewGame();
	string toTableString();
	string getArrayLostPieces(int side); // side=1 | side=2
	bool checkCurrTurnMoveValid(int c, int pos);
	//     bool colorPos(int pos);
	//     bool piecePos(int pos);
	void Move(int from1, int dest1);
	int countPiece(int side);
	bool isLose(int side, int reason);
	bool isLostKing(int side);
	string getArrayKing(int side);
	
	vector<int> getAllMovesFromPos(int pos, bool isFolded = false);//get all pos chess can move
	vector<int> getAllUnSafeMoveFromPos(int pos, bool isFolded = false);//get all unsafe pos chess can move

	bool IsInCheck(int xside2);
	bool IsKingSafe(int xside2);
	bool Attack(int pos, int side);
	int getKingCheckPiece(int xside2);//get index chess check king
	bool moveSave(int from, int dest);
	bool IsKingFace(int pos);
	bool IsFaceToFace(int xside2);
	void setChessIDBySide(int idChess, int side, int pos); // add a chess onto table
	void setChessIDBySide(int idChess, int pos); // add a chess onto table
	void undoMove();
	bool isPosHasPiece(int pos);
	//only for co the
	void ClearData();
	//update 10/7
	vector<int> getKingCheckPieceArray(int xside2);//get index chess check king
private:
	bool isCoUp;
	string printChessName(int name);
};

#endif /* defined(__iCasinov2__LogicInChess__) */
