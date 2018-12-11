/*!
 * \class
 *
 * \brief
 *
 * \author Administrator
 * \date November 2015
 */

#ifndef TienLenMNUtil_h__
#define TienLenMNUtil_h__
#include "cocos2d.h"
#include "CardInfo.h"
USING_NS_CC;
using namespace std;
enum{
    GAME_TABLE_CARD_TYPE_NONE = 0,
    GAME_TABLE_CARD_TYPE_ONE,
    GAME_TABLE_CARD_TYPE_TWO,
    GAME_TABLE_CARD_TYPE_THREE,
    GAME_TABLE_CARD_TYPE_FOUR,
    GAME_TABLE_CARD_TYPE_2,
    GAME_TABLE_CARD_TYPE_DOUBLE_2,
    GAME_TABLE_CARD_TYPE_3_THONG,
    GAME_TABLE_CARD_TYPE_4_THONG,
    GAME_TABLE_CARD_TYPE_SEQUENCE,
    GAME_TABLE_CARD_TYPE_DOUBLE_FOUR,
    GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT,
    GAME_TABLE_CARD_TYPE_HAI_DOI_THONG
};
class TienLenMNUtil{
private:
public:
    
    static bool isTwo(vector<CardInfo*> listc)
    {
        //ham check xem co phai doi khong
       // log("isTwo");
        if (listc.size() < 2)
            return false;
        if (listc.at(0)->getValueCard() != listc.at(1)->getValueCard()){
           // log("CheckTwo: First Round - 2 card not equal or subtance not valid");
            return false;
        }
        else
            return true;
    }
    
    static bool isThree(vector<CardInfo*> listc)
    {/*
      * Ham check xem co phai bo ba ko
      * */
       // log("isThree");
        if (listc.size() < 3)
            return false;
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard() &&
            listc.at(1)->getValueCard() == listc.at(2)->getValueCard()){
           // log("CheckTwo: First Round - 2 card not equal or subtance not valid");
            return true;
        }
        else
            return false;
    }
    static void showArrowInCard(CardInfo* card, bool isShow)
    {
        //        card->removeChildByTag(TAG_ARROW);
        //        Sprite* arrow = Sprite::create("arrow_phom.png");
        //        arrow->setTag(TAG_ARROW);
        //        arrow->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height));
        //        card->addChild(arrow);
        //        if (isShow)
        //        {
        //            arrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), nullptr)));
        //        }
        //        else{
        //            btn->removeChildByTag(TAG_ARROW);
        //        }
    }
    
    static vector<CardInfo*> get4(vector<CardInfo*> listc)
    {
        vector<CardInfo*> res;
        if (listc.size() < 4) return res;
        vector<CardInfo*> cards;
        for (int i = 0; i < listc.size(); ++i){
            cards.push_back(listc.at(i));
        }
        int i = 0;
        while (true){
            if (cards.size() < 4)
                break;
            if (i >= cards.size() - 3)
                return res;
            if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard() &&
                cards.at(i + 1)->getValueCard() == cards.at(i + 2)->getValueCard() &&
                cards.at(i + 2)->getValueCard() == cards.at(i + 3)->getValueCard() &&
                cards.at(i)->getValueCard() != 15){
                res.push_back(cards.at(i));
                res.push_back(cards.at(i + 1));
                res.push_back(cards.at(i + 2));
                res.push_back(cards.at(i + 3));
                
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                return res;
            }
            else
                i++;
        }
        return res;
    }
    
    static vector<CardInfo*> get4WithChooseCard(vector<CardInfo*> listc,vector<CardInfo*> chooseCard)
    {
        
        vector<CardInfo*> res;
        
        if (listc.size() < 4) return res;
        
        bool isShould4 = true;
        if (chooseCard.size() > 2){
            for (int i = 0; i< chooseCard.size()-1;i++){
                if (chooseCard[i]->getValueCard() != chooseCard[i+1]->getValueCard())
                    isShould4 = false;
            }
        }
        if (!isShould4){
            return res;
        }
        
        vector<CardInfo*> cards;
        for (int i = 0; i < listc.size(); ++i){
            cards.push_back(listc.at(i));
        }
        int i = 0;
        while (true){
            if (cards.size() < 4)
                break;
            if (i >= cards.size() - 3)
                return res;
            if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard() &&
                cards.at(i + 1)->getValueCard() == cards.at(i + 2)->getValueCard() &&
                cards.at(i + 2)->getValueCard() == cards.at(i + 3)->getValueCard() &&
                cards.at(i)->getValueCard() != 15){
                res.push_back(cards.at(i));
                res.push_back(cards.at(i + 1));
                res.push_back(cards.at(i + 2));
                res.push_back(cards.at(i + 3));
                
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                cards.erase(cards.begin() + i);
                if (checkValueExistInList(res, chooseCard)){
                    return res;
                }else{
                    res.clear();
                }
            }
            else
                i++;
        }
        return res;
    }
    
    static bool isFour(vector<CardInfo*> listc)
    {
        /*
         * Ham check xem co phai tu quy ko
         * */
        if (listc.size() < 4)
            return false;
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard() &&
            listc.at(1)->getValueCard() == listc.at(2)->getValueCard() &&
            listc.at(2)->getValueCard() == listc.at(3)->getValueCard()){
           // log("CheckFour: First Round - 4 card is equal");
            return true;
        }
        else
            return false;
    }
    
    static bool isSequence(vector<CardInfo*> listc)
    {
        /*
         * Ham check xem co phai day ko
         * */
        std::sort(listc.begin(), listc.end(), less_than_key());
        if (listc.size() < 3) return false;
        bool issequence = true;
        for (int i = 0; i < listc.size() - 1; ++i){
            if (listc.at(i + 1)->getValueCard() - listc.at(i)->getValueCard() != 1 ||
                listc.at(i + 1)->getValueCard() == 15){
                issequence = false;
                break;
            }
        }
        if (issequence)
            log("isSequence ");
        return issequence;
    }
    
    static bool isDongChat(vector<CardInfo*> listc)
    {
        /*
         * Ham check xem co phai day dong chat khong
         * */
        std::sort(listc.begin(), listc.end(), less_than_key());
        if (listc.size() < 1) return false;
        bool isdongchat = false;
        int typecard = listc[0]->getSubstanceCard();
        int count = 1;
        for (int i = 1; i < listc.size(); ++i){
            if (listc[i]->getSubstanceCard() == typecard){
                count++;
            }
        }
        if (count == listc.size())
            isdongchat = true;
        return isdongchat;
    }
    
    static bool is2DoiThong(vector<CardInfo*> listc)
    {
        /*
         * Ham check xem co phai 2 doi thong khong
         * */
        std::sort(listc.begin(), listc.end(), less_than_key());
        if (listc.size() < 4) return false;
        bool is2doithong = false;
        int value= listc[0]->getValueCard();
        int nextvalue = value+1;
        if (listc[1]->getValueCard() == value && listc[2]->getValueCard() == nextvalue && listc[3]->getValueCard() == nextvalue &&
            value != 14 &&
            nextvalue != 15){
            is2doithong = true;
        }
        //is2doithong = false;
        return is2doithong;
    }
    
    static bool have4(vector<CardInfo*> listc, int value)
    {	/*
         * Ham check xem co tu quy trong 1 danh sach quan bai ko
         * */
        
    }
    
    static bool checkRong(vector<CardInfo*> listc)
    {
        /*
         * Ham check xem co sanh rong ko
         * */
    }
    
    static vector<CardInfo*> get2(vector<CardInfo*> listc)
    {
        /*
         * ham lay cac bo doi lien nhau
         * */
    }
    
    static bool is3Thong(vector<CardInfo*> listc)
    {
        if (listc.size() < 6)
            return false;
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard() &&
            listc.at(2)->getValueCard() == listc.at(3)->getValueCard() &&
            listc.at(4)->getValueCard() == listc.at(5)->getValueCard() &&
            listc.at(2)->getValueCard() - listc.at(0)->getValueCard() == 1 &&
            listc.at(4)->getValueCard() - listc.at(2)->getValueCard() == 1 &&
            listc.at(4)->getValueCard() != 15 &&
            listc.at(0)->getValueCard() != 13){//3 doi thong phai khac doi k, a, 2
            return true;
        }
        else
            return false;
    }
    
    static bool is4Thong(vector<CardInfo*> listc)
    {
        if (listc.size() < 8)
            return false;
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard() &&
            listc.at(2)->getValueCard() == listc.at(3)->getValueCard() &&
            listc.at(4)->getValueCard() == listc.at(5)->getValueCard() &&
            listc.at(6)->getValueCard() == listc.at(7)->getValueCard() &&
            listc.at(2)->getValueCard() - listc.at(0)->getValueCard() == 1 &&
            listc.at(4)->getValueCard() - listc.at(2)->getValueCard() == 1 &&
            listc.at(6)->getValueCard() - listc.at(4)->getValueCard() == 1 && listc.at(7)->getValueCard()<15){
            return true;
        }
        else
            return false;
    }
    static vector<CardInfo*> get2Thong(vector<CardInfo*> listc,vector<CardInfo*> listchoose)
    {
        vector<CardInfo*> listcheck = {};
        if (listc.size() < 4)
            return listcheck;
        vector<CardInfo*> cards;
        for (int i = 0; i < listc.size(); ++i)
        {
            cards.push_back(listc.at(i));
        }
        std::sort(cards.begin(), cards.end(), less_than_key());
        cards = removeForListThong(cards,listchoose);
        if (cards.size()<4) return listcheck;
        
        for (int i = 0; i < cards.size() - 3; ++i)
        {
            for (int j = 0; j < 4; j++)
                listcheck.push_back(cards.at(i + j));
            if (is2DoiThong(listcheck))
                return listcheck;
            else
                listcheck.clear();
        }
        return listcheck;
    }
    static vector<CardInfo*> get3Thong(vector<CardInfo*> listc,vector<CardInfo*> listchoose)
    {
        vector<CardInfo*> listcheck = {};
        if (listc.size() < 6)
            return listcheck;
        vector<CardInfo*> cards;
        for (int i = 0; i < listc.size(); ++i)
        {
            cards.push_back(listc.at(i));
        }
        std::sort(cards.begin(), cards.end(), less_than_key());
        cards = removeForListThong(cards,listchoose);
        if (cards.size()<6) return listcheck;
        for (int i = 0; i < cards.size() - 5; ++i)
        {
            for (int j = 0; j < 6; j++)
                listcheck.push_back(cards.at(i + j));
            if (is3Thong(listcheck))
                return listcheck;
            else
                listcheck.clear();
        }
        return listcheck;
    }
    static vector<CardInfo*> get4Thong(vector<CardInfo*> listc,vector<CardInfo*> listchoose)
    {
        vector<CardInfo*> listcheck = {};
        if (listc.size() < 8)
            return listcheck;
        vector<CardInfo*> cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
     
        cards = removeForListThong(cards,listchoose);
        if (cards.size()<8) return listcheck;
        for (int i = 0; i < cards.size() - 7; i++)
        {
            for (int k = 0 ; k < listchoose.size(); k++){
                listcheck.push_back(listchoose[k]);
            }
            for (int j = 0; j < (8 - listchoose.size()); j++){
                listcheck.push_back(cards.at(i + j));
            }
            std::sort(listcheck.begin(), listcheck.end(), less_than_key());

            if (is4Thong(listcheck))
                return listcheck;
            else
                listcheck.clear();
        }
        return listcheck;
    }
    
    //recommend card
    static vector<CardInfo*> getSequence(vector<CardInfo*> listc, int count){
        //tim sanh tu bo bai
        vector<CardInfo*> cards;
        if (count < 3)
            return cards;
        if (listc.size() < count)
            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
        int i = 0;
        while (true)
        {
            if (i >= (cards.size() - 1))
                break;
            if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard())
                cards.erase(cards.begin() + i);
            else
                i++;
        }
        for (i = 0; i < cards.size() - (count - 1); ++i)
        {
            vector<CardInfo*> sublist;
            for (int j = 0; j < count; j++)
                sublist.push_back(cards.at(i + j));
            if (isSequence(sublist))
                return sublist;
        }
        return cards;
    }
    static vector<CardInfo*> getSequenceDongChat(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard){
        //tim sanh tu bo bai voi dau vao la day quan bai da danh tren ban
        vector<CardInfo*> cards;
        int count = priviousplaycard.size();
        if (count >0 && count < 3)
            return cards;
        if (listc.size() < count)
            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
        
        if (count == 0){
            vector<CardInfo*> sublist;
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                sublist.push_back(cards.at(i));
                for (int j = i+1; j < cards.size(); j++){
                    if ((sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard())
                        sublist.push_back(cards.at(j));
                }
                count = sublist.size() ;
                if ( sublist.size() == count && isSequence(sublist))
                {
                    if (isDongChat(sublist)){
                        
                        
                        return sublist;
                        
                    }
                }
            }
        }else{
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                sublist.push_back(cards.at(i));
                for (int j = i+1; j < cards.size(); j++){
                    if (sublist.size()<count && (sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard() && sublist.at(sublist.size()-1)->getSubstanceCard() == cards[j]->getSubstanceCard())
                        //                        if (sublist.size() == (count - 1)){
                        //                            if (cards[j]->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard())
                        //                                sublist.push_back(cards.at(j));
                        //                        }
                        //                        else{
                        sublist.push_back(cards.at(j));
                    //                        }
                }
                if ( sublist.size() == count && isSequence(sublist))
                {
                    if (isDongChat(sublist)){
                        //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                        if ((sublist.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard()) ||
                            
                            (sublist.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard() && sublist.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard())
                            
                            )
                        {
                            
                            return sublist;
                        }
                    }
                }
            }
        }
        
        cards.clear();
        return cards;
    }
    static vector<CardInfo*> getSequence(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard){
        //tim sanh tu bo bai voi dau vao la day quan bai da danh tren ban
        vector<CardInfo*> cards;
        int count = priviousplaycard.size();
        if (count < 3 && count > 0)
            return cards;
        if (listc.size() < count)
            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
        if (count == 0){
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                sublist.push_back(cards.at(i));
                for (int j = i; j < cards.size(); j++){
                    if ((sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard())
                        sublist.push_back(cards.at(j));
                }
                count = sublist.size();
                if (sublist.size() == count && isSequence(sublist))
                {
                    //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                    return sublist;
                }
            }
        }else{
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                sublist.push_back(cards.at(i));
                for (int j = i; j < cards.size(); j++){
                    if (sublist.size()<count && (sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard())
                        if (sublist.size() == (count - 1)){
                            if (cards[j]->getValueCard() == priviousplaycard.at(count - 1)->getValueCard() ){
                                if (cards[j]->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard())
                                    sublist.push_back(cards.at(j));
                            }
                            else if (cards[j]->getValueCard() > priviousplaycard.at(count - 1)->getValueCard()){
                                sublist.push_back(cards.at(j));
                            }
                        }
                        else{
                            sublist.push_back(cards.at(j));
                        }
                }
                if (sublist.size() == count && isSequence(sublist))
                {
                    //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                    if (sublist.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard() ||
                        (
                         sublist.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard()
                         &&
                         sublist.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard()
                         )
                        )
                        return sublist;
                }
            }
        }
        
        cards.clear();
        return cards;
    }
    
    static vector<CardInfo*> checkRecommend2DoiThong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        
        if (listc.size() < 4)
            return chooseCards;
        
        
        while (listc.size()>4){
            
            vector<CardInfo*> list2Thong = get2Thong(listc,chooseCards);
            if (list2Thong.size() < 4)
            {
                break;
            }
            if (priviousplaycard.size() == 4)
            {
                if (list2Thong.at(3)->getValueCard() > priviousplaycard.at(3)->getValueCard()
                    ||
                    (list2Thong.at(3)->getValueCard() == priviousplaycard.at(3)->getValueCard()
                     &&
                     list2Thong.at(3)->getSubstanceCard() > priviousplaycard.at(3)->getSubstanceCard()
                     )
                    )
                {
                    chooseCards = list2Thong;
                    res = true;
                    break;
                }
                else{
                    listc = removeCard(listc,list2Thong,2);
                }
            }
            
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommendOne(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        for (int i = 0; i < listc.size(); ++i)
        {
            if (listc.at(i)->getValueCard() > priviousplaycard.at(0)->getValueCard()
                ||
                (listc.at(i)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()
                 &&
                 listc.at(i)->getValueCard() == priviousplaycard.at(0)->getValueCard()))
            {
                chooseCards.push_back(listc.at(i));
                res = true;
                break;
            }
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommendTwo(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        if (listc.size() < 2)
            return chooseCards;
        for (int i = 0, n = listc.size(); i < n; ++i)
        {
            if (i < n - 1)
            {
                if (listc.at(i)->getValueCard() == listc.at(i + 1)->getValueCard())
                {
                    if ((listc.at(i + 1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard()
                         &&
                         (listc.at(i + 1)->getValueCard() == priviousplaycard.at(1)->getValueCard())
                         )
                        ||
                        listc.at(i + 1)->getValueCard() > priviousplaycard.at(1)->getValueCard())
                    {
                        chooseCards.push_back(listc.at(i));
                        chooseCards.push_back(listc.at(i+1));
                        res = true;
                        break;
                    }
                    
                }
            }
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommendThree(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        
        if (listc.size() < 3)
            return chooseCards;
        for (int i = 0, n = listc.size(); i < n; ++i)
        {
            if (i < n - 2)
            {
                if (listc.at(i)->getValueCard() == listc.at(i + 1)->getValueCard()
                    &&
                    listc.at(i + 1)->getValueCard() == listc.at(i + 2)->getValueCard())
                {
                    if (listc.at(i + 2)->getValueCard() > priviousplaycard.at(2)->getValueCard())
                    {
                        chooseCards.push_back(listc.at(i));
                        chooseCards.push_back(listc.at(i+1));
                        chooseCards.push_back(listc.at(i+2));
                        
                        res = true;
                        break;
                    }
                    
                }
            }
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommendFour(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        
        if (listc.size() < 4)
            return chooseCards;
        for (int i = 0, n = listc.size(); i < n; ++i)
        {
            if (i < n - 3)
            {
                if (listc.at(i)->getValueCard() == listc.at(i + 1)->getValueCard()
                    &&
                    listc.at(i + 1)->getValueCard() == listc.at(i + 2)->getValueCard()
                    &&
                    listc.at(i + 2)->getValueCard() == listc.at(i + 3)->getValueCard())
                {
                    if (listc.at(i + 3)->getValueCard() > priviousplaycard.at(3)->getValueCard())
                    {
                        chooseCards.push_back(listc.at(i));
                        chooseCards.push_back(listc.at(i+1));
                        chooseCards.push_back(listc.at(i+2));
                        chooseCards.push_back(listc.at(i+3));
                        res = true;
                        break;
                    }
                    
                }
            }
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommendDouble2(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        bool res = false;
        vector<CardInfo*> chooseCards;
        
        if (listc.size() < 2)
            return chooseCards;
        for (int i = 0, n = listc.size(); i < n; ++i)
        {
            if (i < n - 1)
            {
                if (listc.at(i)->getValueCard() == listc.at(i + 1)->getValueCard() && listc.at(i)->getValueCard() == 15)
                {
                    if (listc.at(i + 1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard())
                    {
                        chooseCards.push_back(listc.at(i));
                        chooseCards.push_back(listc.at(i+1));
                        res = true;
                        break;
                    }
                    
                }
            }
        }
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommend3Thong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        //3 thong to hon, 4 thong va tu quy
        bool res = false;
        vector<CardInfo*> chooseCards;
        
        if (listc.size() < 6)
            return chooseCards;
        //uu tien 4 thong truoc
        //        vector<CardInfo*> list4Thong = get4Thong(listc);
        //        if (list4Thong.size() == 8)
        //        {
        //            res = true;
        //            return res;
        //        }
        while (listc.size()>4){
            vector<CardInfo*> list3Thong = get3Thong(listc,chooseCards);
            if (list3Thong.size()<6)
                return chooseCards;
            if (priviousplaycard.size() == 6)
            {
                if (list3Thong.at(5)->getValueCard() > priviousplaycard.at(5)->getValueCard()
                    ||
                    (list3Thong.at(5)->getValueCard() == priviousplaycard.at(5)->getValueCard()
                     &&
                     list3Thong.at(5)->getSubstanceCard() > priviousplaycard.at(5)->getSubstanceCard()
                     )
                    )
                {
                    chooseCards = list3Thong;
                    res = true;
                    break;
                }
                else{
                    listc = removeCard(listc,list3Thong,2);
                }
            }
        }
        //        else if (priviousplaycard.size() == 8)// 4 doi thong
        //        {
        //            bool is4T = is4Thong(priviousplaycard);
        //            if (is4T)
        //                res = true;
        //        }
        // 		else if (priviousplaycard.size() == 2){//danh cho truong hop doi 2
        // 			bool isDouble = isTwo(priviousplaycard);
        // 			if (isDouble)
        // 			{
        // 				if (priviousplaycard.at(0)->getValueCard() == 15)
        // 					res = true;
        // 			}
        // 		}
        
        return chooseCards;
    }
    static vector<CardInfo*> checkRecommend4Thong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard)
    {
        vector<CardInfo*> chooseCards;
        bool res = false;
        if (listc.size() < 8)
            return chooseCards;
        vector<CardInfo*> list3Thong = get4Thong(listc,chooseCards);
        if (list3Thong.size() < 8)
            return chooseCards;
        if (priviousplaycard.size() == 8)
        {
            std::sort(priviousplaycard.begin(), priviousplaycard.end(), less_than_key());
            bool is4T = is4Thong(priviousplaycard);
            if (is4T)
            {
                if (list3Thong.at(7)->getValueCard() > priviousplaycard.at(7)->getValueCard()
                    ||
                    (list3Thong.at(7)->getValueCard() == priviousplaycard.at(7)->getValueCard()
                     &&
                     list3Thong.at(7)->getSubstanceCard() > priviousplaycard.at(7)->getSubstanceCard()
                     )
                    )
                {
                    chooseCards = list3Thong;
                    res = true;
                }
                else{
                    list3Thong.clear();
                }
            }
            
        }
        else if (priviousplaycard.size() == 2){//danh cho truong hop doi 2
            bool isDouble = isTwo(priviousplaycard);
            if (isDouble)
            {
                if (priviousplaycard.at(0)->getValueCard() == 15)
                    res = true;
            }
        }
        
        return list3Thong;
    }
    static vector<CardInfo*> checkBetterSequenceDongChat(int value, vector<CardInfo*> listc, int count)
    {
        //ham lay 1 sanh trong bo bai voi dau vao la 1 gia tri de so sanh
        vector<CardInfo*> cards;
        if (count < 3)
            return cards;
        if (listc.size() < count)
            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
        int i = 0;
        while (true)
        {
            if (i >= (cards.size() - 1))
                break;
            if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard())
                cards.erase(cards.begin() + i);
            else
                i++;
        }
        for (i = 0; i < cards.size() - (count - 1); ++i)
        {
            if (cards.at(i)->getValueCard() > value)
            {
                vector<CardInfo*> sublist;
                for (int j = 0; j < count; j++)
                    sublist.push_back(cards.at(i + j));
                if (isDongChat(sublist))
                    return sublist;
            }
        }
        
        return cards;
    }
    static vector<CardInfo*> checkBetterSequence(int value, vector<CardInfo*> listc, int count)
    {
        //ham lay 1 sanh trong bo bai voi dau vao la 1 gia tri de so sanh
        vector<CardInfo*> cards;
        if (count < 3)
            return cards;
        if (listc.size() < count)
            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        std::sort(cards.begin(), cards.end(), less_than_key());
        int i = 0;
        while (true)
        {
            if (i >= (cards.size() - 1))
                break;
            if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard())
                cards.erase(cards.begin() + i);
            else
                i++;
        }
        for (i = 0; i < cards.size() - (count - 1); ++i)
        {
            if (cards.at(i)->getValueCard() > value)
            {
                vector<CardInfo*> sublist;
                for (int j = 0; j < count; j++)
                    sublist.push_back(cards.at(i + j));
                if (isSequence(sublist))
                    return sublist;
            }
        }
        
        return cards;
    }
    
    static vector<CardInfo*> getRecommendOne(vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> cards;
        if (chooseCards.size() == 0)
            return cards;
        if (chooseCards.size() == 1 ){
            if ( priviousplaycard.size() > 0){
                if ((chooseCards.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                    ||
                    (
                     chooseCards.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()
                     &&
                     chooseCards.at(0)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()
                     ))
                    return chooseCards;
            }else{
                return chooseCards;
            }
            
        }
        if (chooseCards.size() > priviousplaycard.size())
            return cards;
        if (priviousplaycard.size() > 0)
        {
            if ((chooseCards.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                ||
                (
                 chooseCards.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()
                 &&
                 chooseCards.at(0)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()
                 )
                )
            {
                cards.push_back(chooseCards.at(0));
            }
        }
        else
        {
            cards = chooseCards;
        }
        
        return cards;
    }
    static vector<CardInfo*> getRecommendTwo(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> cards;
        if (chooseCards.size() == 0)
            return cards;
        if (chooseCards.size() >=2 )
        {
            if (isTwo(chooseCards)){
                if (priviousplaycard.size() == 2){
                    if ((chooseCards.at(1)->getValueCard() > priviousplaycard.at(1)->getValueCard())
                        ||
                        (
                         chooseCards.at(1)->getValueCard() == priviousplaycard.at(1)->getValueCard()
                         &&
                         chooseCards.at(1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard()
                         )
                        )
                        return chooseCards;
                }else{
                    return chooseCards;
                    
                }
                
            }
            
            return cards;
        }
        vector<CardInfo*> sublist;
        sublist.push_back(chooseCards.at(0));
        listc = removeForListThong(listc,chooseCards);
        listc = removeCard(listc, sublist, sublist.size());
        for (int i = 0; i < listc.size(); i++)
        {
            if (chooseCards.at(0)->getIdCard() != listc.at(i)->getIdCard() &&
                chooseCards.at(0)->getValueCard() == listc.at(i)->getValueCard())
            {
                sublist.push_back(listc.at(i));
                break;
            }
        }
        std::sort(sublist.begin(), sublist.end(), less_than_key());
        if (priviousplaycard.size() == 2 && sublist.size() == 2)
        {
            if ((sublist.at(1)->getValueCard() > priviousplaycard.at(1)->getValueCard())
                ||
                (
                 sublist.at(1)->getValueCard() == priviousplaycard.at(1)->getValueCard()
                 &&
                 sublist.at(1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard()
                 )
                )
            {
                cards = sublist;
            }
            if ((sublist.at(1)->getValueCard() < priviousplaycard.at(1)->getValueCard()) || (
                                                                                             sublist.at(1)->getValueCard() == priviousplaycard.at(1)->getValueCard()
                                                                                             &&
                                                                                             sublist.at(1)->getSubstanceCard() < priviousplaycard.at(1)->getSubstanceCard()
                                                                                             ))
            {
                sublist.clear();
                cards = sublist;
            }
        }
        else
        {
            if (sublist.size() == 2)
                cards = sublist;
            else{
                sublist.clear();
                cards = sublist;
            }
        }
        return cards;
    }
    static vector<CardInfo*> getRecommendFour(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        //ham lay danh sach tu quy goi y dua theo 2 quan dau vao
        
        vector<CardInfo*> cards;
        if (chooseCards.size() >=4 ){
            if (isFour(chooseCards)){
                if (priviousplaycard.size() == 4)
                {
                    if (chooseCards.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                        return chooseCards;
                    else
                        return cards;
                    
                }
            }
            return cards;
        }
        if (listc.size() < 4 || chooseCards.size() < 1)
            return cards;
        int valueCardToCompare = chooseCards.at(0)->getValueCard();//gia tri bo ba de lay trong mang
        for (int i = 0 ; i < chooseCards.size();i++){
            if (chooseCards.at(i)->getValueCard() != valueCardToCompare){
                return cards;
            }
        }
        int dem = 0;
        if (priviousplaycard.size() == 4)
        {
            //danh cho chan quan tren ban
            for (int i = 0; i < listc.size(); i++)
            {
                if (listc.at(i)->getValueCard() == valueCardToCompare  && valueCardToCompare > priviousplaycard.at(0)->getValueCard())
                {
                    cards.push_back(listc.at(i));
                    dem++;
                    if (dem == 4)
                    {
                        break;
                    }
                }
            }
        }
        else{
            //goi y danh lan dau tien
            for (int i = 0; i < listc.size(); i++)
            {
                if (listc.at(i)->getValueCard() == valueCardToCompare)
                {
                    cards.push_back(listc.at(i));
                    dem++;
                    if (dem == 4)
                    {
                        break;
                    }
                }
            }
        }
        
        
        if (dem != 4)
            cards.clear();
        if (!isFour(cards))
            cards.clear();
        
        
        return cards;
    }
    static vector<CardInfo*> getRecommendThree(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        //ham lay danh sach quan bai goi y dua theo 2 quan dau vao
        vector<CardInfo*> cards;
        if (chooseCards.size() ==3 )
        {
            if (isThree(chooseCards)){
                if (priviousplaycard.size()==3){
                    if (priviousplaycard.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard()){
                        return chooseCards;
                    }
                }else{
                    return cards;
                }
            }
            return cards;
        }
        if (listc.size() < 3 || chooseCards.size() < 1)
            return cards;
        
        int valueCardToCompare = chooseCards.at(0)->getValueCard();//gia tri bo ba de lay trong mang
        for (int i = 0 ; i < chooseCards.size();i++){
            if (chooseCards.at(i)->getValueCard() != valueCardToCompare){
                return cards;
            }
        }
        for (int i = 0 ; i < chooseCards.size();i++){
            cards.push_back(chooseCards[i]);
        }
        listc = removeCard(listc, cards, cards.size());
        //listc = removeListFromList(listc, chooseCards);
        if (priviousplaycard.size() == 3)
        {
            //danh cho chan quan
            for (int i = 0; i < listc.size(); i++)
            {
                if (listc.at(i)->getValueCard() == valueCardToCompare && cards.size()<3)
                {
                    cards.push_back(listc.at(i));
                }
            }
        }
        else{
            //danh cho luot danh dau tien
            for (int i = 0; i < listc.size(); i++)
            {
                if (listc.at(i)->getValueCard() == valueCardToCompare && cards.size()<3)
                {
                    cards.push_back(listc.at(i));
                }
            }
        }
        if (!isThree(cards))
            cards.clear();
        return cards;
    }
    static bool isHaveBoBa(vector<CardInfo*> listc){
        if (listc.size()<3) return false;
        for (int i = 0; i < listc.size()-2;i++){
            int valueToComPare = listc[i]->getValueCard();
            if (listc[i+1]->getValueCard() == valueToComPare && listc[i+2]->getValueCard() == valueToComPare){
                return true;
            }
        }
        return false;
    }
    static vector<CardInfo*> getRecommendDouble2(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        //ham lay danh sach goi y de chan quan 2
        vector<CardInfo*> cards;
        //        if (chooseCards.size() >=2 )
        //            return cards;
        if (chooseCards.size() < 1 || listc.size() < 4)
        {
            return cards;
        }
        if (chooseCards.size() == 2)
        {
            if (chooseCards.at(0)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()
                &&
                chooseCards.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()
                )
            {
                cards = chooseCards;
            }
        }
        if (chooseCards.size() == 8)
        {
            if (is4Thong(chooseCards)){
                return chooseCards;
            }
        }
        //uu tien cho 4 doi thong vi so luong quan nhieu
        vector<CardInfo*> listthong = get4Thong(listc,chooseCards);
        if (listthong.size() == 8)//neu co thi so sanh tiep voi 2 quan duoc chon
        {
            //neu 2 quan chon co trong 4 doi thong thi remove va lay list do
            bool exist = checkValueExistInList(listthong, chooseCards);
            if (exist)
            {
                cards = listthong;
                return cards;
            }
        }
        return cards;
    }
    static vector<CardInfo*> getRecommend2(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        //ham lay danh sach goi y de chan quan 2
        vector<CardInfo*> cards;
        //        if (chooseCards.size() >=2 )
        //            return cards;
        
        
        
        
        if (chooseCards.size() < 1 || listc.size() < 4)
        {
            return cards;
        }
        if (chooseCards.size() == 1)
        {
            if (chooseCards.at(0)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()
                &&
                chooseCards.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()
                )
            {
                return chooseCards;
            }
        }
        cards = TienLenMNUtil::checkRecommendOne(listc, priviousplaycard);
        if (cards.size() == 1 && chooseCards.size() == 1){
            if (checkListExistInList(cards,chooseCards))
                return cards;
        }
        cards.clear();
        
        //uu tien cho 4 doi thong vi so luong quan nhieu
        vector<CardInfo*> listthong = get4Thong(listc,chooseCards);
        if (listthong.size() == 8)//neu co thi so sanh tiep voi 2 quan duoc chon
        {
            //neu 2 quan chon co trong 4 doi thong thi remove va lay list do
            //            bool exist = checkValueExistInList(listthong, chooseCards);
            //            if (exist)
            //            {
            //                cards = removeListFromList(listthong, chooseCards);
            //
            //            }
            return listthong;
        }
        else{
            
            listthong = get4WithChooseCard(listc,chooseCards);
            if (listthong.size() == 4)
            {
                //neu 2 quan chon co trong 3 doi thong thi remove va lay list do
                //                bool exist = checkValueExistInList(listthong, chooseCards);
                //                if (exist)
                //                {
                //                    cards = removeListFromList(listthong, chooseCards);
                //
                //                }
                return listthong;
                
            }
        }
        
        return cards;
    }
    static vector<CardInfo*> getRecommend2Thong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> listthong;
        vector<CardInfo*> cards;
        
        if (chooseCards.size() < 1 || chooseCards.size() >= 4){
            if (is2DoiThong(chooseCards)){
                if (priviousplaycard.size() == 4){
                    if (chooseCards.at(3)->getValueCard() > priviousplaycard.at(3)->getValueCard()
                        ||
                        (chooseCards.at(3)->getValueCard() == priviousplaycard.at(3)->getValueCard()
                         &&
                         chooseCards.at(3)->getSubstanceCard() > priviousplaycard.at(3)->getSubstanceCard()
                         )
                        )
                    {
                        return chooseCards;
                    }
                }
            }
            return listthong;
        }
        
        while (listc.size()>4){
            listthong = get2Thong(listc,chooseCards);
            if (listthong.size()<4) break;
            
            if (priviousplaycard.size() < 4)//truong hop nay la luot danh dau tien
            {
                bool isExist = checkValueExistInList(listthong, chooseCards);
                if (!isExist)
                    listthong.clear();
                listc.clear();
                return listthong;
            }
            if (listthong.size() == 4)
            {
                //neu 2 quan chon co trong 3 doi thong thi moi kiem tra tiep xem co hop le khong
                bool exist = checkValueExistInList(listthong, chooseCards);
                if (exist)
                {
                    if (listthong.at(3)->getValueCard() > priviousplaycard.at(3)->getValueCard()
                        ||
                        (listthong.at(3)->getValueCard() == priviousplaycard.at(3)->getValueCard()
                         &&
                         listthong.at(3)->getSubstanceCard() > priviousplaycard.at(3)->getSubstanceCard()
                         )
                        )
                    {
                        cards = listthong;
                        break;
                        return cards;
                        
                    }else{
                        listc = removeCard(listc,listthong,2);
                    }
                }
                else{
                    listc = removeCard(listc,listthong,2);
                }
            }
            else{
                listc.clear();
                break;
            }
            
        }
        return cards;
    }
    static vector<CardInfo*> removeCard(vector<CardInfo*> listc, vector<CardInfo*> lstToRemove,int index){
        if (listc.size()<0 || lstToRemove.size()<index) return listc;
        for (int i = 0 ; i < listc.size(); i ++){
            for (int j=0; j< index;j++ ){
                if (listc[i]->getIdCard()==lstToRemove[j]->getIdCard()){
                    listc.erase(listc.begin() + i);
                }
            }
        }
        return listc;
    }
    static vector<CardInfo*> getRecommend3Thong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> listthong;
        vector<CardInfo*> cards;
        if (chooseCards.size() < 1 || chooseCards.size() > 6)
        {
            return listthong;
        }
        if (chooseCards.size() == 6){
            if (is3Thong(chooseCards)){
                if (priviousplaycard.size() < 6){
                    return listthong;
                }else{
                    if (chooseCards.at(5)->getValueCard() > priviousplaycard.at(5)->getValueCard()
                        ||
                        (chooseCards.at(5)->getValueCard() == priviousplaycard.at(5)->getValueCard()
                         &&
                         chooseCards.at(5)->getSubstanceCard() > priviousplaycard.at(5)->getSubstanceCard()
                         )
                        )
                    {
                        return chooseCards;
                    }
                }
            }
        }

        while (listc.size()>4){
            
            listthong = get3Thong(listc,chooseCards);
            if (listthong.size()<6) break;
            if (priviousplaycard.size() < 6)//truong hop nay la luot danh dau tien
            {
                bool isExist = checkValueExistInList(listthong, chooseCards);
                if (!isExist)
                    listthong.clear();
                listc.clear();
                return listthong;
            }
            
            if (priviousplaycard.size() == 6)
            {
                //neu 2 quan chon co trong 3 doi thong thi moi kiem tra tiep xem co hop le khong
                bool exist = checkValueExistInList(listthong, chooseCards);
                if (exist)
                {
                    if (listthong.at(5)->getValueCard() > priviousplaycard.at(5)->getValueCard()
                        ||
                        (listthong.at(5)->getValueCard() == priviousplaycard.at(5)->getValueCard()
                         &&
                         listthong.at(5)->getSubstanceCard() > priviousplaycard.at(5)->getSubstanceCard()
                         )
                        )
                    {
                        cards = removeListFromList(listthong, chooseCards);
                        break;
                        return cards;
                    }
                    else{
                        listc = removeCard(listc,listthong,2);
                    }
                }
                else{
                    listc = removeCard(listc,listthong,2);
                    
                }
            }
            else{
                listc.clear();
            }
        }
        return cards;
    }
    static vector<CardInfo*> getRecommend4Thong(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> listthong;
        if (chooseCards.size() < 1 || chooseCards.size() >= 8){
            if (is4Thong(chooseCards)){
                if (priviousplaycard.size() < 8){
                    return chooseCards;
                }else{
                    if (chooseCards.at(7)->getValueCard() > priviousplaycard.at(7)->getValueCard()
                        ||
                        (chooseCards.at(7)->getValueCard() == priviousplaycard.at(7)->getValueCard()
                         &&
                         chooseCards.at(7)->getSubstanceCard() > priviousplaycard.at(7)->getSubstanceCard()
                         )
                        )
                    {
                        return chooseCards;
                    }
                }
            }
            return listthong;
        }
        listthong = get4Thong(listc,chooseCards);
        if (priviousplaycard.size() < 8)
        {
            //danh cho luot danh dau tien
            bool isExist = checkValueExistInList(listthong, chooseCards);
            if (!isExist)
                listthong.clear();
            return listthong;
        }
        vector<CardInfo*> cards;
        if (listthong.size() == 8)
        {
            //neu 2 quan chon co trong 3 doi thong thi moi kiem tra tiep xem co hop le khong
            bool exist = checkValueExistInList(listthong, chooseCards);
            if (exist)
            {
                if (listthong.at(7)->getValueCard() > priviousplaycard.at(7)->getValueCard()
                    ||
                    (listthong.at(7)->getValueCard() == priviousplaycard.at(7)->getValueCard()
                     &&
                     listthong.at(7)->getSubstanceCard() > priviousplaycard.at(7)->getSubstanceCard()
                     )
                    )
                {
                    cards = removeListFromList(listthong, chooseCards);
                }
                else{
                    listc = removeCard(listc,listthong,2);
                }
            }
        }
        return cards;
    }
    static vector<CardInfo*> getBestSequenceDongChat(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> cards;
        int count = priviousplaycard.size();
        if (count < 3 && count > 0 )
            return cards;
        if (chooseCards.size()<1)
            return cards;
        if (chooseCards.size()>=3 && count>3){
            if (chooseCards.size() == count && isSequence(chooseCards))
            {
                //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                if (chooseCards.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard() ||
                    (
                     chooseCards.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard()
                     &&
                     chooseCards.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard()
                     )
                    )
                    return chooseCards;
            }
        }
        if (listc.size() < count)
            return cards;
        
        std::sort(cards.begin(), cards.end(), less_than_key());
        //        bool isCanSequence = true;
        //        for (int i = 0; i < chooseCards.size()-1;i++){
        //            if (chooseCards[i]->getValueCard() == chooseCards[i+1]->getValueCard() ){
        //                isCanSequence = false;
        //            }
        //        }
        //        if (!isCanSequence)
        //            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        if (count == 0){
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                bool shouldInsert = true;
                for (int k = 0 ; k < chooseCards.size(); k++){
                    sublist.push_back(chooseCards[k]);
                    if (cards[i]->getValueCard() == chooseCards[k]->getValueCard() || cards[i]->getValueCard() == 15 ){
                        shouldInsert = false;
                    }
                }
                if (shouldInsert){
                    sublist.push_back(cards.at(i));
                }
                for (int j = i+1; j < cards.size(); j++){
                    if ((sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard() && sublist.at(sublist.size()-1)->getSubstanceCard() == cards[j]->getSubstanceCard() && cards[j]->getValueCard() != 15){
                        bool shouldInsert = true;
                        for (int m = 0; m < sublist.size();m++){
                            if (cards[j]->getValueCard() == sublist[m]->getValueCard() ){
                                shouldInsert = false;
                            }
                        }
                        if(shouldInsert)
                            sublist.push_back(cards.at(j));
                    }
                    if (isSequence(sublist)){
                        std::sort(sublist.begin(), sublist.end(), less_than_key());
                    }
                }
                std::sort(sublist.begin(), sublist.end(), less_than_key());
                
                count = sublist.size();
                if (sublist.size() == count && isSequence(sublist))
                {
                    if (isDongChat(sublist)){
                        
                        return sublist;
                    }
                }
            }
        }else{
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                bool shouldInsert = true;
                for (int k = 0 ; k < chooseCards.size(); k++){
                    sublist.push_back(chooseCards[k]);
                    if (cards[i]->getValueCard() == chooseCards[k]->getValueCard()  || cards[i]->getValueCard() == 15){
                        shouldInsert = false;
                    }
                }
                if (shouldInsert){
                    sublist.push_back(cards.at(i));
                }
                for (int j = i+1; j < cards.size(); j++){
                    if (sublist.size()<count && (sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard()  && sublist.at(sublist.size()-1)->getSubstanceCard() == cards[j]->getSubstanceCard() && cards[j]->getValueCard() != 15){
                        bool shouldInsert = true;
                        for (int m = 0; m < sublist.size();m++){
                            if (cards[j]->getValueCard() == sublist[m]->getValueCard() ){
                                shouldInsert = false;
                            }
                        }
                        if(shouldInsert)
                            sublist.push_back(cards.at(j));
                    }
                    if (isSequence(sublist)){
                        std::sort(sublist.begin(), sublist.end(), less_than_key());
                    }
                    if (sublist.size() == count) break;
                }
                std::sort(sublist.begin(), sublist.end(), less_than_key());
                
                if (sublist.size() == count && isSequence(sublist))
                {
                    if (isDongChat(sublist)){
                        //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                        if (sublist.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard() ||
                            (
                             sublist.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard()
                             &&
                             sublist.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard()
                             )
                            )
                            return sublist;
                    }
                }
            }
        }
        
        cards.clear();
        return cards;
    }
    
    static vector<CardInfo*> getBestSequence(vector<CardInfo*> listc, vector<CardInfo*> priviousplaycard, vector<CardInfo*> chooseCards)
    {
        vector<CardInfo*> cards;
        int count = priviousplaycard.size();
        if (count < 3 && count > 0 )
            return cards;
        if (chooseCards.size()<1)
            return cards;
        if (chooseCards.size()>=3 && count>3){
            if (chooseCards.size() == count && isSequence(chooseCards))
            {
                //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                if (chooseCards.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard() ||
                    (
                     chooseCards.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard()
                     &&
                     chooseCards.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard()
                     )
                    )
                    return chooseCards;
            }
        }
        if (listc.size() < count)
            return cards;
        
        std::sort(cards.begin(), cards.end(), less_than_key());
        //        bool isCanSequence = true;
        //        for (int i = 0; i < chooseCards.size()-1;i++){
        //            if (chooseCards[i]->getValueCard() == chooseCards[i+1]->getValueCard() ){
        //                isCanSequence = false;
        //            }
        //        }
        //        if (!isCanSequence)
        //            return cards;
        for (int i = 0; i < listc.size(); ++i)
            cards.push_back(listc.at(i));
        if (count == 0){
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                bool shouldInsert = true;
                for (int k = 0 ; k < chooseCards.size(); k++){
                    sublist.push_back(chooseCards[k]);
                    if (cards[i]->getValueCard() == chooseCards[k]->getValueCard() ||  cards[i]->getValueCard() == 15){
                        shouldInsert = false;
                    }
                }
                if (shouldInsert){
                    sublist.push_back(cards.at(i));
                }
                for (int j = i+1; j < cards.size(); j++){
                    if ((sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard() && cards[j]->getValueCard() != 15){
                        bool shouldInsert = true;
                        for (int m = 0; m < sublist.size();m++){
                            if (cards[j]->getValueCard() == sublist[m]->getValueCard() ){
                                shouldInsert = false;
                            }
                        }
                        if(shouldInsert){
                            sublist.push_back(cards.at(j));
                        }
                    }
                    if (isSequence(sublist)){
                        std::sort(sublist.begin(), sublist.end(), less_than_key());
                    }
                }
                std::sort(sublist.begin(), sublist.end(), less_than_key());
                
                count = sublist.size();
                if (sublist.size() == count && isSequence(sublist))
                {
                    return sublist;
                }
            }
        }else{
            if (cards.size() < count)
                return cards;
            vector<CardInfo*> sublist;
            for (int i = 0; i < cards.size() - 1; ++i)
            {
                sublist.clear();
                bool shouldInsert = true;
                for (int k = 0 ; k < chooseCards.size(); k++){
                    sublist.push_back(chooseCards[k]);
                    if (cards[i]->getValueCard() == chooseCards[k]->getValueCard() || cards[i]->getValueCard() == 15){
                        shouldInsert = false;
                    }
                }
                if (shouldInsert){
                    sublist.push_back(cards.at(i));
                }
                for (int j = i+1; j < cards.size(); j++){
                    if (sublist.size()<count && (sublist.at(sublist.size()-1)->getValueCard()+1) == cards[j]->getValueCard() && cards[j]->getValueCard() != 15){
                        bool shouldInsert = true;
                        for (int m = 0; m < sublist.size();m++){
                            if (cards[j]->getValueCard() == sublist[m]->getValueCard() ){
                                shouldInsert = false;
                            }
                        }
                        if(shouldInsert){
                            if (sublist.size() == (count - 1)){
                                if (cards[j]->getValueCard() == priviousplaycard.at(count - 1)->getValueCard() ){
                                    if (cards[j]->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard())
                                        sublist.push_back(cards.at(j));
                                }
                                else if (cards[j]->getValueCard() > priviousplaycard.at(count - 1)->getValueCard()){
                                    sublist.push_back(cards.at(j));
                                }
                            }
                            else{
                                sublist.push_back(cards.at(j));
                            }
                        }
                    }
                    if (isSequence(sublist)){
                        std::sort(sublist.begin(), sublist.end(), less_than_key());
                    }
                    if (sublist.size() == count) break;
                }
                std::sort(sublist.begin(), sublist.end(), less_than_key());
                
                if (sublist.size() == count && isSequence(sublist))
                {
                    //so sanh tiep 2 quan cuoi cua 2 mang xem co hop le khong
                    if (sublist.at(count - 1)->getValueCard() > priviousplaycard.at(count - 1)->getValueCard() ||
                        (
                         sublist.at(count - 1)->getSubstanceCard() > priviousplaycard.at(count - 1)->getSubstanceCard()
                         &&
                         sublist.at(count - 1)->getValueCard() == priviousplaycard.at(count - 1)->getValueCard()
                         )
                        )
                        return sublist;
                }
            }
        }
        
        cards.clear();
        return cards;
    }
    
    static vector<CardInfo*> removeListFromList(vector<CardInfo*> listc1, vector<CardInfo*> listc2)
    {
        vector<CardInfo*> cards = listc1;
        for (int i = 0; i < cards.size(); ++i)
        {
            for (int j = 0; j < listc2.size(); j++)
            {
                if (cards.at(i)->getIdCard() == listc2.at(j)->getIdCard())
                {
                    cards.erase(cards.begin() + i);
                }
            }
        }
        return cards;
    }
    static bool checkListExistInList(vector<CardInfo*> listc1, vector<CardInfo*> listc2)
    {
        bool res = false;
        int count = 0;
        for (int i = 0; i < listc1.size(); ++i)
        {
            for (int j = 0; j < listc2.size(); j++)
            {
                if (listc1.at(i)->getIdCard() == listc2.at(j)->getIdCard())
                {
                    count++;
                }
            }
        }
        if (count == listc2.size())
            res = true;
        return res;
    }
    static bool checkValueExistInList(vector<CardInfo*> listc1, vector<CardInfo*> listc2)
    {
        if (listc1.size() == 0) return false;
        vector<CardInfo*> sublist = listc1;
        ///remove duplicate value
        int h = 0;
        while (true)
        {
            if (h >= sublist.size() - 1)
                break;
            if (sublist.at(h)->getValueCard() == sublist.at(h + 1)->getValueCard())
                sublist.erase(sublist.begin() + h);
            else
                h++;
        }
        bool res = false;
        int count = 0;
        for (int i = 0; i < sublist.size(); ++i)
        {
            for (int j = 0; j < listc2.size(); j++)
            {
                if (sublist.at(i)->getValueCard() == listc2.at(j)->getValueCard())
                {
                    count++;
                }
            }
        }
        if (count == listc2.size())
            res = true;
        return res;
    }
    
    static vector<CardInfo*> removeForListThong(vector<CardInfo*> listc,vector<CardInfo*> listchoose)//remove cac gia tri co 3 phan tu giong nhau trong mang
    {
        vector<CardInfo*> cards = listc;
        if (isHaveBoBa(listchoose)){
            cards.clear();
            return cards;
        }
        int count  = cards.size();
        if (cards.size() < 4) return cards;
        vector<CardInfo*> prevCards;
        while (isHaveBoBa(cards)) {
            if (!isHaveBoBa(cards))
                break;
            for (unsigned int i = 0; i < cards.size(); i++)
            {
                if (i < cards.size() - 2)
                {
                    if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard() &&
                        cards.at(i + 2)->getValueCard() == cards.at(i)->getValueCard()
                        )
                    {
                        if (i < cards.size() - 3){
                            if (cards.at(i + 3)->getValueCard() == cards.at(i + 2)->getValueCard()){
                                bool remove = true;
                                for (int k = 0; k < listchoose.size(); k++){
                                    if (cards.at(i+3)->getValueCard() == listchoose[k]->getValueCard() && cards.at(i+3)->getSubstanceCard() == listchoose[k]->getSubstanceCard()){
                                        remove = false;
                                    }
                                }
                                if (remove){
                                    cards.erase(cards.begin() + (i + 3));
                                }
                            }
                        }
                        bool remove = true;
                        
                        for (int k = 0; k < listchoose.size(); k++){
                            if (cards.at(i)->getValueCard() == listchoose[k]->getValueCard() && cards.at(i)->getSubstanceCard() == listchoose[k]->getSubstanceCard()){
                                remove = false;
                            }
                        }
                        if (remove){
                            cards.erase(cards.begin() +i);
                        }
                        else{
                            remove = true;
                            for (int k = 0; k < listchoose.size(); k++){
                                if (cards.at(i+1)->getValueCard() == listchoose[k]->getValueCard() && cards.at(i+1)->getSubstanceCard() == listchoose[k]->getSubstanceCard()){
                                    remove = false;
                                }
                            }
                            if (remove){
                                cards.erase(cards.begin() +(i+1));
                            }else{
                                remove = true;
                                for (int k = 0; k < listchoose.size(); k++){
                                    if (cards.at(i+2)->getValueCard() == listchoose[k]->getValueCard() && cards.at(i+2)->getSubstanceCard() == listchoose[k]->getSubstanceCard()){
                                        remove = false;
                                    }
                                }
                                if (remove){
                                    cards.erase(cards.begin() +(i+2));
                                }
                            }
                        }
                        
                    }
                }
            }
        }
        return cards;
    }
    static vector<CardInfo*> removeForList2Thong(vector<CardInfo*> listc)//remove cac gia tri co 3 phan tu giong nhau trong mang
    {
        vector<CardInfo*> cards = listc;
        for (unsigned int i = 0; i < cards.size(); i++)
        {
            if (i < cards.size() - 4)
            {
                if (cards.at(i)->getValueCard() == cards.at(i + 1)->getValueCard() &&
                    cards.at(i + 2)->getValueCard() == cards.at(i)->getValueCard()
                    )
                {
                    if (cards.at(i + 3)->getValueCard() == cards.at(i + 2)->getValueCard())
                        cards.erase(cards.begin() + (i + 3));
                    cards.erase(cards.begin() + (i + 2));
                    
                }
            }
        }
        return cards;
    }
};

#endif // TienLenMNUtil_h__

