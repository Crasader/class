//
//  _LayerBet_.h
//  iCasino_v2
//
//  Created by DauA on 6/26/14.
//
//

#ifndef __iCasino_v2__LayerBet__
#define __iCasino_v2__LayerBet__

#include <iostream>
#include "cocos2d.h"
#include "FrameBet.h"
#include "PhomMessDef.h"

USING_NS_CC;
using namespace std;
class BetGame3Cay : public Layer
{
private:
	Layer *LayerResult;

public:
	BetGame3Cay();
	~BetGame3Cay();
	virtual bool init();
	CREATE_FUNC(BetGame3Cay);

	void VisibleAllFrameBet();
	void SetResult4AllUser(const int& pos, string& resuilt, string& score);
	string GetTricks(string& trick);

	FrameBet* GetFrameBetByPos(int pos);
	Layer* GetLayerResult();

	void resetAllBets(int money);
};
	
class Bien3Cay : public Layer{
private:
	LayerBien *bme, *buser1, *buser2, *buser3, *buser4, *buser5, *buser6;
	std::string lstUser;
public:
	Bien3Cay();
	~Bien3Cay();
	bool init();
	bool init(double _money);
	static Bien3Cay* create();
	static Bien3Cay* create(double _money);

	LayerBien* GetLayerBienByPos(int pos);
	void setListUser(std::string _lstUser);
	void displayBien3cay(bool _display);
	void resetAllBien();
};
#endif
