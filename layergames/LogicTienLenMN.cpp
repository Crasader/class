#include "LogicTienLenMN.h"
#include "TienLenMNUtil.h"
#include "mUtils.h"
#include "TienLenMBUtil.h"
#include "AllData.h"
#include "SamUtil.h"
LogicTienLenMN::LogicTienLenMN()
{
    this->_gameid = kGameTienLenMienNam;
}
bool LogicTienLenMN::checkFirst(vector<CardInfo*> listc)
{
   // log("check first");
    bool res = false;
    switch (listc.size())
    {
        case 1:
            res = true;
            if (listc.at(0)->getValueCard() == 15)
                previouscardtype = GAME_TABLE_CARD_TYPE_2;
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_ONE;
            break;
        case 2:
            res = TienLenMNUtil::isTwo(listc);
            if (res == true){
                if (listc.at(0)->getValueCard() == 15)
                    previouscardtype = GAME_TABLE_CARD_TYPE_DOUBLE_2;
                else
                    previouscardtype = GAME_TABLE_CARD_TYPE_TWO;
            }
            break;
        case 3:
            res = TienLenMNUtil::isThree(listc);
            if (res == false){
                res = TienLenMNUtil::isSequence(listc);
                if (res == true){
                    if (TienLenMNUtil::isDongChat(listc)){
                        previouscardtype = GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT;
                    }else
                        previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                }
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_THREE;
            break;
        case 4:
            res = TienLenMNUtil::isFour(listc);
            if (res == false){
                res = TienLenMNUtil::isSequence(listc);
                if (res == true){
                    if (TienLenMNUtil::isDongChat(listc)){
                        previouscardtype = GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT;
                    }else
                        previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                }
                else{
                    res = TienLenMNUtil::is2DoiThong(listc);
                    if (res)
                        previouscardtype = GAME_TABLE_CARD_TYPE_HAI_DOI_THONG;
                }
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
            break;
        case 6:
            res = TienLenMNUtil::is3Thong(listc);
            if (res == false){
                res = TienLenMNUtil::isSequence(listc);
                if (res == true){
                    if (TienLenMNUtil::isDongChat(listc)){
                        previouscardtype = GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT;
                    }else
                        previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                }
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_3_THONG;
            break;
        case 8:
            res = TienLenMNUtil::is4Thong(listc);
            if (res == false){
                res = TienLenMNUtil::isSequence(listc);
                if (res == true){
                    if (TienLenMNUtil::isDongChat(listc)){
                        previouscardtype = GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT;
                    }else
                        previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                }
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_4_THONG;
            break;
        default:
            res = TienLenMNUtil::isSequence(listc);
            if (res == true){
                if (TienLenMNUtil::isDongChat(listc)){
                    previouscardtype = GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT;
                }else
                    previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
            }
    }
    return res;
}

bool LogicTienLenMN::checkOne(vector < CardInfo*> listc)
{
   // log("check one");
    if (listc.size() != 1)
        return false;
    if (listc.at(0)->getValueCard() < priviousplaycard.at(0)->getValueCard()
        || (listc.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard() &&
            listc.at(0)->getSubstanceCard() < priviousplaycard.at(0)->getSubstanceCard())){
           // log("CheckOne: Less then previous value or not greater subtance");
            return false;
        }
    else{
        if (listc.at(0)->getValueCard() == 15)//neu bang 2
            previouscardtype = GAME_TABLE_CARD_TYPE_2;
        return true;
    }
}

bool LogicTienLenMN::checkTwo(vector<CardInfo*> listc)
{
   // log("check two");
    if (listc.size() != 2) return false;
    if (listc.at(0)->getValueCard() < priviousplaycard.at(0)->getValueCard() ||
        (listc.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard() &&
         listc.at(1)->getSubstanceCard() < priviousplaycard.at(1)->getSubstanceCard())){
           // log("CheckTwo: Less then previous value or not greater subtance");
            return false;
        }
    else{
        previouscardtype = GAME_TABLE_CARD_TYPE_TWO;
        if (listc.at(0)->getValueCard() == 15)//neu bang 2
            previouscardtype = GAME_TABLE_CARD_TYPE_DOUBLE_2;
        return true;
    }
}

bool LogicTienLenMN::checkPlay(string listcardinfo)
{
    _list = ""; //reset lai de kiem tra
   // log("check play logic card, listcard =  %s", listcardinfo.c_str());
    vector<string> listcard = mUtils::splitString(listcardinfo, ',');
    vector<int> arrID;
    vector<CardInfo*> listc;
    for (int i = 0; i < listcard.size(); ++i){
        arrID.push_back(atoi(listcard.at(i).c_str()));
        //listc.push_back(getCard(listcard.at(i)));
    }
    std::sort(arrID.begin(), arrID.end());///sap xep lai truoc khi kiem tra
    for (int i = 0; i < arrID.size(); ++i){
        listc.push_back(getCard(arrID.at(i)));
    }
    
    bool res = false;
    if (_gameid == kGameTienLenMienBac){
        return checkPlayMB(listc);
    }
    else if (_gameid == kGameSam || _gameid == kGameSamSolo){
        return checkPlaySam(listc);
    }
    else if (_gameid == kGameTienLenMienNam || _gameid == kGameTLMNDemLa || _gameid == kGameDemLaSolo){
        return checkPlayMN(listc);
    }
}
bool LogicTienLenMN::checkPlayMB(vector<CardInfo*> listc)
{
   // log("check Play TLMB");
    if (priviousplaycard.size() > 0){
        if (priviousplaycard.size() == 1 &&
            priviousplaycard.at(0)->getValueCard() == 15 &&
            listc.size() == 4){
            if (checkFourMB(listc) == false){
                return false;
            }
        }
        else{
            if (listc.size() != priviousplaycard.size()){
                return false;
            }
        }
    }
    
    
    bool res = true;
    if (listc.size() == 1){
        res = checkOneMB(listc);
    }
    else if (listc.size() == 2){
        res = checkTwoMB(listc);
    }
    else if (listc.size() == 3){
        res = checkThreeMB(listc);
    }
    else if (listc.size() == 4){
        res = checkFourMB(listc);
    }
    else{
        res = checkSequenceMB(listc);
    }
    return res;
}

bool LogicTienLenMN::checkPlayMN(vector<CardInfo*> listc)
{
    // 	vector<string> listcard = mUtils::splitString(listcardinfo, ',');
    // 	vector<CardInfo*> listc;
    // 	for (int i = 0; i < listcard.size(); ++i){
    // 		listc.push_back(getCard(listcard.at(i)));
    // 	}
   // log("check Play TLMN");
    bool res = false;
    // 	if (priviousplaycard.size()>0){
    // 		checkFirst(listc);///neu tren ban da co 1 quan thi phai check lan dau tien de xac dinh type
    // // 		if (priviousplaycard.size() == 0)///check neu minh la nguoi lan dau danh thi tra ve true luon
    // // 		{
    // //
    // // 		}
    // 	}
    switch (previouscardtype)
    {
        case GAME_TABLE_CARD_TYPE_HAI_DOI_THONG:
            if (listc.size() == priviousplaycard.size()){
                if (TienLenMNUtil::is2DoiThong(listc)){
                    int len = listc.size();
                    if (listc.at(len - 1)->getValueCard() > priviousplaycard.at(len - 1)->getValueCard()
                        ||
                        (listc.at(len - 1)->getValueCard() == priviousplaycard.at(len - 1)->getValueCard()
                         &&
                         listc.at(len - 1)->getSubstanceCard() > priviousplaycard.at(len - 1)->getSubstanceCard()
                         )
                        ){
                        return true;
                    }
                }
            }
            return res;
        case GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT:
            if (listc.size() == priviousplaycard.size()){
                if (TienLenMNUtil::isSequence(listc)){
                    if (TienLenMNUtil::isDongChat(listc)){
                        int len = listc.size();
                        if (listc.at(len - 1)->getValueCard() > priviousplaycard.at(len - 1)->getValueCard()
                            ||
                            (listc.at(len - 1)->getValueCard() == priviousplaycard.at(len - 1)->getValueCard()
                             &&
                             listc.at(len - 1)->getSubstanceCard() > priviousplaycard.at(len - 1)->getSubstanceCard()
                             )
                            ){
                            return true;
                        }
                    }
                }
            }
            return res;
        case GAME_TABLE_CARD_TYPE_NONE:
            res = checkFirst(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_ONE:
            res = checkOne(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_TWO:
            if (TienLenMNUtil::isTwo(listc))
                res = checkTwo(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_THREE:
            if (TienLenMNUtil::isThree(listc))
            {
                if (listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                    res = true;
            }
            return res;
        case GAME_TABLE_CARD_TYPE_FOUR:
            if (TienLenMNUtil::isFour(listc)){
                if (listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                    res = true;
            }
            else
                res = TienLenMNUtil::is4Thong(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_2:
            if (listc.size() == 1)
                res = checkOne(listc);
            else if (listc.size() == 4){
                res = TienLenMNUtil::isFour(listc);
                if (res){
                    previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
                }
            }
            else if (listc.size() == 8){
                res = TienLenMNUtil::is4Thong(listc);
                if (res){
                    previouscardtype = GAME_TABLE_CARD_TYPE_4_THONG;
                }
            }
            return res;
            
        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
            if (listc.size() == 2) //2 ddoi 2
            {
                res = TienLenMNUtil::isTwo(listc);
                if (res){
                    if (listc.at(1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard() &&
                        listc.at(1)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard() &&
                        listc.at(1)->getValueCard() == 15)//phai la doi hai moi check
                    {
                        return true;
                    }
                    return false;
                }
                return false;
            }
            //        else if (listc.size() == 4){
            //            res = TienLenMNUtil::isFour(listc);
            //            if (res){
            //                previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
            //            }
            //        }
            else if (listc.size() == 8){
                res = TienLenMNUtil::is4Thong(listc);
                if (res){
                    previouscardtype = GAME_TABLE_CARD_TYPE_4_THONG;
                }
            }
            return res;
        case GAME_TABLE_CARD_TYPE_3_THONG:
            if (listc.size() == 6){
                res = TienLenMNUtil::is3Thong(listc);
                if (res){
                    if (listc.at(5)->getValueCard() > priviousplaycard.at(5)->getValueCard())
                        return true;
                    if (listc.at(5)->getValueCard() == priviousplaycard.at(5)->getValueCard() && listc.at(5)->getSubstanceCard() > priviousplaycard.at(5)->getSubstanceCard())
                        return true;
                }
                res = false;
            }
            //        else if (listc.size() == 8){
            //            res = TienLenMNUtil::is4Thong(listc);
            //            if (res == true){
            //                previouscardtype = GAME_TABLE_CARD_TYPE_4_THONG;
            //            }
            //        }
            //        else if (listc.size() == 4)
            //        {
            //            res = TienLenMNUtil::isFour(listc);
            //        }
            return res;
        case GAME_TABLE_CARD_TYPE_4_THONG:
            if (TienLenMNUtil::is4Thong(listc)){
                if (listc.size() == priviousplaycard.size()){
                if (listc.at(7)->getValueCard() > priviousplaycard.at(7)->getValueCard())
                    return true;
                else if (listc.at(7)->getValueCard() == priviousplaycard.at(7)->getValueCard() && listc.at(7)->getSubstanceCard() > priviousplaycard.at(7)->getSubstanceCard())
                    return true;
                else
                    return false;
                }
                return true;
            }
            return res;
        case GAME_TABLE_CARD_TYPE_SEQUENCE:
            if (listc.size() == priviousplaycard.size()){
                if (TienLenMNUtil::isSequence(listc)){
                    int len = listc.size();
                    if (listc.at(len - 1)->getValueCard() > priviousplaycard.at(len - 1)->getValueCard())
                        return true;
                    if (listc.at(len - 1)->getValueCard() == priviousplaycard.at(len - 1)->getValueCard() && listc.at(len - 1)->getSubstanceCard() > priviousplaycard.at(len - 1)->getSubstanceCard())
                        return true;

                }
            }
            return res;
        default:
            return res;
    }
}
bool LogicTienLenMN::checkPlaySam(vector<CardInfo*> listc)
{
   // log("check Play Sam");
    bool res = false;
    // 	if (previouscardtype == GAME_TABLE_CARD_TYPE_NONE){
    // 		checkFirstSam(listc);
    // 		if (priviousplaycard.size() == 0)///check neu minh la nguoi lan dau danh thi tra ve true luon
    // 			return true;
    // 	}
    switch (previouscardtype)
    {
        case GAME_TABLE_CARD_TYPE_NONE:
            res = checkFirstSam(listc);
            // 		if (previouscardtype == GAME_TABLE_CARD_TYPE_SEQUENCE){
            // 			setListReOrder(SamUtil::getListOrder());
            // 		}
            return res;
        case GAME_TABLE_CARD_TYPE_ONE:
            res = checkOneSam(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_TWO:
            if (SamUtil::isTwo(listc))
                res = checkTwoSam(listc);
            return res;
        case GAME_TABLE_CARD_TYPE_THREE:
            if (SamUtil::isThree(listc)){
                if (listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                    res = true;
            }
            return res;
        case GAME_TABLE_CARD_TYPE_FOUR:
            if (SamUtil::isFour(listc)){
                if (listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard())
                    res = true;
            }
            return res;
        case GAME_TABLE_CARD_TYPE_2:
            if (listc.size() == 1)
                res = checkOneSam(listc);
            else if (listc.size() == 4)
            {
                res = SamUtil::isFour(listc);
                if (res == true)
                    previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
            }
            return res;
            
        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
            if (listc.size() == 2){
                res = checkTwoSam(listc);
            }
            else if (listc.size() == 8){
                vector<CardInfo*> list4 = TienLenMNUtil::get4(listc);
                if (list4.size() == 8){
                    res = true;
                    previouscardtype = GAME_TABLE_CARD_TYPE_DOUBLE_FOUR;
                }
            }
            return res;
        case GAME_TABLE_CARD_TYPE_SEQUENCE:
            if (listc.size() == priviousplaycard.size()){
                if (SamUtil::isSequence(listc)){
                    int len = listc.size();
                    int value_1 = listc.at(len - 1)->getValueCard();
                    int value_2 = priviousplaycard.at(len - 1)->getValueCard();
                    //MINHDV - fix loi khi chat de 123, 234 va 10,j,q
                    if (value_2 == 15)//neu thang truoc day la quan 2 thi lay thang gan nhat duoi 2 va tranh quan at
                    {
                        int value_21 = priviousplaycard.at(len - 2)->getValueCard();
                        if (value_21 != 14)//tranh ca quan at vi no van to hon cac quan con lai
                        {
                            if (value_1 > value_21)
                                return true;
                        }
                        else{
                            int value_22 = priviousplaycard.at(len - 3)->getValueCard();
                            if (value_1 > value_22)
                                return true;
                        }
                    }
                    else{
                        if (value_1 > value_2)
                            return true;
                    }
                    
                }
            }
            // 		if (res){//neu check day ok, thi reorder lai listcard danh cho viec sap xep lai day 1,2,3
            // 			setListReOrder(SamUtil::getListOrder());
            // 		}
            return res;
            
        default:
            break;
    }
    return res;
}

void LogicTienLenMN::setPreviousCard(string listc)
{
    if (listc.size() == 0) return;
    priviousplaycard.clear();
   // log("PREVIOUS CARD = %s", listc.c_str());
    vector<string> listcard = mUtils::splitString(listc, ',');
    std::sort(listcard.begin(), listcard.end());
    for (int i = 0; i < listcard.size(); ++i){
        priviousplaycard.push_back(getCard(listcard.at(i)));
    }
    std::sort(priviousplaycard.begin(), priviousplaycard.end(), less_than_key());
    if (_gameid == kGameTienLenMienBac) return;
    ///neu la cac game tien len mien nam, sam thi phai thuc hien checkFirst
    if (_gameid == kGameSam)
        checkFirstSam(priviousplaycard);
    else
        checkFirst(priviousplaycard);
}

void LogicTienLenMN::resetGame()
{
    priviousplaycard.clear();
    previouscardtype = GAME_TABLE_CARD_TYPE_NONE;
}

CardInfo* LogicTienLenMN::getCard(string listCard)
{
    //cong thuc id = value * 4 + subtance;
    int _id = atoi(listCard.c_str());
    int _value = _id / 4;
    int _substance = _id % 4;
    if (_substance == 0){
        _value = _value - 1;
        _substance = 4;
    }
    bool _turnup = true;
    //log("quanbai: %d_%d_%d", _id, _value, _substance);
    return new CardInfo(_id, _value, _substance, _turnup);
}

CardInfo* LogicTienLenMN::getCard(int idCard)
{
    //cong thuc id = value * 4 + subtance;
    int _id = idCard;
    int _value = _id / 4;
    int _substance = _id % 4;
    if (_substance == 0){
        _value = _value - 1;
        _substance = 4;
    }
    bool _turnup = true;
    //log("quanbai: %d_%d_%d", _id, _value, _substance);
    return new CardInfo(_id, _value, _substance, _turnup);
}

bool LogicTienLenMN::checkOneMB(vector<CardInfo*> listc)
{
    if (priviousplaycard.size() > 0){
        if (priviousplaycard.at(0)->getValueCard() == 15){
            if (listc.at(0)->getValueCard() < 15
                ||
                listc.at(0)->getSubstanceCard() < priviousplaycard.at(0)->getSubstanceCard()){
               // log("CheckOne: Previous is 2, and now is less than 2 or less then substance");
                return false;
            }
            
        }
        else{
            if (listc.at(0)->getValueCard() != 15
                &&
                (listc.at(0)->getValueCard() <= priviousplaycard.at(0)->getValueCard()
                 ||
                 listc.at(0)->getSubstanceCard() != priviousplaycard.at(0)->getSubstanceCard()
                 )
                ){
               // log("CheckOne: Less then previous value or not equal subtance");
                return false;
            }
        }
    }
    previouscardtype = GAME_TABLE_CARD_TYPE_ONE;
    return true;
}

bool LogicTienLenMN::checkTwoMB(vector<CardInfo*> listc)
{
    if (listc.at(0)->getValueCard() != listc.at(1)->getValueCard()){
       // log("CheckTow: two card not equal");
        return false;
    }
    
    if (listc.at(0)->getValueCard() != 15){
        if (listc.at(0)->getSubstanceCard() == CardInfo::CARD_SUBSTANCE_TEP
            || listc.at(1)->getSubstanceCard() - listc.at(0)->getSubstanceCard() != 1
            ){
           // log("CheckTow: First Round - 2 card not equal or subtance not valid");
            return false;
        }
    }
    if (priviousplaycard.size() > 0){
        if (priviousplaycard.at(0)->getValueCard() == 15){
            if (listc.at(0)->getValueCard() < 15
                ||
                listc.at(0)->getSubstanceCard() < priviousplaycard.at(0)->getSubstanceCard()
                ||
                listc.at(0)->getSubstanceCard() < priviousplaycard.at(1)->getSubstanceCard()
                ){
               // log("CheckTwo: Previous is 2, and now is less than 2 or less then substance");
                return false;
            }
            else{
                if (listc.at(0)->getValueCard() != 15 &&
                    (listc.at(0)->getValueCard() <= priviousplaycard.at(0)->getValueCard()
                     || listc.at(0)->getSubstanceCard() != priviousplaycard.at(0)->getSubstanceCard()
                     || listc.at(1)->getSubstanceCard() != priviousplaycard.at(1)->getSubstanceCard()
                     )){
                       // log("CheckTwo: Less then previous value or not equal subtance");
                        return false;
                    }
            }
        }
    }
    
    previouscardtype = GAME_TABLE_CARD_TYPE_TWO;
    return true;
}

bool LogicTienLenMN::checkThreeMB(vector<CardInfo*> listc)
{
    if (priviousplaycard.size() <= 0){
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard() &&
            listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
            ){
           // log("CheckThree: First Round - 3 card equal");
            previouscardtype = GAME_TABLE_CARD_TYPE_THREE;
            return true;
        }
        else{
           // log("CheckThree: First Round - 3 card not equal, chec sequence");
            if (TienLenMBUtil::isSequence(listc)){
               // log("CheckThree: First Round - 3 card not equal but is sequence");
                previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                return true;
            }
            else
                return false;
        }
    }
    else{
        if (previouscardtype == GAME_TABLE_CARD_TYPE_THREE){
            //Vi co 4 quan bai nen neu la 3 thi ko the co chuyen trung gia tri
            if (
                (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
                 &&
                 listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
                 &&
                 listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard()
                 &&
                 (listc.at(0)->getValueCard() == 15
                  ||
                  (listc.at(0)->getSubstanceCard() == priviousplaycard.at(0)->getSubstanceCard()
                   && listc.at(1)->getSubstanceCard() == priviousplaycard.at(1)->getSubstanceCard()
                   && listc.at(2)->getSubstanceCard() == priviousplaycard.at(2)->getSubstanceCard()
                   )
                  )
                 )
                ){
               // log("CheckThree: 3 card equal end greate then previous");
                return true;
            }
            else
                return false;
        }
        else if (previouscardtype == GAME_TABLE_CARD_TYPE_SEQUENCE){
            return checkSequenceMB(listc);
        }
        else{
           // log("checkThree.Exception");
            return false;
        }
    }
}

bool LogicTienLenMN::checkFourMB(vector<CardInfo*> listc)
{
    if (priviousplaycard.size() <= 0){
        if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
            && listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
            && listc.at(2)->getValueCard() == listc.at(3)->getValueCard()
            ){
           // log("CheckFour: First Round - 4 card is equal");
            previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
            return true;
        }
        else{
            if (TienLenMBUtil::isSequence(listc)){
               // log("CheckFour: First Round - 4 card not equal but is sequence");
                previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
                return true;
            }
            else
                return false;
        }
    }
    else{
        if (previouscardtype == GAME_TABLE_CARD_TYPE_FOUR){
            if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
                && listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
                && listc.at(2)->getValueCard() == listc.at(3)->getValueCard()
                && listc.at(0)->getValueCard() > priviousplaycard.at(0)->getValueCard()
                ){
               // log("CheckFour: 4 card is equal");
                return true;
            }
            else
                return false;
        }
        else if (previouscardtype == GAME_TABLE_CARD_TYPE_ONE){
            if (listc.at(0)->getValueCard() == listc.at(1)->getValueCard()
                && listc.at(1)->getValueCard() == listc.at(2)->getValueCard()
                && listc.at(2)->getValueCard() == listc.at(3)->getValueCard()
                ){
               // log("CheckFour: 4 card is equal, is greater 2");
                return true;
            }
            else
                return false;
        }
        else if (previouscardtype == GAME_TABLE_CARD_TYPE_SEQUENCE){
            return checkSequenceMB(listc);
        }
        else
        {
           // log("check four exception");
            return false;
        }
    }
}

bool LogicTienLenMN::checkSequenceMB(vector<CardInfo*> listc)
{
   // log("CheckSequence: check sequence %d ", listc.size());
    if (!TienLenMBUtil::isSequence(listc))
        return false;
    if (priviousplaycard.size() <= 0)
    {
       // log("CheckSeQuence: First Round - is sequence");
        return true;
    }
    else{
        if (listc.at(0)->getValueCard() > priviousplaycard.at(priviousplaycard.size() - 1)->getValueCard()
            &&
            listc.at(0)->getSubstanceCard() == priviousplaycard.at(0)->getSubstanceCard()
            )
            return true;
        else
            return false;
    }
    
}

bool LogicTienLenMN::checkFirstSam(vector<CardInfo*> listc)
{
   // log("check first");
    bool res = false;
    switch (listc.size())
    {
        case 1:
            res = true;
            if (listc.at(0)->getValueCard() == 15)
                previouscardtype = GAME_TABLE_CARD_TYPE_2;
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_ONE;
            break;
        case 2:
            res = SamUtil::isTwo(listc);
            if (res == true){
                if (listc.at(0)->getValueCard() == 15)
                    previouscardtype = GAME_TABLE_CARD_TYPE_DOUBLE_2;
                else
                    previouscardtype = GAME_TABLE_CARD_TYPE_TWO;
            }
            break;
        case 3:
            res = SamUtil::isThree(listc);
            if (res == false){
                res = SamUtil::isSequence(listc);
                if (res == true)
                    previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_THREE;
            break;
            
        case 4:
            res = SamUtil::isFour(listc);
            if (res == false){
                res = SamUtil::isSequence(listc);
                if (res == true)
                    previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
            }
            else
                previouscardtype = GAME_TABLE_CARD_TYPE_FOUR;
            break;
            
        default:
            res = SamUtil::isSequence(listc);
            if (res == true)
                previouscardtype = GAME_TABLE_CARD_TYPE_SEQUENCE;
            break;
    }
    return res;
}

bool LogicTienLenMN::checkOneSam(vector < CardInfo*> listc)
{
   // log("Check One");
    if (listc.size() != 1)
        return false;
    if (listc.at(0)->getValueCard() < priviousplaycard.at(0)->getValueCard()){
       // log("CheckOne: Less then previous or equal");
        return false;
    }
    else if (listc.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()){
        // 		if (listc.at(0)->getValueCard() == 15 &&
        // 			listc.at(0)->getSubstanceCard() > priviousplaycard.at(0)->getSubstanceCard()){
        // 			return true;
        // 		}
        // 		else
        return false;
    }
    else
    {
        if (listc.at(0)->getValueCard() == 15)// neu la quan 2
            previouscardtype = GAME_TABLE_CARD_TYPE_2;
        return true;
    }
}

bool LogicTienLenMN::checkTwoSam(vector<CardInfo*> listc)
{
   // log("check Two");
    if (listc.size() != 2)
        return false;
    if (listc.at(0)->getValueCard() < priviousplaycard.at(0)->getValueCard()){
       // log("CheckOne: Less then previous or equal");
        return false;
    }
    else if (listc.at(0)->getValueCard() == priviousplaycard.at(0)->getValueCard()){
        if (listc.at(0)->getValueCard() == 15 &&
            listc.at(1)->getSubstanceCard() > priviousplaycard.at(1)->getSubstanceCard()){
            return true;
        }
        else
            return false;
    }
    else
    {
        previouscardtype = GAME_TABLE_CARD_TYPE_TWO;
        if (listc.at(0)->getValueCard() == 15)// neu bang 2
            previouscardtype = GAME_TABLE_CARD_TYPE_DOUBLE_2;
        return true;
        
    }
}

CardInfo* LogicTienLenMN::getCardPhom(string idCard)
{
    ///	int num = (pID % 13) + 1;
    //int type = (pID / 13) + 1;
    int _id = atoi(idCard.c_str());
    int _value = (_id % 13) + 1;
    int _substance = (_id / 13) + 1;
    // 	if (_substance == 0){
    // 		_value = _value - 1;
    // 		_substance = 4;
    // 	}
    bool _turnup = true;
    //log("quanbai: %d_%d_%d", _id, _value, _substance);
    return new CardInfo(_id, _value, _substance, _turnup);
}

//recommend card

bool LogicTienLenMN::isConsecutive(vector<int> arr)
{
    if (arr.size() < 1)
        return false;
    //get min element in array
    int min = *std::min_element(std::begin(arr), std::end(arr));
    
    //get max element in array
    int max = *std::max_element(std::begin(arr), std::end(arr));
    //max - min + 1 is equal to n, then onley check all elements;
    if (max - min + 1 == arr.size())
    {
        /* Create a temp array to hold visited flag of all elements.
         Note that, calloc is used here so that all values are initialized
         as false */
        bool *visited = (bool *)calloc(arr.size(), sizeof(bool));
        int i;
        for (i = 0; i < arr.size(); i++)
        {
            /* If we see an element again, then return false */
            if (visited[arr[i] - min] != false)
                return false;
            /* If visited first time, then mark the element as visited */
            visited[arr[i] - min] = true;
        }
        /* If all elements occur once, then return true */
        return true;
    }
    return false;// if (max - min  + 1 != n)
}

int LogicTienLenMN::getMin(vector<int> arr)
{
    int min = arr.at(0);
    for (int i = 1; i < arr.size(); i++)
        if (arr.at(i) < min)
            min = arr.at(i);
    return min;
}

int LogicTienLenMN::getMax(vector<int> arr)
{
    int max = arr.at(0);
    for (int i = 1; i < arr.size(); i++)
    {
        if (arr.at(i)>max)
            max = arr.at(i);
        return max;
    }
}

std::string LogicTienLenMN::getListCardFromArray(vector<CardInfo*> _listCard)
{
    string _listStr = "";
    for (int i = 0; i < _listCard.size(); ++i)
    {
        _listStr += StringUtils::format("%d,", _listCard.at(i)->getIdCard());
    }
   // log("List convert %s = ", _listStr.c_str());
    return _listStr;
}

vector<int> LogicTienLenMN::getListIDFromArray(vector<CardInfo*> _listCard)
{
    vector<int> listID;
    for (CardInfo* card : _listCard)
    {
        listID.push_back(card->getIdCard());
    }
    return listID;
}

vector<int> LogicTienLenMN::splitStringtoInt(string &S, const char &str)
{
    vector<int> arrStr;
    string::iterator t, t2;
    for (t = S.begin(); t < S.end();){
        t2 = find(t, S.end(), str);
        if (t != t2)
            arrStr.push_back(atoi(string(t, t2).c_str()));
        if (t2 == S.end())
            break;
        t = t2 + 1;
    }
    return arrStr;
}

vector<CardInfo*> LogicTienLenMN::checkRecommendCard(string listcardinfo)
{
   // log("checkRecommendCard, listcard =  %s", listcardinfo.c_str());
    vector<int> listcard = splitStringtoInt(listcardinfo, ',');
    vector<CardInfo*> listc;
    std::sort(listcard.begin(), listcard.end());///sap xep lai truoc khi kiem tra
    for (int i = 0; i < listcard.size(); ++i){
        listc.push_back(getCard(listcard.at(i)));
    }
    
    bool res = false;
//    if (_gameid == kGameTienLenMienBac){
//        return checkRecommendMB(listc);
//    }
//    else if (_gameid == kGameSam || _gameid == kGameSamSolo){
//        return checkRecommendSam(listc);
//    }
//    else{
        return checkRecommendMN(listc);
 //   }
}

vector<CardInfo*> LogicTienLenMN::checkRecommendMN(vector<CardInfo*> listc)
{
   // log("checkRecommendMN");
    vector<CardInfo*> res ;
    if (priviousplaycard.size() == 0){
        previouscardtype = GAME_TABLE_CARD_TYPE_NONE;
        return res;
    }
    switch (previouscardtype)
    {
        case GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT://
        {
            auto listSequence = TienLenMNUtil::getSequenceDongChat(listc, priviousplaycard);
            if (listSequence.size() == priviousplaycard.size())
                res = listSequence;
            break;
        }
        case GAME_TABLE_CARD_TYPE_HAI_DOI_THONG://
            res = TienLenMNUtil::checkRecommend2DoiThong(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_ONE://chi can kiem tra xem co quan nao to hon thi goi y
            res = TienLenMNUtil::checkRecommendOne(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_TWO://check bo doi
            res = TienLenMNUtil::checkRecommendTwo(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_THREE://check bo ba
            res = TienLenMNUtil::checkRecommendThree(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_FOUR://check tu quy
            res = TienLenMNUtil::checkRecommendFour(listc, priviousplaycard);
            if (res.size()==0)
            {
                vector<CardInfo*> listchoose ;
                vector<CardInfo*> listThong = TienLenMNUtil::get4Thong(listc,listchoose);
                if (listThong.size() ==8 )//neu khong co 3 thong check tiep xem co tu quy hay khong
                {
                    res = listThong;
                }
            }
            break;
        case GAME_TABLE_CARD_TYPE_2: //check 2 thi phai check xem co tu quy hoac 3 doi thong khong, hoac 2 to hon
        {
            res = TienLenMNUtil::checkRecommendOne(listc, priviousplaycard);
            //neu res = false, check tiep truong hop xem co 3 thong khong
            if (res.size()==0)
            {
                vector<CardInfo*> listchoose ;
                
                vector<CardInfo*> listThong = TienLenMNUtil::get4Thong(listc,listchoose);
                if (listThong.size() <= 0)//neu khong co 3 thong check tiep xem co tu quy hay khong
                {
                    vector<CardInfo*> listThong = TienLenMNUtil::get4(listc);
                    if (listThong.size() == 4)
                        res = listThong;
                }
                else{
                    res = listThong;
                }
            }
        }
            break;
        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
            res = TienLenMNUtil::checkRecommendDouble2(listc, priviousplaycard);
            if (res.size() <= 0)
            {
                //            vector<CardInfo*> list4quy = TienLenMNUtil::get4(listc);
                //            if (list4quy.size() == 4)
                //            {
                //                res = true;
                //                break;
                //            }
                // 			res = TienLenMNUtil::checkRecommend3Thong(listc, priviousplaycard);
                // 			if (res == true)
                // 				break;
                vector<CardInfo*> listchoose ;
                vector<CardInfo*> list4quy = TienLenMNUtil::get4Thong(listc,listchoose);
                if (list4quy.size() == 8)
                {
                    res = TienLenMNUtil::checkRecommend4Thong(listc, priviousplaycard);
                    if (res == list4quy)
                        break;
                }
            }
            break;
        case GAME_TABLE_CARD_TYPE_3_THONG:
            res = TienLenMNUtil::checkRecommend3Thong(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_4_THONG:
            res = TienLenMNUtil::checkRecommend4Thong(listc, priviousplaycard);
            break;
        case GAME_TABLE_CARD_TYPE_SEQUENCE:
        {
            vector<CardInfo*> listSequence = TienLenMNUtil::getSequence(listc, priviousplaycard);
            if (listSequence.size() == priviousplaycard.size())
                res = listSequence;
        }
            break;
        default:
            return res;
    }
    return res;
}

bool LogicTienLenMN::checkRecommendMB(vector<CardInfo*> listc)
{
    return false;
}

bool LogicTienLenMN::checkRecommendSam(vector<CardInfo*> listc)
{
//   // log("checkRecommendSam");
//    bool res = false;
//    switch (previouscardtype)
//    {
//        case GAME_TABLE_CARD_TYPE_ONE://chi can kiem tra xem co quan nao to hon thi goi y
//            res = SamUtil::checkRecommendOne(listc, priviousplaycard);
//            break;
//        case GAME_TABLE_CARD_TYPE_2: //check 2 thi phai check xem co tu quy hoac 3 doi thong khong, hoac 2 to hon
//        {
//
//            vector<CardInfo*> list4quy;
//            list4quy = TienLenMNUtil::get4(listc);
//            if (list4quy.size() == 4)
//                res = true;
//
//
//        }
//            break;
//        case GAME_TABLE_CARD_TYPE_TWO://check bo doi
//        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
//            res = SamUtil::checkRecommendTwo(listc, priviousplaycard);
//            break;
//        case GAME_TABLE_CARD_TYPE_THREE://check bo ba
//            res = TienLenMNUtil::checkRecommendThree(listc, priviousplaycard);
//            break;
//        case GAME_TABLE_CARD_TYPE_FOUR://check tu quy
//            res = TienLenMNUtil::checkRecommendFour(listc, priviousplaycard);
//            break;
//            //     case GAME_TABLE_CARD_TYPE_DOUBLE_2:
//            //     {
//            //                                           vector<CardInfo*> list4quy = TienLenMNUtil::get4(listc);
//            //                                           if (list4quy.size() == 4)
//            //                                               res = true;
//            //     }
//            //         break;
//        case GAME_TABLE_CARD_TYPE_SEQUENCE:
//        {
//            vector<CardInfo*> listSequence = SamUtil::getSequence(listc, priviousplaycard);
//            if (listSequence.size() == priviousplaycard.size())
//                res = true;
//        }
//            break;
//        default:
//            break;
//    }
//    return res;
}

vector<CardInfo*> LogicTienLenMN::getRecommend(string listcardinfo, vector<int> chooseCards)
{
   // log("getRecommend");
    //dau tien check xem co quan bai nao do duoc hay khong, neu co thi moi check tiep khong thi return luon
    //	vector<CardInfo*> cards;
    // 	if (!checkRecommendCard(listcardinfo))
    // 	{
    // 		log("Cannot play because not cards avaiable");
    // 		return cards;
    // 	}
    string choosestr = convertVecToString(chooseCards);
    
   // log("getRecommend, listcard =  %s / listchoose = %s", listcardinfo.c_str(), choosestr.c_str());
    vector<int> listcard = splitStringtoInt(listcardinfo, ',');
    vector<CardInfo*> listc;
    vector<CardInfo*> listchooseCards;
    std::sort(listcard.begin(), listcard.end());///sap xep lai truoc khi kiem tra
    std::sort(chooseCards.begin(), chooseCards.end());
    for (int i = 0; i < listcard.size(); ++i)
    {
        listc.push_back(getCard(listcard.at(i)));
    }
    for (int i = 0; i < chooseCards.size(); ++i)
    {
        listchooseCards.push_back(getCard(chooseCards.at(i)));
    }
    //danh cho goi y luot danh dau tien
    if (priviousplaycard.size() <= 0)
        return getRecommendFirst(listc, listchooseCards);
    if (_gameid == kGameTienLenMienBac){
        return getRecommendMB(listc, listchooseCards);
    }
    else if (_gameid == kGameSam || _gameid == kGameSamSolo){
        return getRecommendSam(listc, listchooseCards);
    }
    else{
        return getRecommendMN(listc, listchooseCards);
    }
}

vector<CardInfo*> LogicTienLenMN::getRecommendMN(vector<CardInfo*> listc, vector<CardInfo*> chooseCards)
{
    vector<CardInfo*> cards;
    switch (previouscardtype)
    {
        case GAME_TABLE_CARD_TYPE_HAI_DOI_THONG://chi can kiem tra xem co quan nao to hon thi goi y
            cards = TienLenMNUtil::getRecommend2Thong(listc,priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_ONE://chi can kiem tra xem co quan nao to hon thi goi y
            cards = TienLenMNUtil::getRecommendOne(priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_TWO://check bo doi
            cards = TienLenMNUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_THREE://check bo ba
            cards = TienLenMNUtil::getRecommendThree(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_FOUR://check tu quy
            cards = TienLenMNUtil::getRecommendFour(listc, priviousplaycard, chooseCards);
            if (cards.size()<4){
                vector<CardInfo*> listthong = TienLenMNUtil::get4Thong(listc,chooseCards);
                if (listthong.size() == 8)//neu co thi so sanh tiep voi 2 quan duoc chon
                {
                    //neu 2 quan chon co trong 4 doi thong thi remove va lay list do
                    bool exist = TienLenMNUtil::checkValueExistInList(listthong, chooseCards);
                    if (exist)
                    {
                        cards = TienLenMNUtil::removeListFromList(listthong, chooseCards);
                        
                    }
                    return cards;
                }
            }
            break;
        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
        {
            cards = TienLenMNUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
            if (cards.size() == 0)
            {
                //check tiep 4thong
                cards = TienLenMNUtil::getRecommendDouble2(listc, priviousplaycard, chooseCards);
            }
            else{
                if (cards[0]->getValueCard()!= 15){
                    cards.clear();
                    cards = TienLenMNUtil::getRecommendDouble2(listc, priviousplaycard, chooseCards);
                }
                
            }
        }
            break;
        case GAME_TABLE_CARD_TYPE_2: //check 2 thi phai check xem co tu quy hoac 3 doi thong khong, hoac 2 to hon
            cards = TienLenMNUtil::getRecommend2(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_3_THONG:
            cards = TienLenMNUtil::getRecommend3Thong(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_4_THONG:
            cards = TienLenMNUtil::getRecommend4Thong(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_SEQUENCE:
            cards = TienLenMNUtil::getBestSequence(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_DAY_DONG_CHAT:
            cards = TienLenMNUtil::getBestSequenceDongChat(listc, priviousplaycard, chooseCards);
            break;
        default:
            return cards;
    }
    return cards;
}

vector<CardInfo*> LogicTienLenMN::getRecommendMB(vector<CardInfo*> listc, vector<CardInfo*> chooseCards)
{
    vector<CardInfo*> cards;
    return cards;
}

vector<CardInfo*> LogicTienLenMN::getRecommendSam(vector<CardInfo*> listc, vector<CardInfo*> chooseCards)
{
    vector<CardInfo*> cards;
    switch (previouscardtype)
    {
        case GAME_TABLE_CARD_TYPE_ONE://chi can kiem tra xem co quan nao to hon thi goi y
            cards = SamUtil::getRecommendOne(priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_TWO://check bo doi
            cards = SamUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_THREE://check bo ba
            cards = TienLenMNUtil::getRecommendThree(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_FOUR://check tu quy
            cards = TienLenMNUtil::getRecommendFour(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_2: //check 2 thi phai check xem co tu quy
            cards = SamUtil::getRecommend2(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_DOUBLE_2:
            cards = SamUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
            break;
        case GAME_TABLE_CARD_TYPE_SEQUENCE:
            cards = SamUtil::getBestSequence(listc, priviousplaycard, chooseCards);
            break;
        default:
            return cards;
    }
    return cards;
}

vector<CardInfo*> LogicTienLenMN::getRecommendFirst(vector<CardInfo*> listc, vector<CardInfo*> chooseCards)
{
    /************************************************************************/
    /* ưu tiên: sảnh-> 4 thông -> 3 thông -> tứ quý -> bộ ba -> đôi..                                                                     */
    /************************************************************************/
   // log("getRecommendFirst");
    vector<CardInfo*> cards;
    switch (_gameid)
    {
        case kGameSam:
        case kGameSamSolo:
        {
            if (chooseCards.size() >= 1)
            {
                cards = SamUtil::getBestSequence(listc, priviousplaycard, chooseCards);
                for (int i = 0; i < chooseCards.size(); i++)
                {
                    cards.push_back(chooseCards.at(i));
                }
                bool isSequence = SamUtil::isSequence(cards);
                if (!isSequence || cards.size() < 3)
                {
                    //check tiep la tu quy
                    cards.clear();
                    vector<CardInfo*> sublist = TienLenMNUtil::get4(listc);
                    bool isExist = TienLenMNUtil::checkListExistInList(sublist, chooseCards);
                    if (isExist)
                        cards = sublist;
                    else{
                        //lai check tiep den bo ba
                        cards = TienLenMNUtil::getRecommendThree(listc, priviousplaycard, chooseCards);
                        if (cards.size() == 1)
                        {
                            if (cards.at(0)->getValueCard() != chooseCards.at(0)->getValueCard() &&
                                cards.at(0)->getValueCard() != chooseCards.at(1)->getValueCard())
                            {
                                cards.clear();
                            }
                        }
                        else{
                            cards = SamUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
                            bool isExist = TienLenMNUtil::checkListExistInList(sublist, chooseCards);
                            if (isExist)
                            {
                                if (cards.size() != 2)
                                    cards.clear();
                            }
                            else{
                                cards.clear();
                            }
                            
                        }
                    }
                }
                // 						 if (cards.size() <= 0)
                // 						 {
                //
                // 						 }
            }
        }
            break;
        case kGameTienLenMienNam:
        case kGameTLMNDemLa:
        case kGameDemLaSolo:
        {
            if (chooseCards.size() >= 1)
            {
               // log("getRecommendFirst 1");
                
                vector<CardInfo*> sublist = TienLenMNUtil::get4WithChooseCard(listc,chooseCards);
               // log("getRecommendFirst 7");
                
                bool isExist = TienLenMNUtil::checkListExistInList(sublist, chooseCards);
               // log("getRecommendFirst 8");
                
                if (isExist)
                    cards = sublist;
                else{
                    
                    //lai check tiep den bo ba
                    cards = TienLenMNUtil::getRecommendThree(listc, priviousplaycard, chooseCards);
                    
                   // log("getRecommendFirst 12");
                    
                    if (cards.size() == 3)
                    {
                        if (cards.at(0)->getValueCard() != chooseCards.at(0)->getValueCard())
                        {
                            cards.clear();
                        }
                        else{
                            bool isExist = TienLenMNUtil::checkListExistInList(cards, chooseCards);
                            if (!isExist){
                                cards.clear();
                            }
                        }
                    }
                    else{
                        cards = TienLenMNUtil::getBestSequence(listc, priviousplaycard, chooseCards);
                       // log("getRecommendFirst 1.1");
                        
                        //                for (int i = 0; i < chooseCards.size(); i++)
                        //                {
                        //                    cards.push_back(chooseCards.at(i));
                        //                }
                       // log("getRecommendFirst 2");
                        
                        bool isSequence = TienLenMNUtil::isSequence(cards);
                       // log("getRecommendFirst 3");
                        
                        if (!isSequence || cards.size() < 3)
                        {
                            cards.clear();
                            //check 4 thong
                           // log("getRecommendFirst 4");
                            
                            sublist = TienLenMNUtil::getRecommend4Thong(listc, priviousplaycard, chooseCards);
                           // log("getRecommendFirst 5");
                            
                            if (sublist.size() != 8)
                            {
                                //check 3 thong
                                sublist = TienLenMNUtil::getRecommend3Thong(listc, priviousplaycard, chooseCards);
                               // log("getRecommendFirst 6");
                                
                                if (sublist.size() != 6)
                                {
                                    //check tiep la tu quy
                                    
                                    
                                    
                                    
                                   // log("getRecommendFirst 9");
                                    
                                    sublist = TienLenMNUtil::get2Thong(listc,chooseCards);
                                   // log("getRecommendFirst 10");
                                    
                                    bool isExist = TienLenMNUtil::checkListExistInList(sublist, chooseCards);
                                   // log("getRecommendFirst 11");
                                    
                                    if (isExist)
                                        cards = sublist;
                                    else{
                                        
                                        cards = TienLenMNUtil::getRecommendTwo(listc, priviousplaycard, chooseCards);
                                       // log("getRecommendFirst 13");
                                        
                                        bool isExist = TienLenMNUtil::checkListExistInList(cards, chooseCards);
                                       // log("getRecommendFirst 14");
                                        
                                        if (!isExist){
                                            cards.clear();
                                        }
                                        if (cards.size() != 2)
                                            cards.clear();
                                    }
                                    
                                    
                                }
                                else
                                {
                                    cards = sublist;
                                }
                            }
                            else
                            {
                                cards = sublist;
                            }
                        }
                        
                    }
                }
               
                // 							   if (cards.size() <= 0)
                // 							   {
                //
                // 							   }
                if (cards.size() == 0 && chooseCards.size()==1)
                    cards = chooseCards;
            }
        }
            break;
        default:
            break;
    }
    return cards;
}

std::string LogicTienLenMN::convertVecToString(vector<int> arr)
{
    string liststr;
    for (int i = 0; i < arr.size(); ++i)
        liststr.append(StringUtils::format("%d,", arr.at(i)));
    return liststr;
}










