//
//  Signin.cpp
//  QSMY
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "Signin.h"
#include "SigninCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

Signin::Signin()
	:	m_pCloseButton(NULL)
	,	m_pCell(NULL)
	,	m_pTableContainer(NULL)
	,	m_pArr(NULL)
	,	m_pModel(NULL)
	,	m_pTableView(NULL)
	,	m_uTodayIdx(-1)
	,m_pSignItem0(NULL)
	,m_pSignItem1(NULL)
	,m_pThree(NULL)
	,m_pSeven(NULL)
{
    
}

Signin::~Signin()
{
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pTableContainer);
	CC_SAFE_RELEASE(m_pArr);
	CC_SAFE_RELEASE(m_pTableView);
	CC_SAFE_RELEASE(m_pSignItem0);
	CC_SAFE_RELEASE(m_pSignItem1);
	CC_SAFE_RELEASE(m_pThree);
	CC_SAFE_RELEASE(m_pSeven);
}

bool Signin::init()
{
    if(!DPopup::init(true)) return false;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("ItemBox", ItemBoxLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHeadGradeBg", WarriorHeadGradeBgLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_qiandao/PL_qiandao", pNodeLoaderLibrary);
	CCB_READER_BY_CUSTOM_LOADER("PL_ui/ccb/pl_qiandao/PL_qiandao_cell", pNodeLoaderLibrary , m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pCloseButton->getButton()->setTarget(this, menu_selector(Signin::closeCallBack));


	m_pModel = DM_GET_SIGNIN_MODEL;


	m_pTableView = DTableView::create(this, m_pTableContainer->getContentSize());
	CC_SAFE_RETAIN(m_pTableView);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableContainer->addChild(m_pTableView);
    return true;
}

void Signin::onEnter()
{
	DPopup::onEnter();
	BINDING_ONLY(m_pModel, this, Signin::updateView, D_EVENT_SIGNIN_UPDATE);
	if (m_pModel->isSynced == false)
	{
		HTTP_CLIENT->signinGet();
	}
	else
	{
		updateView();
	}
	
	//DPopup::onEnter();


	
}

void Signin::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DPopup::onExit();
}

void Signin::updateView()
{
	CCArray *pAward = m_pModel->getSigninAward();
	CC_SAFE_RETAIN(pAward);
	CC_SAFE_RELEASE(m_pArr);
	m_pArr = pAward;
	if (m_pModel->isSignedToday())
	{
		m_uTodayIdx = (m_pModel->getDay() == 0) ? 6 : m_pModel->getDay() - 1;
	}
	else
	{
		m_uTodayIdx = m_pModel->getDay();
	}
	m_pTableView->reloadData();

	//顶部显示
	signin::SigninAward *award = NULL;
	if(m_uTodayIdx <= 1 || m_uTodayIdx == 6)
	{
		//显示3天奖励
		award = (signin::SigninAward *)m_pArr->objectAtIndex(2);
		m_pThree->setVisible(true);
		m_pSeven->setVisible(false);
		m_pSignItem0->updateSigninItem(award , 0);
		m_pSignItem1->updateSigninItem(award , 1);
	}
	else{
		//显示7天奖励
		award = (signin::SigninAward *)m_pArr->objectAtIndex(6);
		m_pThree->setVisible(false);
		m_pSeven->setVisible(true);
		m_pSignItem0->updateSigninItem(award , 0);
		m_pSignItem1->updateSigninItem(award , 1);
	}

	//自动签到
	if(!DM_GET_SIGNIN_MODEL->isSignedToday())
	{
		//处理第七天不显示
		if(m_uTodayIdx >= 6)
		{
			m_pTableView->setContentOffset(m_pTableView->maxContainerOffset());
		}
		//动画播放前，把当天的条目清空
		SigninCell* todayCell = dynamic_cast<SigninCell*>(m_pTableView->cellAtIndex(m_uTodayIdx));
		todayCell->setClear();
		doSignin(NULL);
	}
}

bool Signin::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableContainer", CCNode *, m_pTableContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignItem0", ItemBox *, m_pSignItem0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignItem1", ItemBox *, m_pSignItem1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pThree", CCNode *, m_pThree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSeven", CCNode *, m_pSeven);
    return false;
}

CCSize Signin::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

CCTableViewCell *Signin::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	SigninCell *pCell = (SigninCell *)(table->dequeueCell());
	if (!pCell)
	{
		pCell = SigninCell::create();
	}
	pCell->setDelegate(this);
	pCell->setData((signin::SigninAward *)m_pArr->objectAtIndex(idx));
	pCell->setSigned(m_pModel->getDay() > idx);	
	pCell->setTomorrow(m_uTodayIdx + 1 == idx);
	pCell->setTodaySigned(m_uTodayIdx == idx && m_pModel->getDay() > idx);
	return pCell;
}

unsigned int Signin::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pArr)
	{
		return 0;
	}
	
	return m_pArr->count();
}

void Signin::closeCallBack(CCObject* obj)
{
	closePopup();
	//Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Signin::doSignin(SigninCell *pCell)
{
	HTTP_CLIENT->signinSignin(this, callfuncJson_selector(Signin::signinCallBack));
	//SigninCell* todayCell = dynamic_cast<SigninCell*>(m_pTableView->cellAtIndex(m_uTodayIdx));
	//todayCell->play();
}

void Signin::signinCallBack(const Json::Value &request, const Json::Value &response)
{
	if(response["code"].asString().compare(SUC_CODE)!=0) return;
	if(!isRunning()) return;
	//签到成功
	SigninCell* todayCell = dynamic_cast<SigninCell*>(m_pTableView->cellAtIndex(m_uTodayIdx));
	todayCell->play();
	//closePopup();
}
