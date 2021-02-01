//
//  Activeness.cpp
//  QSMY
//
//  Created by OnePiece on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Activeness.h"
#include "ActivenessTableView.h"
#include "../../../managers/GameManager.h"
#include "../Game.h"
#include "ActivenessBox.h"
#include "../common/BoxPreviewView.h"

USING_NS_CC;
using namespace activeness;

Activeness::Activeness()
:m_pContainer(NULL)
,m_pActivenessTableView(NULL)
,m_pBoxes(NULL)
,m_pPoint(NULL)
,m_pAwards(NULL)
,m_nIndex(-1)
,m_pTextLeft(NULL)
,m_pSlideView(NULL)
,m_pBoxContainer(NULL)
,m_pAwardView(NULL)
,m_pDBtnClose(NULL)
,m_pActivenessModel(NULL)
{
	D_RESET_C_ARRAY(m_pButton, ACTIVENESS_PROGRESS_NUM);
	D_RESET_C_ARRAY(m_pLevelPoint, ACTIVENESS_PROGRESS_NUM);
    D_RESET_C_ARRAY(m_pCanGet, ACTIVENESS_PROGRESS_NUM);
    D_RESET_C_ARRAY(m_pProgressDotContainer, ACTIVENESS_PROGRESS_NUM);
    D_RESET_C_ARRAY(m_pBoxAvailable, ACTIVENESS_PROGRESS_NUM);
    D_RESET_C_ARRAY(m_pProgressState, ACTIVENESS_PROGRESS_STATE);
	D_RESET_C_ARRAY(m_pCanGetAni, ACTIVENESS_PROGRESS_NUM);
	D_RESET_C_ARRAY(m_pProgressAni, ACTIVENESS_PROGRESS_NUM);
	D_RESET_C_ARRAY(m_pBoxAvailableAni, ACTIVENESS_PROGRESS_NUM);
    D_RESET_C_ARRAY(m_pRewardGetted, ACTIVENESS_PROGRESS_NUM);
}

Activeness::~Activeness()
{
    removeAllChildren();
    
    CC_SAFE_RELEASE(m_pActivenessTableView);
    CC_SAFE_RELEASE(m_pBoxes);
    CC_SAFE_RELEASE(m_pAwards);
    
    CC_SAFE_RELEASE(m_pPoint);
	CC_SAFE_RELEASE(m_pContainer);
	
	CC_SAFE_RELEASE(m_pTextLeft);
	D_SAFE_RELEASE_C_ARRAY(m_pButton, ACTIVENESS_PROGRESS_NUM);
	D_SAFE_RELEASE_C_ARRAY(m_pLevelPoint, ACTIVENESS_PROGRESS_NUM);
	CC_SAFE_RELEASE(m_pBoxContainer);
    CC_SAFE_RELEASE(m_pDBtnClose);
    D_SAFE_RELEASE_C_ARRAY(m_pCanGet, ACTIVENESS_PROGRESS_NUM);
    D_SAFE_RELEASE_C_ARRAY(m_pProgressDotContainer, ACTIVENESS_PROGRESS_NUM);
    D_SAFE_RELEASE_C_ARRAY(m_pBoxAvailable, ACTIVENESS_PROGRESS_NUM);
    D_SAFE_RELEASE_C_ARRAY(m_pProgressState, ACTIVENESS_PROGRESS_STATE);
	D_SAFE_RELEASE_C_ARRAY(m_pCanGetAni, ACTIVENESS_PROGRESS_NUM);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressAni, ACTIVENESS_PROGRESS_NUM);
	D_SAFE_RELEASE_C_ARRAY(m_pBoxAvailableAni, ACTIVENESS_PROGRESS_NUM);
    D_SAFE_RELEASE_C_ARRAY(m_pRewardGetted, ACTIVENESS_PROGRESS_NUM);
}

bool Activeness::init()
{
    if(!DLayer::init()) return false;
    
    m_pActivenessModel = DM_GET_ACTIVENESS_MODEL;
    m_pAwards = m_pActivenessModel->getAwardInfoList();
    CC_SAFE_RETAIN(m_pAwards);
    
    m_pBoxes = CCArray::create();
    CC_SAFE_RETAIN(m_pBoxes);
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    //pNodeLoaderLibrary->registerCCNodeLoader("ActivenessBox", ActivenessBoxLoader::loader());
    
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_huoyuedu/PL_huoyuedu", pNodeLoaderLibrary);

    std::string prgressIcons[] = {"PL_ui/all/all_dian_an.png", "PL_ui/all/all_dian_liangi.png"};
    //CCSprite* pState[ACTIVENESS_PROGRESS_STATE];
    for(int i=0; i<ACTIVENESS_PROGRESS_STATE; i++)
    {
        m_pProgressState[i] = CCSprite::create(prgressIcons[i].c_str());
		CC_SAFE_RETAIN(m_pProgressState[i]);
    }
    
    /*for(int i=0; i<ACTIVENESS_PROGRESS_NUM; i++)
    {
		m_pProgressDotContainer[i]->addChild(CCSprite::createWithTexture(pState[0]->getTexture()), 0, 0);
		m_pProgressDotContainer[i]->addChild(CCSprite::createWithTexture(pState[1]->getTexture()), 0, 1);
    }
    */
    unsigned int height = m_pContainer->getContentSize().height;
    m_pActivenessTableView = ActivenessTableView::create(CCSizeMake(m_pContainer->getContentSize().width, height-(PL_GET_HEIGHT_DIFF)));
    m_pActivenessTableView->retain();  
	m_pContainer->setPositionY(m_pContainer->getPositionY()+(PL_GET_HEIGHT_DIFF));
    m_pContainer->addChild(m_pActivenessTableView);

    m_pDBtnClose->getButton()->setTarget(this, menu_selector(Activeness::closeCallBack));
    return true;
}

void Activeness::onEnter()
{
	DLayer::onEnter();
	m_nIndex = -1;
	BINDING_EXEC(m_pActivenessModel, this, Activeness::updateView, D_EVENT_ACTIVENESS_UPDATE);
	BINDING_ONLY(m_pActivenessModel, this, Activeness::updateView, D_EVENT_ACTIVENESS_AWARD_DRAWN);
	
}

void Activeness::onExit()
{
    UNBINDING_ALL(m_pActivenessModel, this);
    DLayer::onExit();
}

void Activeness::updateView()
{
    uint32_t point = m_pActivenessModel->getPoint();
    m_pPoint->setString(CCString::createWithFormat("%d", point)->getCString());
	
    AwardInfo *pAward = NULL;
	
    for(uint8_t i=0; i<ACTIVENESS_PROGRESS_NUM; i++)
    {
        pAward =(AwardInfo*)m_pAwards->objectAtIndex(i);
        //CCLOG(CCString::createWithFormat("%u", pAward->getRequirePoint())->getCString());
        m_pLevelPoint[i]->setString(CCString::createWithFormat("%u", pAward->getRequirePoint())->getCString());
        
        /*m_pProgressDotContainer[i]->getChildByTag(1)->setVisible(false);
        m_pProgressDotContainer[i]->getChildByTag(0)->setVisible(false);*/
		m_pProgressDotContainer[i]->removeAllChildren();
		m_pCanGet[i]->removeAllChildren();
		m_pBoxAvailable[i]->removeAllChildren();
        
        m_pRewardGetted[i]->setVisible(false);
        if(m_pActivenessModel->isAwardAvailable(pAward)) //check can get condition
        {
			m_pProgressDotContainer[i]->addChild(CCSprite::createWithTexture(m_pProgressState[1]->getTexture()), 0, 1);
            //m_pCanGet[i]->setVisible(!m_pActivenessModel->isAwardDrawn(pAward));//check is getted
            if(!m_pActivenessModel->isAwardDrawn(pAward)) //not getted play ani
            {
				if(m_pCanGetAni[i] == NULL)//可以领取动画
				{
					m_pCanGetAni[i] = ResourceManager::sharedResourceManager()->getUIAnimate("active_canget");
					CC_SAFE_RETAIN(m_pCanGetAni[i]);
				}
				m_pCanGet[i]->addChild(m_pCanGetAni[i]);

				if(m_pProgressAni[i] == NULL)//可以领取进度动画
				{
					m_pProgressAni[i] = ResourceManager::sharedResourceManager()->getUIAnimate("active_progress");
					CC_SAFE_RETAIN(m_pProgressAni[i]);
				}
				m_pProgressDotContainer[i]->addChild(m_pProgressAni[i]);

				if(m_pBoxAvailableAni[i] == NULL) //箱子打开动画
				{
					m_pBoxAvailableAni[i] = ResourceManager::sharedResourceManager()->getUIAnimate("active_available");
					CC_SAFE_RETAIN(m_pBoxAvailableAni[i]);
				}
				m_pBoxAvailable[i]->addChild(m_pBoxAvailableAni[i]);
            } else
            {
                m_pRewardGetted[i]->setVisible(true);
            }
			
        } else
        {
			m_pProgressDotContainer[i]->addChild(CCSprite::createWithTexture(m_pProgressState[0]->getTexture()), 0, 0);
        }
    }
	
	
	updatePage();
}


bool Activeness::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", cocos2d::CCLabelBMFont *, this->m_pPoint);
    
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTextLeft", CCLabelBMFont *, this->m_pTextLeft);
	//D_CCB_ASSIGN_ARRAY("m_pButton", CCMenuItem *, m_pButton, ACTIVENESS_PROGRESS_NUM);
	D_CCB_ASSIGN_ARRAY("m_pLevelPoint", CCLabelBMFont *, m_pLevelPoint, ACTIVENESS_PROGRESS_NUM);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBoxContainer", CCNode *, this->m_pBoxContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnClose", DButton *, this->m_pDBtnClose);
    D_CCB_ASSIGN_ARRAY("m_pCanGet", CCNode*, m_pCanGet, ACTIVENESS_PROGRESS_NUM);
    D_CCB_ASSIGN_ARRAY("m_pProgressDotContainer", CCNode*, m_pProgressDotContainer, ACTIVENESS_PROGRESS_NUM);
    D_CCB_ASSIGN_ARRAY("m_pBoxAvailable", CCNode*, m_pBoxAvailable, ACTIVENESS_PROGRESS_NUM);
    D_CCB_ASSIGN_ARRAY("m_pRewardGetted", CCNode*, m_pRewardGetted, ACTIVENESS_PROGRESS_NUM);
    return false;
}

SEL_MenuHandler Activeness::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "getRewardCallback", Activeness::getRewardCallback);
    return NULL;
}

void Activeness::showAwardView(activeness::AwardInfo *pAwardInfo)
{    
	m_pAwardView = ActivenessAwardView::create(pAwardInfo);
	m_pAwardView->setDelegate(this);
    addChild(m_pAwardView);    
}

void Activeness::closeCallBack(cocos2d::CCObject *sender)
{
    Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Activeness::getRewardCallback(cocos2d::CCObject *sender)
{
    CCMenuItem *pButton = (CCMenuItem *)sender;
	//可以领取则直接领取，否则查看
	AwardInfo* pAwardInfo = (AwardInfo*)m_pAwards->objectAtIndex(pButton->getTag());
	if(m_pActivenessModel->isAwardAvailable(pAwardInfo) && !m_pActivenessModel->isAwardDrawn(pAwardInfo))
	{
		HTTP_CLIENT->getActivenessAward(pAwardInfo->getRequirePoint());
	} else 
	{
		BoxPreviewView* pAwardView = BoxPreviewView::create(pAwardInfo->getAwardItems());
		pAwardView->showLabel(kCheck);
		addChild(pAwardView, 100);
	}
}

void Activeness::pageCallBack(cocos2d::CCObject * sender)
{
	CCMenuItem *pButton = (CCMenuItem *)sender;
	updatePage();
}

void Activeness::updatePage()
{
    
	
}


void Activeness::closeAwardView()
{
	m_pAwardView->removeFromParent();
	m_pAwardView = NULL;
}

void Activeness::onBackKey()
{
	if (m_pAwardView)
	{
		closeAwardView();
		return;
	}
	BackKeyInterface::onBackKey();
}