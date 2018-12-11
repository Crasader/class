//
//  MauBinhMauBinhCard.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/11/17.
//
//

#ifndef MauBinhMauBinhCard_hpp
#define MauBinhMauBinhCard_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class MauBinhCard : public Sprite
{
private:
    bool isTouched;
    bool isClicked;
    bool isPhom;
    bool isEat;
    
    Size sizeMauBinhCard;
    string url;
    int id;
    float scaleX;
    float scaleY;
    
    string value;
    int type;
    int chi;
    Vec2 orginPosition;
public:
    ~MauBinhCard();
    MauBinhCard(const char *fileName);
    void initialize();
    
    static MauBinhCard* createMauBinhCardWithFileName(const char *fileName);
    
    // call when reset game!
    void resetStatus();
    
    void setTouched(bool isTouched);
    bool getTouched();
    
    void setClicked(bool isClicked);
    bool getClicked();
    
    void setIsLink();
    void reset();
    
    
    void setSize(float wid, float hei);
    Size getSize();
    
    void setValue(string _value);
    string getValue();
    
    void setType(int _type);
    int getType();
    
    void setOrginPos(Vec2 _pos);
    Vec2 getOrginPos();
    
    void setURL(string url);
    const string& getURL() const;
    
    void SetID(const int& id);
    const int& GetID() const;
    
    void SetScaleMauBinhCard(const float& scaleX, const float& scaleY);
    Size GetScaleMauBinhCard();
    float GetScaleMauBinhCardX();
    float GetScaleMauBinhCardY();
    
    void setMauBinhCardAte();
    void setMauBinhCardCanEat();
    
    //
    void SetValuesXiTo();
    
    //Bật cờ check lên nếu là 1 giá trị của dãy values
    void SetIsValuesInList(const int& id_, unsigned int& index);
    
    //Set Id từ dãy giá trị values.
    void setScaleMauBinhCardOriginal();
};


#endif /* MauBinhMauBinhCard_hpp */
