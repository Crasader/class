//
//  MauBinhLayerXepBai.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/11/17.
//
//

#ifndef MauBinhLayerXepBai_hpp
#define MauBinhLayerXepBai_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../SceneManager.h"
#include "ThreadQueueMsg.h"
#include "ChanUtils.h"
#include "../mUtils.h"
#include <algorithm>
#include <cctype>
#include "MauBinhCard.h"
#include "ClientMsgDefs.h"
#include "../GameServer.h"
#include "PhomMessDef.h"
#include "BangCuocMauBinh.h"
//#include "../GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;

#define MB_MAU_THAU 0
#define MB_DOI 20
#define MB_THU 21
#define MB_SAM_CO 22
#define MB_SANH 23
#define MB_THUNG 24
#define MB_CU_LU 25
#define MB_TU_QUY 26
#define MB_THUNG_PHA_SANH 27

class poker
{
public:
    poker() { face = "A23456789TJQK"; suit = "SHCD"; }
    int analyze( string h )
    {
        memset( faceCnt, 0, 13 ); memset( suitCnt, 0, 4 ); vector<string> hand;
        transform( h.begin(), h.end(), h.begin(), (int (*)(int))std::toupper ); istringstream i( h );
        copy( istream_iterator<string>( i ), istream_iterator<string>(), back_inserter<vector<string> >( hand ) );
        if( hand.size() != 5 ) return 100; vector<string>::iterator it = hand.begin();
        sort( it, hand.end() ); if( hand.end() != adjacent_find( it, hand.end() ) ) return 100 ;
        while( it != hand.end() )
        {
            if( ( *it ).length() != 2 ) return 100;
            int n = face.find( ( *it ).at( 0 ) ), l = suit.find( ( *it ).at( 1 ) );
            if( n < 0 || l < 0 ) return 100;
            faceCnt[n]++; suitCnt[l]++; it++;
        }
        cout << h << ": "; return analyzeHand();
    }
private:
    int analyzeHand()
    {
        bool p1 = false, p2 = false, t = false, f = false, fl = false, st = false;
        for( int x = 0; x < 13; x++ )
            switch( faceCnt[x] )
        {
            case 2: if( p1 ) p2 = true; else p1 = true; break;
            case 3: t = true; break;
            case 4: f = true;
        }
        for( int x = 0; x < 4; x++ )if( suitCnt[x] == 5 ){ fl = true; break; }
        
        if( !p1 && !p2 && !t && !f )
        {
            int s = 0;
            for( int x = 0; x < 13; x++ )
            {
                if( faceCnt[x] ) s++; else s = 0;
                if( s == 5 ) break;
            }
            st = ( s == 5 ) || ( s == 4 && faceCnt[0] && !faceCnt[1] );
        }
        
        if( st && fl ) return MB_THUNG_PHA_SANH;
        else if( f ) return MB_TU_QUY;
        else if( p1 && t ) return MB_CU_LU;
        else if( fl ) return MB_THUNG;
        else if( st ) return MB_SANH;
        else if( t ) return MB_SAM_CO;
        else if( p1 && p2 ) return MB_THU;
        else if( p1 ) return MB_DOI;
        return MB_MAU_THAU;
    }
    string face, suit;
    unsigned char faceCnt[13], suitCnt[4];
};
class MauBinhLayerXepBai : public cocos2d::Layer,public PlayerCallBack
{
public:
    MauBinhLayerXepBai();
    ~MauBinhLayerXepBai();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void onEnter();
    void onExit();
    
    void loadCard(string listcard);
    void setListCard(string listcard);
    string getSrcCard(string cardtype);
    string convertCardToCheck(string _value);
    void displayCardValue();
    string linkCard(int type);
    void swapCard(MauBinhCard* _from, MauBinhCard* _to);
    void displayCardWasLinked(int _type, int chi);
    bool kiemtraBinhLungChi1Chi2(int _type);
    void setListCardID(string listid);
    string getListID();
    void onBtnXepXong(Ref* pSender);
    
    int checkChi3La();
    vector<int> getNumChi(int chi);
    bool kiemtraBinhLungChi2Chi3(int _type);
    void setTimeXepBai(int timexep);
    void updateTimeXepBai(float dt);
    void setPointDone(vector<Point> lstPoint);
    void setSkewXDone(vector<float> lstSkew);
    void setSkewYDone(vector<float> lstSkew);
    void setOrgPoint(Vec2 point);
    void reloadCard(float dt);

    vector<Point> getPositionCard();
    CREATE_FUNC(MauBinhLayerXepBai);

private:
    EventListenerTouchOneByOne *listener;
    vector<ImageView*> lstChi1;
    vector<ImageView*> lstChi2;
    vector<ImageView*> lstChi3;
    vector<MauBinhCard*> tongChi;
    
    vector<MauBinhCard*> Chi1;
    vector<MauBinhCard*> Chi2;
    vector<MauBinhCard*> Chi3;
    
    vector<Point> lstPoint;
    vector<Point> lstPointDone;
    vector<float> lstSkewX;
    vector<float> lstSkewY;
    Vec2 orgPoint;
    
    Layout* pnlBg;
    
    MauBinhCard* fromCard;
    MauBinhCard* toCard;
    
    ImageView* bgChi1;
    ImageView* bgChi2;
    ImageView* bgChi3;
    
    Text* txtChi1;
    Text* txtChi2;
    Text* txtChi3;
    Text* txtTimer;
    poker p;
    ImageView* imgBinhLung;
    string lstcardid;
    int timeXepBai = 0;
    BangCuocMauBinh* bangcuoc;
    int cuoc1= 0;
    int cuoc2 = 0;
    int cuoc3 = 0;

   // vector<Rect>
  
};

#endif /* MauBinhLayerXepBai_hpp */
