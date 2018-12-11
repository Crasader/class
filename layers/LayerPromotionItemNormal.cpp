#include "LayerPromotionItemNormal.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../mUtils.h"

#include "Requests/ExtensionRequest.h"

#include "GameServer.h"

USING_NS_CC;
using namespace cocos2d::ui;


LayerPromotionItemNormal::LayerPromotionItemNormal()
{
}


LayerPromotionItemNormal::~LayerPromotionItemNormal()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerPromotionItemNormal::init()
{
	if (!Layer::init())
	{
		return false;
	}
	GameServer::getSingleton().addListeners(this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto root = CSLoader::getInstance()->createNode("LayerItemPopupPromotion.csb");

	ui::Helper::doLayout(root);

	this->addChild(root);
	this->setContentSize(root->getContentSize());
	nID = -1;
	nGameID = -1;

	lblDetailPro = dynamic_cast<Text*>(root->getChildByName("lblDetailPro"));
// 	lblPromoBody = dynamic_cast<Text*>(root->getChildByName("lblPromoBody"));
// 	//Progress
// 	pnlProgress = dynamic_cast<Layout*>(root->getChildByName("pnlProgress"));
// 	ldbProgress = dynamic_cast<LoadingBar*>(pnlProgress->getChildByName("ldbProgress"));
// 	lblProgress = dynamic_cast<Text*>(pnlProgress->getChildByName("lblProgress"));
// 	//
// 	pnlBox = dynamic_cast<Layout*>(root->getChildByName("pnlBox"));
// 	//panel gold
// 	pnlGold = dynamic_cast<Layout*>(root->getChildByName("pnlGold"));
// 	lblGold = dynamic_cast<Text*>(pnlGold->getChildByName("lblGold"));
	//
	btnClainReward = dynamic_cast<Button*>(root->getChildByName("btnReceiver"));
	btnClainReward->setEnabled(true);
	if (btnClainReward != NULL) {
		btnClainReward->addClickEventListener(CC_CALLBACK_1(LayerPromotionItemNormal::onButtonPress, this));
		btnClainReward->setPressedActionEnabled(true);
	}
	isMeSendMsg = false;

	return true;
}

void LayerPromotionItemNormal::onButtonPress(Ref* _ref)
{
	auto _btn = dynamic_cast<Button*>(_ref);
	int proID = _btn->getTag();
	isMeSendMsg = true;
	//
	boost::shared_ptr<ISFSObject> parameters(new SFSObject());
	parameters->PutInt("kmid", nID);//PROMOTION_ID = "kmid";
	parameters->PutInt("kmgid", nGameID);//PROMOTION_GAME_ID = "kmgid";
	boost::shared_ptr<IRequest> request(new ExtensionRequest("kmcmd", parameters));// PROMOTION_CMD = "kmcmd";
	GameServer::getSingleton().Send(request);
    btnClainReward->setEnabled(false);
}

void LayerPromotionItemNormal::setIncreGoldType(int nID, int nType, string kmname, string kmdesc, int kmGold)
{
	typePromotion = nType;
	/*lblPromoBody->setString(kmname + "\n" + kmdesc);*/
	lblDetailPro->setString(kmdesc);
	this->nID = nID;
// 	lblGold->setString("+ " + *mUtils::format_money(kmGold));
// 
// 	pnlProgress->setVisible(nType == tagProgress);
// 	pnlBox->setVisible(nType == tagReceiveMoney);
// 	pnlGold->setVisible(nType == tagIncreGold);
}

void LayerPromotionItemNormal::setReceiveGoldType(int nID, int nType, string kmname, string kmdesc)
{
	typePromotion = nType;
	/*lblPromoBody->setString(kmname + "\n" + kmdesc);*/
	lblDetailPro->setString(kmdesc);
	lblDetailPro->setFontSize(25);
	this->nID = nID;

// 	pnlProgress->setVisible(nType == tagProgress);
// 	pnlBox->setVisible(nType == tagReceiveMoney || nType == tagIncreGold);
// 	pnlGold->setVisible(false);
}

void LayerPromotionItemNormal::setProgressType(int nID, int nType, string kmname, string kmdesc, int kmProgressNumber, string kmProgressText)
{
	typePromotion = nType;
	/*lblPromoBody->setString(kmname + "\n" + kmdesc);*/
	lblDetailPro->setString(kmdesc);
	this->nID = nID;
// 	lblProgress->setString(kmProgressText);
// 	ldbProgress->setPercent( kmProgressNumber );
// 
// 	pnlProgress->setVisible(nType == tagProgress);
// 	pnlBox->setVisible(nType == tagReceiveMoney);
// 	pnlGold->setVisible(nType == tagIncreGold);
}

void LayerPromotionItemNormal::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("============== CMD = %s", cmd->c_str());
	if (cmd->compare("kmres") == 0)//PROMOTION_RESP = "kmres";
	{
		int kmrc = *param->GetInt("kmrc");//PROMOTION_RESULT_CODE = "kmrc";
		long kmv = *param->GetLong("kmv");//PROMOTION_VALUE = "kmv";
		if (kmrc == 0 && isMeSendMsg) {
			btnClainReward->setEnabled(false);
		}
		isMeSendMsg = false;
	}
}
void LayerPromotionItemNormal::setIsUsed(bool isUsed){
    btnClainReward->setEnabled(!isUsed);
}
