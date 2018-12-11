

#ifndef _LayerGameChan_XuongU2_h
#define _LayerGameChan_XuongU2_h

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;

class ButtonXuong;
class LayerGameChan_XuongU
    : public Layer
	, public PlayerCallBack
{
private:
	Label* LblTime;
    Label* LblTitle;
	vector<int> arrIdCuoc;
    vector<ButtonXuong*> XuongArray;
	std::vector<ButtonXuong*> arrListCuoc;

	int CountTimer;
    float _scaleText;
    float _addY;

public:
    LayerGameChan_XuongU();
    virtual ~LayerGameChan_XuongU();

    bool init();
    CREATE_FUNC(LayerGameChan_XuongU)

	void OnButtonXuong(Ref* pSender);
    void OnButtonXuongUClick(Ref *sender, ui::Widget::TouchEventType pType);

	//
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void ItemXuong_Click(Ref *sender, Widget::TouchEventType type);
	string GetValuesMultiCuoc(const int& idx, const int& count);
	int GetIdMultiCuoc(const int& idx, const int& count);
	void CountDownTimer(float dt);

    bool CheckHoaNhaCa(const std::vector<std::string>& arrCuoc);
};

class ButtonXuong : public Button
{
private:
	int	Idx;
	bool IsClicked;
	bool IsMulti;
	int	CounterClick;
	int MaxValues;

	Label* TitleText;
	std::string StrTitleText;

    vector<int> CuocDoi;
    vector<int>Value;
    ui::Scale9Sprite* Blocks;

public:
    //max, value, cuocdoi
    ButtonXuong(const int& max, vector<int> values, vector<int> cuocdois);
	~ButtonXuong();

    static ButtonXuong* createButton(const int& max, vector<int> values, vector<int> cuocdois);
    virtual bool init();

	void SetID(const int& idx);
	const int& GetID() const;

	void SetClicked(const bool& isClicked);
	const bool& GetClicked() const;

	void SetTitleText(std::string& title);

	void SetMaxValues(const int& num);
	void SetIsMulti(const bool& isMul);

	const int& GetMaxValues() const;
	const bool& GetIsMultiValues() const;

	string& GetStringTitle();
	void SetStringTitle(std::string& text);

	int& GetCounter();
    void SetCounter(const int& count);

    void ShowBackground(bool state);
    bool DetectectCuocDoi(const int& idx);
    void PrintCuocDoi();
    void SetCuocDoi(vector<int> &P);
};

#endif
