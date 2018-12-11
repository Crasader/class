#ifndef LayerChatMiniGame_h__
#define LayerChatMiniGame_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerChatMiniGame : public Layer,
	public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate,
    public EditBoxDelegate,
	public PlayerCallBack
{
public:
	LayerChatMiniGame();
	~LayerChatMiniGame();
	CREATE_FUNC(LayerChatMiniGame);
	virtual bool init();

	void loadResByID(int gameid);
	void showChat();
	void hideChat();
protected:
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    virtual void editBoxReturn (EditBox *editBox);
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    virtual void editBoxEditingDidBegin (EditBox *editBox);
    virtual void editBoxEditingDidEnd (EditBox *editBox);

private:
	//add layer chat
	Layout* pnlChat;
	TextField* txtChat;
	TableView* tblChat;
    Layout* pnlContent;
	Button* btnSend;
    struct structChat{
        string Name;
        string Content;
        bool isadmin;
        bool isnotify;
        structChat(string _name, string _content, bool _isadmin,bool _isnotify) :
        Name(_name), Content(_content),isadmin(_isadmin),isnotify(_isnotify){};
    };
	vector<structChat> lstChat;
	Vec2 firstPos;
	Vec2 hidePos;

	void onButtonSend(Ref* pSender);
	float getHeightForCell(std::string _strMsg);
	void parseChat(std::string _message);
    void reloadHis();
	int typeGame = 0;
	//update 4/8
	void parseHistChat(std::string _strMsg);
};

#endif // LayerTCC_DicesHistory_h__
