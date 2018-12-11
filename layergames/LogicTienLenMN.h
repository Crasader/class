/*!
 * \class
 *
 * \brief
 *
 * \author Administrator
 * \date November 2015
 */
#ifndef LogicTienLenMN_h__
#define LogicTienLenMN_h__
#include "cocos2d.h"
#include "CardInfo.h"
USING_NS_CC;
using namespace std;

class LogicTienLenMN{
private:
	int previouscardtype;
	///danh cho tien len mien nam
	bool checkFirst(vector<CardInfo*> listc);//check lan dau de phan chia loai quan
	bool checkOne(vector < CardInfo*> listc);//check xem co phai 1 quan bai khong
	bool checkTwo(vector<CardInfo*> listc);// check 2 quan
	///danh cho tien len mien bac
	bool checkOneMB(vector<CardInfo*> listc);
	bool checkTwoMB(vector<CardInfo*> listc);
	bool checkThreeMB(vector<CardInfo*> listc);
	bool checkFourMB(vector<CardInfo*> listc);
	bool checkSequenceMB(vector<CardInfo*> listc);
	//danh cho sam
	bool checkFirstSam(vector<CardInfo*> listc);//check lan dau de phan chia loai quan
	bool checkOneSam(vector < CardInfo*> listc);//check xem co phai 1 quan bai khong
	bool checkTwoSam(vector<CardInfo*> listc);// check 2 quan

	CardInfo* getCard(string idCard);
	CardInfo* getCard(int idCard);
	CardInfo* getCardPhom(string idCard);

	bool checkPlayMN(vector<CardInfo*> listcardinfo);
	bool checkPlayMB(vector<CardInfo*> listcardinfo);
	bool checkPlaySam(vector<CardInfo*> liscardinfo);
	///recommend card
	vector<CardInfo*> checkRecommendMN(vector<CardInfo*> listcardinfo);//ham gợi ý tlmn
	bool checkRecommendMB(vector<CardInfo*> listcardinfo);
	bool checkRecommendSam(vector<CardInfo*> liscardinfo);

	vector<CardInfo*> getRecommendMN(vector<CardInfo*> listc, vector<CardInfo*> chooseCards);
	vector<CardInfo*> getRecommendMB(vector<CardInfo*> listc, vector<CardInfo*> chooseCards);
	vector<CardInfo*> getRecommendSam(vector<CardInfo*> listc, vector<CardInfo*> chooseCards);

	bool isConsecutive(vector<int> arr);//kiem tra 1 mang co phai la day lien tiep
	int getMin(vector<int> arr);
	int getMax(vector<int> arr);
	vector<CardInfo*> getRecommendFirst(vector<CardInfo*> listc, vector<CardInfo*> chooseCards);//ham lay danh sach cac quan bai goi y khi danh luot dau tien
	string convertVecToString(vector<int> arr);
public:
    vector<CardInfo*> priviousplaycard;

	LogicTienLenMN();
	bool checkPlay(string listc);
	void resetGame();
	void setPreviousCard(string listc);
	CC_SYNTHESIZE(int, _gameid, GameID);
	CC_SYNTHESIZE(string, _list, ListReOrder);
	///recommend card
	vector<CardInfo*> getRecommend(string listc, vector<int> chooseCards);//ham lay danh sach goi y, phai co input la 2 quan
	vector<CardInfo*> checkRecommendCard(string listcardinfo);//ham kiem tra xem co quan bai nao do duoc quan tren ban hay khong
	vector<int> splitStringtoInt(string &S, const char &str);

	vector<int> getListIDFromArray(vector<CardInfo*> _listCard);// ham chuyen mang cardinfo ra 1 mang id cac quan bai
	string getListCardFromArray(vector<CardInfo*> _listCard);// ham chuyen mang cardinfo ra 1 chuoi
};
#endif // LogicTienLenMN_h__


