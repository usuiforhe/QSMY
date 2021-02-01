//
//  UpdateGame.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UpdateGame.h"
#include "../Login.h"
#include "AppMacros.h"
#include "utils/SystemUtil.h"
#include "managers/SoundManager.h"
#include "managers/AnalyticsManager.h"
#include "managers/PlatformManager.h"
#include "utils/TimerUtil.h"
#include "utils/FileUtil.h"


USING_NS_CC;
USING_NS_CC_EXT;

UpdateGame::UpdateGame()
:m_pDelegate(NULL)
,m_pPercent(NULL)
,m_pPercentLabel(NULL)
,m_pModel(NULL)
,m_pTip(NULL)
,m_fCurrentSize(0.f)
,m_uTotalFileCount(0)
,m_uDownloadFileCount(0)
//,m_pRetryNode(NULL)
//,m_pRetry(NULL)
//,m_pUpdate(NULL)
//,m_pUpdateNode(NULL)
,m_pProgressNode(NULL)
,m_fCUrrentPercent(0.f)
{
    D_RESET_C_ARRAY(m_pDownloader,DOWN_LOADER_COUNT);
}

UpdateGame::~UpdateGame()
{
    UNBINDING_ALL(m_pModel, this);
    
    CC_SAFE_RELEASE(m_pTip);
    CC_SAFE_RELEASE(m_pPercent);
    CC_SAFE_RELEASE(m_pPercentLabel);
    D_SAFE_DELETE_C_ARRAY(m_pDownloader,DOWN_LOADER_COUNT);
    
    /*CC_SAFE_RELEASE(m_pRetryNode);
    CC_SAFE_RELEASE(m_pRetry);
    
    CC_SAFE_RELEASE(m_pUpdateNode);
    CC_SAFE_RELEASE(m_pUpdate);*/
    CC_SAFE_RELEASE(m_pProgressNode);
}

bool UpdateGame::init()
{
    if(!DLayer::init()) return false;
    m_pModel = VersionModel::sharedVersionModel();
    createDownLoader();
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_denglu/PL_update");
    
    /*m_pRetryNode->setVisible(false);
    m_pRetry->getButton()->setTarget(this, menu_selector(UpdateGame::doRetry));
    
    m_pUpdateNode->setVisible(false);
    m_pUpdate->getButton()->setTarget(this, menu_selector(UpdateGame::forceUpdate));*/
    
    m_pProgressNode->setVisible(false);
    
    checkUpdate();
    return true;
}

void UpdateGame::showRetry()
{
    //m_pRetryNode->setVisible(true);
	checkUpdate();
}

void UpdateGame::doRetry(cocos2d::CCObject* sender)
{
    checkUpdate();
}

bool UpdateGame::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercent",     DProgressBar *, this->m_pPercent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPercentLabel",CCLabelBMFont *, this->m_pPercentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTip",CCLabelTTF *, this->m_pTip);
   /* CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRetryNode",CCNode *, this->m_pRetryNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRetry",DButton *, this->m_pRetry);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpdateNode",CCNode *, this->m_pUpdateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpdate",DButton *, this->m_pUpdate);*/
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressNode",CCNode *, this->m_pProgressNode);
    
    return false;
}

SEL_MenuHandler UpdateGame::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}


void UpdateGame::createDownLoader()
{
    D_SAFE_DELETE_C_ARRAY(m_pDownloader,DOWN_LOADER_COUNT);
    for (uint8_t i = 0; i<DOWN_LOADER_COUNT; ++i)
    {
        m_pDownloader[i] = new Downloader();
        m_pDownloader[i]->setDelegate(this);
    }
}

void UpdateGame::checkUpdate()
{
    m_pTip->setString(D_LOCAL_STRING("check version").c_str());
    m_pPercent->setPercent(0.f);
    m_pPercentLabel->setString("0%");
    
    BINDING_ONLY(m_pModel, this, UpdateGame::needForceUpdate, D_EVENT_VERSION_FORCE_UPDATE);
    BINDING_ONLY(m_pModel, this, UpdateGame::updated, D_EVENT_VERSION_CHECK_FINISH);
    BINDING_ONLY(m_pModel, this, UpdateGame::doUpdate, D_EVENT_VERSION_START_UPDATE);
    
    VersionModel::sharedVersionModel()->SyncToRemoteVersion();
}

void UpdateGame::needForceUpdate()
{
    m_pProgressNode->setVisible(true);
	m_pModel->forceUpdateVersion();
    /* m_pRetryNode->setVisible(false);
    m_pUpdateNode->setVisible(true);*/
}

void UpdateGame::forceUpdate(cocos2d::CCObject* sender)
{
    m_pModel->forceUpdateVersion();
}

void UpdateGame::updated()
{
    //AnalyticsManager::sharedAnalyticsManager(); //初始化bi统计
	LoginPage pNextPage = kLoginPageSelectServer;
	if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)  pNextPage = KLoginTypeSelect;
    if(m_pDelegate) m_pDelegate->showInfo(pNextPage,kFromUpdate);
}

void UpdateGame::doUpdate()
{
	if (Network3G == SystemUtil::getNetworkStatus())
	{

		DMessageBox::showMessageBox(D_LOCAL_STRING("NetworkWarning").c_str(), this, messageBox_callfunc_selector(UpdateGame::confirmToUpdateBy3G));
	}else
    {
        startUpdate();
    }
	
}

void UpdateGame::startUpdate()
{
    m_pProgressNode->setVisible(true);
    //m_pRetryNode->setVisible(false);
	m_fCurrentSize = 0.f;
	m_uDownloadFileCount = 0;
	m_uTotalFileCount = m_pModel->getDownloadList()->count();
	m_pTip->setString(D_LOCAL_STRING("download file %.2fM",m_pModel->getTotalSize()).c_str());
	setProgress(0.f);
    scheduleUpdate();
    checkDownloadFiles();
}

void UpdateGame::confirmToUpdateBy3G(MessageType type,DMessageBox* targe)
{
	if(type == kMessageTypeCancel)
    {
        showRetry();
        return;
    }
	startUpdate();
}

void UpdateGame::downloadFile(version::FileInfo* pFileInfo)
{
    Downloader* pDownloader = m_pDownloader[0];
    for (uint8_t i = 1; i<DOWN_LOADER_COUNT; ++i)
    {
        if(pDownloader->getWaitingCount()> m_pDownloader[i]->getWaitingCount()) pDownloader = m_pDownloader[i];
    }
    pDownloader->addFile(pFileInfo);
}

void UpdateGame::update(float dt)
{
    float percent = ((m_fCurrentSize+getDownloadBytes())/1024.f/1024.f)/m_pModel->getTotalSize();
    if(percent<m_fCUrrentPercent) percent = m_fCUrrentPercent;
    if(percent>1.f) percent = 1.f;
    m_fCUrrentPercent = percent;
    setProgress(percent);
}

void UpdateGame::setProgress(float percent)
{
    //if(percent>1.f) percent = 1.f;
    //CCLog("-----------------------------%f",percent);
    m_pPercent->setPercent(percent);
    m_pPercentLabel->setString(CCString::createWithFormat("%d%%",(int)(percent*100))->getCString());
}

float UpdateGame::getDownloadBytes()
{
    float bytes = 0.0;
    for (uint8_t i = 0; i<DOWN_LOADER_COUNT; ++i)
    {
        bytes+=m_pDownloader[i]->getCurSize();
    }
    return bytes;
}

void UpdateGame::onComplete(version::FileInfo* pFileInfo)
{
    m_fCurrentSize+= pFileInfo->getSize();
    m_pModel->setFileDownloadStatus(pFileInfo,version::kDownloaded);
	//unzip the zip file
	if(pFileInfo->getFile().length() > 4 && pFileInfo->getFile().substr(pFileInfo->getFile().length() - 4).compare(".zip") == 0)
	{
		CCLOG("to uncompress  file suffix: %s", pFileInfo->getFile().substr(pFileInfo->getFile().length() - 4).c_str());
		FileUtil::uncompress(m_pModel->getStoragePath(), pFileInfo->getFile());
	}
    m_uDownloadFileCount++;
    CCLog("%d/%d",m_uDownloadFileCount,m_uTotalFileCount);
    checkDownloadFiles();
}

void UpdateGame::checkDownloadFiles()
{
    if(m_uDownloadFileCount==m_uTotalFileCount)
    {
        update(0.f);
        unscheduleUpdate();
        scheduleOnce(schedule_selector(UpdateGame::onComplete), 0);
        return;
    }
    for (uint32_t i(0); i<= DOWN_LOADER_COUNT*2; ++i) {
        version::FileInfo* pFileInfo  = static_cast<version::FileInfo*>(m_pModel->getDownloadList()->lastObject());
        if(pFileInfo==NULL) break;
		m_pModel->setFileDownloadStatus(pFileInfo, version::kDownloading);
		downloadFile(pFileInfo);
		m_pModel->getDownloadList()->removeObject(pFileInfo);
    }
}


void UpdateGame::onComplete(float dt)
{
    m_pModel->updateComplete();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCDirector::sharedDirector()->purgeCachedData();
    Lang::purgeLang();
    sp::ArmatureDataManager::purgeArmatureDataManager();
    TimerUtil::purgeTimerUtil();
    DataManager::purgeSharedDataManager();
	SoundManager::purgeSoundManager();
	AnalyticsManager::purgeAnalyticsManager();
    CCScriptEngineManager::purgeSharedManager();
	GameManager::sharedGameManager()->initScriptEngine();
    updated();
}

void UpdateGame::onError(version::FileInfo* pFileInfo,Downloader::ErrorCode errorCode,uint32_t httpCode)
{
    switch (errorCode)
    {
        case Downloader::kCreateFile:
            Tips_Error(D_LOCAL_STRING("UpdateFaildCreateFileError").c_str());
            break;
        case Downloader::kNetwork:
            if(httpCode==0)
            {
                Tips_Error(D_LOCAL_STRING("UpdateFaildNetworkError").c_str());
            }
            else
            {
                Tips_Error(D_LOCAL_STRING("UpdateFaildNetworkError,ErrorCode(%d)",httpCode).c_str());
            }
        default:
            break;
    }
    unscheduleUpdate();
    createDownLoader();
    showRetry();
}
