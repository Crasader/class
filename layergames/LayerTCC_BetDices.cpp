#include "LayerTCC_BetDices.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"
#include "Requests/ExtensionRequest.h"
#include "_Chat_.h"
#include "data_manager.h"
#include "ClientMsgDefs.h"
#include "LayerTCC_PopupMain.h"

LayerBetDice::LayerBetDice()
{

}

LayerBetDice::~LayerBetDice()
{

}

bool LayerBetDice::init()
{
	if (!Layer::init())
		return false;

	Size bet_size = Size(250, 270);
	int startY = 250;
	for (int i = 0; i < 6; i++){
		LayerTCCBet* layerTccBet = LayerTCCBet::create(getImgDiceX2ById(i), i);
		layerTccBet->setAnchorPoint(Vec2::ZERO);
		layerTccBet->setName(getBetTypeById(i));
		int jum = i;
		if (i > 2){
			startY = 0;
			jum = jum - 3;
		}
		layerTccBet->setPosition(bet_size.width*jum, startY);
		this->addChild(layerTccBet);
		//new version
		betCounts.push_back(BetCount(getBetTypeById(i), 0));//set co = 0 het
	}
	loadAmfUser();

	return true;
}

void LayerBetDice::showToast(string mess)
{
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene)
	{
		Chat* layerChat = Chat::create(mess, -1);
		currScene->addChild(layerChat, ZORDER_LIST::ZORDER_TOAST);
	}
}

void LayerBetDice::initBetInfo(string _list)
{
	if (0 == _list.compare(""))
	{
		string initStr = "TOM:100:0:0;CUA:100:0:0;CA:100:0:0;BAU:100:0:0;HUOU:100:0:0;GA:100:0:0;TOM:1000:0:0;CUA:1000:0:0;CA:1000:0:0;BAU:1000:0:0;HUOU:1000:0:0;GA:1000:0:0;TOM:10000:0:0;CUA:10000:0:0;CA:10000:0:0;BAU:10000:0:0;HUOU:10000:0:0;GA:10000:0:0";
		vector<string> arrInfo = mUtils::splitString(initStr, ';');
		for (int i = 0; i < arrInfo.size(); i++)
		{
			vector<string> infos = mUtils::splitString(arrInfo.at(i), ':');
			if (infos.size() >= 4)
			{
				string _bettype = infos.at(0);
				int _betroom = atoi(infos.at(1).c_str());
				double _userbet = atol(infos.at(2).c_str());
				double _totalbet = atol(infos.at(3).c_str());
				this->listBetInfo.push_back(DiceInfo(_betroom, _bettype, _userbet, _totalbet));
			}
		}
	}
	else{
		this->listBetInfo.clear();
		vector<string> arrUserBet = mUtils::splitString(_list, ';');
		for (int i = 0; i < arrUserBet.size(); i++)
		{
			vector<string> infos = mUtils::splitString(arrUserBet.at(i), ':');
			if (infos.size() >= 4){
				string _bettype = infos.at(0);
				int _betroom = atoi(infos.at(1).c_str());
				double _userbet = atol(infos.at(2).c_str());
				double _totalbet = atol(infos.at(3).c_str());
				this->listBetInfo.push_back(DiceInfo(_betroom, _bettype, _userbet, _totalbet));
			}
		}
		this->loadBetInfo();
	}
}

void LayerBetDice::setBetInfoClient(double userbet, double totalbet, string bettype)
{
	for (int i = 0; i < this->listBetInfo.size(); i++){
		if (this->listBetInfo.at(i).betroom == this->betroom){
			if (0 == this->listBetInfo.at(i).bettype.compare(bettype)){
				this->listBetInfo.at(i).userbet = userbet;
				this->listBetInfo.at(i).totalbet = totalbet;
				break;
			}
		}
	}

	//new version
	int countFlag = 0;// neu lon hon 3 thi khong set
	for (int i = 0; i < betCounts.size(); i++)
	{
		if (betCounts.at(i).flag == 1)
		{
			countFlag++;
		}
	}

	if (countFlag < dataManager.getAppConfig().tccLimitCount)
	{
		for (int i = 0; i < betCounts.size(); i++)
		{
			if (0 == betCounts.at(i).bettype.compare(bettype) &&
				0 == betCounts.at(i).flag) //add flag
			{
				betCounts.at(i).flag = 1;
				break;
			}
		}
	}

}

void LayerBetDice::setBetInfo(string myName, string uid, string userbet, string totalbet, int betroom)
{
	if (this->listBetInfo.size() > 0)
	{
		if (0 != myName.compare(uid))
		{
			bool isNeedReload = false;
			vector<string> arrBetTotal = mUtils::splitString(totalbet, '|');
			for (int i = 0; i < arrBetTotal.size(); i++){
				vector<string> arrTotalInfos = mUtils::splitString(arrBetTotal.at(i), ':');
				if (arrTotalInfos.size() == 2)
				{
					string bettype = arrTotalInfos.at(0);
					string totalvl = arrTotalInfos.at(1);
					for (int k = 0; k < this->listBetInfo.size(); k++){
						if (this->listBetInfo.at(k).betroom == betroom){
							if (0 == this->listBetInfo.at(k).bettype.compare(bettype))
							{
								double _total = atol(totalvl.c_str());
								if (this->listBetInfo.at(k).totalbet < _total)
								{
									isNeedReload = true;
									this->listBetInfo.at(k).totalbet = _total;
								}
								break;
							}
						}
					}
				}
			}

			if (isNeedReload)
				this->loadBetInfo();
		}
	}
	else{
		//this->showToast("Lỗi trong quá trình lấy dữ liệu");
		log("Lỗi trong quá trình lấy dữ liệu");
	}

}

void LayerBetDice::rollBackBet(int betroom, string bettype, int betvl)
{
	for (int i = 0; i < this->listBetInfo.size(); i++){
		if (this->listBetInfo.at(i).betroom == betroom){
			if (0 == this->listBetInfo.at(i).bettype.compare(bettype))
			{
				this->listBetInfo.at(i).userbet -= betvl;
				this->listBetInfo.at(i).totalbet -= betvl;
				this->loadBetInfo();
				break;
			}
		}
	}
}

void LayerBetDice::loadBetInfo()
{
	loadAmfUser();
	//load thong tin tien dat cuoc theo bet room
	for (int i = 0; i < this->listBetInfo.size(); i++) {
		int betRoom = this->listBetInfo[i].betroom;
		if (this->betroom == betRoom) { //chi lay thong tin cua bet room do
			string betType = this->listBetInfo[i].bettype;
			double userBet = this->listBetInfo[i].userbet;
			double totalBet = this->listBetInfo[i].totalbet;
			LayerTCCBet* layerDice = this->getLayerTCCBetByName(betType);
			if (layerDice != NULL) {
				layerDice->setCurrentBet(userBet, totalBet);
			}
		}
	}

}

void LayerBetDice::startGame()
{
	for (int i = 0; i < 6; i++){
		LayerTCCBet* layerTccBet = this->getLayerTCCBetByName(getBetTypeById(i));
		if (layerTccBet != NULL)
			layerTccBet->startGame();
	}
}

void LayerBetDice::resetGame()
{
	loadAmfUser();
	for (int i = 0; i < 6; i++){
		LayerTCCBet* layerTccBet = this->getLayerTCCBetByName(getBetTypeById(i));
		if (layerTccBet != NULL)
			layerTccBet->resetGame();
		//new version
		betCounts.at(i).flag = 0;
	}
	for (int i = 0; i < this->listBetInfo.size(); i++){
		this->listBetInfo.at(i).totalbet = 0;
		this->listBetInfo.at(i).userbet = 0;
	}
}

void LayerBetDice::endGame()
{
	for (int i = 0; i < 6; i++){
		LayerTCCBet* layerTccBet = this->getLayerTCCBetByName(getBetTypeById(i));
		if (layerTccBet != NULL)
			layerTccBet->endGame();
	}
}

void LayerBetDice::setResult(string _string)
{
	//"rg":"BAU_CA_TOM"
	vector<string> arrStr = mUtils::splitString(_string, '_');
	if (arrStr.size() == 3){
		for (int i = 0; i < arrStr.size(); i++){
			LayerTCCBet *layerBet = this->getLayerTCCBetByName(arrStr.at(i));
			if (layerBet)
				layerBet->showResult();
		}
	}
}

LayerTCCBet* LayerBetDice::getLayerTCCBetByName(string _name)
{
	LayerTCCBet* layerBet = dynamic_cast<LayerTCCBet*>(this->getChildByName(_name));
	return layerBet;
}
void LayerBetDice::setTypeGame(int type){
    this->typeMoney = type;
}
void LayerBetDice::loadAmfUser()
{

    
    if (typeMoney == 0){
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        if (mySelf != NULL)
        {
            boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amf");
            if (amf_ptr != NULL)
                this->amfUser = *amf_ptr->GetDoubleValue();
        }
    }
    else{
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        if (mySelf != NULL)
        {
            boost::shared_ptr<UserVariable> amf_ptr = mySelf->GetVariable("amfs");
            if (amf_ptr != NULL)
                this->amfUser = *amf_ptr->GetDoubleValue();
        }
    }
	
}

bool LayerBetDice::isOutLimitBet(string bettype)
{
	//new version
	bool isOk = false;
	int countFlag = 0;
	for (int i = 0; i < betCounts.size(); i++)
	{
		if (betCounts.at(i).flag == 1)
		{
			countFlag++;
		}
	}
	if (countFlag < dataManager.getAppConfig().tccLimitCount)
		isOk = false;
	else{
		for (int i = 0; i < betCounts.size(); i++)
		{
			if (0 == betCounts.at(i).bettype.compare(bettype))
			{
				if (1 == betCounts.at(i).flag)
					isOk = false;
				else
					isOk = true;
				break;
			}
		}
	}
	return isOk;
}

void LayerBetDice::pushFlagBet(string bettype)
{
	for (int i = 0; i < betCounts.size(); i++)
	{
		if (0 == betCounts.at(i).bettype.compare(bettype))
		{
			betCounts.at(i).flag = 1;
			break;
		}
	}
}

LayerTCCBet::LayerTCCBet(string _url, int _tag)
{
	this->_url = _url;
	this->_tag = _tag;
	imgStroke = NULL;
	Image_2 = NULL;
	imgX2 = NULL;
	imgX3 = NULL;
	btnBetCoin = NULL;
	Sprite_2 = NULL;
	lblTotalBet = NULL;
	lblUserBet = NULL;
}

LayerTCCBet::~LayerTCCBet()
{

}

LayerTCCBet* LayerTCCBet::create(string _url, int tag)
{
	LayerTCCBet * layerTCCBet = new LayerTCCBet(_url, tag);
	if (layerTCCBet && layerTCCBet->init())
	{
		layerTCCBet->autorelease();
		return layerTCCBet;
	}
	else{
		delete layerTCCBet;
		layerTCCBet = NULL;
		return NULL;
	}
}

bool LayerTCCBet::init()
{
	if (!Layer::init())
		return false;
	isInit = true;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_Bet.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	this->setAnchorPoint(Vec2::ZERO);

	this->imgStroke = (Sprite*)root->getChildByName("imgStroke");
	this->btnBetCoin = (Button*)root->getChildByName("btnBetCoin");
	this->Image_2 = (ImageView*)root->getChildByName("Image_2");
	this->Sprite_2 = (Sprite*)root->getChildByName("Sprite_2");
	this->lblTotalBet = (Text*)root->getChildByName("lblTotalBet");
	this->lblUserBet = (Text*)root->getChildByName("lblUserBet");
	if (this->btnBetCoin)
	{
		this->btnBetCoin->setScale9Enabled(false);
		btnBetCoin->setTouchEnabled(true);
		//this->btnBetCoin->setContentSize(Size(213, 213));
		this->btnBetCoin->loadTextureNormal(_url);
		this->btnBetCoin->setTag(_tag);
		this->btnBetCoin->addTouchEventListener(CC_CALLBACK_2(LayerTCCBet::onButtonBet, this));
	}

	Size bet_size = Size(130, 130);
	this->imgX2 = Sprite::create("x2.png");
	this->imgX2->setAnchorPoint(Vec2::ZERO);
	this->imgX2->setPosition(Vec2(bet_size.width, bet_size.height));
	this->addChild(this->imgX2, 10);

	this->imgX3 = Sprite::create("x3.png");
	this->imgX3->setAnchorPoint(Vec2::ZERO);
	this->imgX3->setPosition(bet_size.width, bet_size.height);
	this->addChild(this->imgX3, 10);

	this->timer = ProgressTimer::create(this->imgStroke);
	this->timer->setType(ProgressTimer::Type::RADIAL);
	this->timer->setPosition(this->imgStroke->getPosition());
	this->timer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	root->addChild(this->timer, -10);

	this->startPos = this->Image_2->getPosition();
	this->resetGame();

	return true;
}

void LayerTCCBet::onButtonBet(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		LayerBetDice *parent = dynamic_cast<LayerBetDice*>(this->getParent());
		if (parent)
		{
			int bet_value = parent->getBetValue();
			//int bet_room = parent->getBetRoom();
			//for new version - set bet default = 1k
			int bet_room = 1000;
			double amfUser = parent->getAmfUser();
			this->getMinMoney();
            
			if (amfUser < bet_value ){
				parent->showToast(dataManager.GetSysString(33));
				return;
			}

            //LayerBetDice* parent = (LayerBetDice*)this->getParent();
            //if (parent != NULL){
                if (SceneManager::getSingleton().getCurrRoomType() == parent->getTypeMoney()){
                    if ( bet_value > (amfUser - this->MinBetValue)){
                        auto toast = Chat::create(dataManager.GetSysString(33), -1);
                        this->addChild(toast);
                        return;
                    }
                }
           // }
           
			if (bet_value == 0 || bet_room == -1){
				parent->showToast(dataManager.GetSysString(34));
				return;
			}
			if (parent->isOutLimitBet(this->getName()))
			{
				parent->showToast(StringUtils::format("%s %d %s",dataManager.GetSysString(35).c_str(), dataManager.getAppConfig().tccLimitCount,dataManager.GetSysString(36).c_str()));
				return;
			}
			amfUser -= bet_value;
			string bet_type = this->getName();
			this->currUserBet += bet_value;
			this->currTotalBet += bet_value;
			parent->setAmfUser(amfUser);
			this->setClientBet(this->currUserBet, this->currTotalBet, bet_type);

			boost::shared_ptr<ISFSObject> param(new SFSObject());
			param->PutUtfString(EXT_FIELD_GAME_BET_VALUE, StringUtils::format("%d", bet_value));
			param->PutUtfString(EXT_FIELD_GAME_BET_TYPE, bet_type);
			param->PutUtfString(EXT_FIELD_GAME_BET_ROOM, StringUtils::format("%d", bet_room));

			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_TCC_GAME_BET_REQ, param));
			GameServer::getSingleton().Send(request);
		}
	}
}

void LayerTCCBet::showLabelUser(bool isShow)
{
	Size sizeImage = this->Image_2->getContentSize();
	this->Image_2->stopAllActions();
	if (isShow)
	{
		auto actionMove = MoveTo::create(0.1, Vec2(startPos.x, startPos.y));
		this->Image_2->runAction(Spawn::create(Show::create(), actionMove, NULL));
		this->Image_2->setVisible(isShow);
	}
	else{
		if (!isInit)
		{
			auto actionMove = MoveTo::create(0.1, Vec2(this->startPos.x, this->startPos.y - sizeImage.height / 2));
			this->Image_2->runAction(Sequence::create(actionMove, Hide::create(), NULL));
		}
	}
	this->lblUserBet->setVisible(isShow);
}

void LayerTCCBet::showX2(bool isShow)
{
	this->imgX2->setVisible(isShow);
	if (isShow){
		auto scale_action = ScaleBy::create(0.5, 1.2, 1.2);
		Sequence *action_seq = Sequence::create(scale_action, scale_action->reverse(), NULL);
		auto action = Repeat::create(action_seq, 5);
		this->imgX2->runAction(action);
	}
	else{
		this->imgX2->stopAllActions();
	}
}

void LayerTCCBet::showX3(bool isShow)
{
	this->imgX3->setVisible(isShow);
	if (isShow){
		auto scale_action = ScaleBy::create(0.5, 1.2, 1.2);
		Sequence *action_seq = Sequence::create(scale_action, scale_action->reverse(), NULL);
		auto action = Repeat::create(action_seq, 5);
		this->imgX3->runAction(action);
	}
	else{
		this->imgX3->stopAllActions();
	}
}

void LayerTCCBet::showImgStroke(bool isShow)
{
	if (isShow){
		auto actionTimer = RepeatForever::create(ProgressFromTo::create(1, 100, 0));
		this->timer->runAction(Show::create());
		this->timer->runAction(actionTimer);
	}
	else{
		this->timer->stopAllActions();
		this->timer->setVisible(false);
	}
}

void LayerTCCBet::setCurrentBet(double userBet, double totalBet)
{
	this->currTotalBet = totalBet;
	this->currUserBet = userBet;
	if (userBet > 0){
		this->showLabelUser(true);
		LayerBetDice *parent = dynamic_cast<LayerBetDice*>(this->getParent());
		if (parent)
		{
			parent->pushFlagBet(this->getName());
		}
	}
	else {
		this->showLabelUser(false);
	}
	if (isInit)
		isInit = false;
	this->lblUserBet->setString(formatMoneySymbol(userBet));
	this->lblTotalBet->setString(formatMoneySymbol(totalBet));
}

void LayerTCCBet::setClientBet(double userBet, double totalBet, string bettype)
{
	LayerBetDice* parent = dynamic_cast<LayerBetDice*>(this->getParent());
	if (parent){
		parent->setBetInfoClient(userBet, totalBet, bettype);
	}
	if (userBet > 0){
		this->showLabelUser(true);
	}
	else {
		this->showLabelUser(false);
	}
	this->lblUserBet->setString(formatMoneySymbol(userBet));
	this->lblTotalBet->setString(formatMoneySymbol(totalBet));
}

void LayerTCCBet::setBetUser(double betvl)
{
	this->showLabelUser(true);
	this->lblUserBet->setString(formatMoneySymbol(betvl));
}

void LayerTCCBet::setBetTotal(double betvl)
{
	this->lblTotalBet->setString(formatMoneySymbol(betvl));
}

void LayerTCCBet::getMinMoney()
{
	if (SceneManager::getSingleton().getGameID() == kGameLieng ||
		SceneManager::getSingleton().getGameID() == kGameXiTo)
	{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf)
		{
			boost::shared_ptr<UserVariable> gameChip_ptr = mySelf->GetVariable(EXT_FIELD_GAME_CHIP);
			if (gameChip_ptr)
			{
				this->MinBetValue = *gameChip_ptr->GetDoubleValue();
			}
		}
	}
	else{
		vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
		int gameID = SceneManager::getSingleton().getGameID();
		int bet_ratio = 1;
		for (int i = 0; i < arrBet.size(); ++i)
		{
			if (arrBet.at(i).gameid == gameID)
			{
				bet_ratio = arrBet.at(i).ratioBet;
				break;
			}
		}
		string money = "0";
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (room != NULL && room->GetVariable("params") != NULL) {
			std::string paramString = *room->GetVariable("params")->GetStringValue();
			std::vector<std::string> arrInfo = mUtils::splitString(paramString, '@');
			if (arrInfo.size() > 0) {
				money = arrInfo.at(0);
				if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia)
					money = "0";
			}
		}
		this->MinBetValue = bet_ratio*(atoi(money.c_str()));
	}
}


void LayerTCCBet::endGame()
{
	this->btnBetCoin->setTouchEnabled(false);
	this->count = 0;
}

void LayerTCCBet::resetGame()
{
	this->showLabelUser(false);
	this->count = 0;
	this->currTotalBet = 0;
	this->currUserBet = 0;
	this->btnBetCoin->setTouchEnabled(false);
	this->lblTotalBet->setString("0");
	this->lblUserBet->setString("0");
	this->showX2(false);
	this->showX3(false);
	this->showImgStroke(false);
}

void LayerTCCBet::startGame()
{
	this->btnBetCoin->setTouchEnabled(true);
}

void LayerTCCBet::showResult()
{
	this->count += 1;
	this->showImgStroke(true);
	this->showX3(false);
	this->showX2(false);
	switch (this->count)
	{
	case 2:
		this->showX2(true);
		break;
	case 3:
		this->showX3(true);
		break;
	default:
		break;
	}
}




