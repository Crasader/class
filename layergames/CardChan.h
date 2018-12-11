//
//  CardChan.h
//  AppDemo
//
//  Created by DauA on 5/13/14.
//
//

#ifndef __AppDemo__CardChan__
#define __AppDemo__CardChan__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

struct ExecuteInfo
{
	float beginX;
	float beginY;
	float startPos;
	float cornor;
	float width;
	float height;
	float duration_;
};

class FindCard
{
public:
	FindCard(const bool& flag, const int& num, const int& sui)
		:flag(flag),
		 number(num),
         suite(sui){}
    ~FindCard(){log("delete info search");}
	bool flag;
	int number;
	int suite;
};

class CardChan : public ImageView
{
private:
    int IdCard;
	bool Flag;

	//for chan
	int NumberCard;
    int SuiteCard;
	int SttCard;
    bool IsAnimating;

    //
    bool Used;

    bool IsDraw;

	ImageView* Border;
	Vec2 CurrentPosition;

    int servId;

public:
	CardChan();
	~CardChan();
    CREATE_FUNC(CardChan)

    static CardChan* createInstance(std::string path, TextureResType texType = TextureResType::PLIST);
    bool init(std::string path, TextureResType texType = TextureResType::PLIST);
	bool init();
	const int& GetID() const;
	const bool& GetFlag();

    bool InUsed();
    void SetUsed(bool use);

	void SetID(const int& n);
	void SetFlag(const bool& b);
	void SetSizeCard(const float& toWidth, const float& toHeight);
	//for chan
	void SetNumber(const int& num);
    void SetSuite(const int& sui);

	const int& GetNumber() const;
    const int& GetSuite() const;

	//
	void SetSTT(const int& stt_);
	const int& GetSTT();

	void SetVisibleBorder(bool display = false);

	// Functor
	void DisplayCardInListNoc(const ExecuteInfo& info);
	void RotateCardChan(const ExecuteInfo& info);
	void MoveCardCuaChi(const float& y);
	void MoveCardWinner(const Vec2& point);
	bool FindCardIsClick(const FindCard* info);
	bool FindCardIfNotFoundClick(const FindCard* info);
	void ResetCard();
	void CardIsClick(const CardChan* pCards);
    void UnclockAnimating();

    //
    void ResetPosition();
    void SetCurrentPos(const Vec2& pos);

    void SetIsDraw(bool draw);
    bool GetIsDraw();

    void SetServId(const int& id);
    const int& GetServId() const;
};


#endif /* defined(__AppDemo__CardChan__) */
