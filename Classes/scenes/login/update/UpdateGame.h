//
//  UpdateGame.h
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__UpdateGame__
#define __QSMY__UpdateGame__

#include "AppInclude.h"
#include "components/components.h"
#include "model/VersionModel.h"
#include "Downloader.h"

#define DOWN_LOADER_COUNT 4

class LoginDelegate;

class UpdateGame
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public DownloaderDelegate
{
    CC_SYNTHESIZE(LoginDelegate*, m_pDelegate, Delegate);
    
public:
    UpdateGame();
    virtual ~UpdateGame();
    
    CREATE_FUNC(UpdateGame);
    
    bool init();
    
    void update(float dt);
private:
    
    
    void checkUpdate();
    void updated();
    void doUpdate();
    void needForceUpdate();
    void forceUpdate(cocos2d::CCObject* sender);
    
    void startUpdate();
    
    void downloadFile(version::FileInfo* pFileInfo);
    
    //void onProgress();
    void onComplete(version::FileInfo*);
    void onError(version::FileInfo*,Downloader::ErrorCode errorCode,uint32_t httpCode);

    void showRetry();
    void doRetry(cocos2d::CCObject* sender);
    void setProgress(float percent);
    
    void onComplete(float dt);
    
    void createDownLoader();
    float getDownloadBytes();
    void checkDownloadFiles();
    
private:
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

private:
    DProgressBar            *m_pPercent;
    cocos2d::CCLabelBMFont  *m_pPercentLabel;
    cocos2d::CCLabelTTF     *m_pTip;
    
    /*DButton                 *m_pRetry;
    cocos2d::CCNode         *m_pRetryNode;
    
    DButton                 *m_pUpdate;
    cocos2d::CCNode         *m_pUpdateNode;*/
    
    cocos2d::CCNode         *m_pProgressNode;
    
private:
    VersionModel            *m_pModel;
    float                   m_fCurrentSize;
    float                   m_fCUrrentPercent;
private:
    Downloader              *m_pDownloader[DOWN_LOADER_COUNT];
    uint32_t                m_uTotalFileCount;
    uint32_t                m_uDownloadFileCount;

	void confirmToUpdateBy3G(MessageType type,DMessageBox* targe);
};

#endif /* defined(__QSMY__UpdateGame__) */
