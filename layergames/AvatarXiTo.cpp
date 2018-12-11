#include "AvatarXiTo.h"
#include "../Common.h"
#include <boost/algorithm/string/trim.hpp>

AvatarXiTo::~AvatarXiTo()
{

}

AvatarXiTo* AvatarXiTo::create(){
	AvatarXiTo* a = new AvatarXiTo();
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

AvatarXiTo::AvatarXiTo() : Avatar(false), flag(NULL), lblTypeBet(NULL)
{

}

bool AvatarXiTo::init(){
	if (!Avatar::init()){
		return false;
	}

	this->flag = Sprite::create("flag.png");
	this->layerWidget->addChild(this->flag);
	this->flag->setVisible(false);

	this->lblTypeBet = Label::createWithSystemFont("","", 25);
	this->lblTypeBet->setColor(_colorTextNormal);
	this->lblTypeBet->setScale(1);
	this->lblTypeBet->setAnchorPoint(Vec2(0.5, 0));
	this->lblTypeBet->setPositionY(this->sizeThis.height / 2 + 3);
	this->lblTypeBet->setVisible(false);
	this->layerWidget->addChild(this->lblTypeBet);

	return true;
}
void AvatarXiTo::reloadCoin(){
	//int roomType = SceneManager::getSingleton().getCurrRoomType();
	this->icc->setScale(0.75);
	//if (roomType == 0){//1-hao, 0-moc
	//	icc->setTexture("moc3-icon-moc.png");
	//}
	//else{
	//	icc->setTexture("moc3-icon-hao.png");
	//}
}
void AvatarXiTo::showTypeBet(const string& type) {
	std::string s = type;
	boost::algorithm::trim(s);
	if (s.size() == 0)
		return;
	this->layerWidget->removeChildByTag(tag_STRING_CHAT);
	this->lblTypeBet->setString(type);
	this->lblTypeBet->setVisible(true);
}

void AvatarXiTo::hideTypeBet() {
	this->lblTypeBet->setVisible(false);
}

Sprite* AvatarXiTo::getFlag() {
	return this->flag;
}

void AvatarXiTo::ResetAll() {
	Avatar::ResetAll();
	this->flag->setVisible(false);
	this->lblTypeBet->setVisible(false);
}

void AvatarXiTo::SetInfo(boost::shared_ptr<XiToInfoPlayer> info)
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

void AvatarXiTo::showChat(string& mess){
	this->lblTypeBet->setVisible(false);
	Avatar::showChat(mess);
}

