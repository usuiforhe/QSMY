//
//  kingTombfightCell.cpp
//  LibQSMY
//
//  Created by jk on 14-8-5.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombfightCell.h"
#include "KingTombGuildList.h"
#include "managers/GameManager.h"


KingTombfightCell::KingTombfightCell()
:m_pNameEnemyTf(NULL)
,m_pNameTf(NULL)
,m_pEnemyContainer(NULL)
,m_pContainer(NULL)
,m_pAddFriendBtn(NULL)
,m_pdefaultFace(NULL)
,m_pdefaultEnemyFace(NULL)
,m_pClickImg(NULL)
,m_pLvEnemyTF(NULL)
,m_pLvTF(NULL)
,m_pEnemyWinImg(NULL)
,m_pWinImg(NULL)
,m_pIndex(0)
,m_pisFight1(NULL)
,m_pisFight2(NULL)
,m_pAttack(NULL)
,m_pEnemy(NULL)
,m_pDelegate(NULL)
{

}

KingTombfightCell::~KingTombfightCell(){
    CC_SAFE_RELEASE(m_pNameEnemyTf);
    CC_SAFE_RELEASE(m_pNameTf);
    CC_SAFE_RELEASE(m_pEnemyContainer);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pAddFriendBtn);
    CC_SAFE_RELEASE(m_pdefaultFace);
    CC_SAFE_RELEASE(m_pClickImg);
    CC_SAFE_RELEASE(m_pdefaultEnemyFace);
    CC_SAFE_RELEASE(m_pLvEnemyTF);
    CC_SAFE_RELEASE(m_pLvTF);
    CC_SAFE_RELEASE(m_pEnemyWinImg);
    CC_SAFE_RELEASE(m_pWinImg);
    CC_SAFE_RELEASE(m_pisFight1);
    CC_SAFE_RELEASE(m_pisFight2);
    CC_SAFE_RELEASE(m_pAttack);
    CC_SAFE_RELEASE(m_pEnemy);
}



//设置 敌方显示
void KingTombfightCell::setEnemyData(kingTomb::userData* data){
    m_pEnemyContainer->removeAllChildren();
    if(!data){
        m_pNameEnemyTf->setString("");
        m_pLvEnemyTF->setString("");
        m_pdefaultEnemyFace->setVisible(true);
        m_pEnemyWinImg->setVisible(false);
    }else{
        m_pNameEnemyTf->setString(data->getName().c_str());
        m_pdefaultEnemyFace->setVisible(false);
        CCSprite* pHead = ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(data->getSid());
        pHead->setFlipX(true);
		m_pEnemyContainer->addChild(pHead);
        m_pLvEnemyTF->setString(D_CSTRING_FROM_UINT(data->getLv()));
        m_pEnemyWinImg->setVisible(data->getDie());
        if(data->getDie()){
            pHead->setShaderProgram(D_SHADER_MASK_GRAY);
        }
    }
}

//我方正在攻击
void KingTombfightCell::attackIsFight(bool b){
    m_pisFight1->setVisible(b);
}
//地方正在攻击
void KingTombfightCell::enemyIsFight(bool b){
    m_pisFight2->setVisible(b);
}


//设置 我方面实现
void KingTombfightCell::setAttackData(kingTomb::userData* data,bool isSelete){
    m_pContainer->removeAllChildren();
    if(!data){
        m_pNameTf->setString("");
        m_pAddFriendBtn->setEnabled(true);
        m_pClickImg->setVisible(true);
        m_pdefaultEnemyFace->setVisible(true);
        m_pLvTF->setString("");
        m_pWinImg->setVisible(false);
        m_pdefaultFace->setVisible(true);
    }else{
        m_pLvTF->setString(D_CSTRING_FROM_UINT(data->getLv()));
        m_pNameTf->setString(data->getName().c_str());
        m_pAddFriendBtn->setEnabled(isSelete);
        m_pClickImg->setVisible(false);
        m_pdefaultFace->setVisible(false);
        CCSprite* pHead = ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(data->getSid());
        m_pContainer->addChild(pHead);
        m_pWinImg->setVisible(data->getDie());
        if(data->getDie()){
            pHead->setShaderProgram(D_SHADER_MASK_GRAY);
        }
    }
}


//设置自己的数据显示
void KingTombfightCell::setMyData(){
    m_pContainer->removeAllChildren();
    m_pLvTF->setString(D_CSTRING_FROM_UINT(DM_GET_USER_MODEL->getUserInfo()->getUserLv()));
    m_pNameTf->setString(DM_GET_USER_MODEL->getUserInfo()->getUserName().c_str());
    m_pAddFriendBtn->setEnabled(false);
    m_pClickImg->setVisible(false);
    m_pdefaultEnemyFace->setVisible(false);
    m_pWinImg->setVisible(false);
    m_pdefaultFace->setVisible(false);
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(DM_GET_FORMATION_MODEL->getFormationLeader()));
}


void KingTombfightCell::addFriendCallBack(cocos2d::CCObject *sender){
    this->getDelegate()->addAttack(m_pIndex);
}


//设置是否是 可以点击添加助战的状态
void KingTombfightCell::isActivate(bool b){
    m_pContainer->removeAllChildren();
    m_pNameTf->setString("");

	m_pAddFriendBtn->setEnabled(b);

    m_pClickImg->setVisible(b);
    m_pdefaultEnemyFace->setVisible(true);
    m_pdefaultFace->setVisible(true);
    m_pLvTF->setString("");
    m_pWinImg->setVisible(false);
}

bool KingTombfightCell::onAssignCCBMemberVariable(CCObject *pTarget,const char *pMemberVariableName, CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameEnemyTf", CCLabelTTF *, m_pNameEnemyTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameTf", CCLabelTTF *, m_pNameTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnemyContainer", CCNode *, m_pEnemyContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddFriendBtn", CCMenuItem *, m_pAddFriendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pdefaultFace", CCNode *, m_pdefaultFace);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pdefaultEnemyFace", CCNode *, m_pdefaultEnemyFace);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLvEnemyTF", CCLabelBMFont *, m_pLvEnemyTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLvTF", CCLabelBMFont *, m_pLvTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnemyWinImg", CCNode *, m_pEnemyWinImg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWinImg", CCNode *, m_pWinImg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pisFight2", CCNode *, m_pisFight2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pisFight1", CCNode *, m_pisFight1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCNode *, m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEnemy", CCNode *, m_pEnemy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClickImg", UIAnimate *, m_pClickImg);

    return false;
}

SEL_MenuHandler KingTombfightCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName )
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( this, "addFriendCallBack",KingTombfightCell::addFriendCallBack ) ;
    return NULL ;
}