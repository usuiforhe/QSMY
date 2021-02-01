//
//  RechargeView.cpp
//  充值列表
//
//  Created by cll on 13-6-24.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "RechargeView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "Account.h"
#include "managers/PlatformManager.h"


USING_NS_CC;
USING_NS_CC_EXT;

#define  RECHARGE_CELL_SIZE		CCSizeMake(640, 270)


/************************ recharge select class begin*******************/

RechargeView::RechargeView()
:m_pListView(NULL)
,m_pDelegate(NULL)
,m_pContainer(NULL)
,m_pViewVipButton(NULL)
,m_pCellData(NULL)
,m_pUserModel(NULL)
,m_pVipLevelCurrent(NULL)
,m_pRechargeInfos(NULL)
,m_pCloseButton(NULL)
,m_pPBVip(NULL)
,m_pVipTips(NULL)
,m_pBMFNextVip(NULL)
,m_pNodeNextVip(NULL)
,m_pNodeMaxVip(NULL)
,m_pCell(NULL)
,m_pExp(NULL)
,m_pExpInfoNode(NULL)
,m_pExpCoin(NULL)
,m_pExpLevel(NULL)
,m_pTitleNode(NULL)
,m_pNodeFirst(NULL)
{
    D_RESET_C_ARRAY(m_pBox, VIP_BOX_MAX);
	D_RESET_C_ARRAY(m_pItemBox, RECHARGE_VIEW_FIRST_RECHARGE_BOX);
}


RechargeView::~RechargeView()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pViewVipButton);
    CC_SAFE_RELEASE(m_pCellData);
    
    CC_SAFE_RELEASE(m_pVipLevelCurrent);
    CC_SAFE_RELEASE(m_pRechargeInfos);
    CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pPBVip);
	CC_SAFE_RELEASE(m_pVipTips);
	CC_SAFE_RELEASE(m_pBMFNextVip);
	CC_SAFE_RELEASE(m_pNodeNextVip);
	CC_SAFE_RELEASE(m_pNodeMaxVip);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pExp);

	CC_SAFE_RELEASE(m_pExpInfoNode);
	CC_SAFE_RELEASE(m_pExpCoin);
	CC_SAFE_RELEASE(m_pExpLevel);
	CC_SAFE_RELEASE(m_pTitleNode);
	CC_SAFE_RELEASE(m_pNodeFirst);
	D_SAFE_RELEASE_C_ARRAY(m_pBox, VIP_BOX_MAX);
	D_SAFE_RELEASE_C_ARRAY(m_pItemBox, RECHARGE_VIEW_FIRST_RECHARGE_BOX);
}


bool RechargeView::init()
{
	if(!DLayer::init())
	{
		return false;
	}
    m_pUserModel = DM_GET_USER_MODEL;
    
    m_pRechargeInfos = CCArray::create();
    CC_SAFE_RETAIN(m_pRechargeInfos);
    
    m_pCellData = readCCData("PL_ui/ccb/pl_chongzhi/pl_chongzhi_cell");
    CC_SAFE_RETAIN(m_pCellData);
    
	CCB_READER_DATA(m_pCellData, m_pCell);
	CC_SAFE_RETAIN(m_pCell);

    CCArray* pTemp = NULL;
    for(int i=0;i<DM_GET_PLATFORM_MODEL->getRechargeList()->count();++i)
    {
        m_pRechargeInfos->addObject(DM_GET_PLATFORM_MODEL->getRechargeList()->objectAtIndex(i));
    }
    
    loadUI();
    
	

    m_pListView = DTableView::create(this,PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
    
	m_pContainer->addChild(m_pListView);
     m_pViewVipButton->getButton()->setTarget(this, menu_selector(RechargeView::viewVipCallBack));
     m_pCloseButton->getButton()->setTarget(this, menu_selector(RechargeView::closeCallBack));
	return true;
}

void RechargeView::loadUI()
{
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_chongzhi/pl_chongzhi");
}

bool RechargeView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFVip", CCLabelBMFont *, this->m_pVipLevelCurrent);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnVip", DButton *, this->m_pViewVipButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnClose", DButton *, this->m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBVip", DProgressBar *, this->m_pPBVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipTips", CCLabelTTF *, this->m_pVipTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFNextVip", CCLabelBMFont *, this->m_pBMFNextVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNextVip", CCNode *, this->m_pNodeNextVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeMaxVip", CCNode *, this->m_pNodeMaxVip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelTTF *, this->m_pExp);	

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpInfoNode", CCNode *, this->m_pExpInfoNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpCoin", CCLabelTTF *, this->m_pExpCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExpLevel", CCLabelTTF *, this->m_pExpLevel);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleNode", CCNode *, this->m_pTitleNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFirst", CCNode *, this->m_pNodeFirst);
	
	D_CCB_ASSIGN_ARRAY("m_pBox", AwardItemSmall *, m_pBox, VIP_BOX_MAX);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pItemBox", ItemBox *, m_pItemBox, RECHARGE_VIEW_FIRST_RECHARGE_BOX);
    
    return false;
}

SEL_MenuHandler RechargeView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", RechargeView::closeCallBack);
    return NULL;
}


void RechargeView::onEnter()
{
	NotificationCenter::sharedNotificationCenter()->addObserver(this,notification_callfuncO_selector(RechargeView::updateAfterRecharge),D_EVENT_USER_CHARGE_SUC);
    BINDING_EXEC(m_pUserModel->getUserInfo(), this, RechargeView::updateVipInfo, D_EVENT_USERINFO_UPDATE);
	updateRechargeView();
    DLayer::onEnter();
}

void RechargeView::onExit()
{
	NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    UNBINDING_ALL(m_pUserModel->getUserInfo(), this);
	DLayer::onExit();
}

void RechargeView::updateAfterRecharge(CCObject *obj)
{
	updateVipInfo();
	updateRechargeView();
}

void RechargeView::updateVipInfo()
{
	//uint8_t nextVipLv = (m_pUserModel->getUserInfo()->getVipLevel()==0)?m_pUserModel->getUserInfo()->getVipLevel()+2:m_pUserModel->getUserInfo()->getVipLevel()+1;
	
	uint8_t nextVipLv = m_pUserModel->getUserInfo()->getVipLevel()+1;
	m_pVipLevelCurrent->setString(CCString::createWithFormat("%d",m_pUserModel->getUserInfo()->getVipLevel())->getCString());
	m_pBMFNextVip->setString(D_CSTRING_FROM_UINT(nextVipLv));

    if(m_pUserModel->getUserInfo()->getVipLevel()>=m_pUserModel->getMaxVipLevel())
    {
		m_pExpInfoNode->setVisible(false);
		m_pVipTips->setVisible(true);
        m_pVipTips->setString(D_LOCAL_STRING("VipMaxTip").c_str());
		m_pNodeMaxVip->setVisible(true);
		m_pNodeNextVip->setVisible(false);
		m_pTitleNode->setVisible(false);
    }else
	{
		//正确处理title
		m_pTitleNode->setVisible(true);

		CCArray *m_pVipInfos = DM_GET_USER_MODEL->getVipInfos();
		user::VipInfo* nextVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLv-1));
		user::VipInfo* nowVipInfo = NULL;
		if(nextVipLv != 1)
		{
			nowVipInfo = static_cast<user::VipInfo*>(m_pVipInfos->objectAtIndex(nextVipLv-2));
		}


		m_pNodeMaxVip->setVisible(false);
		m_pNodeNextVip->setVisible(true);
		//m_pVipTips->setString(D_LOCAL_STRING("Need%dToVip%d", m_pUserModel->getToVipLevelCost(nextVipLv), nextVipLv).c_str());
		m_pExpCoin->setString(CCString::createWithFormat("%d" , m_pUserModel->getToVipLevelCost(nextVipLv))->getCString());
		m_pExpLevel->setString(CCString::createWithFormat("%d" , nextVipLv)->getCString());

		user::VipInfo *pVip = DM_GET_USER_MODEL->getVipInfoByVipLevel(nextVipLv);
		CCArray *pPkg = DM_GET_ITEM_MODEL->getVipPkgInfo(pVip->getVipItemId());
        for (int i = 0; i < pPkg->count() && i < VIP_BOX_MAX; i++)
        {
			drop::Drop *pDrop = (drop::Drop *)pPkg->objectAtIndex(i);
			m_pBox[i]->updateView(pDrop);
        }

		//顯示本機別以上的進度
		int pointDetla = (nextVipLv==1) ? nextVipInfo->getPoint() : nextVipInfo->getPoint() - nowVipInfo->getPoint();//兩級相差多少元寶
		int pointHadDetla = (nextVipLv==1) ? m_pUserModel->getUserInfo()->getAmount() : m_pUserModel->getUserInfo()->getAmount() - nowVipInfo->getPoint();//在這個級別上沖了多少元寶
		if(pointHadDetla < 0) pointHadDetla = 0;
		m_pExp->setString(CCString::createWithFormat("%d/%d" , pointHadDetla , pointDetla)->getCString());	
		m_pPBVip->setPercent(0.0f);
		m_pPBVip->setPercent((float)pointHadDetla/pointDetla,0.5);
	}
	
	//首充判断
	if(!DM_GET_USER_MODEL->getUserInfo()->isPayed())
	{		
		showFirstRecharge();
	}
	else
	{
		//隐藏掉首充内容
		m_pNodeFirst->setVisible(false);
	}
}

void RechargeView::showFirstRecharge()
{
	//显示隐藏
	m_pNodeFirst->setVisible(true);
	m_pTitleNode->setVisible(false);
	m_pNodeNextVip->setVisible(false);

	//处理首充送的东西
	for(uint8_t i = 0 ; i < RECHARGE_VIEW_FIRST_RECHARGE_BOX ; i++ )
	{
		user::FirstPunchInfo* pInfo = (user::FirstPunchInfo*)(DM_GET_USER_MODEL->getFirstPunchInfos()->objectAtIndex(i));
		if(pInfo->getType() == drop::kDropTypeItem)
		{
			m_pItemBox[i]->updateDropItem(drop::Drop::create(item::ItemInfo::create(pInfo->getID(), pInfo->getNum())));
		}
		else if(pInfo->getType() == drop::kDropTypeEquip){
			m_pItemBox[i]->updateDropItem(drop::Drop::create(equip::EquipCurInfo::create(pInfo->getID())));
		}
	}
}

void RechargeView::updateRechargeView()
{	
	//更新列表
	m_pListView->reloadData();
}


void RechargeView::viewVipCallBack(cocos2d::CCObject * obj)
{
	if(m_pDelegate) m_pDelegate->showInfo(kAccountTypeVipView);
}

void RechargeView::closeCallBack(cocos2d::CCObject * obj)
{
	if(m_pDelegate) m_pDelegate->closeInfo();
}

void RechargeView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //CCLOG("recharge cell touched at index: %i", cell->getIdx());
}

CCSize RechargeView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
    
}

CCTableViewCell* RechargeView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	
	RechargeViewCell *cell = (RechargeViewCell*)table->dequeueCell();
    
	if (!cell)
	{
		cell = RechargeViewCell::create(m_pCellData);
	}
    
    cell->setDelegate(m_pDelegate);

    cell->setRechargeInfo(static_cast<platform::RechargeInfo*>(m_pRechargeInfos->objectAtIndex(idx)));
	
    return cell;
}

uint32_t RechargeView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pRechargeInfos->count();
}

/************************ recharge select class end*******************/


void RechargeFirstView::loadUI()
{
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_chongzhi/pl_chongzhi");
 //   CCB_READER_AND_ADDCHILD("ui/ccb/account/rechargeFirst");
}


/************************ recharge cell class begin**********************/

RechargeViewCell::RechargeViewCell()
	:m_pDelegate(NULL)
	, m_pBMFPoint(NULL)
	, m_pBMFAddition(NULL)
	, m_pBMFPrice(NULL)
	, m_pDBtnCharge(NULL)
	, m_pRechargeInfo(NULL)
	, m_pMonthCard(NULL)
	, m_pNoMonthCard(NULL)
	, m_pMonthCardPrice(NULL)
	, m_pIconContainer(NULL)
	, m_pMCFirst(NULL)
	, m_pMCDay(NULL)
	, m_pMCTimes(NULL)
{
}

RechargeViewCell::~RechargeViewCell()
{
	CC_SAFE_RELEASE(m_pMCFirst);
	CC_SAFE_RELEASE(m_pMCDay);
	CC_SAFE_RELEASE(m_pMCTimes);
	CC_SAFE_RELEASE(m_pBMFPoint);
	CC_SAFE_RELEASE(m_pBMFAddition);
	CC_SAFE_RELEASE(m_pBMFPrice);
	CC_SAFE_RELEASE(m_pDBtnCharge);
	CC_SAFE_RELEASE(m_pRechargeInfo);
	CC_SAFE_RELEASE(m_pMonthCard);
	CC_SAFE_RELEASE(m_pNoMonthCard);
	CC_SAFE_RELEASE(m_pMonthCardPrice);
	CC_SAFE_RELEASE(m_pIconContainer);
}

RechargeViewCell* RechargeViewCell::create(cocos2d::extension::CCData *pData)
{
    RechargeViewCell* pRet = new RechargeViewCell();
    if(pRet&& pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}


bool RechargeViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
	m_pDBtnCharge->getButton()->setTarget(this, menu_selector(RechargeViewCell::callBackCharge));
	return true;
}

bool RechargeViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPoint", CCLabelBMFont *, m_pBMFPoint);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFAddition", CCLabelBMFont *, m_pBMFAddition);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFPrice", CCLabelBMFont *, m_pBMFPrice);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCharge", DButton *, m_pDBtnCharge);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMonthCard", CCNode *, m_pMonthCard);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoMonthCard", CCNode *, m_pNoMonthCard);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMonthCardPrice", CCLabelBMFont *, m_pMonthCardPrice);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode *, m_pIconContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMCFirst", CCLabelBMFont *, m_pMCFirst);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMCDay", CCLabelBMFont *, m_pMCDay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMCTimes", CCLabelBMFont *, m_pMCTimes);
    return false;
}

void RechargeViewCell::setRechargeInfo(platform::RechargeInfo* pInfo)
{
	CC_SAFE_RETAIN(pInfo);
	CC_SAFE_RELEASE(m_pRechargeInfo);
	m_pRechargeInfo = pInfo;

	m_pIconContainer->removeAllChildren();
	m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getRechargeIcon(m_pRechargeInfo->getResId()));

	if(pInfo->getMonthCardId() == 0){
		m_pMonthCard->setVisible(false);
		m_pNoMonthCard->setVisible(true);
		m_pBMFPoint->setString(D_CSTRING_FROM_UINT(pInfo->getShowPoint()));
		//显示送多少
		DM_GET_USER_MODEL->getUserInfo()->isPayed() ? m_pBMFAddition->setString(D_CSTRING_FROM_UINT(pInfo->getShowExt())) : m_pBMFAddition->setString(D_CSTRING_FROM_UINT(pInfo->getShowExtFirst()));
		
		m_pBMFPrice->setString(pInfo->getPrice().c_str());
	}
	else{
		m_pMonthCard->setVisible(true);
		m_pNoMonthCard->setVisible(false);
		m_pMonthCardPrice->setString(pInfo->getPrice().c_str());
		m_pBMFPrice->setString(pInfo->getPrice().c_str());
		//月卡新增信息
		DM_GET_USER_MODEL->getUserInfo()->isPayed() ? m_pMCFirst->setString(D_CSTRING_FROM_UINT(pInfo->getMCPoint())) : m_pMCFirst->setString(D_CSTRING_FROM_UINT(pInfo->getMCPointFirst()));
		m_pMCTimes->setString(D_CSTRING_FROM_UINT(pInfo->getMCTimes()));
		m_pMCDay->setString(D_CSTRING_FROM_UINT(pInfo->getMCDay()));
	}
	
}

void RechargeViewCell::updateCellView()
{
}

void RechargeViewCell::callBackCharge(CCObject *pObj)
{
	if(m_pDelegate) m_pDelegate->recharge(m_pRechargeInfo);
}

/************************ recharge cell class end**********************/




