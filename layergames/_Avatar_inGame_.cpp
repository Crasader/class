
#include "_Avatar_inGame_.h"
#include "SceneManager.h"
#include "GameServer.h"
#include "mUtils.h"
#include "ChanUtils.h"

LayerAvatarInGame::LayerAvatarInGame():
    playerLeft(NULL),
    playerRight(NULL),
    playerTop(NULL),
    playerBottom(NULL),
    isSpector(true),
    myAI("")
{
}

LayerAvatarInGame::~LayerAvatarInGame(){
    this->removeAllChildrenWithCleanup(true);
}

bool LayerAvatarInGame::init() {
	if (!Layer::init())
		return false;

    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(Vec2(0, 0));

	this->myAI = SceneManager::getSingleton().getMyName();

	Size sizeAdd = ChanUtils::getSizePos();

    //Avatar me;
    this->playerMe = AvatarChanIsMe::create();
    Size ss = this->playerMe->getSizeThis();
    this->playerMe->setPosition (ss.width / 2 + 20 - sizeAdd.width, ss.height / 2 + 20 - sizeAdd.height);
    this->playerMe->setVisibleLayerInvite(false);
    this->playerMe->setMoney(100000);
    this->playerMe->setName("");

    this->playerLeft = AvatarChanNormal::create();
    this->playerRight = AvatarChanNormal::create();
    this->playerTop = AvatarChanNormal::create();
    this->playerBottom = AvatarChanNormal::create();
	//this->playerBottom->loadImgIconRechardMe();

    float AVATAR_L_SPACE = 15;
    float w_avatar = 109;

    this->playerLeft->setPosition(Vec2(AVATAR_L_SPACE + w_avatar - sizeAdd.width, HEIGHT_DESIGN / 2));
    this->playerRight->setPosition(Vec2(WIDTH_DESIGN - AVATAR_L_SPACE - w_avatar + sizeAdd.width, HEIGHT_DESIGN / 2));
    this->playerTop->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN - w_avatar + sizeAdd.height));
    this->playerBottom->setPosition(Vec2(WIDTH_DESIGN / 2, 200 - sizeAdd.height));
    this->playerBottom->setVisible(false);

    this->playerMe->setTag(kUserMe);
    this->playerLeft->setTag(kUserLeft);
    this->playerRight->setTag(kUserRight);
    this->playerTop->setTag(kUserTop);
    this->playerBottom->setTag(kUserBot);

    this->addChild(this->playerLeft);
    this->addChild(this->playerRight);
    this->addChild(this->playerTop);
    this->addChild(this->playerBottom);
    this->addChild(this->playerMe);

    return true;
}

void LayerAvatarInGame::SetIsSpect(bool isSpect){
    this->isSpector = isSpect;
}

int LayerAvatarInGame::getUserCount()
{
    return 0;
}

void LayerAvatarInGame::setIcon(const int& pos, char *url){
    if (this->getUserByPos(pos) == NULL) 
		return;
    this->getUserByPos(pos)->setIcon(url);
}

void LayerAvatarInGame::setFlag(const int& pos, bool isShow){
    if (this->getUserByPos(pos) == NULL) 
		return;
    this->getUserByPos(pos)->setFlag(isShow);
}

void LayerAvatarInGame::setName(const int& pos, string name){
    if (this->getUserByPos(pos) == NULL) 
		return;
    this->getUserByPos(pos)->setName(name);
}

void LayerAvatarInGame::setMoney(const int& pos, string money){
    if (this->getUserByPos(pos) == NULL) 
		return;
    this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame::setMoney(const int& pos, int money) {
	if (this->getUserByPos(pos) == NULL) 
		return;
	this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame::setMoney(const int& pos, double money) {
	if (this->getUserByPos(pos) == NULL) 
		return;
	this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame::setReady(const int& pos, bool isReady){
    if (this->getUserByPos(pos) == NULL) 
		return;
    this->getUserByPos(pos)->setReady(isReady);
}

void LayerAvatarInGame::setVisibleLayerInvite(const int& pos, bool isShow) {
	Avatar *user = this->getUserByPos(pos);
	if (user != NULL)
		user->setVisibleLayerInvite(isShow);
}

void LayerAvatarInGame::setUnReadyAllUser() {
    this->playerMe->setReady(false);
    this->playerLeft->setReady(false);
    this->playerRight->setReady(false);
    this->playerTop->setReady(false);
    this->playerBottom->setReady(false);
}

void LayerAvatarInGame::setListUserByTienLen(string listUser) {

}

Avatar* LayerAvatarInGame::getUserByPos(const int& pos){
    switch(pos){
    case kUserBot:
        return this->playerBottom;
    case kUserTop:
        return this->playerTop;
    case kUserLeft:
        return this->playerLeft;
    case kUserRight:
        return this->playerRight;
    case kUserMe:
        return this->playerMe;
    }
    return NULL;
}

void LayerAvatarInGame::ShowBiBao(const int& pos, bool state) {
    AvatarChanNormal* user = (AvatarChanNormal*)this->getUserByPos(pos);
    if (user != NULL) {
        if (state) {
            user->ShowBiBao();
        } else {
            user->HideBiBao();
        }
    }
}

 void LayerAvatarInGame::SetMeIsBoss(bool isBoss){
     this->playerLeft->SetTaoLaBot(isBoss);
     this->playerRight->SetTaoLaBot(isBoss);
     this->playerTop->SetTaoLaBot(isBoss);
     this->playerBottom->SetTaoLaBot(isBoss);
     this->playerMe->SetTaoLaBot(isBoss);
 }

void LayerAvatarInGame::SetEndGame(){
    this->playerMe->HideBiBao();
    this->playerRight->HideBiBao();
    this->playerLeft->HideBiBao();
    this->playerBottom->HideBiBao();
    this->playerTop->HideBiBao();

    this->playerMe->stopTimer();
    this->playerLeft->stopTimer();
    this->playerRight->stopTimer();
    this->playerTop->stopTimer();
    this->playerBottom->stopTimer();

    if(this->playerMe->getChildByTag(20) != NULL)
        this->playerMe->removeChildByTag(20);

    if(this->playerLeft->getChildByTag(20) != NULL)
        this->playerLeft->removeChildByTag(20);

    if(this->playerRight->getChildByTag(20) != NULL)
        this->playerRight->removeChildByTag(20);

    if(this->playerTop->getChildByTag(20) != NULL)
        this->playerTop->removeChildByTag(20);

    if(this->playerBottom->getChildByTag(20) != NULL)
        this->playerBottom->removeChildByTag(20);
}

int LayerAvatarInGame::getPosByName(string pName) {
    return -1;
}

int LayerAvatarInGame::getPosByAccountID(string aI) {
	return -1;
}

void LayerAvatarInGame::runTimer(const int& posUser) {
    this->stopAllTimer();
	Avatar *avatar =  getUserByPos(posUser);
    if(avatar == NULL)
        return;
	avatar->startTimer();
}

void LayerAvatarInGame::stopAllTimer() {
    this->playerMe->stopTimer();
    this->playerLeft->stopTimer();
    this->playerRight->stopTimer();
    this->playerTop->stopTimer();
    this->playerBottom->stopTimer();
}

bool LayerAvatarInGame::isSpectator() {
    return this->isSpector;
}

void LayerAvatarInGame::resetAll(){
    this->playerMe->ResetAll();
    this->playerLeft->ResetAll();
    this->playerRight->ResetAll();
    this->playerTop->ResetAll();
    this->playerBottom->ResetAll();
}

bool LayerAvatarInGame::isStartedGame()
{
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room == NULL)
		return false;

	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv == NULL)
		return false;

	string s = *rv->GetStringValue();
	vector<string> lstBet = mUtils::splitString(s, '@');
    bool isStartedGame = false;

    isStartedGame = 0 == lstBet.at(1).compare("1");

	if (isStartedGame) {
		log("Ban dang choi!");
    } else {
       // log("Ban chua choi!");
    }

	return isStartedGame;
}

void LayerAvatarInGame::stopAllBlinkAvatar(){
    if(this->playerMe->getChildByTag(20) != NULL)
        this->playerMe->removeChildByTag(20);

    if(this->playerLeft->getChildByTag(20) != NULL)
        this->playerLeft->removeChildByTag(20);

    if(this->playerRight->getChildByTag(20) != NULL)
        this->playerRight->removeChildByTag(20);

    if(this->playerTop->getChildByTag(20) != NULL)
        this->playerTop->removeChildByTag(20);

    if(this->playerBottom->getChildByTag(20) != NULL)
        this->playerBottom->removeChildByTag(20);
}

void LayerAvatarInGame::runTimerWithTime(const int& posUser, const float& time)
{
    this->stopAllTimer();
    Avatar *avatar = this->getUserByPos(posUser);
    if (avatar == NULL)
        return;
	avatar->startTimer(time);
}

bool LayerAvatarInGame::SetListSpectToEmpty(std::string& list, const std::string& myAI) {
    bool ret = false;

           this->ResetListSpect();

          // log("list spectator = %s", list.c_str());

           if (list == "")
               return ret;

           vector<string> arr = mUtils::splitString(list, ';');
           for (int i = 0; i < arr.size(); ++i) {
               std::string str1 = arr.at(i);
               boost::algorithm::trim(str1);
               if (str1 == "")
                   continue;

               vector<string> info = mUtils::splitString(str1, ':');
               if (0 == info.size())
                   continue;

               str1 = info.at(1);
               boost::algorithm::trim(str1);

               if (str1 == myAI) {
                   ret = true;
               }

               PlayerChanInfo player = ChanUtils::GetPlayerInfo(str1);

               Avatar* ava = this->GetEmptyAvatar();
               if (ava != NULL) {
                   ava->setVisibleLayerInvite(false);
                   ava->setAI(player.aI, true);
                   ava->setName(player.name);
                   ava->setMoney(player.money);
                   ava->setIcon(player.aal);
                   ava->setMeIsBoss(false);
                   ava->setPlayerRegist();
               } else {
                   break;
               }
           }

//           if (ret && this->isSpector) {
//               this->playerMe->setVisible(false);
//           } else {
//               this->playerMe->setVisible(true);
//           }
           return ret;
}

Avatar* LayerAvatarInGame::GetEmptyAvatar() {
    if (this->playerBottom->getAI() == "") {
        return this->playerBottom;
    }

    if (this->playerRight->getAI() == "") {
        return this->playerRight;
    }

    if (this->playerTop->getAI() == "") {
        return this->playerTop;
    }

    if (this->playerLeft->getAI() == "") {
        return this->playerLeft;
    }

    return NULL;
}

void LayerAvatarInGame::ResetListSpect() {
    if (this->playerBottom->forGuess) {
        this->playerBottom->ResetAll();
    }
    if (this->playerLeft->forGuess) {
        this->playerLeft->ResetAll();
    }
    if (this->playerRight->forGuess) {
        this->playerRight->ResetAll();
    }
    if (this->playerTop->forGuess) {
        this->playerTop->ResetAll();
    }
}

void LayerAvatarInGame::SetEmotionInPos(const int &pos, string& chat)
{
    switch(pos){
        case kUserMe:
        this->playerMe->showChat(chat);
        break;
    case kUserBot:
        this->playerBottom->showChat(chat);
        break;
    case kUserLeft:
        this->playerLeft->showChat(chat);
        break;
    case kUserRight:
        this->playerRight->showChat(chat);
        break;
    case kUserTop:
        this->playerTop->showChat(chat);
        break;
    }
}

void LayerAvatarInGame::showIconLeaveGame(bool state){
    this->playerMe->showIconLeaveGame(state);
}

void LayerAvatarInGame::checkSameIp(){
    vector<Avatar*> arrs;
    arrs.push_back(this->playerMe);
    arrs.push_back(this->playerLeft);
    arrs.push_back(this->playerRight);
    arrs.push_back(this->playerTop);
    arrs.push_back(this->playerBottom);

    for(int i = 0; i < arrs.size(); ++i){

        Avatar* a = arrs.at(i);
        string ip = a->getIp();
        string ai = a->getAI();

        a->showSameIP(false);

        if(ai != "") {
            for(int j = 0; j < arrs.size(); ++j){
                if(i != j) {

                    Avatar* b = arrs.at(j);
                    string ip2 = b->getIp();
                    string ai2 = b->getAI();

                    if(ai2 != "" && ai2 != ai){
						if (ip == ip2 &&
							0 != ip.compare("")
							&&
							0 != ip.compare("undefined")
							&&
							0 != ip2.compare("")
							&&
							0 != ip2.compare("undefined")){
                            a->showSameIP(true);
                            break;
                        }
                    }

                }
            }
        }
    }

}
