//
//  LayerLoadingStore.hpp
//  iCasino_v4-mobile
//
//  Created by Sherlock Tuan on 10/4/18.
//

#ifndef LayerLoadingStore_hpp
#define LayerLoadingStore_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerLoadingStore: public cocos2d::Layer,public SingLeton<LayerLoadingStore>{
public:
    LayerLoadingStore();
    ~LayerLoadingStore();
    virtual bool init();
    static LayerLoadingStore& getSingleton(void);
    static LayerLoadingStore* getSingletonPtr(void);
    static cocos2d::Scene* createScene();
    void layerPopupThongBaoCallBack(Ref *pSender);
    void showPopupFailed(int tag);
    void timeOutDownload(float dt);
    void delayUpDate(float dt);
    void gotoFakeLoading(float dt);
    void gotoGame(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
    void startDownloadResouces();
    CREATE_FUNC(LayerLoadingStore);
private:
    
    LoadingBar* loadingBar;
    
    bool isDownloading = false;
    Text* txtLoading;
    AssetsManagerEx* _am;
    int timeoutDownLoad = 0;
  
    void LoadStrings(const char* file);
    std::string _sysStrings[2048];
    vector<string> splitString(string &S, const char &str);
};

#endif /* LayerLoadingStore_hpp */
