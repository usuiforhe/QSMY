//
//  AnnounceCell.h
//  QSMY
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AnnounceCell__
#define __QSMY__AnnounceCell__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"

#define ANNOUNCE_CELL_WIDTH                     640.f
#define ANNOUNCE_CELL_BOTTOM_HEIGHT             30.f
#define ANNOUNCE_CELL_NAV_HEIGHT                40.f
#define ANNOUNCE_CELL_HEIGHT_PLUS				10.f


class AnnounceDelegate;

class AnnounceCell
: public cocos2d::extension::CCTableViewCell
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
{
    
public:
    
    CC_SYNTHESIZE(AnnounceDelegate*, m_pDelegate, Delegate);
    
    AnnounceCell();
    
    virtual ~AnnounceCell();
    
    static AnnounceCell * create(cocos2d::extension::CCData * pData);
    
    bool init(cocos2d::extension::CCData * pData);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){ return NULL;};
    
    void setAnnounce(announce::AnnounceInfo * pAnnounce);
    

    
private:
    cocos2d::CCNode * m_pContainer;
    cocos2d::CCLabelTTF * m_pTitle;
    cocos2d::CCLabelTTF * m_pContent;
    cocos2d::extension::CCScale9Sprite * m_pBgSpirite;
    cocos2d::CCNode * m_pButtonContainer;
    // cocos2d::CCNode * m_pButton2Container;
    cocos2d::CCNode * m_pImageGroup;
    cocos2d::CCNode * m_pImageContainer;
    //cocos2d::CCNode * m_pHotArea;
    
    announce::AnnounceInfo * m_pAnnounce;
    
    CC_SYNTHESIZE(float, m_fHeight, Height);
    
private:
    void navCallBack(cocos2d::CCObject * sender);
};

#endif /* defined(__QSMY__AnnounceCell__) */
