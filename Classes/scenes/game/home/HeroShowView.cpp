//
//  HeroShowView.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "HeroShowView.h"
#include "managers/ResourceManager.h"
#include "../Game.h"

//动画持续时间
#define HERO_SHOW_TIME	4.2f
#define HIDING_ACTION_TAG	115
using namespace navigation;
USING_NS_CC;
USING_NS_CC_EXT;

HeroShowView::HeroShowView()
:m_pFormationModel(NULL)
,m_pDelegate(NULL)
,m_pDPageViewLoop(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pGrade(NULL)
,m_nCurrentPage(0)
,m_pWarriorTypeMark(NULL)
,m_pDialogEffect(NULL)
,m_pDialog(NULL)
,m_pFormationContainer(NULL)
,m_pLabelTalk(NULL)
,m_bTalking(false)
{
    
}

HeroShowView::~HeroShowView()
{
    CC_SAFE_RELEASE(m_pDPageViewLoop);
    CC_SAFE_RELEASE(m_pItems);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
    CC_SAFE_RELEASE(m_pDialogEffect);
    //CC_SAFE_RELEASE(m_pDialog);
	CC_SAFE_RELEASE(m_pFormationContainer);
	CC_SAFE_RELEASE_NULL(m_pLabelTalk);
}

bool HeroShowView::init()
{
    if(!DLayer::init()) return false;
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    
    /*m_pDialog = ResourceManager::sharedResourceManager()->getUIAnimate("home_dialog", sp::ZORDER);
    CC_SAFE_RETAIN(m_pDialog);*/

    m_pItems = CCArray::create();
    m_pItems->retain();
    
    
    m_pDPageViewLoop = DPageViewLoop::create(this,CCSizeMake(500, 600));
    m_pDPageViewLoop->setClippingToBounds(true);
    m_pDPageViewLoop->setDelegate(this);
    m_pDPageViewLoop->setPosition(ccp(-50,-90));
    m_pDPageViewLoop->setAutoMove(true);
    m_pDPageViewLoop->setFade(false);
    m_pDPageViewLoop->setMoveInterval(D_CONFIG_FLOAT_FOR_KEY(HERO_AUTO_MOVE_INTERVAL));
    m_pDPageViewLoop->retain();
    //addChild(m_pDPageViewLoop);
    
    
    CCClippingNode* pNode = CCClippingNode::create();
    pNode->addChild(m_pDPageViewLoop);
    addChild(pNode);
    
    
    CCLayerColor *stencil = CCLayerColor::create(ccc4(255,255,255,255),500,600);
    
    pNode->setStencil(stencil);
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_home/PL_Home_hero");
    
// 	CCSprite * formationBtn = CCSprite::create("PL_ui/home/home_btn_zr.png");
//  	formationBtn->setContentSize(CCSizeMake(79, 146));
// 	m_pFormationContainer->addChild(formationBtn);
	CCMenuItemImage *formationBtn = CCMenuItemImage::create("PL_ui/home/home_btn_zr.png", "PL_ui/home/home_btn_zr_up.png");
	CCMenu *pMenu = CCMenu::createWithItem(formationBtn);
	pMenu->setPosition(ccp(0, 0));
	pMenu->setTouchEnabled(false);
	m_pFormationContainer->addChild(pMenu);
	formationBtn->setTag(0);
	formationBtn->setTarget(this, menu_selector(HeroShowView::viewCallback));

	PL_MOVE_NODE_BELOW_CENTER_Y(this);

    return true;
}

bool HeroShowView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
   
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFormationContainer", CCNode *, this->m_pFormationContainer);
    return false;
}

void HeroShowView::scrollViewDidScroll(CCScrollView* view)
{
    uint16_t page = m_pDPageViewLoop->getCurrentPage()%m_pItems->count();
    if(m_nCurrentPage==page) return;
    m_nCurrentPage = page;
    updateInfo();
}

void HeroShowView::updateInfo()
{
    if(m_nCurrentPage>=m_pItems->count()) return;
    CCMenu * pItem = (CCMenu *)m_pItems->objectAtIndex(m_nCurrentPage);
    int32_t wid = pItem->getTag();
    warrior::WarriorCurInfo * pWarriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
    m_pLevel->setString(CCString::createWithFormat("%d",pWarriorInfo->getWarriorLv())->getCString());
    m_pName->setString(pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
    //m_pGrade->setColor(DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pWarriorInfo->getBaseInfo()->getRealGrade()));
    m_pWarriorTypeMark->setWarriorInfo(pWarriorInfo->getBaseInfo());
    
	//新增人物等级大于1才开放对话
    if(m_pDialogEffect != NULL && DM_GET_USER_MODEL->getUserInfo()->getUserLv() > 1)
    {
        //m_pDialogEffect->removeAllChildrenWithCleanup(false);
        if(m_bTalking)
		{
			hideHeroTalk();
			//直接结束
			return;
		}


        uint32_t val = rand() % 100;
        if(D_CONFIG_UINT_FOR_KEY(HERO_DIALOG_PROB) > val)
        {   
            val = rand() % 100;
            cocos2d::CCString * pContent(NULL);
            if(D_CONFIG_UINT_FOR_KEY(HERO_WARRIOR_DIALOG_PROB) > val)
            {
                warrior::WarriorDialogInfo * pDialogInfo = DM_GET_WARRIOR_MODEL->getWarriorDialogInfo(wid);
                if(pDialogInfo != NULL)
                {
                    pContent = pDialogInfo->getDialog();
                }
            }
            
            if(pContent == NULL)
            {
                pContent = DM_GET_NOTICE_MODEL->getTipsInfo()->getDialog();
            }
            
			//重新加载动画
			m_pDialogEffect->removeAllChildren();
			m_pDialog = ResourceManager::sharedResourceManager()->getUIAnimate("home_dialog", sp::ZORDER);
            
            cocos2d::CCLabelTTF * pLabel = cocos2d::CCLabelTTF::create(pContent->getCString(), D_FONT_HELVETICA, D_FONT_SIZE_10);
            //增加一个指针
			CC_SAFE_RELEASE_NULL(m_pLabelTalk);
			m_pLabelTalk = pLabel;
			CC_SAFE_RETAIN(m_pLabelTalk);

			pLabel->setFontFillColor(ccc3(0, 0, 0),true);
			pLabel->setColor(ccc3(0, 0, 0));
            pLabel->setDimensions(CCSizeMake(210, 110));
            pLabel->setAnchorPoint(ccp(0,1));
            pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
            sp::SPContainer *pSPContainer =  sp::SPContainer::create(m_pDialog->getRenderType());
            pSPContainer->addChild(pLabel);
            m_pDialog->getBone("home_dialog")->replaceDisplayAt(pSPContainer, 0);
            m_pDialog->getAnimation()->playByIndex(0,-1,-1,true);
			//避免重复添加 并做定时隐藏
            if(m_pDialog->getParent() == NULL) m_pDialogEffect->addChild(m_pDialog);
			CCAction* pAction = CCSequence::create(CCDelayTime::create(HERO_SHOW_TIME),CCCallFunc::create(this, callfunc_selector(HeroShowView::hideHeroTalk)),NULL);
			pAction->setTag(HIDING_ACTION_TAG);
			this->runAction(pAction);
			m_bTalking = true;
        }
    }
}

void HeroShowView::hideHeroTalk()
{
	if(m_bTalking && m_pDialog)
	{
		this->stopActionByTag(HIDING_ACTION_TAG);
		m_pDialog->getAnimation()->playByIndex(1,-1,-1,true);
		m_bTalking = false;
	}
	if(m_pLabelTalk) m_pLabelTalk->setVisible(false);
}

float HeroShowView::cellSizeForTable(DPageView *view)
{
    return 450.0f;
}

CCNode* HeroShowView::cellAtIndex(DPageView *view, unsigned int idx)
{
    CCNode* pItem = (CCNode*)m_pItems->objectAtIndex(idx);
    return pItem;
}

uint32_t HeroShowView::numberOfCellsInTableView(DPageView *view)
{
    return m_pItems->count();
}

void HeroShowView::onEnter()
{
    BINDING_EXEC(m_pFormationModel,this,HeroShowView::onFormationUpdate,D_EVENT_FORMATION_UPDATE);
    DLayer::onEnter();
}

void HeroShowView::onExit()
{
    UNBINDING_ALL(m_pFormationModel, this);
    DLayer::onExit();
}


void HeroShowView::onFormationUpdate()
{
    m_pItems->removeAllObjects();
    
    uint32_t count = m_pFormationModel->getUnlockedPositionsCount();
    uint32_t wid = 0;
    
    for (uint32_t i=0; i<count; ++i)
    {
        wid = m_pFormationModel->getUnlockedPositionInfoByIndex(i)->iWarriorId;
        if(wid==0) continue;
        
        warrior::WarriorCurInfo* pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
		if(!pWarrior) continue;
        
        CCMenuItem * pItem = CCMenuItem::create(this,menu_selector(HeroShowView::viewCallback));
        
        pItem->setAnchorPoint(D_ANCHOR_POINT_LEFTBOTTOM);
		pItem->setContentSize(CCSizeMake(450, 450));
        // warrior::WarriorCurInfo * pWarriorInfo = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
      //  CCNode* pIcon = ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(pWarrior->getAvatarId());
		CCNode* pIcon = ResourceManager::sharedResourceManager()->getWarriorIcon(pWarrior->getAvatarId());
       // pIcon->setAnchorPoint(D_ANCHOR_POINT_LEFTBOTTOM);
        pIcon->setPosition(ccp(pItem->getContentSize().width * 0.5,160));
		pIcon->setScale(0.85f);
        pItem->addChild(pIcon);
        //        CCLabelTTF* pTTF = CCLabelTTF::create(pWarriorInfo->baseInfo->warrior_name.c_str(), D_FONT_DEFAULT, D_FONT_SIZE_18);
        //        pTTF->setAnchorPoint(D_ANCHOR_POINT_CENTER);
        //        pTTF->setPosition(ccp(150,0));
        //        pItem->addChild(pTTF);		
       

        CCMenu *pMenu = CCMenu::create(pItem,NULL);
        pMenu->setTag(wid);
        pItem->setTag(wid);
        pMenu->setTouchEnabled(false);
        m_pItems->addObject(pMenu);
    }
    
    m_pDPageViewLoop->reloadData();
    updateInfo();
}

void HeroShowView::viewCallback(CCObject * obj)
{
    if(m_pDelegate)
    {
        CCMenuItem *pItem =(CCMenuItem *)obj;
        if(pItem)
        {
            m_pDelegate->NavigateTo(kNavFormation , false , CCInteger::create(pItem->getTag()));
        }
    }
}
