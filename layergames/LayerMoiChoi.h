//
//  LayerMoiChoi.h
//  iCasinov2
//
//  Created by AlexDao on 6/3/14.
//
//

#ifndef iCasinov2_LayerMoiChoi_h
#define iCasinov2_LayerMoiChoi_h

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

class LayerMoiChoi
: public cocos2d::Layer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
, public cocos2d::extension::TableViewDataSource
, public cocos2d::extension::TableViewDelegate
, public PlayerCallBack

{
private:
    Node* nodeTable;
    TableView* tblTable;
    Label* lblTitle;
    struct StructFriendInfo{
		int uid;
        string aI;
		string aN;
		string amf;
    };
	vector<StructFriendInfo> lstFriendInfos;
	cocos2d::EventListener* _touchListener;
    //    enum {
    //        tag_cellTime,
    //        tag_cellAmount,
    //        tag_cellDetails
    //    };
public:
    LayerMoiChoi();
	~LayerMoiChoi();

//	virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;
    
    void loadAllDatas();
    
    CREATE_FUNC(LayerMoiChoi);
    
	// CCBSelectorResolver interfaces
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
	virtual void onNodeLoaded(cocos2d::Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
	virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName){
		return NULL;
	};
    
    void onButtonClose(Ref* pSender);
	//TABLE VIEW
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    //Server
   virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
};

class LayerMoiChoiLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LayerMoiChoiLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LayerMoiChoi);
};
#endif
