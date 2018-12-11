#include "AvatarPoker.h"
#include "../Common.h"
#include <boost/algorithm/string/trim.hpp>

AvatarPoker::~AvatarPoker()
{

}

AvatarPoker* AvatarPoker::create(){
	AvatarPoker* a = new AvatarPoker();
	if (a && a->init()){
		a->autorelease();
		return a;
	}
	else{
		delete a;
		a = NULL;
		return a;
	}
}

AvatarPoker::AvatarPoker() : Avatar(false), flag(NULL), lblTypeBet(NULL)
{

}

bool AvatarPoker::init(){
	if (!Avatar::init()){
		return false;
	}

	this->flag = Sprite::create("flag.png");
	this->layerWidget->addChild(this->flag);
	this->flag->setVisible(false);
    
    this->typePlayer = Sprite::create(StringUtils::format("%sD.png",PATH_POKER));
    this->typePlayer->setPosition(Vec2(100,100));
    this->typePlayer->setVisible(false);
    this->layerWidget->addChild(this->typePlayer);
    
    
    this->betType = Sprite::create(StringUtils::format("%s%sallin-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
    this->betType->setPosition(Vec2(0,-50));
    this->betType->setVisible(false);
    this->layerWidget->addChild(this->betType,LOCAL_ZORDER_CIR_AVATAR+1);
    //this->flag->setVisible(false);
    

	this->lblTypeBet = Label::createWithSystemFont("","", 25);
	this->lblTypeBet->setColor(_colorTextNormal);
	this->lblTypeBet->setScale(1);
	this->lblTypeBet->setAnchorPoint(Vec2(0.5, 0));
	this->lblTypeBet->setPositionY(this->sizeThis.height / 2 + 3);
	this->lblTypeBet->setVisible(false);
	this->layerWidget->addChild(this->lblTypeBet);

    
    
	return true;
}
void AvatarPoker::setTypePlayer(int type){
    //return;
    this->typePlayer->setVisible(true);
    switch(type){
        case 1:
            this->typePlayer->setTexture(StringUtils::format("%sD.png",PATH_POKER));
            break;
        case 2:
            this->typePlayer->setTexture(StringUtils::format("%sSB.png",PATH_POKER));

            break;
        case 3:
            this->typePlayer->setTexture(StringUtils::format("%sBB.png",PATH_POKER));
            break;
    }
}

void AvatarPoker::setPosDeal(int type){
    switch(type){
        case 1:
            this->typePlayer->setPosition(Vec2(120,60));
            break;
        case 2:
            break;
        case 3:
            this->typePlayer->setPosition(Vec2(-120,100));

            break;
    }
}
void AvatarPoker::reloadCoin(){
	//int roomType = SceneManager::getSingleton().getCurrRoomType();
	this->icc->setScale(0.75);
	//if (roomType == 0){//1-hao, 0-moc
	//	icc->setTexture("moc3-icon-moc.png");
	//}
	//else{
	//	icc->setTexture("moc3-icon-hao.png");
	//}
}
void AvatarPoker::showTypeBet(const int& type) {
      /* DTAG_BTN_POKER_BET=1,
    DTAG_BTN_POKER_FOLLOW =5 ,
    DTAG_BTN_POKER_FOLD=0,
    DTAG_BTN_POKER_BET_ALL=7,
    DTAG_BTN_POKER_GIVE = 2*/
    //GAME_TABLE_STATUS_BET_VALUE = 8;
    
    //DTAG_BTN_POKER_BET_ALL = 7 Theo server
    this->TypeBet = type;
    this->betType->setVisible(true);
    switch(type){
        case DTAG_BTN_POKER_FOLD:
            this->betType->setTexture(StringUtils::format("%s%sfold-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
        case DTAG_BTN_POKER_BET:
            this->betType->setTexture(StringUtils::format("%s%sbet-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
        case DTAG_BTN_POKER_FOLLOW:
            this->betType->setTexture(StringUtils::format("%s%scall-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
        case 7:
            this->betType->setTexture(StringUtils::format("%s%sallin-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
        case DTAG_BTN_POKER_GIVE:
            this->betType->setTexture(StringUtils::format("%s%scheck-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
        case 8:
            this->betType->setTexture(StringUtils::format("%s%sraise-icon.png",PATH_POKER,SceneManager::getSingleton().getFolderLang().c_str()));
            break;
    }
//    std::string s = type;
//    boost::algorithm::trim(s);
//    if (s.size() == 0)
//        return;
//    this->layerWidget->removeChildByTag(tag_STRING_CHAT);
//    this->lblTypeBet->setString(type);
//    this->lblTypeBet->setVisible(true);
}

void AvatarPoker::hideTypeBet() {
    if (this->TypeBet != DTAG_BTN_POKER_FOLD)
        this->betType->setVisible(false);
}

Sprite* AvatarPoker::getFlag() {
	return this->flag;
}

void AvatarPoker::ResetAll() {
    this->TypeBet = -1;
	Avatar::ResetAll();
    this->betType->setVisible(false);
    this->typePlayer->setVisible(false);
	this->flag->setVisible(false);
	this->lblTypeBet->setVisible(false);
}

void AvatarPoker::SetInfo(boost::shared_ptr<XiToInfoPlayer> info)
{
	this->setVisibleLayerInvite(false);
	this->setAI(info->aI);
	/*moc3-change thay aN = adn*/
	string s = info->aN;
	/*moc3-end change*/
	boost::algorithm::trim(s);
	if (0 == s.compare(""))
		s = info->aI;
	this->setName(s);
	this->setIcon(info->aal);
	this->setMoney(info->aM);
	this->setFlag(info->isBoss);
	this->setReady(info->isReady);

	// Nếu là chủ phòng thì luôn hiển thị trạng thái unready
	if (info->isBoss) {
		this->setReady(false);
	}
}

void AvatarPoker::showChat(string& mess){
	this->lblTypeBet->setVisible(false);
	Avatar::showChat(mess);
}

void AvatarPoker::showLoseEffect(){
//    this->setColor(Color3B::GRAY);
//    this->typePlayer->setColor(Color3B(150, 150, 150));
//    this->Avatar::setColor(Color3B(150, 150, 150));
//    this->Avatar::setColor(Color3B(150, 150, 150));
//    this->ava
//    Avatar::bg->set
//    this->setCo
    this->Avatar::setColor(Color3B::GRAY);
}
void AvatarPoker::showNormalEffect(){
//    this->typePlayer->setColor(Color3B::WHITE);
    this->Avatar::setColor(Color3B(Color3B::WHITE));
//    this->setOpacity(100);
}
void AvatarPoker::showEffectWin(){
    auto emitter = ParticleSystemQuad::create("highlight.plist");
    emitter->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    emitter->setPosition(Vec2(0, 0));
    emitter->setAutoRemoveOnFinish(true);
    //            emitter->setLife(0.3);
    //            emitter->setTotalParticles(5);
    TextureCache* tc = Director::getInstance()->getTextureCache();
    Texture2D* coin = tc->addImage("light_effect.png");
    emitter->setTexture(coin);
    this->addChild(emitter);
}

