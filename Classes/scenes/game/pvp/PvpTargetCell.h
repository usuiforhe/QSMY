//
//  PvpTargetCell.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpTargetCell__
#define __QSMY__PvpTargetCell__

#define	PVP_CELL_USER_COUNT	3

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/PvpModel.h"
#include "RankUser.h"

struct PvpDelegate;

#pragma mark -----------PvpTargetCell------------
class PvpTargetCell
 : public cocos2d::extension::CCTableViewCell, public cocos2d::extension::CCBMemberVariableAssigner
 , public cocos2d::extension::CCBSelectorResolver
{
public:
	PvpTargetCell();
    
	virtual ~PvpTargetCell();
    
    static PvpTargetCell * create(cocos2d::extension::CCData* pData);
    
	/**
	 *	@brief	初始化
	 *
	 *	@return	是否成功
	 */
	virtual bool init(cocos2d::extension::CCData* pData);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
	/**
	 *	@brief	设置PVP协议
	 *
	 *	@param 	pDelegate 	PVP协议
	 */
	inline void setDelegate(PvpDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
		for (int i = 0; i < PVP_CELL_USER_COUNT; i++)
		{
			m_pRankUser[i]->setDelegate(m_pDelegate);
		}
		
	}
    
    /**
     *	@brief	设置对手信息
     *
     *	@param 	target 	对手信息
     */
    void setTargetInfo(pvp::ChTargetInfo * pTargetInfo0, pvp::ChTargetInfo *pTargetInfo1, pvp::ChTargetInfo *pTargetInfo2, bool autoPos = true);
    
	void setBossVisible(bool bBossVisible);
protected:    
    /**
     *	@brief	更新单元视图
     */
    virtual void updateView();
    
protected:
    PvpDelegate * m_pDelegate;
    
	cocos2d::CCNode * m_pContainer;		//背景\头像\按钮
	cocos2d::CCNode * m_pSelfContainer; //自己的背景
	cocos2d::CCNode * m_pTargetContainer;//目标的背景
    cocos2d::CCLabelBMFont * m_pRank;   //排名
    cocos2d::CCLabelBMFont * m_pLevel;  //等级
    cocos2d::CCLabelTTF * m_pName;      //名字
    cocos2d::CCLabelBMFont * m_pPointGrow;  //每10分钟积分成长
    
    DButton * m_pButton;
    cocos2d::CCSprite * m_pSpriteChallenge;
    cocos2d::CCSprite * m_pSpriteRefresh;
    cocos2d::CCSprite * m_pSpriteView;

	cocos2d::CCNode *m_pNodeRoot;

	RankUser *m_pRankUser[PVP_CELL_USER_COUNT];
	float m_pCellPosY[PVP_CELL_USER_COUNT];

	CCNode	*m_pCCBBoss;
	CCLabelTTF	*m_pTTFName;
	CCLabelTTF	*m_pTTFCount;

	void callBackBoss(CCObject *pObj);
};

#pragma mark -----------PvpTopRankCell------------
class PvpTopRankCell:public PvpTargetCell
{
public:
    static PvpTopRankCell * create(cocos2d::extension::CCData* pData);
    
    virtual ~PvpTopRankCell(){};
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
protected:
    virtual void updateView();
    void onViewClick(cocos2d::CCObject *sender);
};

#pragma mark -----------PvpViewCell------------
class PvpViewCell:public PvpTargetCell
{
public:
    static PvpViewCell * create(cocos2d::extension::CCData* pData);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
protected:
    virtual void updateView();
    void onRefreshClick(cocos2d::CCObject *sender);
    void onChallengeClick(cocos2d::CCObject *sender);
};

#pragma mark ------------PvpBossView--------------
class PvpBossView:
public cocos2d::CCNode,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    PvpBossView():m_pName(NULL), m_pDesc(NULL), m_pChContainer(NULL), m_pBossTimes(NULL), m_pButton(NULL), m_pDelegate(NULL)
    {
        
    }
    
    virtual ~PvpBossView();
    
	CREATE_FUNC(PvpBossView);
    
    virtual bool init();
    
    void updateView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	CC_SYNTHESIZE(PvpDelegate *, m_pDelegate, Delegate);
    
private:
    void onChallengeClick(cocos2d::CCObject *sender);
    
private:
    cocos2d::CCLabelTTF * m_pName;
    cocos2d::CCLabelTTF * m_pDesc;
    cocos2d::CCNode * m_pChContainer;
    cocos2d::CCLabelBMFont * m_pBossTimes;
    DButton * m_pButton;
};

class PvpBossViewLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PvpBossViewLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PvpBossView);
};

#pragma mark ------------PvpRobotView--------------
class PvpRobotView:
public cocos2d::CCNode,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    PvpRobotView():m_pName(NULL), m_pDesc(NULL), m_pButton(NULL), m_pDelegate(NULL)
    {
        
    }
    
    virtual ~PvpRobotView();
    
	CREATE_FUNC(PvpRobotView);
    
    virtual bool init();
    
    void updateView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	CC_SYNTHESIZE(PvpDelegate *, m_pDelegate, Delegate);
    void onChallengeClick(cocos2d::CCObject *sender);
private:
    cocos2d::CCLabelTTF * m_pName;
    cocos2d::CCLabelTTF * m_pDesc;
    DButton * m_pButton;
};

class PvpRobotViewLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PvpRobotViewLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PvpRobotView);
};


#endif /* defined(__QSMY__PvpTargetCell__) */
