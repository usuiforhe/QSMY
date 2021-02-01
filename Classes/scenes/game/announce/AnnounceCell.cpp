//
//  AnnounceCell.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AnnounceCell.h"
#include "utils/StringUtil.h"
#include "Announce.h"

USING_NS_CC;
USING_NS_CC_EXT;


AnnounceCell::AnnounceCell()
: m_pContainer(NULL)
, m_pTitle(NULL)
, m_pContent(NULL)
, m_pBgSpirite(NULL)
, m_pButtonContainer(NULL)
//, m_pButton2Container(NULL)
, m_pAnnounce(NULL)
, m_pImageGroup(NULL)
, m_pImageContainer(NULL)
//, m_pHotArea(NULL)
, m_fHeight(0.f)
{
    
}

AnnounceCell::~AnnounceCell()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTitle);
    CC_SAFE_RELEASE(m_pContent);
    CC_SAFE_RELEASE(m_pBgSpirite);
    CC_SAFE_RELEASE(m_pButtonContainer);
    CC_SAFE_RELEASE(m_pAnnounce);
    CC_SAFE_RELEASE(m_pImageGroup);
    CC_SAFE_RELEASE(m_pImageContainer);
    //CC_SAFE_RELEASE(m_pHotArea);
    // CC_SAFE_RELEASE(m_pButton2Container);
}

AnnounceCell * AnnounceCell::create(cocos2d::extension::CCData *pData)
{
    AnnounceCell * pRet = new AnnounceCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

bool AnnounceCell::init(cocos2d::extension::CCData *pData)
{
    if(!CCTableViewCell::init())
    {
        return false;
    }
    
    CCB_READER_DATA_AND_ADDCHILD(pData);
    return true;
}

bool AnnounceCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitle", CCLabelTTF *, m_pTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent", CCLabelTTF *, m_pContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgSpirite", CCScale9Sprite *, m_pBgSpirite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonContainer", CCNode *, m_pButtonContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton2Container", CCNode *, m_pButton2Container);    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImageGroup", CCNode *, m_pImageGroup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImageContainer", CCNode *, m_pImageContainer);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHotArea", CCNode *, m_pHotArea);
    
    return false;
}


cocos2d::SEL_MenuHandler AnnounceCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "navCallBack", AnnounceCell::navCallBack);
    return NULL;
}

void AnnounceCell::navCallBack(cocos2d::CCObject * sender)
{
    if(m_pDelegate) m_pDelegate->NavigationTo(m_pAnnounce);
}

void AnnounceCell::setAnnounce(announce::AnnounceInfo *pAnnounce)
{
    if(m_pAnnounce == pAnnounce || pAnnounce == NULL)
    {
        return;
    }
    
    CC_SAFE_RETAIN(pAnnounce);
    CC_SAFE_RELEASE(m_pAnnounce);
    m_pAnnounce = pAnnounce;
    
    m_pImageGroup->setVisible(pAnnounce->hasPic());
    
    m_pImageContainer->removeAllChildren();
	CCNode* pPic = pAnnounce->getPic();
	if (pPic)
	{
		m_pImageContainer->addChild(pPic);
	}
    
    m_pContent->setDimensions(CCSizeMake(m_pContent->getDimensions().width, 0));
    
    m_pTitle->setString(m_pAnnounce->getTitle().c_str());
    m_pContent->setString(m_pAnnounce->getContent().c_str());

    float contentHeight = m_pContent->getContentSize().height+m_pAnnounce->getPicSize().height;
    
    
    m_pContent->setPositionY(-m_pAnnounce->getPicSize().height);
    
    
    m_fHeight = contentHeight + ANNOUNCE_CELL_NAV_HEIGHT;

    
    //m_pHotArea->setContentSize(CCSize(m_pHotArea->getContentSize().width,m_fHeight));
	
    if(m_pAnnounce->isNeedNav())
    {
        m_pButtonContainer->setVisible(true);
        //m_pButton2Container->setVisible(true);
        m_fHeight+=ANNOUNCE_CELL_NAV_HEIGHT;
    }
    else
    {
        m_pButtonContainer->setVisible(false);
        //m_pButton2Container->setVisible(false);
    }
    
    m_pButtonContainer->setPositionY(-m_fHeight);
    
    
    m_pContainer->setPositionY(m_fHeight);
    CCSize sz = m_pBgSpirite->getContentSize();
    sz.height = m_fHeight;
    m_pBgSpirite->setPreferredSize(sz);
    
    
}


