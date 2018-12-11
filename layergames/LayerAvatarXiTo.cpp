//
//  LayerAvatarXiTo.cpp
//  iCasinov2
//
//  Created by DauA on 5/28/14.
//
//

#include "LayerAvatarXiTo.h"
#include "GameServer.h"
#include "mUtils.h"
#include "_Number_.h"
#include "AllData.h"
#include "_Chat_.h"
#include "ChanUtils.h"

LayerAvatarXiTo::LayerAvatarXiTo()
    :User_IsMe(NULL),
    User_Left_Bottom(NULL),
    User_Right_Bottom(NULL),
    User_Left_Top(NULL),
    User_Right_Top(NULL) {
        GameServer::getSingleton().addListeners(this);

}

LayerAvatarXiTo::~LayerAvatarXiTo(){
    GameServer::getSingleton().removeListeners(this);

	this->removeAllChildrenWithCleanup(true);
   // log("Destructor LayerAvatarXiTo");
}

bool LayerAvatarXiTo::init(){
    if(!Layer::init())
        return false;

    this->setAnchorPoint(Vec2(0, 0));
    //User_me
    this->User_IsMe = AvatarXiTo::create();
    this->User_IsMe->setPosition(Vec2(720, 200));
	//User_IsMe->loadImgIconRechardMe();

    //User Left-Bottom
    this->User_Left_Bottom = AvatarXiTo::create();
    this->User_Left_Bottom->setAnchorPoint(Vec2(0, 0.5));
    this->User_Left_Bottom->setPosition(Vec2(230, (HEIGHT_DESIGN >> 1) - 150));

    //User Right-Bottom
    this->User_Right_Bottom = AvatarXiTo::create();
    this->User_Right_Bottom->setAnchorPoint(Vec2(1, 0.5));
    this->User_Right_Bottom->setPosition(Vec2(WIDTH_DESIGN - 230, (HEIGHT_DESIGN >> 1) - 150));

    //User Left Top
    this->User_Left_Top = AvatarXiTo::create();
    this->User_Left_Top->setAnchorPoint(Vec2(0, 0.5));
    this->User_Left_Top->setPosition(Vec2(350, 830));

    //User Right Top
    this->User_Right_Top = AvatarXiTo::create();
    this->User_Right_Top->setAnchorPoint(Vec2(1, 0.5));
    this->User_Right_Top->setPosition(Vec2(WIDTH_DESIGN - 350, 830));

    this->User_IsMe->setTag(USER_ME);
    this->User_Left_Bottom->setTag(USER_LEFT_BOT);
    this->User_Right_Bottom->setTag(USER_RIGHT_BOT);
    this->User_Left_Top->setTag(USER_LEFT_TOP);
    this->User_Right_Top->setTag(USER_RIGHT_TOP);

    this->addChild(this->User_IsMe);
    this->addChild(this->User_Left_Bottom);
    this->addChild(this->User_Right_Bottom);
    this->addChild(this->User_Left_Top);
    this->addChild(this->User_Right_Top);

    return true;
}

void LayerAvatarXiTo::ResetAll(){
    this->User_IsMe->ResetAll();
    this->User_Left_Bottom->ResetAll();
    this->User_Right_Bottom->ResetAll();
    this->User_Left_Top->ResetAll();
    this->User_Right_Top->ResetAll();
}

void LayerAvatarXiTo::ResetAvatar(Avatar* avatar)
{
	avatar->setVisibleLayerInvite(true);
	avatar->setReady(false);
	avatar->setName("");
	avatar->setMoney("");
	avatar->setAI("");
}

void LayerAvatarXiTo::SetIcon(const int& pos, const char *url){
    //this->getUserByPos(pos)->setIcon(url);
}

void LayerAvatarXiTo::SetFlag(const int& pos, bool isShow){
    this->GetUserByPos(pos)->setFlag(false);
}

void LayerAvatarXiTo::SetName(const int& pos,const char *name){
    this->GetUserByPos(pos)->setName(name);
}

void LayerAvatarXiTo::SetMoney(const int& pos,const char *money ){
    this->GetUserByPos(pos)->setMoney(money);
}

void LayerAvatarXiTo::StopAllTimer(){
    this->User_IsMe->stopTimer();
    this->User_Left_Bottom->stopTimer();
    this->User_Right_Bottom->stopTimer();
    this->User_Left_Top->stopTimer();
    this->User_Right_Top->stopTimer();
}

void LayerAvatarXiTo::UnReadyAllUser(){
    this->User_IsMe->setReady(false);
    this->User_Left_Bottom->setReady(false);
    this->User_Right_Bottom->setReady(false);
    this->User_Left_Top->setReady(false);
    this->User_Right_Top->setReady(false);
}

AvatarXiTo* LayerAvatarXiTo::GetUserByPos(const int& pos){
    switch (pos) {
        case USER_RIGHT_TOP:
            return this->User_Right_Top;
        case USER_RIGHT_BOT:
            return this->User_Right_Bottom;
        case USER_LEFT_TOP:
            return this->User_Left_Top;
        case USER_LEFT_BOT:
            return this->User_Left_Bottom;
        case USER_ME:
            return this->User_IsMe;
    }
    return NULL;
}

void LayerAvatarXiTo::RunTimer(const int& pos)
{
    this->StopAllTimer();
    AvatarXiTo* a = this->GetUserByPos(pos);
    if(a != NULL){
        a->startTimer();
    }
}

void LayerAvatarXiTo::ShowChatByPos(const int& pos, const string& mes)
{
    Avatar* avat = this->GetUserByPos(pos);
    if(avat != NULL)
    {
        string s = mes;
        avat->showChat(s);
    }
//	if (pos == -1 || 0 == mes.compare(""))
//		return;

//	Chat *newMes = Chat::create(mes, pos);
//	Point point;

//	switch (pos) {
//	case USER_ME:
//		point.setPoint(180, 176);
//		break;

//	case USER_LEFT_TOP:
//		point.setPoint(95, 433);
//		break;

//	case USER_LEFT_BOT:
//		point.setPoint(20, 285);
//		break;

//	case USER_RIGHT_TOP:
//		point.setPoint(WIDTH_DESIGN - newMes->getSize().width - 95, 433);
//		break;

//	case USER_RIGHT_BOT:
//		point.setPoint(WIDTH_DESIGN - newMes->getSize().width - 20, 285);
//		break;
//	default:
//		point.setPoint((WIDTH_DESIGN - newMes->getSize().width) / 2, (HEIGHT_DESIGN - newMes->getSize().height) / 2);
//		newMes->setStatusByServer(true);
//		break;
//	}

//	newMes->setPosition(point);
//	this->addChild(newMes);
}

 void LayerAvatarXiTo::ShowTypeBetInPos(const int& pos, const string& mes){
     AvatarXiTo* a = this->GetUserByPos(pos);
     if(a != NULL){
         string s = mes;
         a->showChat(s);
     }
 }

void LayerAvatarXiTo::hideAllBetType(){
    this->User_IsMe->hideTypeBet();
    this->User_Left_Bottom->hideTypeBet();
    this->User_Right_Bottom->hideTypeBet();
    this->User_Left_Top->hideTypeBet();
    this->User_Right_Top->hideTypeBet();
}

void LayerAvatarXiTo::SetIsBoss(const int &pos, bool isBoss)
{
//    AvatarXiTo* ava = this->GetUserByPos(pos);
//    if(ava != NULL){
//        ava->setMeIsBoss(isBoss);
//    }
}

bool LayerAvatarXiTo::SetListSpectToEmpty(string& list, const string& myAI){
    bool ret = false;

    this->ResetListSpect();
   // log("list spectator chau ria ============================== %s", list.c_str());

    if (list == "")
        return ret;

    vector<string> arr = mUtils::splitString(list, '-');
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
            lstSpec.push_back(ava);
        } else {
            break;
        }
    }
    return ret;
}
void LayerAvatarXiTo::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    if(ptrEventParamValueUser == NULL) return;
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    
    if (ptrNotifiedUser == NULL) return;
    
    double money = 0;
    if (ptrNotifiedUser->GetVariable(EXT_FIELD_GAME_CHIP) != NULL)
    money = *ptrNotifiedUser->GetVariable(EXT_FIELD_GAME_CHIP)->GetDoubleValue();
    
    std::string uid = *ptrNotifiedUser->Name();
    
   // log("Money of %s is %f", uid.c_str(), money);
    
    for (int i = 0; i < this->lstSpec.size(); i++) {
        if (std::strcmp(this->lstSpec[i]->getAI().c_str(),uid.c_str()) == 0) {
            this->lstSpec[i]->setMoney(money);
        }
    }
}
AvatarXiTo* LayerAvatarXiTo::GetEmptyAvatar(){

    if (this->User_IsMe->getAI() == "") {
        return this->User_IsMe;
    }

    if (this->User_Right_Bottom->getAI() == "") {
        return this->User_Right_Bottom;
    }

    if (this->User_Right_Top->getAI() == "") {
        return this->User_Right_Top;
    }

    if (this->User_Left_Top->getAI() == "") {
        return this->User_Left_Top;
    }

    if (this->User_Left_Bottom->getAI() == "") {
        return this->User_Left_Bottom;
    }

    return NULL;
}

void LayerAvatarXiTo::ResetListSpect(){
    if (this->User_IsMe->forGuess) {
        this->User_IsMe->ResetAll();
    }
    if (this->User_Left_Top->forGuess) {
        this->User_Left_Top->ResetAll();
    }
    if (this->User_Left_Bottom->forGuess) {
        this->User_Left_Bottom->ResetAll();
    }
    if (this->User_Right_Top->forGuess) {
        this->User_Right_Top->ResetAll();
    }
    if (this->User_Right_Bottom->forGuess) {
        this->User_Right_Bottom->ResetAll();
    }
    lstSpec.clear();
}

void LayerAvatarXiTo::showIconLeaveGame(bool state){
    this->User_IsMe->showIconLeaveGame(state);
}

void LayerAvatarXiTo::checkSameIp(){
    vector<Avatar*> arrs;
    arrs.push_back(this->User_IsMe);
    arrs.push_back(this->User_Left_Top);
    arrs.push_back(this->User_Left_Bottom);
    arrs.push_back(this->User_Right_Top);
    arrs.push_back(this->User_Right_Bottom);

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
                        if(ip == ip2 
							&&
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
