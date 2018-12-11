#include "QueueTaiXiu.h"


BetInfo::BetInfo() : mType(""), mTotalBet(0), mPlayerBet(0){

}

BetInfo::~BetInfo(){
    cout << "BetInfo destructor: " << this->mType << "\n";
}

string& BetInfo::GetType(){
    return this->mType;
}

long& BetInfo::GetTotalBet(){
    return this->mTotalBet;
}

long& BetInfo::GetPlayerBet(){
    return this->mPlayerBet;
}

void BetInfo::SetTotalBet(const long& val){
    this->mTotalBet = val;
}

void BetInfo::SetPlayerBet(const long& val){
    this->mPlayerBet += val;
}

void BetInfo::SetType(const string& type){
    this->mType = type;
}

QueueTaiXiu::QueueTaiXiu(){
    this->pArrayInfo.clear();
}

QueueTaiXiu::~QueueTaiXiu(){
    this->RemoveAll();
}

void QueueTaiXiu::Push(const string &type, const long &total, const long &playerbet)
{
    cout << "push: " << type << " : " << total << " : " << playerbet <<  "\n";
    BetInfo* info = this->GetBetInfo(type);
    if(info == NULL){
        info = new BetInfo();
        this->pArrayInfo.push_back(info);
    }

    info->SetType(type);
    info->SetTotalBet(total);
    info->SetPlayerBet(playerbet);
}

void QueueTaiXiu::RemoveAll(){
    vector<BetInfo*>::iterator it;
    for ( it = this->pArrayInfo.begin(); it != this->pArrayInfo.end(); ){
        delete *it;
        it = this->pArrayInfo.erase(it);
    }
    this->pArrayInfo.clear();
}

vector<BetInfo*>& QueueTaiXiu::GetArrayBetInfo(){
    return this->pArrayInfo;
}

BetInfo* QueueTaiXiu::GetBetInfo(const string& type){
    for(int i = 0; i < this->pArrayInfo.size(); ++i){
        BetInfo* info = this->pArrayInfo.at(i);
        if(info->GetType() == type){
            return info;
        }
    }
    return NULL;
}

