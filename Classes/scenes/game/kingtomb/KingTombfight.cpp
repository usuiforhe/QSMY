//
//  kingTombfight.cpp
//  LibQSMY
//
//  Created by jk on 14-8-5.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombfight.h"
#include "KingTombGuildList.h"

KingTombfight::KingTombfight()
:m_pNameTf(NULL)
,m_pfightCell1(NULL)
,m_pfightCell2(NULL)
,m_pfightCell3(NULL)
,m_pFightCells(NULL)
,m_pmodel(NULL)
,m_pLeft(NULL)
,m_pRight(NULL)
,m_pos(0)
{
	for (uint8_t i =0; i<TOMB_FIGHT_ROW; ++i)
	{
		m_pIndex[i] = NULL;
	}
}


KingTombfight::~KingTombfight()
{
    CC_SAFE_RELEASE(m_pNameTf);
    CC_SAFE_RELEASE(m_pfightCell1);
    CC_SAFE_RELEASE(m_pfightCell2);
    CC_SAFE_RELEASE(m_pfightCell3);
    CC_SAFE_RELEASE(m_pFightCells);
	CC_SAFE_RELEASE(m_pLeft);
	CC_SAFE_RELEASE(m_pRight);
	for (uint8_t i =0; i<TOMB_FIGHT_ROW; ++i)
	{
		CC_SAFE_RELEASE(m_pIndex[i]);
	}
}

bool KingTombfight::init(){
	if(!DPopup::init())
	{
		return false;
	}
    
    m_pmodel = DM_GET_KINGTOMB_MODEL;
    CCNodeLoaderLibrary *pNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary() ;
    CUSTOM_CCB_LOADER( pNodeLoaderLibrary ) ;
    pNodeLoaderLibrary->registerCCNodeLoader( "KingTombfightCell",KingTombfightCellLoader::loader() ) ;
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER( "PL_ui/ccb/pl_banghui/pl_banghui_tanxian_liebiao",
                                             pNodeLoaderLibrary ) ;
    m_pFightCells = CCArray::create();
    m_pFightCells->retain();
    m_pFightCells->addObject(m_pfightCell1);
    m_pFightCells->addObject(m_pfightCell2);
    m_pFightCells->addObject(m_pfightCell3);
    
    for(int i = 0 ;i<FIGHT_PLAY_MAX_NUM ;i++){
        KingTombfightCell * m_pfightCell = static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i));
        m_pfightCell->setIndex(i);
        m_pfightCell->attackIsFight(false);
        m_pfightCell->enemyIsFight(false);
        m_pfightCell->setDelegate(this);
    }

	for (uint8_t i =0; i<TOMB_FIGHT_ROW; ++i)
	{
		m_pIndex[i]->setVisible(false);
	}

	m_pLeft->getButton()->setTarget(this , menu_selector(KingTombfight::backBtnCallBack));
	m_pRight->getButton()->setTarget(this , menu_selector(KingTombfight::fightCallBack));
    return true;
}


bool KingTombfight::onAssignCCBMemberVariable(CCObject *pTarget,const char *pMemberVariableName, CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameTf", CCRichLabelTTF *, m_pNameTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pfightCell1", KingTombfightCell *, m_pfightCell1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pfightCell2", KingTombfightCell *, m_pfightCell2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pfightCell3", KingTombfightCell *, m_pfightCell3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLeft", DButton *, m_pLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRight", DButton *, m_pRight);

	D_CCB_ASSIGN_ARRAY("m_pIndex",CCNode*,this->m_pIndex,TOMB_FIGHT_ROW);
    return false;
}

//战斗按钮回调
void KingTombfight::fightCallBack(cocos2d::CCObject *sender){
    Json::Value assist(Json::arrayValue);
    for(int i = 1; i<FIGHT_PLAY_MAX_NUM ; i++){
        guild::GuildUserBasics* guildUser = m_pmodel->getSeleteUseByPos(i);
        if(guildUser){
            assist.append(guildUser->getUserId());
        }
    }
    m_pmodel->battleStartDragonValley(m_pos,assist);
}


void KingTombfight::onBackKey(){
    m_pmodel->removeSeleteUse();
    DPopup::onBackKey();
}

//返回按钮回调
void KingTombfight::backBtnCallBack(cocos2d::CCObject *sender){
    this->closePopup();
    m_pmodel->removeSeleteUse();
}

void KingTombfight::setData(int pos){
    m_pos = pos;
    updateView();
}

void KingTombfight::updateView(){
    kingTomb::kingTombInfo* info = m_pmodel->getKingTombInfoByPos(m_pos);
    //如果过期  战斗结束   关闭战斗界面
    if(!info ||info->getState()==kKingTombStateIsWork||info->isTimeLimit()||m_pmodel->getDragonEnd()){
        if(!info){
            m_pmodel->removeSeleteUse();
        }
        //如果战斗结束 并且胜利 播放 战斗胜利动画
        if(m_pmodel->getDragonEnd()){
            if(!m_pmodel->getDragonWin())
                m_pmodel->removeKingTombInfoByPos(m_pos,true);
            m_pmodel->setDragonEnd(false);
        }
        closePopup();
        return;
    }
    
    m_pNameTf->setString(m_pmodel->getTombTypeInfo(info->getMyInfo()->getType())->getName().c_str());

    
    kingTomb::kingTombTypeInfo *typeInfo = m_pmodel->getTombTypeInfo(info->getMyInfo()->getType());
    KingTombfightCell * m_pfightCell;
    
    //初始化 显示
    for(int i = 0; i<FIGHT_PLAY_MAX_NUM;i++){
        m_pfightCell = static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i));
        m_pfightCell->setEnemyData(NULL);
        m_pfightCell->attackIsFight(false);
        m_pfightCell->enemyIsFight(false);
        if(typeInfo->getFightNum()>i){
            m_pfightCell->setAttackData(NULL,false);
        }
        else{
            m_pfightCell->isActivate(false);
        }
    }
    m_pfightCell1->setMyData();
    m_pfightCell1->attackIsFight(true);
    
    //设置我方显示
    if(info->getIsFight()){
        //如果是战斗中  更具后端数据显示
        for(int i = 0; i<FIGHT_PLAY_MAX_NUM;i++){
            m_pfightCell = static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i));
            m_pfightCell->isActivate(false);
        }
        bool attackFightIndex = false;
        for(int i =0 ;i<info->getattackUserData()->count();i++){
            m_pfightCell = static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i));
            kingTomb::userData *data = static_cast< kingTomb::userData *>(info->getattackUserData()->objectAtIndex(i));
            m_pfightCell->setAttackData(data,false);
            if(data->getDie()==false &&!attackFightIndex){
                m_pfightCell->attackIsFight(true);
                attackFightIndex = true;
            }else{
                m_pfightCell->attackIsFight(false);
            }
        }
    }else{
        //如果 不是战斗中  则显示自己的 数据  和 按照选择助战好友 的数据来显示 
        for(int i = 1; i<FIGHT_PLAY_MAX_NUM ; i++){
            guild::GuildUserBasics* guildUser = m_pmodel->getSeleteUseByPos(i);
            if(guildUser){
                kingTomb::userData *user = kingTomb::userData::create();
                user->setDie(0);
                user->setSid(guildUser->getLeaderDress());
                user->setLv(guildUser->getLevel());
                user->setName(guildUser->getName());
                if(typeInfo->getFightNum()>i)
                    static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i))->setAttackData(user,true);
                else{
                    static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i))->isActivate(false);
                }
            }
        }
    }
    bool enemyFightIndex = false;

    // 设置敌方 显示
    for(int i =0 ;i<info->getdefendUserData()->count();i++){
		if(m_pIndex[i]) m_pIndex[i]->setVisible(true);

        m_pfightCell = static_cast<KingTombfightCell *>(m_pFightCells->objectAtIndex(i));
        kingTomb::userData *data = static_cast< kingTomb::userData *>(info->getdefendUserData()->objectAtIndex(i));
        m_pfightCell->setEnemyData(data);
        if(data->getDie()==false && !enemyFightIndex){
            m_pfightCell->enemyIsFight(true);
            enemyFightIndex = true;
        }else{
            m_pfightCell->enemyIsFight(false);
        }
    }
}

void KingTombfight::addAttack(uint32_t index){
    guild::GuildUser* userInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (!userInfo ||userInfo->getGuildID() ==0)
	{
        Tips_Alert(D_LOCAL_STRING("GuildErrorNoGuild"));
	}else{
        this->m_pDelegate->openGuildListPanel(index);
        this->closePopup();
    }
}

void KingTombfight::onEnter()
{
    BINDING_ONLY(m_pmodel, this, KingTombfight::updateView, D_EVENT_KINGTOMBLIST_GET);
    CCNode::onEnter();
}

void KingTombfight::onExit()
{
    UNBINDING_ALL(m_pmodel, this);
	CCNode::onExit();
}