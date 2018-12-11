#include "ChanOrderAlg.h"

ChanOrderAlg::ChanOrderAlg()
{
    this->lstChanCa.clear();
}

ChanOrderAlg::~ChanOrderAlg (){
//    cout << "Destructor ChanOrderAlg \n";
    vector<ChanCa*>::iterator it;
    for ( it = this->lstChanCa.begin(); it != this->lstChanCa.end(); ){
        delete *it;
        it = this->lstChanCa.erase(it);
    }
    this->lstChanCa.clear();
}

void ChanOrderAlg::Push(const int &nu, const int &su)
{
    ChanCa* chanca = this->GetChanCa(nu);
    if(chanca == NULL){
        chanca = new ChanCa(nu);
        this->lstChanCa.push_back(chanca);
    }
    chanca->PushSuite(su);
}

ChanCa* ChanOrderAlg::GetChanCa(const int &nu)
{
    for(int i = 0; i < this->lstChanCa.size(); ++i){
        if(this->lstChanCa.at(i)->GetCardNumer() == nu){
            return this->lstChanCa.at(i);
        }
    }
    return NULL;
}

void ChanOrderAlg::Calculator(){
    int chan = 0, ca = 0, badau = 0, que = 0;
    for(int i = 0; i < this->lstChanCa.size(); ++i){
        ChanCa* c = this->lstChanCa.at(i);
        c->Prints();
        c->Calculator();
        chan += c->GetChan();
        ca += c->GetCa();
        que += c->GetQue();
        badau += c->GetBaDau();
    }
    cout << " ================================================================================= \n";
    cout << chan << " Chan, " << ca << " Ca, " << badau << " Ba Dau, " << que << " Que" << endl;
}

bool ChanOrderAlg::CheckU(){
    int chan = 0, ca = 0, badau = 0, que = 0;

    for(int i = 0; i < this->lstChanCa.size(); ++i){
        ChanCa* c = this->lstChanCa.at(i);
        c->Calculator();
        chan += c->GetChan();
        ca += c->GetCa();
        que += c->GetQue();
        badau += c->GetBaDau();
    }

    cout << " ================================ Check U ======================================= \n";
    cout << chan << " Chan, " << ca << " Ca, " << badau << " Ba Dau, " << que << " Que" << endl;
    if(chan >= 5) {
        if((que == 1 && badau == 0) || (que == 0 && badau == 1)){
            return true;
        }
        else{
            return false;
        }
    } else {
        return false;
    }

    return true;
}

ChanCa::ChanCa(const int &cardnu)
{
    this->cardnu = cardnu;
    this->cardsu.clear();
    //
    this->SoChan = 0;
    this->SoCa = 0;
    this->SoBaDau = 0;
    this->SoQue = 0;
}

ChanCa::~ChanCa()
{
//    cout << "Destructor ChanCa \n";
    this->cardsu.clear();
}

void ChanCa::PushSuite(const int &cardsu)
{
    this->cardsu.push_back(cardsu);
}

int &ChanCa::GetCardNumer()
{
    return this->cardnu;
}

void ChanCa::Calculator()
{
    std::sort(this->cardsu.begin(), this->cardsu.end());

    if(this->cardsu.size() == 1)
    {
        this->SoQue = 1;
    }
    else if(this->cardsu.size() == 2)
    {
        if(this->cardsu[0] == this->cardsu[1])
        {
            this->SoChan = 1;
        }
        else
        {
            this->SoCa = 1;
        }
    }
    else
    {
        for(int i = 0; i < this->cardsu.size() - 1;) {
            bool bRet = false;
            for(int j = i + 1; j < this->cardsu.size(); ++j) {
                if(this->cardsu[i] == this->cardsu[j]){
                    this->SoChan++;
                    this->cardsu.erase(this->cardsu.begin() + j);
                    this->cardsu.erase(this->cardsu.begin() + i);
                    bRet = true;
                    break;
                }
            }
            if(!bRet) ++i;
            if(0 == this->cardsu.size())
                break;
        }

        //Đếm hết chắn thì sẽ còn lại 1 trong 3 trường hợp sau.
        if(this->cardsu.size() == 3)
        {
            this->SoBaDau = 1;
        }

        if(this->cardsu.size() == 2)
        {
            this->SoCa = 1;
        }

        if(this->cardsu.size() == 1)
        {
            this->SoQue = 1;
        }
    }
    cout << SoChan << " : " << SoCa << " : " << SoBaDau << " : " << SoQue << endl;
}

int ChanCa::GetCa(){
    return this->SoCa;
}

int ChanCa::GetChan(){
    return this->SoChan;
}

int ChanCa::GetBaDau(){
    return this->SoBaDau;
}

int ChanCa::GetQue(){
    return this->SoQue;
}

void ChanCa::Prints()
{
    cout << "cardnu: " << this->cardnu << " suites: ";
    for (int i = 0; i < this->cardsu.size(); ++i) {
        cout << " " << this->cardsu[i];
    }
    cout << "\n";
}
