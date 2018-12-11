#include "PlayerTaiXiuInfo.h"

PlayerTaiXiuInfo::PlayerTaiXiuInfo()
{
    this->SoTienThuc = 0;
    this->SoTienAo = 0;
    this->SoTienDatCuocClient = 0;
    this->SoTienDatCuocServer = 0;
    this->ArrayBet.clear();
}

void PlayerTaiXiuInfo::initMoney (const long& money) {
    this->SoTienThuc = money;
    this->SoTienAo = money;
}

void PlayerTaiXiuInfo::reset () {
    this->SoTienThuc = 0;
    this->SoTienAo = 0;
    this->SoTienDatCuocClient = 0;
    this->SoTienDatCuocServer = 0;
    this->ArrayBet.clear();
}

void PlayerTaiXiuInfo::TruTienThuc (const long& money) {
    this->SoTienAo -= money;
    this->SoTienDatCuocClient += money;
}

void PlayerTaiXiuInfo::SetSoTienThuc (const long& money) {
    this->SoTienThuc = money;
}

void PlayerTaiXiuInfo::PushBet (std::string& bet) {
    this->ArrayBet.push_back(bet);
}

bool PlayerTaiXiuInfo::CheckBet(const std::string& bet) {
    try {
        for (int i = 0; i < this->ArrayBet.size(); ++i) {
            if (this->ArrayBet.at(i) == bet) {
                //this->ArrayBet.splice(i, 1);
                return true;
            }
        }
        return false;
    } catch (...) {
        return false;
    }
}

vector<string>& PlayerTaiXiuInfo::GetAllBet(){
    return this->ArrayBet;
}

