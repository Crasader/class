#include "BoBaiChan.h"

BoBaiChan* BoBaiChan::instance = NULL;

BoBaiChan::BoBaiChan()
{
    this->mark = 0;
    for(int i = 0; i < 130; i++) {
        CardChan *pCard = CardChan::createInstance("3_2.png");
        pCard->setLocalZOrder(i);
        pCard->setVisible(false);
        pCard->setRotation(0);
        pCard->setTouchEnabled(false);
        pCard->SetID(i);
        this->arrayAllCards.push_back(pCard);
    }
}

BoBaiChan::~BoBaiChan()
{
    this->arrayCanChiu.clear();
    for_each(this->arrayAllCards.begin(), this->arrayAllCards.end(), std::bind2nd(mem_fun(&CardChan::removeFromParentAndCleanup), true));
    this->arrayAllCards.clear();
   // log("Bo Bai Chan Destructor");
}

BoBaiChan *BoBaiChan::GetInstance()
{
    if(instance == NULL){
        instance = new BoBaiChan();
    }
    return instance;
}

void BoBaiChan::Destroy()
{
    delete instance;
    instance = NULL;
}

CardChan *BoBaiChan::PopCard()
{
    CardChan* pCard = NULL;
    vector<CardChan*>::iterator it = std::find_if(this->arrayAllCards.begin(), this->arrayAllCards.end(), mem_fun(&CardChan::InUsed));
    if(it != this->arrayAllCards.end ())
    {
        pCard = (*it);
        pCard->setRotation(0);
        pCard->SetUsed (true);
        if(pCard->GetID() >= this->mark){
            this->mark = pCard->GetID();
        }
        return pCard;
    }
    else
    {
        pCard = CardChan::create();
        pCard->setLocalZOrder((int)this->arrayAllCards.size());
        pCard->loadTexture("3_2.png", Widget::TextureResType::PLIST);
        pCard->setVisible(false);
        pCard->setRotation(0);
        pCard->setTouchEnabled(false);
        pCard->SetID ((int)this->arrayAllCards.size());
        this->arrayAllCards.push_back(pCard);
        pCard->SetUsed (true);
        if(pCard->GetID() >= this->mark){
            this->mark = pCard->GetID();
        }
        return pCard;
    }
}

void BoBaiChan::BackCard(CardChan *pCard)
{
    pCard->ResetCard ();
}

void BoBaiChan::ResetAllCards()
{
    this->mark = 0;
    this->arrayCanChiu.clear();
    if(this->arrayAllCards.size () > 130){
        for(vector<CardChan*>::iterator it = this->arrayAllCards.begin () + 130; it < this->arrayAllCards.end (); ++it){
            (*it)->removeFromParentAndCleanup(true);
            this->arrayAllCards.erase(it);
        }
    }
    for_each(this->arrayAllCards.begin(), this->arrayAllCards.end(), mem_fun(&CardChan::ResetCard));
    int ZorderCount = 0;
    for (vector<CardChan*>::iterator iter = this->arrayAllCards.begin(); iter != this->arrayAllCards.end(); ++iter) {
        (*iter)->setLocalZOrder(ZorderCount++);
    }
}

std::vector<CardChan *> &BoBaiChan::GetArrayCards()
{
    return this->arrayAllCards;
}

void BoBaiChan::SetCardCanChiu(string &ls)
{
    this->arrayCanChiu.clear();
   // log("Nhung la bai co the chiu: %s", ls.c_str());
    if(0 != ls.compare(""))
    {
        vector<string> arr = mUtils::splitString (ls, ';');
        for(int i = 0; i < arr.size (); ++i){
            try {
                vector<string> info = mUtils::splitString(arr.at (i), ':');
                CardOrg card;
                card.cardnu = atoi(info.at(1).c_str());
                card.cardsu = atoi(info.at(2).c_str());
                this->arrayCanChiu.push_back(card);
            } catch(std::out_of_range& e){
               // log("%s", e.what());
            } catch(std::exception& e){
               // log("%s", e.what());
            }
        }
    }
}

//Khong duoc remove card o day
void BoBaiChan::RemoveElemsChiu(const CardOrg& card)
{
    if(this->arrayCanChiu.size () > 0){
        for(int i = 0; i < this->arrayCanChiu.size (); ++i){
            try{
                if(this->arrayCanChiu.at(i).cardnu == card.cardnu &&
                        this->arrayCanChiu.at (i).cardsu == card.cardsu){
                    this->arrayCanChiu.erase (this->arrayCanChiu.begin() + i);
                }
            }catch(std::out_of_range& e){
               // log("%s", e.what());
            }catch(std::exception& e){
               // log("%s", e.what());
            }
        }
    }
}

bool BoBaiChan::CheckCanChiu(const CardOrg& card){
    if(this->arrayCanChiu.size () == 0) return false;
    for(int i = 0; i < this->arrayCanChiu.size (); ++i){
        if(card.cardnu == this->arrayCanChiu.at(i).cardnu && card.cardsu == this->arrayCanChiu.at(i).cardsu){
            return true;
        }
    }
    return false;
}

void BoBaiChan::ShowCardU(const int &cardnu, const int &cardsu)
{
    if(this->mark > 0){
        for(int i = this->mark; i >= 0; --i){
            try{
                CardChan* pCard = this->arrayAllCards.at(i);
                if(pCard->GetNumber() == cardnu && pCard->GetSuite() == cardsu){
                    if (pCard->GetIsDraw()) {
                        pCard->SetVisibleBorder(true);
                    }
                    else {
                        try {
                            CardChan* __card = this->arrayAllCards.at(i - 1);
                            if (__card->GetNumber() == cardnu && __card->GetSuite() == cardsu && __card->GetIsDraw()) {
                                __card->SetVisibleBorder(true);
                            } else {
                                pCard->SetVisibleBorder(true);
                            }
                        }
                        catch(std::out_of_range& e){
                            pCard->SetVisibleBorder(true);
                        }catch(std::exception& e){
                            pCard->SetVisibleBorder(true);
                        }catch(...){
                            pCard->SetVisibleBorder(true);
                        }
                    }
                    break;
                }
            }catch(std::out_of_range& e){

            }catch(std::exception& e){

            }catch(...){

            }
        }
    }
}

void BoBaiChan::ShowU(const int& id){
    for (int i = 0; i < this->arrayAllCards.size(); ++i) {
        try {
            CardChan* card = this->arrayAllCards.at(i);
            if (card != NULL) {
                int _id = card->GetServId();
                if (_id == id) {
                    card->SetVisibleBorder(true);
                    break;
                }
            }
        } catch (...) {

        }
    }
}



