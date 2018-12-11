#ifndef LayerMenuNhiemVuInGame_hpp
#define LayerMenuNhiemVuInGame_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
USING_NS_CC;
using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;
class LayerMenuNhiemVuInGame: public Layer,
public PlayerCallBack
{
public:
    LayerMenuNhiemVuInGame();
    ~LayerMenuNhiemVuInGame();
    Label* labelNumber;
    virtual bool init();
    void onButtonClicked(Ref* pSender);
    bool isTaskDone(string status);
    void notiTaskDone();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
    CREATE_FUNC(LayerMenuNhiemVuInGame);
private:
    Sprite* bg;
    vector<string> lstTaskDone;
    bool isFirst;
    int showTab;
    int currGameForRqot;
    int currGameForRqdt;
    
    struct task{
        int idt;
        int lvl;
    };
    vector<task> lstdone;
};

#endif /* LayerMenuNhiemVuInGame_hpp */
