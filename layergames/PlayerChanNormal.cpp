#include "PlayerChanNormal.h"
#include "BoBaiChan.h"

PlayerChanNormal::PlayerChanNormal()
{

}

PlayerChanNormal::~PlayerChanNormal()
{

}

void PlayerChanNormal::ActionDiscardByServer(const CardOrg& card){
    card.show();
   // log("%s đánh bài", this->aI.c_str ());

    CardChan *pCard = BoBaiChan::GetInstance()->PopCard();

    pCard->stopAllActions();

    pCard->loadTexture(ChanUtils::FindTypeCard(card.cardnu, card.cardsu), Widget::TextureResType::PLIST);
    pCard->SetNumber(card.cardnu);
    pCard->SetSuite(card.cardsu);
    pCard->SetSizeCard(W_CARD, H_CARD);

    pCard->setPosition(this->GetPosBegin());
    pCard->setVisible(true);

    MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardCuaChi());
    CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(PlayerChanNormal::ResortCardCuaChi, this, pCard, this));
    DelayTime *delay = DelayTime::create(this->duration);
    RotateTo* rotateTo = RotateTo::create(this->duration, 0);

    this->BaiCuaChi.push_back(pCard);
    pCard->runAction(Sequence::create(delay, moveTo, callfun, NULL));
    pCard->runAction(rotateTo);
}

void PlayerChanNormal::ActionTraCuaByServer(const CardOrg& card)
{
    card.show();
    CardChan *pCard = BoBaiChan::GetInstance()->PopCard();
    pCard->stopAllActions();

    pCard->loadTexture(ChanUtils::FindTypeCard(card.cardnu, card.cardsu), Widget::TextureResType::PLIST);
    pCard->SetSizeCard(W_CARD, H_CARD);
    pCard->SetNumber(card.cardnu);
    pCard->SetSuite(card.cardsu);
    pCard->setTouchEnabled(false);

    PlayerChan* toPlayer = this->GetPlayer(card.tuser);
    if(toPlayer == NULL) return;

   // log("%s trả cửa cho %s", this->GetAI().c_str(), toPlayer->GetAI().c_str());

    pCard->setPosition(this->GetPosBegin());
    pCard->setVisible(true);

    MoveTo* moveTo = MoveTo::create(this->duration, toPlayer->PosCardCuaChi());
    CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(PlayerChanNormal::ResortCardCuaChi, this, pCard, this));
    RotateTo* rotateTo = RotateTo::create(this->duration, 0);

    toPlayer->AddCardToCuaChi(pCard);
    pCard->runAction(Sequence::create(moveTo, callfun, nullptr));
    pCard->runAction(rotateTo);
}

void PlayerChanNormal::ActionChuyenBaiByServer(const CardOrg& card){
    card.show();
   // log("Chuyen bai tu tren tay xuong duoi tay binh thuong");
    CardChan *pCard = BoBaiChan::GetInstance()->PopCard();
    pCard->stopAllActions();

    pCard->loadTexture(ChanUtils::FindTypeCard(card.cardnu, card.cardsu), Widget::TextureResType::PLIST);
    pCard->SetNumber(card.cardnu);
    pCard->SetSuite(card.cardsu);
    pCard->SetSizeCard(W_CARD, H_CARD);

    pCard->setPosition(this->GetPosBegin());
    pCard->setVisible(true);
    MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay2());
    DelayTime* delay = DelayTime::create(this->duration);
    RotateTo* rotateTo = RotateTo::create(this->duration, 0);

    this->AddCardToDuoiTay2(pCard);

    pCard->runAction(Sequence::create(delay, moveTo, nullptr));
    pCard->runAction(rotateTo);
}

void PlayerChanNormal::ActionAnCuaTrenByServer(const CardOrg &card)
{
   // log("Player normal an bai");
    PlayerChan::ActionAnCuaTrenByServer(card);
}

void PlayerChanNormal::ActionChuyenBaiChiuByServer(const CardOrg &card)
{
    for(int i = 0; i < 3; ++i) {
        // Lay info la bai
        CardChan* pCard = BoBaiChan::GetInstance()->PopCard();
        pCard->stopAllActions();

        pCard->loadTexture(ChanUtils::FindTypeCard(card.cardnu, card.cardsu), Widget::TextureResType::PLIST);
        pCard->SetNumber(card.cardnu);
        pCard->SetSuite(card.cardsu);
        pCard->SetSizeCard(W_CARD, H_CARD);

        //Hien thi
        pCard->setPosition(this->GetPosBegin());
        pCard->setVisible(true);

        // Di chuyen xuong duoi tay
        MoveTo *moveto = MoveTo::create(this->duration, this->PosChiu());
        RotateTo *rotateBy = RotateTo::create(this->duration, 0);
        this->AddCardToDuoiTay2(pCard);

        pCard->runAction(moveto);
        pCard->runAction(rotateBy);
    }
}

int PlayerChanNormal::ActionDisCardByClient(){
    return 0;
}

int PlayerChanNormal::ActionChiuCardByClient(){
    return 0;
}

int PlayerChanNormal::ActionTakeCardByClient(const CardOrg& lastCard){
    return 0;
}

int PlayerChanNormal::SetListCards(string &listcards)
{
    return 0;
}
