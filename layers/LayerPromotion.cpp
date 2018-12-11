#include "LayerPromotion.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SceneManager.h"
#include "GameServer.h"
#include "../layergames/_Chat_.h"
#include "../mUtils.h"
#include "layergames/ChanUtils.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPromotion::LayerPromotion() :
mListViewPromoItems(NULL)
{
}

LayerPromotion::~LayerPromotion()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPromotion::init()
{
	if (!Layer::init())
	{
		return false;
	}
	GameServer::getSingleton().addListeners(this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupPromotion.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	//add event listenner
	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose != NULL){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPromotion::onButtonClose, this));
		btnClose->setPressedActionEnabled(true);
	}
	mListViewPromoItems = dynamic_cast<ListView*>(rootNode->getChildByName("lsvPromoItems"));

	loadAllDatas();
	
	return true;
}

void LayerPromotion::onButtonClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

bool LayerPromotion::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerPromotion::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPromotion::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerPromotion::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerPromotion::loadAllDatas()
{
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return;

	const int tagReceiveMoney = 1;
	const int tagIncreGold = 2;
	const int tagProgress = 3;

	bool isHasData = false;
    mListViewPromoItems->removeAllChildrenWithCleanup(true);
	vector<PromotionInfo> lstPromos = SceneManager::getSingleton().getLstPromotionInfo();
	for (int i = 0; i < lstPromos.size(); i++){
		PromotionInfo _pro = lstPromos.at(i);
		LayerPromotionItemNormal* _childLayer = LayerPromotionItemNormal::create();

		_childLayer->setAnchorPoint(Point(0, 1));

		int _type = (_pro.kmid == 2 ? tagProgress : (_pro.kmid == 1) ? tagReceiveMoney : tagIncreGold);
		switch (_type) {
		case tagProgress:
			if (_pro.lstPromotionOptions.size() == 0)
				continue;
			_childLayer->setProgressType(_pro.kmid, _type, _pro.kmname
				, _pro.kmdesc, 45, "5/10");
			break;
		case tagReceiveMoney:
			_childLayer->setReceiveGoldType(_pro.kmid, _type, _pro.kmname
				, _pro.kmdesc);
			break;
		case tagIncreGold:
			_childLayer->setReceiveGoldType(_pro.kmid, _type, _pro.kmname
				, _pro.kmdesc);
			break;
		}

		isHasData = true;
		_childLayer->setPositionY(mListViewPromoItems->getPositionY() - _childLayer->getContentSize().height*2 + 30 );
        _childLayer->setIsUsed(_pro.isUsed);
		mListViewPromoItems->addChild(_childLayer);
	}
//	if (isHasData){
//		Chat *toast = Chat::create("Hiện tại chưa có khuyến mại nào dành cho bạn!", -1);
//		this->addChild(toast);
//	}
}

Layout* LayerPromotion::createItemNormal(string body, int tagID)
{
	LayerPromotionItemNormal* _layer = LayerPromotionItemNormal::create();
	//_layer->setDatas(body, tagID);

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(_layer->getContentSize());
	custom_item->addChild(_layer);
	return custom_item;
}

Layout* LayerPromotion::createItemProgress(string body, string sProgress, int tagID)
{
	LayerPromotionItemProgress* _layer = LayerPromotionItemProgress::create();
	_layer->setDatas(body, sProgress, tagID, 100);

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(_layer->getContentSize());
	custom_item->addChild(_layer);
	return custom_item;//
}

void LayerPromotion::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("============== CMD Parent = %s", cmd->c_str());
	if (cmd->compare("kmres") == 0)//PROMOTION_RESP = "kmres";
	{
		int kmrc = *param->GetInt("kmrc");//PROMOTION_RESULT_CODE = "kmrc";
		long kmv = *param->GetLong("kmv");//PROMOTION_VALUE = "kmv";
		log("LayerPromotion::OnExtensionResponse - kmrc=%d -- rc=%d", kmrc, kmv);
		//PROMOTION_VALUE = "kmv";
		if (kmrc == 0) {
			Chat *toast = Chat::create("Chúc mừng, bạn đã nhận được " + *mUtils::format_money(kmv) + " xu!", -1);
			this->addChild(toast);
            
            vector<PromotionInfo> lstPromos = SceneManager::getSingleton().getLstPromotionInfo();
            for (int i = 0; i < lstPromos.size(); i++){
                PromotionInfo info = lstPromos.at(i);
                    if (info.kmid == *param->GetInt("kmid")) {
                        info.isUsed = true;
                        SceneManager::getSingleton().getPromotionHandlers().updateListPromotionInfo(info);
                        break;
                    }
                }            
            //
            this->loadAllDatas();
		}
		else {
			Chat *toast = Chat::create("Bạn chưa đủ điều kiện nhận khuyến mại!", -1);
			this->addChild(toast);
		}
		//
	}
}

void LayerPromotion::onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{
	if (response && response->getResponseCode() == 200){

		//log("i am here");
		std::vector<char>* buffer = response->getResponseData();
		std::string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());
		rapidjson::Document d;
		d.Parse<0>(res.c_str());
		log("i am here");
		if (d.IsArray()){
			log("here children");
			rapidjson::SizeType size = d.Size();
		}
	}
	else{
		log("Error!!");
	}
}

