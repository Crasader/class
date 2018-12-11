//
//  mI.cpp
//  iCasino_v4
//
//  Created by Tuan Anh on 1/7/16.
//
//
//#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WINRT && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX )
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "mIAP.h"
#include "Requests/ExtensionRequest.h"
#include "layers/LayerPopupThongBao.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "data_manager.h"
#include "layergames/PhomMessDef.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "layergames/NativeHelper.h"
#endif
void mIAP::loadIAP(){
   // log("load product");
  //  IAP::setDebug(true);
    IAP::setListener(this);
    IAP::init();
    //if (dataManager::getA)
    SceneManager::getSingleton().showLoading();
}

void mIAP::onIAP(sdkbox::Product product)
{
    CCLOG("product %s", product.name.c_str());
    IAP::purchase(product.name);

}


void mIAP::onInitialized(bool ok)
{
    CCLOG("%s : %d", __func__, ok);
    
}

void mIAP::onSuccess(const Product &p)
{
    CCLOG("Succes Purchase");
    CCLOG("Purchase Succes Purchase: %s", p.receiptCipheredPayload.c_str());
    CCLOG("Purchase Succes Purchase: %s", p.currencyCode.c_str());
     CCLOG("Purchase Succes Purchase: %s", p.receipt.c_str());
    SceneManager::getSingleton().hideLoading();
    
   // IOS::verifyInapp(p.receiptCipheredPayload);
    
    string token = mUtils::getMD5KeyInApp(*(GameServer::getSingleton().getSmartFox()->SessionToken())+SceneManager::getSingleton().getMyName());
    int  osid = 2; //ios
    std::string devID = "";
    /*bool isConnectting = true;*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osid = 1;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
    else
        devID = NativeHelper::getDeviceId();
    /*isConnectting = NativeHelper::hasConnection();*/
    //devID = NativeHelper::getDeviceID();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osid = 2;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
    else
        devID = IOS::getDeviceID();
#endif
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("iappt", currItem);
    params->PutUtfString("iapptk", token);
    params->PutInt("osid", osid);// Operatorsystems
    params->PutInt("tm", this->type);// Operatorsystems
    params->PutUtfString("devid", devID);// decvice id
    boost::shared_ptr<IRequest> request(new ExtensionRequest("riapp", params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}
void mIAP::purchaseItem(int idItem,int type){
    this->type =type;
    this->currItem = idItem+1;

//    string token = mUtils::getMD5KeyInApp(*(GameServer::getSingleton().getSmartFox()->SessionToken())+SceneManager::getSingleton().getMyName());
//    int  osid = 2; //ios
//    std::string devID = "";
//    /*bool isConnectting = true;*/
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    osid = 1;
//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
//        devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
//    else
//        devID = NativeHelper::getDeviceId();
//    /*isConnectting = NativeHelper::hasConnection();*/
//    //devID = NativeHelper::getDeviceID();
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    osid = 2;
//    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
//        devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
//    else
//        devID = IOS::getDeviceID();
//#endif
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    params->PutInt("iappt", this->currItem);
//    params->PutUtfString("iapptk", token);
//    params->PutInt("osid", osid);// Operatorsystems
//    params->PutUtfString("devid", devID);// decvice id
//    params->PutInt("tm", this->type);// Operatorsystems
//
//    boost::shared_ptr<IRequest> request(new ExtensionRequest("riapp", params));
//    GameServer::getSingleton().Send(request);
//
//    SceneManager::getSingleton().showLoading();
//
//    return;
    //
    
    
    
    SceneManager::getSingleton().showLoading();
    CCLOG("id: %d ",idItem);
    CCLOG("size: %d ",_products.size());
    if (_products.size()<=0 || idItem > _products.size())
        return;
    onIAP(_products.at(idItem));
    SceneManager::getSingleton().showLoading(60);
    //SceneManager::getSingleton().setTimeOutLoading(60);
}
void mIAP::removeListener(){
    IAP::removeListener();
}
void mIAP::onFailure(const Product &p, const std::string &msg)
{
    CCLOG("Purchase Failed: %s", msg.c_str());
    SceneManager::getSingleton().hideLoading();
}

void mIAP::onCanceled(const Product &p)
{
    CCLOG("Purchase Canceled: %s", p.id.c_str());
    SceneManager::getSingleton().hideLoading();
}

void mIAP::onRestored(const Product& p)
{
    //CCLOG("Purchase Restored: %s", p.receiptCipheredPayload.c_str());
    //CCLOG("Purchase Restored: %s", p.currencyCode.c_str());
    //SceneManager::getSingleton().hideLoading();
}

void mIAP::updateIAP(const std::vector<sdkbox::Product>& products)
{
    //
    _products = products;
    
    SceneManager::getSingleton().hideLoading();
    for (int i=0; i < _products.size(); i++)
    {
        CCLOG("IAP: ========= IAP Item =========");
        CCLOG("IAP: Name: %s", _products[i].name.c_str());
        CCLOG("IAP: ID: %s", _products[i].id.c_str());
        CCLOG("IAP: Title: %s", _products[i].title.c_str());
        CCLOG("IAP: Desc: %s", _products[i].description.c_str());
        CCLOG("IAP: Price: %s", _products[i].price.c_str());
        CCLOG("IAP: Price Value: %f", _products[i].priceValue);
    }
}

void mIAP::onProductRequestSuccess(const std::vector<Product> &products)
{
    updateIAP(products);
    SceneManager::getSingleton().hideLoading();
}

void mIAP::onProductRequestFailure(const std::string &msg)
{
    CCLOG("Fail to load products");
    SceneManager::getSingleton().hideLoading();
}

void mIAP::onRestoreComplete(bool ok, const std::string &msg)
{
    CCLOG("%s:%d:%s", __func__, ok, msg.data());
}
#endif
