//
//  Card.cpp
//  iCasinoCard
//
//  Created by ;; on 5/8/14.
//
//
//
#include "_Card_.h"
#include "../SceneManager.h"
//

Card::Card(const char *filename) {
    this->initWithSpriteFrameName(filename);
    initialize();
}
void Card::setFoldCard(){
    this->initWithSpriteFrameName("card_back.png");
}
void Card::initialize() {

    this->setVisible(false);
    
	this->sizeCard = Size(159, 208);
    this->isTouched = false;
    this->isClicked = false;
    this->isPhom = false;
    this->isEat =  false;
	this->_type = -1;
	this->_recom = false;
    this->scaleX = sizeCard.width / this->getContentSize().width;
    this->scaleY = sizeCard.height / this->getContentSize().height;
    
    this->SetScaleCard(this->scaleX, this->scaleY);
    
    this->setOpacity(255);
	this->setColor(Color3B::WHITE);
    // TienLen
    this->setRotation(0);
}

void Card::resetStatus() {
    initialize();
}

void Card::setTouched(bool isTouched) {
    this->isTouched = isTouched;
}

bool Card::getTouched() {
    return this->isTouched;
}

void Card::setClicked(bool isClicked) {
    this->isClicked = isClicked;
}

bool Card::getClicked() {
    return this->isClicked;
}

void Card::setPhom(bool isPhom) {
    this->isPhom = isPhom;
}

bool Card::getPhom() {
    return this->isPhom;
}

void Card::setEat(bool isEat) {
    this->isEat = isEat;
}

bool Card::getEat() {
    return this->isEat;
}

void Card::setSize(float wid, float hei) {
    this->sizeCard.setSize(wid, hei);
}

Size Card::getSize() {
    return this->sizeCard;
}

void Card::setURL(string url) {
    this->url = url;
}

const string& Card::getURL() const {
    return this->url;
}

void Card::SetID(const int& id) {
    this->id = id;
}

const int& Card::GetID() const{
    return this->id;
}

void Card::SetScaleCard(const float& scaleX, const float& scaleY) {
    this->setScaleX(scaleX);
    this->setScaleY(scaleY);
}


Size Card::GetScaleCard() {
    return Size(scaleX, scaleY);
}

float Card::GetScaleCardX() {
    return this->scaleX;
}

float Card::GetScaleCardY() {
    return this->scaleY;
}

void Card::setCardAte() {
    //this->setOpacity(180);
	this->isEat = true;
	this->setColor(Color3B(70, 255, 180));
}

void Card::SetValuesXiTo()
{
	if(!isClicked)
		this->setColor(Color3B(150, 250, 250));
}

void Card::SetIsValuesInList(const int& id_, unsigned int& index)
{
	index++;
	if(id == id_)
		isClicked = true;
}

void Card::SetIdInList(const int& id_, unsigned int& index)
{
	index++;
	this->id = id_;
}

Card::~Card()
{
//	log("Destructor Cards");
}

void Card::showArrowEat()
{
	auto arrowEat = Sprite::create("focus-an.png");
	arrowEat->setAnchorPoint(Vec2(0, 0.5));
	arrowEat->setPosition(Vec2(0, this->getContentSize().height));
	arrowEat->setTag(24);
	this->addChild(arrowEat);
	arrowEat->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), nullptr)));
}

void Card::removeArrowEat()
{
	this->removeChildByTag(24);
}

void Card::setScaleCardOriginal()
{
	this->setScale(sizeCard.width / this->getContentSize().width, 
		sizeCard.height / this->getContentSize().height);
}


