//
//  _Chess_.h
//  iCasinov2
//
//  Created by Thanhhv on 6/5/14.
//
//

#ifndef __iCasinov2___Chess___
#define __iCasinov2___Chess___

#include <iostream>
#include "cocos2d.h"
#include "CommonChess.h"

USING_NS_CC;
using namespace std;

class Chess : public Sprite {
private:
    int id_name;
    int id_pos;
    int id_pos_init;
	int side; // DARK or LIGHT - side 1 đo - ở trên, side 2 đen - ở dưới
	int name; // PAWN, BISHOP, ...
	int rawName;//true name of chess
    bool isClicked;
	bool isUndoMove;

	Sprite* shadowSprite;
	MotionStreak* _streak;
	Sprite* justPoint;
	struct ChessRecord {
		int oldID;
		float uiPosX;
		float uiPosY;
	};

	void callbackAnim();
public:
    //Chess(int id_name, int id_pos);
	static Chess* create(int side, int name, int id_pos);
	static Chess* create(int side, int name, int id_pos, CHESS_TYPE type);
	bool init();
	Chess(int side, int name, int id_pos);
	Chess(int side, int name, int id_pos, CHESS_TYPE type);
    ~Chess();
    
    // setter, getter
    void setIDName(int id);
    int  getIDName();
    void setIDPos(int id);
    int  getIDPos();
	void setIDPosInit(int id);
    int  getIDPosInit();
	int  getName();
	void setName(int id);
	void setSide(int _side);
	int  getSide();
	int  getRawName();
	void setRawName(int id);
	CC_SYNTHESIZE(int, fkType, FakeType);
	CC_SYNTHESIZE(CHESS_TYPE, type, TypeChess);// typeChess la loai quan co: cờ đá, cờ thủy tinh,...
	CC_SYNTHESIZE(int, set_pos, PosConfig);
	CC_SYNTHESIZE(Vec2, pos_init, PosInit);
	CC_SYNTHESIZE(int, initName, InitName);
	CC_SYNTHESIZE(int, liveID, LiveID);//nhung quan co duoc load tu the co set duoc setID = 1, ko co = -1
    void setClick(bool isClick);

	std::vector<ChessRecord> listRecord;//luu lai nuoc di quan co
	void addRecord(int _idPos,float _uiPosX, float _uiPosY);//pos id
	void clearRecord();
	void undoMove();
	void hideShadow(bool isHide);
	void hideJustPoint(bool isHide);

	string getURL();
	/************************************************************************/
	/*type = 0 - normal, 1- cờ đá, 2 cờ gỗ, 3 cờ đồng, 4 cờ sắt, 5 cờ bạc
	6 cờ vàng, 7 cờ thủy tinh, 8 cờ lửa                      */
	/************************************************************************/

	string getUrlByType(const CHESS_TYPE& type = CHESS_TYPE::SIMPLE);

	bool isFolded();

	//change sprite image
	void setImageChess(CHESS_TYPE type);
	string getStr();
	string getMapChess();
	//animation chieu tuong
	void showKingCheck(bool isCheck, Vec2 destPos);
	void showIsChecked(bool isChecked);
	void addMoveSprite();
	void moveChess(Vec2 dest);
};

#endif /* defined(__iCasinov2___Chess___) */
