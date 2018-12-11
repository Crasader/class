#include "AvatarInGame3Doi.h"
#include "SceneManager.h"
#include "GameServer.h"
#include "mUtils.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"
LayerAvatarInGame3Doi::LayerAvatarInGame3Doi()
{
    
}

LayerAvatarInGame3Doi::~LayerAvatarInGame3Doi(){
   // log("Destructor LayerAvatarInGame3Doi");
}

bool LayerAvatarInGame3Doi::init() {
    if (!Layer::init())
        return false;
    isGuess = false;
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(Vec2(0, 0));
    this->setTag(tag_layer_avatar_52);
    this->typeGame = 0;
    this->myName = SceneManager::getSingleton().getMyName();
    this->myAI = SceneManager::getSingleton().getMyName();
    this->isSpector = isSpectator();
    // 	if (GameServer::getSingleton().getSmartFox()->MySelf() != NULL)
    // 		this->isSpector = GameServer::getSingleton().getSmartFox()->MySelf()->IsSpectator();
    
   // log("this->myName = SceneManager::getSingleton().getMyName() = %s", this->myName.c_str());
    Size sizeAdd = ChanUtils::getSizePos();
    Size winSize = Director::getInstance()->getVisibleSize();
    
    Avatar52La *me = Avatar52La::create(false, 1);
    Avatar52La *left = Avatar52La::create(false, 1);
    Avatar52La *right = Avatar52La::create(false, 1);
    Avatar52La *topleft = Avatar52La::create(false, 1);
    Avatar52La *topRight = Avatar52La::create(false, 1);
    Avatar52La *bottom = Avatar52La::create(false, 1);
    Avatar52La *bottomRight = Avatar52La::create(false, 1);
    
    me->setAnchorPoint(Vec2(0.5,0.5));
    left->setAnchorPoint(Vec2(0.5,0.5));
    right->setAnchorPoint(Vec2(0.5,0.5));
    topleft->setAnchorPoint(Vec2(0.5,0.5));
    topRight->setAnchorPoint(Vec2(0.5,0.5));
    bottom->setAnchorPoint(Vec2(0.5,0.5));
    bottomRight->setAnchorPoint(Vec2(0.5,0.5));
    
    
    
    left->setPosition(Vec2(AVATAR_L_SPACE + W_AVATAR + 20 + sizeAdd.width, winSize.height / 2 + SIZE_ADD-100));
    right->setPosition(Vec2(WIDTH_DESIGN - AVATAR_L_SPACE - 20 - W_AVATAR - sizeAdd.width, winSize.height / 2 + SIZE_ADD));
    topleft->setPosition(Vec2(WIDTH_DESIGN / 2-450, HEIGHT_DESIGN - W_AVATAR + sizeAdd.height-50));
    topRight->setPosition(Vec2(WIDTH_DESIGN / 2+250, HEIGHT_DESIGN - W_AVATAR + sizeAdd.height-50));
    bottom->setPosition(Vec2(WIDTH_DESIGN / 2 + sizeAdd.width-500, 200 + sizeAdd.height + SIZE_ADD-50));
    me->setPosition(Vec2(WIDTH_DESIGN / 2 + sizeAdd.width-500, 200 + sizeAdd.height + SIZE_ADD-50));;
    bottomRight->setPosition(Vec2(WIDTH_DESIGN / 2 + sizeAdd.width+500, 200 + sizeAdd.height + SIZE_ADD-50));

    bottom->setVisible(false);
    me->setVisible(false);
    me->setTag(kUserMe);
    left->setTag(kUserLeft);
    right->setTag(kUserRight);
    topleft->setTag(kUserTopLeft);
    topRight->setTag(kUserTopRight);
    bottom->setTag(kUserBot);
    bottomRight->setTag(kUserBotRight);

    
    this->addChild(left);
    this->addChild(right);
    this->addChild(topleft);
    this->addChild(topRight);
    this->addChild(bottom);
    this->addChild(bottomRight);
    this->addChild(me);
    
    
    
    return true;
}

void LayerAvatarInGame3Doi::setIcon(const int& pos, char *url){
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setIcon(url);
}

void LayerAvatarInGame3Doi::setFlag(const int& pos, bool isShow){
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setFlag(isShow);
}

void LayerAvatarInGame3Doi::setName(const int& pos, string name){
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setName(name);
}

void LayerAvatarInGame3Doi::setMoney(const int& pos, string money){
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame3Doi::setMoney(const int& pos, int money) {
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame3Doi::setMoney(const int& pos, double money) {
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setMoney(money);
}

void LayerAvatarInGame3Doi::setReady(const int& pos, bool isReady){
    if (this->getUserByPos(pos) == NULL)
        return;
    this->getUserByPos(pos)->setReady(isReady);
}

void LayerAvatarInGame3Doi::setVisibleLayerInvite(const int& pos, bool isShow) {
    Avatar52La *user = this->getUserByPos(pos);
    if (user != NULL)
        user->setVisibleLayerInvite(isShow);
}

void LayerAvatarInGame3Doi::setUnReadyAllUser() {
    getUserByPos(kUserMe)->removeResult();
    getUserByPos(kUserLeft)->removeResult();
    getUserByPos(kUserRight)->removeResult();
    getUserByPos(kUserTopLeft)->removeResult();
    getUserByPos(kUserTopRight)->removeResult();
    getUserByPos(kUserBotRight)->removeResult();
    getUserByPos(kUserBot)->removeResult();
}

void LayerAvatarInGame3Doi::resetGame() {
    // Ready status
    this->setUserCount(0);
    setUnReadyAllUser();
}

void LayerAvatarInGame3Doi::setListUserByPhom(string listUser) {
    //resetGame();
    this->typeGame = PHOM;
    this->listUser = listUser;
    const char c1 = PHOM_CHAR_SPLIT_1;
    const char c2 = PHOM_CHAR_SPLIT_2;
    formatAndStore(c1, c2);
}

void LayerAvatarInGame3Doi::setListUserByTienLen(string listUser) {
    this->typeGame = TIEN_LEN;
    this->listUser = listUser;
    this->isSpector = this->isSpectator();
    const char c1 = TL_CHAR_SPLIT_1;
    const char c2 = TL_CHAR_SPLIT_2;
    formatAndStore(c1, c2);
}

void LayerAvatarInGame3Doi::formatAndStore(const char &c1, const char &c2) {
    char1 = c1;
    char2 = c2;
    vector<string> arrUsers = mUtils::splitString(this->listUser, c1);
    
    // release arr
    arrName.clear();
    arrFlag.clear();
    arrURL.clear();
    arrAI.clear();
    arrMoney.clear();
    arrMoneyDouble.clear();
    arrRD.clear();
    int size = arrUsers.size();
    for (int i = 0; i < size; i++) {
        vector<string> arr = mUtils::splitString(arrUsers[i], c2);
        if (arr.size() < 3)
        {
            continue;
        }
        string id = arr[0];
        string name = arr[1];
        string flag = arr[2];
        //if(typeGame==TIEN_LEN){
        string rd = arr[3];
        arrRD.push_back(rd == "1" ? true : false);
        
        //}
        arrFlag.push_back(flag);
        
        // url icon - moc3-change aN=>adn
        boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(name);
        
        if (userInfo == NULL
            || userInfo->GetVariable("aal") == NULL
            || userInfo->GetVariable("amf") == NULL
            || userInfo->GetVariable("amfs") == NULL
            || userInfo->GetVariable("adn") == NULL){
            arrAI.push_back(name);
            arrName.push_back(name);
            arrURL.push_back("");
            arrMoney.push_back(0);
            arrMoneyDouble.push_back(-1);//danh cho truong hop bi disconnect
            continue;
        }
        
        // url
        boost::shared_ptr<string> url = userInfo->GetVariable("aal")->GetStringValue();
        arrURL.push_back(url->c_str());
        
        // Money
        if (SceneManager::getSingleton().getCurrRoomType() == 0){
            boost::shared_ptr<double> amf = userInfo->GetVariable("amf")->GetDoubleValue();
            arrMoneyDouble.push_back(*amf);
            arrMoney.push_back(((int)(*amf)));
        }else{
            boost::shared_ptr<double> amf = userInfo->GetVariable("amfs")->GetDoubleValue();
            arrMoneyDouble.push_back(*amf);
            arrMoney.push_back(((int)(*amf)));
        }
        
        // account ID
        boost::shared_ptr<string> aI = userInfo->Name();
        arrAI.push_back(aI->c_str());
        
        // account Name- moc3-change aN=>adn
        boost::shared_ptr<string> aN = userInfo->GetVariable("adn")->GetStringValue();
        if (aN != NULL){
            if (strcmp(aN->c_str(), "") == 0)
                arrName.push_back(name);
            else
                arrName.push_back(aN->c_str());
        }
        // 		if (aN->compare("") == 0)
        // 			arrName.push_back(aI->c_str());
        // 		else
        // 			arrName.push_back(aN->c_str());
    }
    
    updateUsers();
}

Avatar52La* LayerAvatarInGame3Doi::getUserByPos(const int& pos){
    if (pos < 0)
        return NULL;
    if (this->getChildByTag(pos) == NULL)
        return NULL;
    return (Avatar52La*) this->getChildByTag(pos);
}

int LayerAvatarInGame3Doi::getIndexInArrByName(string& name) const {
    for (int i = 0; i < (int)arrName.size(); i++) {
        if (strcmp(arrName[i].c_str(), name.c_str()) == 0)
            return i;
    }
    return -1;
}

int LayerAvatarInGame3Doi::getIndexInArrByAccountID(string aI) {
    for (int i = 0; i < (int)arrAI.size(); i++) {
        if (strcmp(arrAI[i].c_str(), aI.c_str()) == 0) return (i < 6 ? i : -1); // trả lại giá trị -1 nếu tên nằm ngoài danh sách 4 người đầu tiên
    }
    return -1;
}

int LayerAvatarInGame3Doi::getPosByName(string pName) {
    
    return getPosByAccountID(pName);
    
    //
    int pos = getIndexInArrByName(this->myName);
    
    if (pos == -1)
    {
        isGuess = true;
        //return (getIndexInArrByName(pName));
        pos = getIndexInArrByName(pName);
        switch (pos) {
            case 0:
                return kUserBot;
            case 1:
                return kUserBotRight;
            case 2:
                return kUserRight;
            case 3:
                return kUserTopRight;
            case 4:
                return kUserTopLeft;
            case 5:
                return kUserLeft;
        }
        
        return -1;
    }
    // tra lai vi tri
    else{
        isGuess = false;
        int countUser = arrName.size();
        for (int i = 0; i < countUser; i++) {
            if (strcmp(arrName[i].c_str(), pName.c_str()) == 0) {
                if (i == pos) {
                    return kUserMe;
                }
                else if (i == (pos + 2) % countUser) {
                    return kUserRight;
                }
                else if (i == (pos + 3) % countUser) {
                    return kUserTopRight;
                }
                else if (i == (pos + 4) % countUser) {
                    return kUserTopLeft;
                }
                else if (i == (pos + 5) % countUser) {
                    return kUserLeft;
                }
                else if (i == (pos + 1) % countUser) {
                    return kUserBotRight;
                }
                
                break;
            }
        }
    }
    
    return -1;
}

int LayerAvatarInGame3Doi::getPosByAccountID(string aI) {
    int pos = getIndexInArrByAccountID(this->myAI);
    //log("getPosByAccountID aI=%s, myAI=%s, pos=%d", aI.c_str(), myAI.c_str(), pos);
    
//    kUserLeft = 0,
//    kUserRight, // 1
//    kUserTop,   // 2
//    kUserBot,   // 3
//    kUserMe,     // 4
//    kUserTopLeft,   // 5
//    kUserTopRight,   // 6
//    kUserBotRight   //7
    
    if (pos == -1)
    {
        isGuess = true;
        //return (getIndexInArrByName(pName));
        pos = getIndexInArrByAccountID(aI);
        switch (pos) {
            case 0:
                return kUserBot;
            case 1:
                return kUserBotRight;
            case 2:
                return kUserRight;
            case 3:
                return kUserTopRight;
            case 4:
                return kUserTopLeft;
            case 5:
                return kUserLeft;
        }
        
        return -1;
    }
    // tra lai vi tri
    else{
        isGuess = false;
        int countUser = arrAI.size();
        for (int i = 0; i < countUser; i++) {
            if (strcmp(arrAI[i].c_str(), aI.c_str()) == 0) {
                if (i == pos) {
                    return kUserMe;
                }
                else if (i == (pos + 1) % countUser) {
                    return kUserBotRight;
                }
                else if (i == (pos + 2) % countUser) {
                    return kUserRight;
                }
                else if (i == (pos + 3) % countUser) {
                    return kUserTopRight;
                }
                else if (i == (pos + 4) % countUser) {
                    return kUserTopLeft;
                }
                else if (i == (pos + 5) % countUser) {
                    return kUserLeft;
                }
               
                
                break;
            }
        }
    }
    
    return -1;
}

string LayerAvatarInGame3Doi::getNameByPos(int pPos) {
    int pos = getIndexInArrByName(this->myName);
    
    if (pos == -1) {
        this->isGuess = true;
        if (pPos < (int)arrName.size() && pPos >= 0) return arrName.at(pPos);
    }
    else {
        this->isGuess = false;
        int countUser = arrName.size();
        switch (pPos) {
            case kUserMe:
                return this->myName;
                break;
                
            case kUserRight:
                return arrName[(pPos + 1) % countUser];
                break;
                
            case kUserTop:
                return arrName[(pPos + 2) % countUser];
                break;
                
            case kUserLeft:
                return arrName[(pPos + 3) % countUser];
                break;
        }
    }
    
    return "";
}

string LayerAvatarInGame3Doi::getAccountIDByPos(int pPos) {
    int pos = getIndexInArrByAccountID(this->myAI);
    
    if (pos == -1) {
        this->isGuess = true;
        if (pPos < (int)arrAI.size() && pPos >= 0) return arrAI.at(pPos);
    }
    else {
        this->isGuess = false;
        int countUser = arrAI.size();
        switch (pPos) {
            case kUserMe:
                return this->myAI;
                break;
            case kUserRight:
                return arrAI[(pPos + 1) % countUser];
                break;
                
            case kUserTop:
                return arrAI[(pPos + 2) % countUser];
                break;
                
            case kUserLeft:
                return arrAI[(pPos + 3) % countUser];
                break;
        }
    }
    
    return "";
}

void LayerAvatarInGame3Doi::updateUsers() {
    if (arrName.size() != arrFlag.size() || arrName.size() != arrURL.size() || arrURL.size() != arrAI.size()) {
        return;
    }
    
    // Mỗi khi nhảy tới đây thì cần làm 2 việc sau:
    // 1. Làm "sạch" toàn bộ các Avatar:
    //   1.1 Avatar của mình: ẩn đi,
    //						  trạng thái ready=false, BtnReady -> title="Sẵn sàng", -> visible=false???
    //                        ai="";
    //	 1.2 Avatar user khác hiện lên các InviteUser, ready=false
    //   1.3 Toàn bộ thông tin về tiền được reset
    // 2. Cập nhật lại thông tin:
    //   2.0:
    //	 2.1: nếu của mình thì hiện lên Avatar, BtnReady->visible=true
    //   2.2: các user khác thì cập nhật & ẩn đi Invite
    // ---
    
    // 1. Làm "sạch"
    Avatar52La *avaMe = getUserByPos(kUserMe);
    Avatar52La *avaLeft = getUserByPos(kUserLeft);
    Avatar52La *avaRight = getUserByPos(kUserRight);
    Avatar52La *avaTopLeft = getUserByPos(kUserTopLeft);
    Avatar52La *avaTopRight = getUserByPos(kUserTopRight);
    Avatar52La *avaBot = getUserByPos(kUserBot);
    Avatar52La *avaBotRight = getUserByPos(kUserBotRight);

    // 	avaMe->setVisible(false);
    // 	//avaMe->setReady(false);
    // 	avaMe->setName("");
    // 	avaMe->setMoney("");
    // 	avaMe->setAI("");
    /*Button *btnReady=NULL;
     LayerPlayGamePhom *layerGamePhom;
     LayerPlayGameTLMN *layerGameTLMN;
     switch (this->typeGame) {
     case PHOM:
     layerGamePhom = SceneManager::getSingleton().getLayerGamePhom();
     btnReady = layerGamePhom->getButtonByTag(kTagButtonReady);
     break;
     case TIEN_LEN:
     layerGameTLMN = SceneManager::getSingleton().getLayerGameTLMN();
     btnReady = layerGameTLMN->getButtonByTag(kTagButtonReady);
     break;
     }
     if (btnReady == NULL) return;
     btnReady->setTitleText("Sẵn sàng");
     btnReady->setEnabled(false);*/
    
    avaLeft->setVisibleLayerInvite(true);
    //avaLeft->setReady(false);
    avaLeft->setName("");
    avaLeft->setMoney("");
    avaLeft->setAI("");
    avaLeft->showSameIP(false);
    //
    avaRight->setVisibleLayerInvite(true);
    //avaRight->setReady(false);
    avaRight->setName("");
    avaRight->setMoney("");
    avaRight->setAI("");
    avaRight->showSameIP(false);
    //
    avaTopLeft->setVisibleLayerInvite(true);
    //avaTop->setReady(false);
    avaTopLeft->setName("");
    avaTopLeft->setMoney("");
    avaTopLeft->setAI("");
    avaTopLeft->showSameIP(false);
    
    avaTopRight->setVisibleLayerInvite(true);
    //avaTop->setReady(false);
    avaTopRight->setName("");
    avaTopRight->setMoney("");
    avaTopRight->setAI("");
    avaTopRight->showSameIP(false);
    //
    avaBot->setVisibleLayerInvite(true);
    //avaBot->setReady(false);
    avaBot->setName("");
    avaBot->setMoney("");
    avaBot->setAI("");
    avaBot->showSameIP(false);
    
    
    avaBotRight->setVisibleLayerInvite(true);
    //avaBot->setReady(false);
    avaBotRight->setName("");
    avaBotRight->setMoney("");
    avaBotRight->setAI("");
    avaBotRight->showSameIP(false);
    
    
    avaLeft->setVisbleLabelGuess(false);
    avaTopLeft->setVisbleLabelGuess(false);
    avaTopRight->setVisbleLabelGuess(false);
    avaRight->setVisbleLabelGuess(false);
    avaBotRight->setVisbleLabelGuess(false);
    avaMe->setVisbleLabelGuess(false);
    avaBot->setVisbleLabelGuess(false);

    avaLeft->setFlag(false);
    avaTopLeft->setFlag(false);
    avaTopRight->setFlag(false);
    avaRight->setFlag(false);
    avaBot->setFlag(false);
    avaMe->setFlag(false);
    avaBotRight->setFlag(false);

    // 2. Update info :D
    int length = arrName.size();
    this->setUserCount(length);
    int gameID = SceneManager::getSingleton().getGameID();
    for (int i = 0; i < length; i++) {
        string name = arrName[i];
        string flag = arrFlag[i];
        string url = arrURL[i];
        string aI = arrAI[i];
        int money = arrMoney[i];
        double moneyDouble = arrMoneyDouble.at(i);
        
        int pos = getPosByAccountID(aI);

        if (pos < 0)
            continue;
        if (!isGuess){ //neu minh la thang choi
            getUserByPos(kUserBot)->setVisible(false);
            getUserByPos(kUserMe)->setVisible(true);
            //getUserByPos(kUserBot)->setPositionY(-200);
        }
        
        else {
            getUserByPos(kUserBot)->setVisible(true);
            //getUserByPos(kUserBot)->//(true);
            getUserByPos(kUserMe)->setFlag(false);
        }
        Avatar52La *user = getUserByPos(pos);
        user->setName(name);
        user->showSameIP(false);
        //user->setFlag(atoi(flag.c_str()) == 1);
        //
        bool meIsBoss = (atoi(flag.c_str()) == 1);
        user->hideLayerInvite();
        user->setIcon(url);
        user->setAI(aI);
        //user->setMoney(money);
        user->setMoney(moneyDouble);
        if (pos == kUserMe){
            this->getUserByPos(kUserLeft)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kUserRight)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kUserBot)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kUserBotRight)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kUserTopLeft)->setMeIsBoss(meIsBoss);
            this->getUserByPos(kUserTopRight)->setMeIsBoss(meIsBoss);
            
        }
    }
    if (SceneManager::getSingleton().getGameID() == kGameMauBinh){
        if (isGuess){
            getUserByPos(kUserMe)->setVisible(false);
        }else{
            showAvatarMe(kUserMe, myName);
            
        }
    }else{
        showAvatarMe(kUserMe, myName);
    }
    //check same ip and show up
    checkSameIP();
}

void LayerAvatarInGame3Doi::runTimer(const int& posUser) {
    stopAllTimer();
    Avatar52La *avatar = getUserByPos(posUser);
    if (avatar == NULL) return;
    avatar->startTimer();
}

void LayerAvatarInGame3Doi::stopAllTimer() {
    getUserByPos(kUserMe)->stopTimer();
    getUserByPos(kUserLeft)->stopTimer();
    getUserByPos(kUserRight)->stopTimer();
    getUserByPos(kUserTopLeft)->stopTimer();
    getUserByPos(kUserTopRight)->stopTimer();
    getUserByPos(kUserBotRight)->stopTimer();
    getUserByPos(kUserBot)->stopTimer();
}

bool LayerAvatarInGame3Doi::isSpectator() {
    if (listUser.size() == 0) return true;
    vector<string> arr = mUtils::splitString(this->listUser, char1);
    int size = arr.size();
    for (int i = 0; i < size; i++){
        vector<string> arrInfo = mUtils::splitString(arr.at(i), char2);
        if (arrInfo.size() < 2) continue;
        string ai = arrInfo.at(1);
        if (strcmp(ai.c_str(), myAI.c_str()) == 0) return false;
    }
    
    return true;
}

void LayerAvatarInGame3Doi::resetAll(){
    // 	setFlag(kUserMe, false);
    // 	setFlag(kUserLeft, false);
    // 	setFlag(kUserRight, false);
    // 	setFlag(kUserTop, false);
    //
    // 	setName(kUserMe, "");
    // 	setName(kUserLeft, "");
    // 	setName(kUserRight, "");
    // 	setName(kUserTop, "");
    //
    // 	setMoney(kUserMe, "");
    // 	setMoney(kUserLeft, "");
    // 	setMoney(kUserRight, "");
    // 	setMoney(kUserTop, "");
    //
    // 	this->getUserByPos(kUserLeft)->setVisibleLayerInvite(true);
    // 	this->getUserByPos(kUserRight)->setVisibleLayerInvite(true);
    // 	this->getUserByPos(kUserTop)->setVisibleLayerInvite(true);
    //
    // 	this->getUserByPos(kUserLeft)->hideTypeVip();
    // 	this->getUserByPos(kUserRight)->hideTypeVip();
    // 	this->getUserByPos(kUserTop)->hideTypeVip();
    //
    // 	this->getUserByPos(kUserLeft)->setVisbleLabelGuess(false);
    // 	this->getUserByPos(kUserRight)->setVisbleLabelGuess(false);
    // 	this->getUserByPos(kUserTop)->setVisbleLabelGuess(false);
    // 	this->getUserByPos(kUserBot)->setVisbleLabelGuess(false);
    // 	this->getUserByPos(kUserMe)->setVisbleLabelGuess(false);
    getUserByPos(kUserLeft)->resetAll();
    getUserByPos(kUserRight)->resetAll();
    getUserByPos(kUserTopLeft)->resetAll();
    getUserByPos(kUserTopRight)->resetAll();
    getUserByPos(kUserBotRight)->resetAll();
    getUserByPos(kUserBot)->resetAll();
    getUserByPos(kUserMe)->resetAll();
}

bool LayerAvatarInGame3Doi::isStartedGame()
{
    boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (room == NULL)
        return false;
    boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
    if (rv == NULL)
        return false;
    boost::shared_ptr<string> rv_ptr = rv->GetStringValue();
    if (rv_ptr == NULL)
        return false;
    string s = rv_ptr->c_str();
    vector<string> lstBet = mUtils::splitString(s, '@');
    bool isStartedGame = false;
    lstBet.at(1).compare("1") == 0 ? (isStartedGame = true) : (isStartedGame = false);
    if (isStartedGame) {
       // log("Ban dang choi!");
    }
    else log("Ban chua choi!");
    
    return isStartedGame;
}

void LayerAvatarInGame3Doi::stopAllBlinkAvatar(){
    this->getUserByPos(kUserLeft)->removeChildByTag(20);
    this->getUserByPos(kUserRight)->removeChildByTag(20);
    this->getUserByPos(kUserTopLeft)->removeChildByTag(20);
    this->getUserByPos(kUserTopRight)->removeChildByTag(20);
    this->getUserByPos(kUserBotRight)->removeChildByTag(20);
    this->getUserByPos(kUserBot)->removeChildByTag(20);
}

void LayerAvatarInGame3Doi::runTimerWithTime(const int& posUser, const float& time)
{
    stopAllTimer();
    Avatar52La *avatar = getUserByPos(posUser);
    if (avatar == NULL) return;
    avatar->startTimer(time);
}

void LayerAvatarInGame3Doi::showAvatarMe(int tag, string userName)
{
    Avatar52La *avaMe = getUserByPos(tag);
    if (avaMe->getUserName().size() > 0)//da co thong tin
        return;
    
    boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(userName);
    
    if (userInfo == NULL){
        return;
    }
    
    // url
    boost::shared_ptr<UserVariable> url = userInfo->GetVariable("aal");
    // Money
    boost::shared_ptr<UserVariable> amf = userInfo->GetVariable(EXT_FIELD_USER_AMF_GOLD);
    //update 21/8
    
    int currType = SceneManager::getSingleton().getCurrRoomType();
    if (currType == 1)
    {
        amf = userInfo->GetVariable(EXT_FIELD_USER_AMF_SILVER);
    }
    
    // account ID
    if (url != NULL && amf != NULL)
    {
        avaMe->hideLayerInvite();
        avaMe->setIcon(url->GetStringValue()->c_str());
        avaMe->setAI(userName);
        avaMe->setMoney(*amf->GetDoubleValue());
        /*moc3-change aN=>adn*/
        boost::shared_ptr<UserVariable> aN = userInfo->GetVariable("adn");
        if (aN != NULL){
            if (strcmp(aN->GetStringValue()->c_str(), "") == 0)
                avaMe->setName(userName);
            else
                avaMe->setName(aN->GetStringValue()->c_str());
        }
        else{
            avaMe->setName(userName);
        }
        
    }
    
}

void LayerAvatarInGame3Doi::removeUserFromList(string _name)
{
    Avatar52La *user = getUserByPos(this->getPosByName(_name));
    if (user == NULL) return;
    user->stopTimer();
    user->setVisibleLayerInvite(true);
    user->setName("");
    user->setMoney("");
    user->setAI("");
}

void LayerAvatarInGame3Doi::setResult(int pos, int rescode, int type, bool isOnGame, double money)
{
    Avatar52La *user = getUserByPos(pos);
    if (user == NULL)
        return;
    user->setResult(rescode, type, isOnGame, money);
}

void LayerAvatarInGame3Doi::setListUserGuess(string _list, char charSplit)
{
    if (this->_count > 6) return;
    if (strcmp(_list.c_str(), "") == 0)
    {
        this->resetGuessUser();
        return;
    }
    vector<string> arrUser = mUtils::splitString(_list, charSplit);
    this->setGuessCount(arrUser.size());
    int length = arrUser.size() > (6 - this->_count) ? 6 - this->_count : arrUser.size();
    if (length > 0){
        this->resetGuessUser();
    }
    for (int i = 0; i < length; i++)
    {
        vector<string> info = mUtils::splitString(arrUser.at(i), ':');
        if (info.size() >1)
        {
            string userName = info.at(1);
            Avatar52La *userFree = this->getEmptyAvatar();
            if (userFree != NULL)
            {
                int tag = userFree->getTag();
                this->showAvatarMe(tag, userName);
                userFree->setVisbleLabelGuess(true);
            }
        }
        
    }
}

Avatar52La* LayerAvatarInGame3Doi::getEmptyAvatar()
{
    for (int i = 10; i >= 0; i--){
        Avatar52La* user = this->getUserByPos(i);
        if (user == NULL || i == kUserBot) continue;
        if (strcmp(user->getAI().c_str(), "") == 0)
            return user;
    }
    return NULL;
}

void LayerAvatarInGame3Doi::resetGuessUser()
{
    for (int i = 0; i < 10; i++)
    {
        Avatar52La *user = this->getUserByPos(i);
        if (user == NULL) continue;
        if (user->isChauRia())
        {
            user->setVisibleLayerInvite(true);
            user->setName("");
            user->setMoney("");
            user->setAI("");
        }
    }
    
}

void LayerAvatarInGame3Doi::checkSameIP()
{
    for (int i = 0; i < arrAI.size(); i++)
    {
        for (int k = i + 1; k < arrAI.size(); k++)
        {
            int pos1 = this->getPosByName(arrAI.at(i));
            int pos2 = this->getPosByName(arrAI.at(k));
            Avatar52La* user1 = getUserByPos(pos1);
            Avatar52La* user2 = getUserByPos(pos2);
            if (user1 && user2)
            {
                string username1 = user1->getAI();
                string username2 = user2->getAI();
                if (0 != username1.compare(username2))
                {
                    string ip1 = user1->getUserIP();
                    string ip2 = user2->getUserIP();
                    if (0 == ip1.compare(ip2)
                        &&
                        0 != ip1.compare("")
                        &&
                        0 != ip1.compare("undefined")
                        &&
                        0 != ip2.compare("")
                        &&
                        0 != ip2.compare("undefined")
                        )
                    {
                        user1->showSameIP(true);
                        user2->showSameIP(true);
                    }
                }
            }
        }
    }
}
