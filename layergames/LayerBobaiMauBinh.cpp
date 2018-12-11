//
//  LayerBobaiMauBinh.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/20/17.
//
//

#include "LayerBobaiMauBinh.h"
#include "mUtils.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"
#include "../AllData.h"
#include "LayerGameMauBinh.h"
#include "AvatarInGame52La.h"
#include "../SceneManager.h"
#include "LayerKetQuaMauBinh.h"
#include "Avatar52La.h"
#include "ThangTrangMauBinh.h"
LayerBoBaiMauBinh::LayerBoBaiMauBinh(){
    
}
LayerBoBaiMauBinh::~LayerBoBaiMauBinh(){
    
}
bool LayerBoBaiMauBinh::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(Vec2(0, 0));
    this->setContentSize(Size(WIDTH_DESIGN,HEIGHT_DESIGN));
    Size sizeAdd = ChanUtils::getSizePos();
    Size winSize = Director::getInstance()->getVisibleSize();
    BaiMauBinh *left = BaiMauBinh::create();
    BaiMauBinh *right = BaiMauBinh::create();
    BaiMauBinh *top = BaiMauBinh::create();
    BaiMauBinh *bottom = BaiMauBinh::create();
    BaiMauBinh *me = BaiMauBinh::create();
    
    left->setLocalZOrder(15);
    right->setLocalZOrder(15);
    top->setLocalZOrder(15);
    bottom->setLocalZOrder(15);
    me->setLocalZOrder(15);
    
    
    //    left->setAnchorPoint(Vec2(0.5,0.5));
    //    right->setAnchorPoint(Vec2(0.5,0.5));
    //    top->setAnchorPoint(Vec2(0.5,0.5));
    //    bottom->setAnchorPoint(Vec2(0.5,0.5));
    //    me->setAnchorPoint(Vec2(0.5,0.5));
    
    
    left->setPosition(Vec2(AVATAR_L_SPACE + W_AVATAR + 190 + sizeAdd.width, winSize.height / 2 -250+ SIZE_ADD));
    right->setPosition(Vec2(WIDTH_DESIGN - 350 - right->getContentSize().width, winSize.height / 2 -300+ SIZE_ADD+50));
    top->setPosition(Vec2(WIDTH_DESIGN / 2-100, HEIGHT_DESIGN - W_AVATAR + sizeAdd.height-250));
    bottom->setPosition(Vec2(WIDTH_DESIGN / 2 -100,  sizeAdd.height + SIZE_ADD));
    me->setPosition(Vec2(WIDTH_DESIGN / 2 -100,  sizeAdd.height + SIZE_ADD));
    
    left->setTag(kUserLeft);
    right->setTag(kUserRight);
    top->setTag(kUserTop);
    bottom->setTag(kUserBot);
    me->setTag(kUserMe);
    
//    left->setVisible(false);
//    right->setVisible(false);
//    top->setVisible(false);
//    bottom->setVisible(false);
//    me->setVisible(false);
    
    
//    ThangTrangMauBinh* thangtrang = ThangTrangMauBinh::create();
//    thangtrang->setBoBai(this->convertListCard("51,50,52,42,38,54,53,24,21,58,33,26,14,"));
//    thangtrang->setThangTrangType(6);
//    thangtrang->setName("wẻwerwerwefwewe");
//    this->addChild(thangtrang,20,this->tag_thang_trang);
   
    this->addChild(left);
    this->addChild(right);
    this->addChild(top);
    this->addChild(bottom);
    this->addChild(me);
    
    this->txtSoChi = Label::createWithSystemFont("", "", FONT_SIZE_NORMAL,
                                                 Size(400, 200),
                                                 TextHAlignment::CENTER, TextVAlignment::CENTER);
    this->txtSoChi->setAnchorPoint(Vec2(0.5, 0.5));
    this->txtSoChi->setPosition(WIDTH_DESIGN/2,
                                HEIGHT_DESIGN/2 + sizeAdd.height);
    this->txtSoChi->setColor(Color3B(255, 165, 0));
    this->txtSoChi->setVisible(false);
    this->addChild( this->txtSoChi);
    if (SceneManager::getSingleton().isNagaWin){
        this->txtSoChi->setColor(Color3B::WHITE);
    }
    
    
    this->clock  = ImageView::create("clock.png");
    this->clock->setPosition(Vec2(WIDTH_DESIGN/2,HEIGHT_DESIGN/2+50+sizeAdd.height ));
    this->clock->setAnchorPoint(Vec2(0.5,0.5));
    this->clock->setScale(0.8);
    //this->addChild(this->clock);
    
    
    this->imgTime = ImageView::create(StringUtils::format("MauBinh/%sthoi-gian-xep-bai.png",SceneManager::getSingleton().getFolderLang().c_str()));
    this->imgTime->setPosition(Vec2(WIDTH_DESIGN/2,HEIGHT_DESIGN/2+30+sizeAdd.height ));
    this->imgTime->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(this->imgTime);
    
    
    this->txtTimer = Label::createWithBMFont("font_countdown.fnt", "20", TextHAlignment::CENTER);
    this->txtTimer->setAnchorPoint(Vec2(0.5, 0.5));
    this->txtTimer->setPosition(Vec2(650,305));
    this->imgTime->addChild(this->txtTimer);
    this->imgTime->setVisible(false);
    
    return true;
}
void LayerBoBaiMauBinh::runTimerReady(int _time){
    this->removeChildByTag(this->tag_ket_qua);
    //    auto ketqua = (LayerKetQuaMauBinh*)this->getChildByTag(this->tag_ket_qua);
    //    if (ketqua){
    //        ketqua->startReady(_time);
    //    }
}
void LayerBoBaiMauBinh::removeTimer(){
    this->removeChildByTag(this->tag_ket_qua);
}

BaiMauBinh* LayerBoBaiMauBinh::getBoBaiByPos(int pos){
    if (pos == -1)
        return NULL;
    BaiMauBinh* bobai = (BaiMauBinh*)this->getChildByTag(pos);
    return bobai;
}
void LayerBoBaiMauBinh::actionDealCard(string list){
    this->resetGame();
    vector<string> lstuid = mUtils::splitString(list, ';');
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    for (int i = 0; i < lstuid.size(); i++) {
        auto lstcard = mUtils::splitString(lstuid[i],'|');
        if (lstcard.size()<2)
            return;
        BaiMauBinh* bobai = this->getBoBaiByPos(layerAvatar->getPosByName(lstcard[0]));
        this->giveCardByPos(layerAvatar->getPosByName(lstcard[0]));
        if (bobai != NULL){
            // this->addChild(xepbai);
            if (this->IsMee(lstcard[0])){
                bobai->setIsme(true);
                this->myListIDCard = lstcard[1];
                this->myListCard = this->convertListCard(lstcard[1]);
            }
        }
    }
    //
}
void LayerBoBaiMauBinh::playSound(int type)
{
    if (!mUtils::isSoundOn()) return;
    string soundPath;
    switch (type)
    {
        case 103:
            soundPath = "thang_roi_1.mp3";
            break;
        case 104:
            soundPath = "thua_roi_1.mp3";
            break;
        case 102:
            soundPath = "ring_ring.mp3";
            break;
        case 100:
            soundPath = "chia_bai.mp3";
            break;
        default:
            break;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}
void LayerBoBaiMauBinh::giveCardByPos(int pos){
    if (pos == -1)
        return;
    Size sizeAdd = ChanUtils::getSizePos();
    
    for (int i = 0; i< 13; i ++){
        Sprite* sp = Sprite::createWithSpriteFrameName("card_back.png");
        sp->setAnchorPoint(Vec2(0.5,0.5));
        sp->setPosition(Vec2(WIDTH_DESIGN/2,HEIGHT_DESIGN/2 + sizeAdd.height));
        sp->setScale(0.75);
        sp->setLocalZOrder(30/ ((i)/5+1)  + (i)%5 );
        sp->setTag(458+i);
        
        this->addChild(sp);
        
        sp->runAction(Sequence::create(DelayTime::create(0.1*i),RotateTo::create(0.1, this->getBoBaiByPos(pos)->getRotationXByID(i), this->getBoBaiByPos(pos)->getRotationYByID(i)),MoveTo::create(0.1,Vec2(this->getChildByTag(pos)->getPositionX() + this->getBoBaiByPos(pos)->getPosByID(i).x, this->getChildByTag(pos)->getPositionY() + this->getBoBaiByPos(pos)->getPosByID(i).y)),NULL));
        this->playSound(100);
    }
    
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::XoeBoBai, this,pos));
    this->runAction(Sequence::create(DelayTime::create(0.1),callfunc,NULL));
}
void LayerBoBaiMauBinh::XoaBaiChia(){
    for (int i = 0;i<13;i++){
        this->removeChildByTag(458+i);
    }
}
void LayerBoBaiMauBinh::XoeBoBai(int pos){
    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
    if (bobai)
    {
        bobai->setVisible(true);
        bobai->dealCard(this->myListCard);
        if (bobai->getIsme()){
            bobai->setScale(1.3);
            CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::showXepBaiMauBinh, this));
            this->runAction(Sequence::create(DelayTime::create(0.15*13),callfunc,NULL));
        }
    }
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::startTimerAvatar, this,pos));
    this->runAction(Sequence::create(DelayTime::create(0.1*14),callfunc,NULL));
}
void LayerBoBaiMauBinh::hideClock(){
    this->imgTime->setVisible(false);
}

void LayerBoBaiMauBinh::startTimerAvatar(int pos){
    if (pos == -1)
        return;
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    if (this->isSpector)
        this->imgTime->setVisible(true);
    
    layerAvatar->runTimerBinh(pos, this->timeXepBai);
    
}
void LayerBoBaiMauBinh::LatBaiHoa(string lstcard){
    this->imgTime->setVisible(false);
    this->timeXepBai = 0;
    vector<string> lstuid = mUtils::splitString(lstcard, ';');
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    for (int i = 0; i < lstuid.size(); i++) {
        auto lstcard = mUtils::splitString(lstuid[i],'|');
        if (lstcard.size()<2)
            return;
        BaiMauBinh* bobai = this->getBoBaiByPos(layerAvatar->getPosByName(lstcard[0]));
        if (bobai != NULL){
            bobai->reloadBoBai(this->convertListCard(lstcard[1]));
            bobai->latbai();
            bobai->runAction(ScaleTo::create(0.2, 1.3));
        }
    }
}
void LayerBoBaiMauBinh::latBai(string lst){
    this->imgTime->setVisible(false);
    vector<string> lstuid = mUtils::splitString(lst, ';');
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    for (int i = 0; i < lstuid.size(); i++) {
        auto lstcard = mUtils::splitString(lstuid[i],'|');
        if (lstcard.size()<2)
            return;
        BaiMauBinh* bobai = this->getBoBaiByPos(layerAvatar->getPosByName(lstcard[0]));
        if (bobai != NULL){
            bobai->reloadBoBai(this->convertListCard(lstcard[1]));
        }
    }
}
void LayerBoBaiMauBinh::showXepBaiMauBinh() {
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent){
        parent->hideCuoc();
    }
    
    this->hideClock();
    
    this->xepbai = MauBinhLayerXepBai::create();
    
    this->xepbai->setListCardID(this->myListIDCard);
    this->xepbai->setListCard(convertListXepBai(this->myListIDCard));
    this->xepbai->setTag(this->tag_xep_bai);
    this->xepbai->setTimeXepBai(this->timeXepBai);
    this->xepbai->setLocalZOrder(20);
    this->xepbai->setScale(0);
//    this->xepbai->setOrgPoint(this->getBoBaiByPos(kUserMe)->getPosition());
//    this->xepbai->setPointDone(this->getBoBaiByPos(kUserMe)->getListPos());
//    this->xepbai->setSkewXDone(this->getBoBaiByPos(kUserMe)->getListSkewX());
//    this->xepbai->setSkewYDone(this->getBoBaiByPos(kUserMe)->getListSkewY());

    this->xepbai->runAction(ScaleTo::create(0.2,1));
    this->addChild(this->xepbai);
    this->isXepBai = true;
    this->setLocalZOrder(150);
    this->getBoBaiByPos(kUserMe)->setVisible(false);
    //this->getBoBaiByPos(kUserMe)->runActionXepBai(this->xepbai->getPositionCard());
    
}
void LayerBoBaiMauBinh::setIsSpector(bool _is){
    this->isSpector = _is;
}

void LayerBoBaiMauBinh::hideXepBaiMauBinh(){
    this->removeChildByTag(this->tag_xep_bai);
}
bool LayerBoBaiMauBinh::IsMee(string name){
    if (name == SceneManager::getSingleton().getMyName())
        return true;
    return false;
}
void LayerBoBaiMauBinh::resetGame(){
    this->stopAllActions();
    this->removeChildByTag(this->tag_xep_bai);
    this->removeChildByTag(this->tag_ket_qua);
    
    BaiMauBinh* me = (BaiMauBinh*)this->getChildByTag(kUserMe);
    BaiMauBinh* left = (BaiMauBinh*)this->getChildByTag(kUserLeft);
    BaiMauBinh* right = (BaiMauBinh*)this->getChildByTag(kUserRight);
    BaiMauBinh* bot = (BaiMauBinh*)this->getChildByTag(kUserBot);
    BaiMauBinh* top = (BaiMauBinh*)this->getChildByTag(kUserTop);
    this->txtTimer->stopAllActions();
    this->txtTimer->setOpacity(255);
    me->setVisible(false);
    this->removeChildByTag(this->tag_thang_trang);
    me->resetGame();
    this->isSpector = false;
    
    left->setVisible(false);
    left->resetGame();
    
    right->setVisible(false);
    right->resetGame();
    
    bot->setVisible(false);
    bot->resetGame();
    
    top->setVisible(false);
    top->resetGame();
    
    this->txtSoChi->setVisible(false);
    this->_isAnTrang = false;
    this->_isAnTrangThat = false;
    this->isMe = false;
    this->myListCard = "";
    this->myListIDCard = "";
    this->lstketqua.clear();
    this->timeXepBai = 0;
    this->strketqua = "";
    
    this->imgTime->setVisible(false);
    
    
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    this->isXepBai = false;
    
    layerAvatar->setLocalZOrder(100);
}
void LayerBoBaiMauBinh::setIsXepBai(bool _xepbai){
    if (_xepbai){
        this->playSound(102);
    }
    this->isXepBai = _xepbai;
    
    this->txtSoChi->setVisible(false);
}
bool LayerBoBaiMauBinh::getIsXepBai(){
    return this->isXepBai;
}
void LayerBoBaiMauBinh::showMyListCard(){
    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(kUserMe));
    if (bobai){
        if (this->timeXepBai > 0){
            this->imgTime->setVisible(true);
        }
        bobai->reloadBoBai(this->myListCard);
        bobai->reloadPos();
        this->getChildByTag(kUserMe)->setVisible(true);
        //this->runAction(Sequence::create(DelayTime::create(0.2),CallFunc::create([this](){ this->getChildByTag(kUserMe)->setVisible(true);}), NULL));

    }
}
void LayerBoBaiMauBinh::resetGameByPos(int pos){
    if (pos == -1)
        return;
    this->getBoBaiByPos(pos)->bocuoc();
}
string LayerBoBaiMauBinh::convertListXepBai(string lst){
    auto lstid = mUtils::splitString(lst,',');
    string arraycard = "";
    for (int i = 0; i < lstid.size(); i ++){
        string cardid = lstid[i];
        int idcard = atoi(cardid.c_str());
        int _value = idcard / 4;
        int _sub = idcard % 4;
        
        if (_sub == 0){
            _value = _value - 1;
            _sub = 4;
        }
        
        //2_1-2_3-4_1-9_4-11_1;11_2-12_3-13_4-11_3-10_2;9_1-2_4-9_2
        
        if (i == 5 || i == 10)
            arraycard.append(";");
        else if (i != 0 && i != (lstid.size()))
            arraycard.append("-");
        
        
        switch (_value)
        {
            case 15:
                arraycard.append("2_");
                break;
            default:
                arraycard.append(StringUtils::format("%d_",_value));
                break;
        }
        switch (_sub)
        {
            case 1:
                arraycard.append("4");
                break;
            case 2:
                arraycard.append("3");
                break;
            case 4:
                arraycard.append("1");
                break;
            case 3:
                arraycard.append("2");
                break;
        }
        
    }
    return arraycard;
}

string LayerBoBaiMauBinh::convertListCard(string lst){
    auto lstid = mUtils::splitString(lst,',');
    string arraycard = "";
    for (int i = 0; i < lstid.size(); i ++){
        string cardid = lstid[i];
        int idcard = atoi(cardid.c_str());
        int _value = idcard / 4;
        int _sub = idcard % 4;
        
        if (_sub == 0){
            _value = _value - 1;
            _sub = 4;
        }
        
        if (i != 0 && i != (lstid.size()))
            arraycard.append(";");
        
        switch (_value)
        {
            case 15:
                arraycard.append("2_");
                break;
            default:
                arraycard.append(StringUtils::format("%d_",_value));
                break;
        }
        switch (_sub)
        {
            case 1:
                arraycard.append("4");
                break;
            case 2:
                arraycard.append("3");
                break;
            case 4:
                arraycard.append("1");
                break;
            case 3:
                arraycard.append("2");
                break;
        }
        
    }
   // log("card %s",arraycard.c_str());
    return arraycard;
}
bool LayerBoBaiMauBinh::isAnTrang(int code){
    if (code ==  SPECIAL_CARD_TYPE_SANH_RONG_CUON || code ==  SPECIAL_CARD_TYPE_SANH_RONG || code ==  SPECIAL_CARD_TYPE_LUC_PHE_BON||code ==  SPECIAL_CARD_TYPE_3_CAI_SANH||code ==  SPECIAL_CARD_TYPE_3_CAI_THUNG ||code ==  SPECIAL_5DOI_1SAM){
        return true;
    }
    return false;
}
void LayerBoBaiMauBinh::setMyListIDCard(string lst){
    this->myListIDCard = lst;
    this->myListCard = this->convertListCard(lst);
}
void LayerBoBaiMauBinh::showSochi(string str){
    //thanhcuacay04|0|-1;-1;-1|-1|-1|-1|-1;-1;-1|-3|0/tanbm2|0|0;0;0|20|20|20|1;1;1|3|0/
    this->imgTime->setVisible(false);
    vector<string> lst = mUtils::splitString(str,'/');
    //ketqua
    //    string name;
    //    string baidacbiet;
    //    string baiantrang;
    //    string baicacchi;
    //    string chimoibai;
    //    string tongtien;
    
    
    
    this->getBoBaiByPos(kUserMe)->setVisible(true);
    this->imgTime->setVisible(false);
    int listkq = lst.size();
    
    for (int i = 0; i < lst.size(); i ++){
        vector<string> lstkq = mUtils::splitString(lst[i],'|');
        if (lstkq.size() < 12)
            continue;
        
        ketqua kq;
        kq.name = lstkq[0];
        kq.baidacbiet = lstkq[2];
        kq.baiantrang = lstkq[1];
        kq.baicacchi = lstkq[3] + ";" + lstkq[4] + ";" +lstkq[5];
        kq.chimoibai = lstkq[6];
        kq.tongtien = lstkq[7] + ";" + lstkq[8];
        kq.sapho = lstkq[9];
        kq.saplang = lstkq[10];
        auto lstbl = mUtils::splitString(lstkq[11],';');
        
        kq.binhlung = lstbl[0];
        kq.chibinhlung = lstbl[1];
        
        if (this->isAnTrang(atoi(lstkq[1].c_str()))){
            kq.sochi = false;
            kq.isAnTrang = true;
            this->_isAnTrang = true;
            this->_isAnTrangThat = true;
            listkq--;
        }
        else{
            kq.sochi = true;
        }
        if (atoi(lstkq[1].c_str()) == -1){
            kq.isBinhLung = true;
            this->_isAnTrang = true;
            kq.sochi = false;
            listkq--;
        }
        else{
            kq.isBinhLung = false;
            kq.sochi = true;
        }
        this->lstketqua.push_back(kq);
    }
    int count = 0;
    for (int i = 0; i < this->lstketqua.size();i++){
        if (this->lstketqua[i].isBinhLung) {
            count++;
        }
    }
    
    if (this->_isAnTrangThat || (this->lstketqua.size()-count)<=1){
        for (int i = 0; i < this->lstketqua.size();i++){
            this->lstketqua[i].sochi = false;
        }
    }
    vector<string> sochi1;
    vector<string> sochi2;
    vector<string> sochi3;
    string sobachi = "";
    string antrang = "";
    
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    
    layerAvatar->setLocalZOrder(115);
    this->setLocalZOrder(110);
    for (int i  = 0; i < this->lstketqua.size(); i ++){
        ketqua kq = this->lstketqua[i];
        this->imgTime->setVisible(false);
        if (!this->_isAnTrang){
            auto lstbobai = mUtils::splitString(kq.baicacchi,';');
            auto lstchi = mUtils::splitString(kq.chimoibai,';');
            auto lstbaidb = mUtils::splitString(kq.baidacbiet,';');
            if (lstbobai.size() < 3 || lstchi.size() < 3)
                return;
            string chi1 = kq.name + ";" + lstbaidb[0]+ ";" +kq.baiantrang + ";" + lstbobai[0] + ";" + lstchi[0];
            sochi1.push_back(chi1);
            string chi2 = kq.name + ";" + lstbaidb[1]+ ";" +kq.baiantrang + ";" + lstbobai[1] + ";" + lstchi[1];
            sochi2.push_back(chi2);
            
            string chi3 = kq.name + ";" + lstbaidb[2]+ ";" +kq.baiantrang + ";" + lstbobai[2] + ";" + lstchi[2];
            sochi3.push_back(chi3);
            
            sobachi+= (kq.name + "|" + kq.sapho + "|" + kq.saplang + "/");
        }else{
            if (this->lstketqua.size() == 2){
                if (kq.sochi){
                    int pos = layerAvatar->getPosByName(kq.name);
                    if (pos == -1)
                        continue;
                    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
                    
                    if (bobai){
                        bobai->latbai();
                        auto lst = mUtils::splitString(kq.tongtien,';');
                        if (lst.size()<2)
                            continue;
                        bobai->setPos(pos);
                        if (atoi(kq.binhlung.c_str()) == 0)
                            bobai->setChi(lst[0]);
                        else
                            bobai->setChi(kq.chibinhlung);
                    }
                }else{
                    int pos = layerAvatar->getPosByName(kq.name);
                    if (pos == -1)
                        continue;
                    Avatar52La* avatar = layerAvatar->getUserByPos(pos);
                    layerAvatar->setLocalZOrder(118);
                    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
                    if (avatar){
                        if (atoi(kq.baiantrang.c_str()) !=0){
                            avatar->setResult(atoi(kq.baiantrang.c_str()), kGameMauBinh, false, 0);
                            Size sizeAdd = ChanUtils::getSizePos();
                            
                            auto lst = mUtils::splitString(kq.tongtien,';');
                            if (lst.size()<2)
                                continue;
                            if (atoi(kq.binhlung.c_str()) == 0){
                                if (atoi(lst[0].c_str()) > 0){
                                    ThangTrangMauBinh* thangtrang = ThangTrangMauBinh::create();
                                    thangtrang->setBoBai(bobai->getListBoBai());
                                    thangtrang->setThangTrangType(atoi(kq.baiantrang.c_str()));
                                    thangtrang->setName(kq.name);
                                    this->addChild(thangtrang,20,this->tag_thang_trang);
                                }
                            }
                        }
                    }
                    
                    if (bobai){
                        bobai->latbai();
                        bobai->setPos(pos);
                        auto lst = mUtils::splitString(kq.tongtien,';');
                        if (lst.size()<2)
                            continue;
                        if (atoi(kq.binhlung.c_str()) == 0){
                            bobai->setChi(lst[0]);
                            
                            
                        }
                        else
                            bobai->setChi(kq.chibinhlung);
                    }
                }
                continue;
            }
            else{
                if (kq.sochi){
                    auto lstbobai = mUtils::splitString(kq.baicacchi,';');
                    auto lstchi = mUtils::splitString(kq.chimoibai,';');
                    auto lstbaidb = mUtils::splitString(kq.baidacbiet,';');
                    if (lstbobai.size() < 3 || lstchi.size() < 3)
                        return;
                    string chi1 = kq.name + ";" + lstbaidb[0]+ ";" +kq.baiantrang + ";" + lstbobai[0] + ";" + lstchi[0];
                    sochi1.push_back(chi1);
                    string chi2 = kq.name + ";" + lstbaidb[1]+ ";" +kq.baiantrang + ";" + lstbobai[1] + ";" + lstchi[1];
                    sochi2.push_back(chi2);
                    
                    string chi3 = kq.name + ";" + lstbaidb[2]+ ";" +kq.baiantrang + ";" + lstbobai[2] + ";" + lstchi[2];
                    sochi3.push_back(chi3);
                    
                    sobachi+= (kq.name + "|" + kq.sapho + "|" + kq.saplang + "/");
                    
                    int pos = layerAvatar->getPosByName(kq.name);
                    if (pos == -1)
                        continue;
                    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
                    
                    if (bobai){
                        //bobai->latbai();
                        bobai->setPos(pos);
                        if (listkq==1)
                            bobai->latbai();
                        auto lst = mUtils::splitString(kq.tongtien,';');
                        if (lst.size()<2)
                            continue;
                        if (atoi(kq.binhlung.c_str()) == 0)
                            bobai->setChi(lst[0]);
                        else
                            bobai->setChi(kq.chibinhlung);
                    }
                }
                else{
                    int pos = layerAvatar->getPosByName(kq.name);
                    if (pos == -1)
                        continue;
                    layerAvatar->setLocalZOrder(118);
                    Avatar52La* avatar = layerAvatar->getUserByPos(pos);
                    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
                    if (avatar){
                        if (atoi(kq.baiantrang.c_str()) !=0){
                            avatar->setResult(atoi(kq.baiantrang.c_str()), kGameMauBinh, false, 0);
                            Size sizeAdd = ChanUtils::getSizePos();
                            auto lst = mUtils::splitString(kq.tongtien,';');
                            if (lst.size()<2)
                                continue;
                            if (atoi(kq.binhlung.c_str()) == 0){
                                if (atoi(lst[0].c_str()) > 0){
                                    ThangTrangMauBinh* thangtrang = ThangTrangMauBinh::create();
                                    thangtrang->setBoBai(bobai->getListBoBai());
                                    thangtrang->setThangTrangType(atoi(kq.baiantrang.c_str()));
                                    thangtrang->setName(kq.name);
                                    this->addChild(thangtrang,20,this->tag_thang_trang);
                                }
                            }
                        }
                    }
                    if (bobai){
                        bobai->latbai();
                        bobai->setPos(pos);
                        auto lst = mUtils::splitString(kq.tongtien,';');
                        if (lst.size()<2)
                            continue;
                        if (atoi(kq.binhlung.c_str()) == 0){
                            bobai->setChi(lst[0]);
                        }
                        else
                            bobai->setChi(kq.chibinhlung);
                    }
                    continue;
                }
                
            }
        }
    }
    
    if (lst.size()==1){
        return;
    }
    
    count = 0;
    for (int i = 0; i < this->lstketqua.size();i++){
        if (this->lstketqua[i].isBinhLung) {
            count++;
        }
    }
    
    if (this->_isAnTrangThat || (this->lstketqua.size()-count)<=1){
        return;
    }
    
    if (lst.size()==2 && this->_isAnTrang){
        return;
    }
    if (listkq ==1)
        return;
    CallFunc* beginSoChifunc = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::beginSoChi, this));
    this->runAction(Sequence::create(DelayTime::create(0.8),beginSoChifunc,NULL));
    
    this->strketqua = str;
    
    
    //
    int _timesochi = 4;
    
    if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
        boost::shared_ptr<RoomVariable> timesochi = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("timesochi");
        if (timesochi != NULL)
        {
            if (timesochi->GetIntValue())
            {
                _timesochi = *timesochi->GetIntValue();
            }
        }
    }

    CallFunc* sobachifunc = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::soBaChi, this,sobachi));
    this->runAction(Sequence::create(DelayTime::create(1+ _timesochi + _timesochi+3),sobachifunc,NULL));
    for (int i = 0 ; i < sochi1.size(); i++){
        auto ketquachi1 = mUtils::splitString(sochi1[i],';');
        int pos = layerAvatar->getPosByName(ketquachi1[0]);
        if (pos == -1)
            return;
        CallFunc* callfuncchi1 = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::SoChiByPos, this,pos,1,ketquachi1[1]+ ";" +ketquachi1[2]+ ";" +ketquachi1[3]+ ";" +ketquachi1[4]));
        this->runAction(Sequence::create(DelayTime::create(1),callfuncchi1,NULL));
        
        auto ketquachi2 = mUtils::splitString(sochi2[i],';');
        CallFunc* callfuncchi2 = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::SoChiByPos, this,pos,2,ketquachi2[1]+ ";" +ketquachi2[2]+ ";" +ketquachi2[3]+ ";" +ketquachi2[4]));
        this->runAction(Sequence::create(DelayTime::create(1+ _timesochi),callfuncchi2,NULL));
        
        auto ketquachi3 = mUtils::splitString(sochi3[i],';');
        CallFunc* callfuncchi3 = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::SoChiByPos, this,pos,3,ketquachi3[1]+ ";" +ketquachi3[2]+ ";" +ketquachi3[3]+ ";" +ketquachi3[4]));
        this->runAction(Sequence::create(DelayTime::create(1+ _timesochi + _timesochi),callfuncchi3,NULL));
        
        CallFunc* callfuncchi4 = CallFunc::create(CC_CALLBACK_0(LayerBoBaiMauBinh::LatBai, this,pos));
        this->runAction(Sequence::create(DelayTime::create(1+ _timesochi + _timesochi+2),callfuncchi4,NULL));
    }
    this->imgTime->setVisible(false);
    
}
void LayerBoBaiMauBinh::LatBai(int pos){
    this->txtSoChi->setVisible(false);
    if (pos == -1)
        return;
    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
    if (bobai){
        bobai->LatBaiCuoiVan();
    }
    
}
void LayerBoBaiMauBinh::beginSoChi(){
    this->imgTime->setVisible(false);
    this->timeXepBai = 0;
    BaiMauBinh* me = (BaiMauBinh*)this->getChildByTag(kUserMe);
    BaiMauBinh* left = (BaiMauBinh*)this->getChildByTag(kUserLeft);
    BaiMauBinh* right = (BaiMauBinh*)this->getChildByTag(kUserRight);
    BaiMauBinh* bot = (BaiMauBinh*)this->getChildByTag(kUserBot);
    BaiMauBinh* top = (BaiMauBinh*)this->getChildByTag(kUserTop);
    
//    me->runAction(ScaleTo::create(0.3, 1.2));
//    left->runAction(ScaleTo::create(0.3, 1.2));
//    right->runAction(ScaleTo::create(0.3, 1.2));
//    bot->runAction(ScaleTo::create(0.3, 1.2));
//    top->runAction(ScaleTo::create(0.3, 1.2));

    
    me->setChi("");
    left->setChi("");
    right->setChi("");
    bot->setChi("");
    top->setChi("");
    this->imgTime->setVisible(false);
    this->removeChildByTag(this->tag_xep_bai);
}
void LayerBoBaiMauBinh::soBaChi(string str){
    this->txtSoChi->setString(dataManager.GetSysString(920));
    this->txtSoChi->setVisible(true);
    this->removeChildByTag(this->tag_xep_bai);
    
    auto lst = mUtils::splitString(str,'/');
    for (int i = 0; i < lst.size(); i++ ){
        auto kq = mUtils::splitString(lst[i],'|');
        if (kq.size() < 3)
            return;
        LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
        if (parent == NULL)
            return;
        LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
        if (layerAvatar == NULL)
            return;
        
        auto lstsapho = mUtils::splitString(kq[1],';');
        auto lstsaplang = mUtils::splitString(kq[2],';');
        
        int pos = layerAvatar->getPosByName(kq[0]);
        if (pos == -1)
            return;
        Avatar52La* avatar = layerAvatar->getUserByPos(pos);
        BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
        layerAvatar->setLocalZOrder(135);
        
        if (atoi(lstsapho[0].c_str()) == 1){
            //bat sap ho
            if (avatar)
            {
                avatar->setResult(100, kGameMauBinh, true, 1);
            }
            if (bobai){
                bobai->setPos(pos);
                
                bobai->setChi(lstsapho[1]);
            }
        }
        if (atoi(lstsapho[0].c_str()) == 2){
            //bi sap ho
            if (avatar)
            {
                avatar->setResult(101, kGameMauBinh, true, 1);
            }
            if (bobai){
                bobai->setPos(pos);
                
                bobai->setChi(lstsapho[1]);
            }
        }
        
        if (atoi(lstsaplang[0].c_str()) == 1){
            //bat sap lang
            if (avatar)
            {
                avatar->setResult(102, kGameMauBinh, true, 1);
            }
            if (bobai){
                bobai->setPos(pos);
                
                bobai->setChi(lstsaplang[1]);
            }
        }
        if (atoi(lstsaplang[0].c_str()) == 2){
            //bi sap lang
            if (avatar)
            {
                avatar->setResult(103, kGameMauBinh, true, 1);
            }
            if (bobai){
                bobai->setPos(pos);
                
                bobai->setChi(lstsaplang[1]);
            }
        }
    }
}
void LayerBoBaiMauBinh::showKetQua(string str){
    this->removeChildByTag(this->tag_thang_trang);
    this->txtSoChi->setVisible(false);
    this->imgTime->setVisible(false);
 
}
void LayerBoBaiMauBinh::SoChiByPos(int pos,int chi,string ketqua){
    this->imgTime->setVisible(false);
    if (pos == -1)
        return;
    BaiMauBinh* bobai = (BaiMauBinh*)(this->getChildByTag(pos));
    if (bobai){
        
        bobai->setPos(pos);
        bobai->soChiEffect(chi,ketqua);
        this->txtSoChi->setVisible(true);
        this->txtSoChi->setString(StringUtils::format("%s %d",dataManager.GetSysString(919).c_str(),chi));
    }
    
}
void LayerBoBaiMauBinh::setTimeXepBai(int timxep){
    this->timeXepBai = timxep;
//    if (this->txtTimer->isVisible()){
//        this->txtSoChi->setVisible(false);
//    }else{
//        this->txtSoChi->setVisible(true);
//    }
    this->txtTimer->setString(StringUtils::format("%s : \n%d",dataManager.GetSysString(918).c_str(),this->timeXepBai));
    if (timxep <= 15){
        this->txtTimer->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3),FadeIn::create(0.3),NULL)));
    }
    
}
void LayerBoBaiMauBinh::onEnter()
{
    Layer::onEnter();
    
}

void LayerBoBaiMauBinh::onExit()
{
    Layer::onExit();
}

void LayerBoBaiMauBinh::setCardRejoin(string list)
{
    vector<string> lstuid = mUtils::splitString(list, ';');
    LayerGameMauBinh* parent = (LayerGameMauBinh*)this->getParent();
    if (parent == NULL)
        return;
    LayerAvatarInGame52La* layerAvatar = (LayerAvatarInGame52La*)parent->getChildByTag(100);
    if (layerAvatar == NULL)
        return;
    for (int i = 0; i < lstuid.size(); i++) {
        auto lstcard = mUtils::splitString(lstuid[i], '|');
        if (lstcard.size() < 2)
            return;
        BaiMauBinh* bobai = this->getBoBaiByPos(layerAvatar->getPosByName(lstcard[0]));
        if (bobai != NULL){
            // this->addChild(xepbai);
            if (this->IsMee(lstcard[0])){
                bobai->setIsme(true);
                this->myListIDCard = lstcard[1];
                this->myListCard = this->convertListCard(lstcard[1]);
            }
        }
    }
}

