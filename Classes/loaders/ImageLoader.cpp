//
//  ImageLoader.cpp
//  QSMY
//
//  Created by wanghejun on 14-2-12.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "ImageLoader.h"
#include "managers/GameManager.h"
#include "utils/md5.h"
#include "network/ImageDownloader.h"

USING_NS_CC;

#define DOWNLOAD_DIR CCFileUtils::sharedFileUtils()->getWritablePath()+"dimages/"


ImageLoader::ImageLoader()
:localFile("")
{
    
}

ImageLoader::~ImageLoader()
{
    
}

ImageLoader* ImageLoader::create(const char* path)
{
    ImageLoader* pRt = new ImageLoader();
    if(pRt && pRt->init(path))
    {
        pRt->autorelease();
        return pRt;
    }
    CC_SAFE_DELETE(pRt);
    return NULL;
}

bool ImageLoader::init(const char* path)
{
    if(!CCNode::init()) return false;
    
    std::string localFileName = md5(path);
    
    std::string suffix("");
    
    std::string _path(path);
    
    if(std::string::npos != _path.find('.'))
    {
        for (int32_t i(_path.length()-1); i>=0; --i) {
            suffix.insert(suffix.begin(), sizeof(char), _path[i]);
            if(_path[i]=='.') break;
        }
    }
    
    localFile = DOWNLOAD_DIR+localFileName+suffix;
    
    CCTexture2D *pTexture = NULL;
    if((pTexture = CCTextureCache::sharedTextureCache()->textureForKey(localFile.c_str())))
    {
        CCSprite* pSprite = CCSprite::createWithTexture(pTexture);
        addChild(pSprite);
        return true;
    }
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(localFile))
    {
        CCSprite* pSprite = CCSprite::create(localFile.c_str());
        addChild(pSprite);
        return true;
    }
    
    
    CCNode* m_pLoading = ResourceManager::sharedResourceManager()->getUIAnimate("loading");
    addChild(m_pLoading);
    CC_SAFE_RETAIN(this);
    ImageRequest* pDownLoader = ImageDownloader::getInstance()->download(path,localFile.c_str());
    if(pDownLoader)pDownLoader->addObserver(this,notification_callfunc_selector(ImageLoader::onDownLoadComplete),ImageRequest::DOWNLOAD_COMPLETE);
    return true;
   
}

void ImageLoader::onDownLoadComplete()
{
    removeAllChildren();
    CCLOG("image onload %s",localFile.c_str());
    CCSprite* pSprite = CCSprite::create(localFile.c_str());
    if(pSprite) addChild(pSprite);
    CC_SAFE_RELEASE(this);
}
