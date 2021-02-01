//
//  WishCell.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WishCell__
#define __QSMY__WishCell__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"
#include "../../../model/WishModel.h"
#include "../warrior/WarriorListView.h"

struct WishDelegate;

#pragma mark ------------------WishCell--------------------
/**
 @brief     普通许愿格子
 **/
class WishCell:
public cocos2d::CCNode,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    WishCell():
    m_pGrade(NULL),
    m_pLight(NULL),
	m_pNodeGreen(NULL),
	m_pNodeBlue(NULL),
	m_pNodePurple(NULL),
	m_pNodeOrange(NULL),
    m_eColorGrade(warrior::kWarGradeGreen)
    {
        
    }
    
    virtual ~WishCell()
    {
        CC_SAFE_RELEASE(m_pGrade);
        CC_SAFE_RELEASE(m_pLight);
		CC_SAFE_RELEASE(m_pNodeGreen);
		CC_SAFE_RELEASE(m_pNodeBlue);
		CC_SAFE_RELEASE(m_pNodePurple);
		CC_SAFE_RELEASE(m_pNodeOrange);
    }
    
    CREATE_FUNC(WishCell);
    D_INIT_DEFAULT(cocos2d::CCNode);
    
    virtual void showLight()
    {
        m_pLight->setVisible(true);
    }
    
    virtual void hideLight()
    {
        m_pLight->setVisible(false);
    }
    
    virtual void setColorGrade(warrior::WarriorGrade grade);
    
    virtual warrior::WarriorGrade getColorGrade()
    {
        return m_eColorGrade;
    }
    
    virtual bool isUnlocked()
    {
        return true;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
	cocos2d::CCNode * m_pNodeGreen;
	cocos2d::CCNode * m_pNodeBlue;
	cocos2d::CCNode * m_pNodePurple;
	cocos2d::CCNode * m_pNodeOrange;

protected:
    cocos2d::CCSprite * m_pGrade;
    cocos2d::CCSprite * m_pLight;
    
    warrior::WarriorGrade m_eColorGrade;
};

#pragma mark ------------------WishCellVip--------------------
/**
 @brief     VIP许愿格子，可塞入弟子
 **/
class WishCellVip:
public WishCell,
public WarriorSelectDelegate
{
public:
    WishCellVip():
    WishCell(),
    m_pHead(NULL),
    m_pLock(NULL),
    m_pSelectWarrior(NULL),
    m_pVipContainer(NULL),
    m_pVipLevel(NULL),
    m_pButton(NULL),
    m_pWishItem(NULL),
    m_pEffectNode(NULL),
    m_pDelegate(NULL)
    {
        
    }
    
    virtual ~WishCellVip()
    {
        CC_SAFE_RELEASE(m_pHead);
        CC_SAFE_RELEASE(m_pLock);
        CC_SAFE_RELEASE(m_pSelectWarrior);
        CC_SAFE_RELEASE(m_pVipContainer);
        CC_SAFE_RELEASE(m_pVipLevel);
        CC_SAFE_RELEASE(m_pButton);
        CC_SAFE_RELEASE(m_pWishItem);
        CC_SAFE_RELEASE(m_pEffectNode);
    }
    
    CREATE_FUNC(WishCellVip);
    D_INIT_DEFAULT(WishCell);
    
    virtual bool isUnlocked();
    
    /**
     *	@brief	设置许愿代理
     *
     *	@param 	delegate 	代理
     */
    void setDelegate(WishDelegate * delegate)

    {
        m_pDelegate = delegate;
    }
    
    void setSelectedWarrior(warrior::WarriorCurInfo * pWarrior);
    virtual void setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType){}
    /**
     *	@brief	更新视图
     */
    void updateView();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
protected:
    
    /**
     *	@brief	选择弟子按钮回调
     *
     *	@param 	sender 	发送者
     */
    void onSelectWarriorClick(cocos2d::CCObject * sender);
    
    void unlockWarrior(cocos2d::CCObject * sender);
    
protected:
    cocos2d::CCNode * m_pHead;
    cocos2d::CCSprite * m_pLock;
    cocos2d::CCSprite * m_pSelectWarrior;
    cocos2d::CCNode * m_pVipContainer;
    cocos2d::CCLabelBMFont * m_pVipLevel;
    cocos2d::CCMenuItem * m_pButton;
    cocos2d::CCNode * m_pEffectNode;
    
    CC_SYNTHESIZE_RETAIN(wish::WishItem * , m_pWishItem, WishItem);
    
    WishDelegate * m_pDelegate;
};

class WishCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WishCellLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WishCell);
};

class WishCellVipLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WishCellVipLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WishCellVip);
};

#endif /* defined(__QSMY__WishCell__) */
