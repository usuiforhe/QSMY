//
//  kingTombfightCell.h
//  LibQSMY
//  战斗 角色显示 cell
//  Created by jk on 14-8-5.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#ifndef __LibQSMY__KingTombfightCell__
#define __LibQSMY__KingTombfightCell__

#include "AppInclude.h"
#include "../Game.h"
#include "managers/DataManager.h"

using namespace cocos2d;


class KingTombfightCell;

class KingTombfightCellDelegate
{
public:
	virtual void addAttack(uint32_t index) = 0;
};

class KingTombfightCell
:public cocos2d::CCNode
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver

{
    
    CC_SYNTHESIZE(KingTombfightCellDelegate *, m_pDelegate, Delegate);
    CC_SYNTHESIZE(uint32_t, m_pIndex, Index);
    

public:
    KingTombfightCell();
    ~KingTombfightCell();
    

    CREATE_FUNC( KingTombfightCell ) ;
    D_INIT_DEFAULT(CCNode);

    
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,
                                           const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
    //-----------CCBSelectorResolver------------------
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};
    
public:
    
    void addFriendCallBack(cocos2d::CCObject *sender);
    
    CCLabelTTF *m_pNameEnemyTf;
    CCLabelTTF *m_pNameTf;
    CCNode *m_pEnemyContainer;
    CCNode *m_pContainer;
    CCMenuItem *m_pAddFriendBtn;
    CCLabelBMFont *m_pLvEnemyTF;
    CCLabelBMFont *m_pLvTF;
    CCNode * m_pEnemyWinImg;
    CCNode * m_pWinImg;
    
//    CCLabelBMFont *m_pNumTF;
    CCNode *m_pdefaultFace;
    UIAnimate *m_pClickImg; //点击邀请好友的动画
    CCNode *m_pdefaultEnemyFace;
    
    CCNode *m_pisFight1;
    CCNode *m_pisFight2;

    CCNode *m_pAttack;
    CCNode *m_pEnemy;
    
    
    
    void setEnemyData(kingTomb::userData* data);
    void setAttackData(kingTomb::userData* data,bool isSelete);
    void setMyData();
    
    void isActivate(bool b);
    
    //我方正在攻击
    void attackIsFight(bool b);
    //地方正在攻击
    void enemyIsFight(bool b);

    

};

class KingTombfightCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(KingTombfightCellLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(KingTombfightCell);
};

#endif /* defined(__LibQSMY__kingTombfightCell__) */
