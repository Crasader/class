#include "LayerTCC_PopupXHMoQuy.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "TCCMsgDef.h"
#include "layers/LayerPopupThongBao.h"

bool LayerTCC_PopupXHMoQuy::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_PopupXHMoQuy.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());

	// 	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 	root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 	root->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pnlMain = (Layout *)root->getChildByName("pnlMain");
	if (pnlMain)
	{
		createButton();
		createUI();
	}
	return true;
}

void LayerTCC_PopupXHMoQuy::createButton()
{
	Button* btnClose = (Button*)pnlMain->getChildByName("btnClose");
	if (btnClose)
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupXHMoQuy::onButtonClose, this));

	Button* btnBack = (Button*)pnlMain->getChildByName("btnback");
	if (btnBack)
		btnBack->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupXHMoQuy::onButtonClose, this));
	Button* btnRule = (Button*)pnlMain->getChildByName("btnRule");
	if (btnRule)
		btnRule->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupXHMoQuy::onButtonRule, this));
}

void LayerTCC_PopupXHMoQuy::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

LayerTCC_PopupXHMoQuy::LayerTCC_PopupXHMoQuy()
{
	GameServer::getSingleton().addListeners(this);
}

LayerTCC_PopupXHMoQuy::~LayerTCC_PopupXHMoQuy()
{
	GameServer::getSingleton().removeListeners(this);
}

void LayerTCC_PopupXHMoQuy::setListRank(string hisTop, string hisMe)
{
	//list user "mrtuan2|TOM|1;mrtuan2|CUA|1;mrtuan2|CA|1;mrtuan2|BAU|4;mrtuan2|TOM|1;tester33|HUOU|0;"
	//list me "BAU|0;TOM|0;CUA|0;CA|0;HUOU|0;GA|0"
	vector<string> arrHisTop = mUtils::splitString(hisTop, ';');
	for (int i = 0; i < arrHisTop.size(); i++)
	{
		vector<string> arrinfo = mUtils::splitString(arrHisTop.at(i), '|');
		if (arrinfo.size() >= 3)
		{
			string uid = arrinfo.at(0);
			string bettype = arrinfo.at(1);
			string point = arrinfo.at(2);
			Text* labelUser = getLabelByBetType(bettype, arrLblUser);
			Text* labelPoint = getLabelByBetType(bettype, arrLblPoint);
			if (atoi(point.c_str()) > 0)
			{
				if (labelUser)
				{
					labelUser->setString(formatUserName(uid, 10));
					labelUser->setName(uid);
					labelUser->setTouchEnabled(true);
					labelUser->addTouchEventListener(CC_CALLBACK_2(LayerTCC_PopupXHMoQuy::onLabelUserTouch, this));
				}
				if (labelPoint)
					labelPoint->setString(point);
			}
		}
	}

	vector<string> arrHisMe = mUtils::splitString(hisMe, ';');
	for (int i = 0; i < arrHisMe.size(); i++)
	{
		vector<string> arrinfo = mUtils::splitString(arrHisMe.at(i), '|');
		if (arrinfo.size() >= 2)
		{
			string bettype = arrinfo.at(0);
			string point = arrinfo.at(1);
			Text* labelPoint = getLabelByBetType(bettype, arrLblPointMe);
			if (labelPoint)
				labelPoint->setString(point);
		}
	}
}

void LayerTCC_PopupXHMoQuy::createUI()
{
	llblThongBao = (Text*)pnlMain->getChildByName("llblThongBao");
	if (llblThongBao)
	{
		llblThongBao->setString(dataManager.getAppConfig().notifiQT);
	}
	//pnlMain->setSwallowTouches(false);
	for (int i = 0; i < 6; i++)
	{
		string betType = getBetTypeById(i);
		Text* lblUser = (Text*)pnlMain->getChildByName(StringUtils::format("lblUser%d", i + 1));
		if (lblUser)
		{
			lblUser->setName(betType);
			arrLblUser.push_back(lblUser);
		}
		Text* lblPoint = (Text*)pnlMain->getChildByName(StringUtils::format("lblPoint%d", i + 1));
		if (lblPoint)
		{
			lblPoint->setName(betType);
			arrLblPoint.push_back(lblPoint);
		}
		Text* lblPointMe = (Text*)pnlMain->getChildByName(StringUtils::format("lblPointMe%d", i + 1));
		if (lblPointMe)
		{
			lblPointMe->setName(betType);
			arrLblPointMe.push_back(lblPointMe);
		}
	}
}

Text* LayerTCC_PopupXHMoQuy::getLabelByBetType(string bettype, vector<Text*> arrLabel)
{
	for (int i = 0; i < arrLabel.size(); i++)
	{
		Text* label = arrLabel.at(i);
		if (0 == label->getName().compare(bettype))
			return label;
	}
	return NULL;
}

void LayerTCC_PopupXHMoQuy::onButtonRule(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto currScene = Director::getInstance()->getRunningScene();
		if (currScene)
		{
			LayerPopupWebView* layerTut = LayerPopupWebView::create();
			layerTut->setWebView(LINK_EVENT_TCC);
			currScene->addChild(layerTut, ZORDER_LIST::ZORDER_POPUP_RANK);
		}
	}
}

void LayerTCC_PopupXHMoQuy::onLabelUserTouch(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Text* labelUser = (Text*)pSender;
		string uidFull = labelUser->getName();
		if (0 != uidFull.compare(""))
		{
			SceneManager::getSingleton().showChatWithCurrentScene(uidFull, -1);
		}
	}
}

void LayerTCC_PopupXHMoQuy::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (0 == ptrNotifiedCmd->compare(EXT_EVENT_TCC_GAME_RESULT)
		|| 0 == ptrNotifiedCmd->compare(EXT_EVENT_TCC_END))
	{
		event_EXT_EVENT_TCC_GAME_RESULT(param);
	}
}

void LayerTCC_PopupXHMoQuy::event_EXT_EVENT_TCC_GAME_RESULT(boost::shared_ptr<ISFSObject> param)
{
	//danh cho diem tich luy
	boost::shared_ptr<string> ghtp_ptr = param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	boost::shared_ptr<string> gtbuc_ptr = param->GetUtfString(EXT_FIELD_GAME_TOTAL_USER_BET_CELL);
	string listHisTop = "";
	string listHisMe = "";
	if (ghtp_ptr)
	{
		listHisTop = ghtp_ptr->c_str();
	}
	if (gtbuc_ptr)
	{
		listHisMe = gtbuc_ptr->c_str();
	}
	this->setListRank(listHisTop, listHisMe);
}
