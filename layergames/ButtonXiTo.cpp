#include "ButtonXiTo.h"
#include "../Common.h"
#include "ui/CocosGUI.h"
#include "../data_manager.h"
#include "../AllData.h"
ButtonXiTo::ButtonXiTo(const std::string& name) : allow(false), name(name)
{

}

ButtonXiTo::~ButtonXiTo()
{

}

ButtonXiTo* ButtonXiTo::create(const std::string& name){
    ButtonXiTo* b = new ButtonXiTo(name);
    if(b && b->init())
    {
        b->autorelease();
        return b;
    }
    else
    {
        delete b;
        b = NULL;
        return NULL;
    }
}

bool ButtonXiTo::init(){
    if(!Button::init())
        return false;

    return true;
}

bool& ButtonXiTo::isAllow() {
    return this->allow;
}

void ButtonXiTo::setAllow(bool allow) {
    this->allow = allow;
}

void ButtonXiTo::resetState () {
    this->allow = false;
    this->setVisible(false);
}

void ButtonXiTo::createSuggest(){
    ImageView* img = ImageView::create("xito-to-money.png");
    img->setAnchorPoint(Vec2(1, 0.5));
    img->setPosition(Vec2(0, 118 / 2));
    this->addChild(img);

    this->money = Label::createWithSystemFont(dataManager.GetSysString(569),"", 30);
    this->money->setColor(Color3B::WHITE);
    this->money->setAnchorPoint(Vec2(0.5, 0.5));
    this->money->setPosition(Vec2(214 / 2 - 10, 108 / 2));
    this->money->setScale(1.2);
    img->addChild(this->money);
}

void ButtonXiTo::setMoney(const std::string& money, bool b){
    if (this->money != NULL) {
        if (b) {
            this->money->setScale(1.15);
        }else{
            this->money->setScale(1.2);
        }
        this->money->setString(money);
    }
}


