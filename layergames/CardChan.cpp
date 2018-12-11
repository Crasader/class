//
//  CardChan.cpp
//  AppDemo
//
//  Created by DauA on 5/13/14.
//
//

#include "CardChan.h"
#include "AllData.h"

CardChan::CardChan()
    :
    Border(NULL),
    IdCard(-1),
    Flag(false),
    NumberCard(0),
    SuiteCard(0),
    IsAnimating(false),
    Used(false),
    IsDraw(false),
    servId(-1),
    CurrentPosition(Vec2::ZERO) {
}

CardChan::~CardChan() {
   // log("Card Chan, Destructor");
}

CardChan* CardChan::createInstance(std::string path, TextureResType texType){
    CardChan* card = new CardChan();
    if(card && card->init(path, texType))
    {
        card->autorelease();
        return card;
    }
    else
    {
        delete card;
        card = NULL;
        return NULL;
    }
}

bool CardChan::init(std::string path, TextureResType texType){
    if(!ImageView::init(path, texType))
        return false;

    this->Border = ImageView::create("border_cardchan.png");
    if(this->Border != NULL){
        this->Border->setTouchEnabled(false);
        this->Border->setVisible(false);
        this->addChild(this->Border);
    }

    return true;
}

bool CardChan::init() {
    if(!ImageView::init())
        return false;

    this->Border = ImageView::create("border_cardchan.png");
    if(this->Border != NULL){
        this->Border->setTouchEnabled(false);
        this->Border->setVisible(false);
        this->addChild(this->Border);
    }

    return true;
}

const bool& CardChan::GetFlag() {
    return this->Flag;
}

const int& CardChan::GetID() const {
    return this->IdCard;
}

void CardChan::SetFlag(const bool& b) {
    this->Flag = b;
}

void CardChan::SetID(const int& n) {
    this->IdCard = n;
}

void CardChan::SetSizeCard(const float& toWidth, const float& toHeight) {
    this->setAnchorPoint(Vec2(0.5, 0));
    if(this->Border != NULL)
    {
        this->Border->setPositionY(this->getContentSize().height / 2);
        this->Border->setPositionX(this->getContentSize().width / 2);
    }

    this->setScaleX(toWidth / this->getContentSize().width);
    this->setScaleY(toHeight / this->getContentSize().height);
}

void CardChan::SetNumber(const int& num) {
    this->NumberCard = num;
}

void CardChan::SetSuite(const int& sui) {
    this->SuiteCard = sui;
}

const int& CardChan::GetNumber() const {
    return this->NumberCard;
}

const int& CardChan::GetSuite() const {
    return this->SuiteCard;
}

void CardChan::SetSTT(const int& stt_) {
    this->SttCard = stt_;
}

const int& CardChan::GetSTT() {
    return this->SttCard;
}

void CardChan::DisplayCardInListNoc(const ExecuteInfo& info) {
    MoveTo *moveTo = MoveTo::create(0.8, Vec2(info.startPos + this->SttCard * info.width, (HEIGHT_DESIGN >> 1) - info.height));
    this->runAction(Sequence::create(moveTo, NULL));
}

void CardChan::RotateCardChan(const ExecuteInfo& info) {
    if (this->IsAnimating) {
        this->stopAllActions();
    }
    this->IsAnimating = true;

    float newCornor = -((info.startPos) / 2 * info.cornor) + info.cornor * this->SttCard;
    float rad = newCornor * PI / 180;

    float xNew = info.beginX + info.height * sin(rad);
    float yNew = info.beginY - (info.height * (1 - cos(rad)));

    this->CurrentPosition = Vec2(xNew, yNew);

    MoveTo *moveTo = MoveTo::create(info.duration_, Vec2(xNew, yNew));
    RotateTo *rotateTo = RotateTo::create(info.duration_, newCornor);
    CallFunc *callfunc = CallFunc::create(CC_CALLBACK_0(CardChan::UnclockAnimating, this));

    this->runAction(Sequence::create(moveTo, callfunc, NULL));
    this->setVisible(true);
    this->runAction(rotateTo);
}

void CardChan::MoveCardCuaChi(const float& y) {
    MoveTo *moveTo = MoveTo::create(0.3, Vec2(this->getPositionX(), y));
    this->runAction(moveTo);
}

void CardChan::MoveCardWinner(const Vec2& point) {
    float x = point.x + this->getPositionX();
    float y = point.y + this->getPositionY();
    this->setPosition(Vec2(x, y));
}

bool CardChan::FindCardIsClick(const FindCard* info) {
    return (this->Flag == info->flag && this->NumberCard == info->number && this->SuiteCard == info->suite);
}

void CardChan::ResetCard() {
    this->setColor(Color3B(255, 255, 255));
    this->setVisible(false);
    this->setTouchEnabled(false);
    this->setRotation(0);
    this->stopAllActions();
    this->SetUsed (false);
    if(this->Border != NULL)
        this->Border->setVisible(false);
    this->IsDraw = false;
    this->servId = -1;
}

void CardChan::SetIsDraw(bool draw) {
     this->IsDraw = draw;
}

bool CardChan::GetIsDraw() {
    return this->IsDraw;
}

void CardChan::CardIsClick(const CardChan* pCards) {
    Vec2 pPoint = this->CurrentPosition;
    float	rad = this->getRotation() * PI / 180;
    //! Neu dung la this
    if (this->NumberCard == pCards->GetNumber() &&
            this->SuiteCard == pCards->GetSuite()  &&
            this->IdCard == pCards->GetID()) {
        if (!this->Flag) {
            this->setPosition(Vec2(pPoint.x + 40 * sin(rad), pPoint.y + 40 * cos(rad)));
            this->Flag = true;
        } else {
            //!this->setPosition(Vec2(pPoint.x - 20 * sin(rad), pPoint.y - 20 * cos(rad)));
            this->setPosition(this->CurrentPosition);
            this->SetFlag(false);
        }
    }

    //! Neu khong phai la this
    else {
        if (this->Flag) {
            //!this->setPosition(Vec2(pPoint.x - 20 * sin(rad), pPoint.y - 20 * cos(rad)));
            this->setPosition(this->CurrentPosition);
            this->Flag = false;
        }
    }
}

bool CardChan::FindCardIfNotFoundClick(const FindCard* info) {
    return (this->NumberCard == info->number && this->SuiteCard == info->suite);
}

void CardChan::UnclockAnimating() {
    this->IsAnimating = false;
    //this->Flag = false;
}

void CardChan::ResetPosition()
{
    this->setPosition (this->CurrentPosition);
    this->Flag = false;
}

void CardChan::SetCurrentPos(const Vec2 &pos)
{
    this->CurrentPosition = pos;
}

void CardChan::SetVisibleBorder(bool display /*= false*/) {
    if(this->Border != NULL)
        this->Border->setVisible(display);
}

bool CardChan::InUsed(){
    return this->Used == false;
}

void CardChan::SetUsed(bool use){
    this->Used = use;
}

void CardChan::SetServId(const int& id){
    this->servId = id;
}

const int& CardChan::GetServId() const{
    return this->servId;
}


