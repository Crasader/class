
#ifndef demo_LayerGameChan_KetQua_h
#define demo_LayerGameChan_KetQua_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "LayerChatInGame.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;

class LayerGameChan_KetQua : public Layer
{
private:
    Sprite *TableContainer;
    LayerMocChatInGame* layerChat;

	Ref* m_callback;
	SEL_CallFuncO m_callbackListener;
	int Counter;

    std::string Result;

public:
    LayerGameChan_KetQua(const std::string& result, LayerMocChatInGame* layerChat);
    virtual ~LayerGameChan_KetQua();

    static LayerGameChan_KetQua* createInstance(const std::string& result, LayerMocChatInGame* layerChat);

	void OnButtonClick(Ref *pSender, ui::Widget::TouchEventType pType);
	void SetCallBack_Close(Ref* target, SEL_CallFuncO callfun);
	void CaptureScreenAndShare();
    void AutoClose(float dt);
    void SetLayerChat(LayerMocChatInGame* layerChat);

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

protected:
    virtual bool init();
    virtual void onEnter() override;

private:
    string GetCuocSac(string& list_cuoc, const bool& type);
    Button* CreateButton(const char* _title, const Vec2& _position, const int& _tag);
    int LengthUtf8String(const char* s);
    string& FormatStringUtf8(string& str, const int& WIDTH = 50);
    void DisplayResult(string& resuilt);
    void SendMsg(std::string& msg);
};

#endif
