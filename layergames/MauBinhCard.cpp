//
//  MauBinhMauBinhCard.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/11/17.
//
//

#include "MauBinhCard.h"
MauBinhCard::MauBinhCard(const char *filename) {
    this->initWithSpriteFrameName(filename);
    initialize();
}
void MauBinhCard::initialize() {
    //this->setVisible(false);
    
    this->sizeMauBinhCard = Size(214, 280);
    this->isTouched = false;
    this->isClicked = false;
    
    
    this->scaleX = sizeMauBinhCard.width / this->getContentSize().width;
    this->scaleY = sizeMauBinhCard.height / this->getContentSize().height;
    
    this->SetScaleMauBinhCard(this->scaleX, this->scaleY);
    
    this->setOpacity(255);
    
    // TienLen
    this->setRotation(0);
    this->value = "";
    this->type = 0;
}

void MauBinhCard::resetStatus() {
    initialize();
}

void MauBinhCard::setTouched(bool isTouched) {
    this->isTouched = isTouched;
}

bool MauBinhCard::getTouched() {
    return this->isTouched;
}

void MauBinhCard::setClicked(bool isClicked) {
    this->isClicked = isClicked;
}

bool MauBinhCard::getClicked() {
    return this->isClicked;
}

void MauBinhCard::setSize(float wid, float hei) {
    this->sizeMauBinhCard.setSize(wid, hei);
}

Size MauBinhCard::getSize() {
    return this->sizeMauBinhCard;
}

void MauBinhCard::setURL(string url) {
    this->url = url;
}

const string& MauBinhCard::getURL() const {
    return this->url;
}

void MauBinhCard::SetID(const int& id) {
    this->id = id;
}

const int& MauBinhCard::GetID() const{
    return this->id;
}

void MauBinhCard::SetScaleMauBinhCard(const float& scaleX, const float& scaleY) {
    this->setScaleX(scaleX);
    this->setScaleY(scaleY);
}


Size MauBinhCard::GetScaleMauBinhCard() {
    return Size(scaleX, scaleY);
}

float MauBinhCard::GetScaleMauBinhCardX() {
    return this->scaleX;
}

float MauBinhCard::GetScaleMauBinhCardY() {
    return this->scaleY;
}

void MauBinhCard::setMauBinhCardAte() {
    //this->setOpacity(180);
    this->isEat = true;
    this->setColor(Color3B(70, 255, 180));
}

void MauBinhCard::SetValuesXiTo()
{
    if(!isClicked)
        this->setColor(Color3B(150, 250, 250));
}

void MauBinhCard::SetIsValuesInList(const int& id_, unsigned int& index)
{
    index++;
    if(id == id_)
        isClicked = true;
}


MauBinhCard::~MauBinhCard()
{
    //	log("Destructor MauBinhCards");
}

void MauBinhCard::setIsLink(){
    this->setColor(Color3B::WHITE);
}
void MauBinhCard::reset(){
   // this->setColor(Color3B(150, 250, 250));
    this->setColor(Color3B(150, 150, 150));

}
void MauBinhCard::setScaleMauBinhCardOriginal()
{
    this->setScale(sizeMauBinhCard.width / this->getContentSize().width,
                   sizeMauBinhCard.height / this->getContentSize().height);
}

void MauBinhCard::setValue(string _value){
    this->value = _value;
}
string MauBinhCard::getValue(){
    return this->value;
}
void MauBinhCard::setType(int _type){
    this->type = _type;
}
int MauBinhCard::getType(){
    return this->type;
}
void MauBinhCard::setOrginPos(Vec2 _pos){
    this->orginPosition = _pos;
}
Vec2 MauBinhCard::getOrginPos(){
    return this->orginPosition;
}
