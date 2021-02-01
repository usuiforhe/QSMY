//
//  kingTombGuildList.cpp
//  LibQSMY
//
//  Created by jk on 14-8-7.
//  Copyright (c) 2014年 jk. All rights reserved.
//

#include "KingTombGuildList.h"
#include "KingTombfight.h"

#define GUILD_LIST_CELL_SIZE	CCSizeMake(581 , 128)

KingTombGuildList::KingTombGuildList()
:m_pTableContainer(NULL)
,m_pTitleTf(NULL)
,m_pCloseButton(NULL)
,m_pTableView(NULL)
,m_pIndex(0)
,m_pos(0)
,m_pList(NULL)
,m_pModel(NULL)
,m_pDelegate(NULL)
{
}

KingTombGuildList::~KingTombGuildList(){
    CC_SAFE_RELEASE(m_pTableContainer);
    CC_SAFE_RELEASE(m_pTitleTf);
	CC_SAFE_RELEASE(m_pCloseButton);
    CC_SAFE_RELEASE(m_pList);
}



bool KingTombGuildList::init(){
	if(!DPopup::init())
	{
		return false;
	}
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jl_cy") ;
	//CCNode* pCell = NULL;
 //   CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_tanxian_jl_cy_cell", pCell);
 //   m_pCellSize = pCell->getContentSize();
	//CC_SAFE_RELEASE(pCell);

	m_pCellSize = GUILD_LIST_CELL_SIZE;
    m_pList = CCArray::create();
    CC_SAFE_RETAIN(m_pList);
    
    m_pModel = DM_GET_KINGTOMB_MODEL;

    m_pTableView = DTableView::create(this, m_pTableContainer->getContentSize());
	m_pTableView->setDirection( kCCScrollViewDirectionVertical ) ;
	m_pTableView->setVerticalFillOrder( kCCTableViewFillTopDown ) ;
	m_pTableView->setDelegate( this ) ;
	m_pTableContainer->addChild(m_pTableView);

    m_pTitleTf->setString(DM_GET_GUILD_MODEL->getGuildInfo()->getName().c_str());
	m_pCloseButton->getButton()->setTarget(this , menu_selector(KingTombGuildList::closeCallBack));
    return true;
}


void KingTombGuildList::refresh(){
    m_pList->removeAllObjects();
    for(int i = 0; i<DM_GET_GUILD_MODEL->getGuildUser()->count();i++){
        guild::GuildUserBasics *user  = static_cast<guild::GuildUserBasics *>(DM_GET_GUILD_MODEL->getGuildUser()->objectAtIndex(i));
        if(m_pModel->isAssistUser(user->getUserId())==false &&m_pModel->isSeleteUser(user->getUserId())==false){
            if(user->getUserId()!=DM_GET_USER_MODEL->getUserInfo()->getUserId())
                m_pList->addObject(user);
        }
    }
    m_pTableView->reloadData();
}


bool KingTombGuildList::onAssignCCBMemberVariable(CCObject *pTarget,const char *pMemberVariableName, CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleTf", CCLabelTTF *, m_pTitleTf);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableContainer", CCNode *, m_pTableContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
    return false;
}

CCTableViewCell* KingTombGuildList::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	KingTombGuildListCell *pCell = (KingTombGuildListCell *)table->dequeueCell();
	if (!pCell)
	{
		pCell = KingTombGuildListCell::create();
		pCell-> setDelegate(this);
	}
    
	pCell->setData(static_cast<guild::GuildUserBasics *>(m_pList->objectAtIndex(idx)));
	return pCell;
}

uint32_t KingTombGuildList::numberOfCellsInTableView(CCTableView *table)
{
    return m_pList->count();
}


CCSize KingTombGuildList::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCellSize;
}

void KingTombGuildList::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}


//关闭按钮回调
void KingTombGuildList::closeCallBack(cocos2d::CCObject *sender){
    closePopup();
    this->m_pDelegate->openFightPanel();
}


void KingTombGuildList::onBackKey(){
    m_pModel->removeSeleteUse();
    DPopup::onBackKey();
}



//选择 组站 人
void KingTombGuildList::guildPlaySelete(guild::GuildUserBasics *pdata){
    m_pModel->addSeleteUse(pdata,m_pIndex);
    closePopup();
    this->m_pDelegate->openFightPanel();
}

void KingTombGuildList::onEnter()
{
    DPopup::onEnter();
	refresh();
}

void KingTombGuildList::onExit()
{
	DPopup::onExit();
}



