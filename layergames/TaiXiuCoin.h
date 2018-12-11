#ifndef TAIXIUCOIN_H
#define TAIXIUCOIN_H

#include "TaiXiuMessageDefs.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;

struct CoinStruct{
    std::string imgnormal;
    std::string imgselect;
    int value;

    CoinStruct(const std::string& normal, const std::string& select, const int& val){
        imgnormal = normal;
        imgselect = select;
        value = val;
    }

    CoinStruct(){
        imgnormal = "";
        imgselect = "";
        value = 0;
    }
};

class TaiXiuCoin
{
protected:
    Button* pMenuItem;
    std::string pName;
    std::string pType;
    int pTag;

    std::string pImageNormal;
    std::string pImageSelect;

public:
    TaiXiuCoin(Button* item, const std::string& name, const std::string& type, const int& tag);
    ~TaiXiuCoin();

    std::string GetType() const;
    int GetTag() const;
    Vec2 GetPos() const;

    void SetSelected(bool select);

    Vec2 GetPosWorld();

    virtual CoinStruct GetValue() = 0;

};

class TaiXiuCoin1 : public TaiXiuCoin{
public:
    TaiXiuCoin1(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin100 : public TaiXiuCoin{
public:
    TaiXiuCoin100(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin500 : public TaiXiuCoin{
public:
    TaiXiuCoin500(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin1K : public TaiXiuCoin{
public:
    TaiXiuCoin1K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin2K : public TaiXiuCoin{
public:
	TaiXiuCoin2K(Button* item, const std::string& name, const std::string& type, const int& tag);
	CoinStruct GetValue();
};

class TaiXiuCoin5K : public TaiXiuCoin{
public:
    TaiXiuCoin5K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin10K : public TaiXiuCoin{
public:
    TaiXiuCoin10K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin50K : public TaiXiuCoin{
public:
    TaiXiuCoin50K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin100K : public TaiXiuCoin{
public:
    TaiXiuCoin100K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin500K : public TaiXiuCoin{
public:
    TaiXiuCoin500K(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin1M : public TaiXiuCoin{
public:
    TaiXiuCoin1M(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoin5M : public TaiXiuCoin{
public:
    TaiXiuCoin5M(Button* item, const std::string& name, const std::string& type, const int& tag);
    CoinStruct GetValue();
};

class TaiXiuCoinFactory{
public :
    static TaiXiuCoin* CreateTaiXiuCoin(Button* item, const std::string& name, const std::string& type, const int& tag);
};

#endif // TAIXIUCOIN_H
