//
//  mI.hpp
//  iCasino_v4
//
//  Created by Tuan Anh on 1/7/16.
//
//

#ifndef mI_hpp
#define mI_hpp
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "PluginIAP/PluginIAP.h"
#include "cocos2d.h"
using namespace sdkbox;
class mIAP :  public sdkbox::IAPListener
{
public:
     void loadIAP();
     void onIAP(sdkbox::Product product);
     void purchaseItem(int idItem, int type);
    void removeListener();
    int currItem;
    int type;
private:
    
    void updateIAP(const std::vector<sdkbox::Product>& products);
    
    virtual void onInitialized(bool ok) override;
    
    virtual void onSuccess(sdkbox::Product const& p) override;
    
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
    
    virtual void onCanceled(sdkbox::Product const& p) override;
    
    virtual void onRestored(sdkbox::Product const& p) override;
    
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
    
    virtual void onProductRequestFailure(const std::string &msg) override;
    void onRestoreComplete(bool ok, const std::string &msg);
    std::vector<sdkbox::Product> _products;
};

#endif /* mI_hpp */
#endif
