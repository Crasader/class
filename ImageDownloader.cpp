//
//  ImageDownloader.cpp
//  iCasino_v2
//
//  Created by HoangDD on 7/10/14.
//
//

#include "ImageDownloader.h"
#include "mUtils.h"
#include <fstream>
#include <exception>
#include "base/CCConfiguration.h"
#include "SceneManager.h"
void ImageDownloader::loadDefaultImage(Node* pImgTarget)
{

}

void ImageDownloader::downLoadImage(Node* pImgTarget, string url, int _type, Size size, string fileNameEx){

	this->_size = size;
	type = _type;

	//Get File name
	string fileName = fileNameEx;
	if (0 == fileNameEx.compare("")){
		fileName = mUtils::getFileName(url);
	}

	//Kiem tra 
	if (0 == url.compare("") || 0 == fileName.compare(""))
	{
		return;
	}

	//
	//Check filename existed ?

	//for encrypt - add to head extend name to file name download
//    string _header_non_crypt = Configuration::getInstance()->getValue("non_en").asString();
//    if (!_header_non_crypt.empty() &&
//        !Configuration::getInstance()->getValue("encryptkey").asString().empty())
//    {
//        fileName.insert(0, _header_non_crypt);
//    }

	std::string writablePath = FileUtils::getInstance()->getWritablePath();
	writablePath.append(fileName);
    log ("writeable path : %s",writablePath.c_str());

	ifstream f(writablePath.c_str());
	if (f)
	{
		//existed
		Sprite* pSprite = NULL;
		try
		{
			pSprite = Sprite::create(writablePath.c_str());
		}
		catch (...)
		{
			pSprite = NULL;
		}

		if (pSprite != NULL)
		{
			try
			{
                log ("đã có mẹ nó rồi");
                if (pImgTarget == NULL) throw "_nodeImagePointer == NULL";
				//load avatar
				this->loadImage(pImgTarget, pSprite);

				return;
			}
			catch (const char* e)
			{
				log("Exception %s", e);
			}
			catch (...)
			{
				log("Exception Unknow");
			}
		}
	}
    
    log ("buoc2 : %s",writablePath.c_str());

	//Add 2 queue
	ImgInfo imgInfo;
	imgInfo.pNodeTarget = pImgTarget;
	imgInfo.url = url;
	imgInfo.path = writablePath;
	this->lstImgInfo.push(imgInfo);
	this->downloadNextImage();
}

void ImageDownloader::loadImage(cocos2d::Node* pImgTarget, cocos2d::Sprite* pSprite){
	if (pImgTarget == NULL || pSprite == NULL || pSprite->getTexture() == NULL)
		return;
	do
	{
		if (type == 1){
			if (0 == pImgTarget->getReferenceCount())
			{
				break;
			}
			pImgTarget->removeChildByTag(this->_pTagChild, true);
			pSprite->setAnchorPoint(Vec2(0.5, 0.5));
			pSprite->setPosition(Vec2(pImgTarget->getContentSize().width / 2, pImgTarget->getContentSize().height / 2));
			pSprite->setTag(this->_pTagChild);
            Size size = pSprite->getContentSize();
            float w = size.width > size.height ? size.height : size.width;
            float sc = this->_size.width / w;
            pSprite->setScale(sc);
			pImgTarget->addChild(pSprite, 2);
			log("here download img");

		}
		else{
			if (0 == pImgTarget->getReferenceCount())
			{
               // log("here download img 3");
				break;
			}
			pImgTarget->removeChildByTag(this->_pTagChild, true);
			pSprite->setAnchorPoint(Vec2(0.5, 0.5));
			Size size = pSprite->getContentSize();
			float w = size.width > size.height ? size.height : size.width;
			float sc = this->_size.width / w;
			pSprite->setScale(sc);
			pSprite->setTag(this->_pTagChild);
			pImgTarget->addChild(pSprite, 2);
			log("here download img 2");

		}
	} while (0);
}

ImageDownloader::ImageDownloader()
{
	this->_pTagChild = 203;
	this->_size = Size(143, 143);

	this->setIsStop(false);
	log("ImageDownloader Constructor");
	this->isDownloading = false;

	this->_downloader.reset(new network::Downloader());
	this->_downloader->onFileTaskSuccess = std::bind(&ImageDownloader::onFileTaskSuccess, this, std::placeholders::_1);
	this->_downloader->onTaskError = std::bind(&ImageDownloader::onTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}
void ImageDownloader::stopDownload(){
	this->isDownloading = false;
	std::queue<ImgInfo> emptyQueue;
	std::swap(lstImgInfo, emptyQueue);
	this->_downloader.reset();
}
ImageDownloader::~ImageDownloader()
{
	log("Destructor ImageDownloader");
}

void ImageDownloader::downloadNextImage()
{
    log ("download");
	if (this->getIsStop()){
        log ("stop");
		this->isDownloading = false;
		return;
	}

	if (!this->isDownloading){
    if (0 == this->lstImgInfo.size()){
            log ("rỗng");

			return;
    }

		try{
			ImgInfo imgInfo = this->lstImgInfo.front();
			//this->lstImgInfo.pop();
			//reset encrypt key
			//Configuration::getInstance()->setValue("encryptkey",Value(""));
            log ("download image : %s",imgInfo.url.c_str());
			if (this->_downloader == NULL)
				this->_downloader.reset(new network::Downloader());
			this->_downloader->onFileTaskSuccess = std::bind(&ImageDownloader::onFileTaskSuccess, this, std::placeholders::_1);
			this->_downloader->onTaskError = std::bind(&ImageDownloader::onTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			string _fileName = mUtils::getFileName(imgInfo.url);
            log ("download _fileName : %s",_fileName.c_str());

			this->_downloader->createDownloadFileTask(imgInfo.url, imgInfo.path, "img");
			this->isDownloading = true;
		}
		catch (std::out_of_range& e){
            log ("exception out range");

		}
		catch (std::exception& e){
            log ("exception no body");

		}
		catch (...){

		}
	}
}

void ImageDownloader::onFileTaskSuccess(const cocos2d::network::DownloadTask& task)
{
	Texture2D* texture = nullptr;
    log ("Success");

	do
	{
		if (std::string::npos == task.storagePath.find(".png"))
		{
			break;
		}
		if (this->getIsStop()){
			this->isDownloading = false;
			return;
		}

		if (0 == this->lstImgInfo.size()){
			this->isDownloading = false;
			return;
		}

		ImgInfo imgInfo = this->lstImgInfo.front();
		this->lstImgInfo.pop();

		if (imgInfo.pNodeTarget == NULL){

		}

		else
		{
			Sprite* pSprite = Sprite::create(task.storagePath);
			if (pSprite == NULL)
			{

			}
			else
			{
				this->loadImage(imgInfo.pNodeTarget, pSprite);
			}
		}

		this->isDownloading = false;
		this->downloadNextImage();

	} while (0);
	CC_SAFE_RELEASE(texture);
}

void ImageDownloader::onTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    log ("error download: %s",errorStr.c_str());
	if (this->getIsStop()){
		this->isDownloading = false;
		return;
	}

	if (0 == this->lstImgInfo.size()){
		this->isDownloading = false;
		return;
	}

	ImgInfo imgInfo = this->lstImgInfo.front();
	this->lstImgInfo.pop();

	if (imgInfo.pNodeTarget == NULL)
	{

	}
	else
	{

	}

	this->isDownloading = false;
	this->downloadNextImage();
}





//
ImageFromUrl::ImageFromUrl(){
    this->request = new (std::nothrow) HttpRequest();
   // this->request->autorelease();
}
ImageFromUrl::~ImageFromUrl(){
    //delete this->request;
}
void ImageFromUrl::loadImage(Node* _node,string url)
{
    
    
    
    if (!this->request){
        this->request = new (std::nothrow) HttpRequest();
        this->request->autorelease();

    }
    
    
    
   // log("onHttpRequestCompleted In the request");
    this->node = _node;
    this->url = url;
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(ImageFromUrl::onRequestImgCompleted, this));
    request->setTag("Post test2");
    HttpClient::getInstance()->send(request);
    request->release();
    SceneManager::getSingleton().showLoading(5);
}


void ImageFromUrl::onRequestImgCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!this->request){
        return;
    }
    SceneManager::getSingleton().hideLoading();

   // log("In HelloWorld");
   // log("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
    if (!response)
    {
       // log("onHttpRequestCompleted - No Response");
        return;
    }
    
   // log("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());
    
    if (!response->isSucceed())
    {
       // log("onHttpRequestCompleted - Response failed");
       // log("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
        return;
    }
   // log("onHttpRequestCompleted - Response code: %s", response->getResponseDataString());

    if (!this->node) return;
    std::vector<char> *buffer = response->getResponseData();
    const char* file_char = buffer->data();
    Image * image = new  Image ();
    image-> initWithImageData ( reinterpret_cast<const unsigned char*>(&(buffer->front())), buffer->size());
    std::string writablePath = FileUtils::sharedFileUtils()->getWritablePath();
    string fileName = mUtils::getFileName(this->url);
    writablePath.append(fileName);
    image->saveToFile(writablePath.c_str());
    Texture2D * texture = new  Texture2D ();
    texture-> initWithImage (image);
    Sprite * sprite = Sprite :: createWithTexture (texture);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->setContentSize(this->node->getContentSize());
    sprite->setPosition(Vec2(this->node->getContentSize().width/2-20,this->node->getContentSize().height/2-20));
    this->node->addChild(sprite,90);
    this->node = NULL;
   // log("onHttpRequestCompleted height %f", sprite->getContentSize().height);
    
}
void ImageFromUrl::stopDownload(){
    this->node = NULL;
    this->request = NULL;
   // delete this->request;
    //this->request->setResponseCallback(NULL);
}

