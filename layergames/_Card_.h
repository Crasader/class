//
//  Card.h
//  iCasinoCard
//
//  Created by Thanhhv on 5/8/14.
//
//

#ifndef __iCasinoCard__Card__
#define __iCasinoCard__Card__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Card : public Sprite
{
private:
    bool isTouched;
    bool isClicked;
    bool isPhom;
    bool isEat;
    
    Size sizeCard;
    string url;
    int id;
    float scaleX;
    float scaleY;
    
public:
	~Card();
    Card(const char *fileName);
    void initialize();
    
    static Card* createCardWithFileName(const char *fileName);
    
    // call when reset game!
    void resetStatus();
    
    void setTouched(bool isTouched);
    bool getTouched();
    
    void setClicked(bool isClicked);
    bool getClicked();
    
    void setPhom(bool isPhom);
    bool getPhom();
    
    void setEat(bool isEat);
    bool getEat();
    //void setEatEffect();
    
    void setSize(float wid, float hei);
    Size getSize();
    
    void setURL(string url);
    const string& getURL() const;
    
    void SetID(const int& id);
    const int& GetID() const;
    
    void SetScaleCard(const float& scaleX, const float& scaleY);
    Size GetScaleCard();
    float GetScaleCardX();
    float GetScaleCardY();
    
    void setCardAte();
    void setCardCanEat();
    void setFoldCard();

	//
	void SetValuesXiTo();

	//Bật cờ check lên nếu là 1 giá trị của dãy values
	void SetIsValuesInList(const int& id_, unsigned int& index);

	//Set Id từ dãy giá trị values.
	void SetIdInList(const int& id_, unsigned int& index);
	CC_SYNTHESIZE(int, from, FromUser);//danh cho viec animation truoc an quan cua phom
	CC_SYNTHESIZE(int, _type, TypePos);//danh dau vi tri position khi danh xuong ban cua quan bai - danh cho game tien len
	CC_SYNTHESIZE(bool, _recom, Recommend);//danh dau day co phai quan duoc goi y de ha hay ko
	void showArrowEat();
	void removeArrowEat();
	void setScaleCardOriginal();
};

struct CardFunctor{
	void (Card::*func)(const int&, unsigned int&);
	int a; 
	unsigned int b;
	CardFunctor(void (Card::*f)(const int&, unsigned int&), const int& a, unsigned int& b):func(f),a(a),b(b){};
	void operator()(Card* i)
	{(i->*func)(a, b);}
};

#endif /* defined(__iCasinoCard__Card__) */
