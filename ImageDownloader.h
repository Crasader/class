//
//  ImageDownloader.h
//  iCasino_v2
//
//  Created by HoangDD on 7/10/14.
//
//

#ifndef __iCasino_v2__ImageDownloader__
#define __iCasino_v2__ImageDownloader__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
using namespace cocos2d::network;
/*
 *Lớp này có tác dụng download 1 image from http url về máy, và hiển thị lên 1 Node
 */

class ImageDownloader : public Ref{
private:
	struct ImgInfo
	{
		Node* pNodeTarget;
		string url;
		string path;
	};
	std::shared_ptr<network::Downloader> _downloader;
	queue<ImgInfo> lstImgInfo;

	bool isDownloading;

	void downloadNextImage();
public:
	ImageDownloader();
	~ImageDownloader();
	void downLoadImage(Node* pImgTarget, string url, int _type = 0, cocos2d::Size size = cocos2d::Size(143, 143), std::string fileNameEx = "");
	//void onImageDownLoaded(cocos2d::network::Downloader HttpClient *sender, cocos2d::network::HttpResponse *response);
	void loadDefaultImage(Node* pImgTarget);
	void onFileTaskSuccess(const cocos2d::network::DownloadTask& task);
	void onTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
	void stopDownload();
public:
	CC_SYNTHESIZE(bool, isStop, IsStop)

private:
	cocos2d::Size _size;
	int _pTagChild;
	void loadImage(cocos2d::Node* pImgTarget, cocos2d::Sprite* pSprite);
	int type;

};

class ImageFromUrl : public Ref{
public:
    ~ImageFromUrl();
    ImageFromUrl();
    void onRequestImgCompleted(HttpClient *sender, HttpResponse *response);
    void loadImage(Node* _node,string url);
    Node* node;
    string url = "";
    void stopDownload();
    HttpRequest* request;
};

#endif /* defined(__iCasino_v2__ImageDownloader__) */
