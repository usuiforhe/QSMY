//
//  Mail.cpp
//  邮件
//
//  Created by cll on 13-7-18.
//
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Mail.h"
#include "MailListView.h"
#include "../../../managers/GameManager.h"
#include "../Game.h"
#include "../user/userInfo.h"

USING_NS_CC;
using namespace navigation;


/************************ mail class begin*****************************/

Mail::Mail()
//:m_pMailListView(NULL)
:m_pContainer(NULL)
,m_pDelegate(NULL)
,m_pMailModel(NULL)
,m_pReciveAllItem(NULL)
,m_pCloseButton(NULL)
,m_pDTabView(NULL)
,m_pBtnBox(NULL)
,m_pNewMailBg(NULL)
,m_pNewMailAttBg(NULL)
,m_pNewMailLabel(NULL)
,m_pNewMailAttLabel(NULL)
,m_pNav(NULL)
{
    
}


Mail::~Mail()
{
	//CC_SAFE_RELEASE(m_pMailListView);
	//CC_SAFE_RELEASE(m_pMailAwardListView);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pReciveAllItem);
    CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pDTabView);
	CC_SAFE_RELEASE(m_pBtnBox);
	CC_SAFE_RELEASE(m_pNewMailBg);
	CC_SAFE_RELEASE(m_pNewMailAttBg);
	CC_SAFE_RELEASE(m_pNewMailLabel);
	CC_SAFE_RELEASE(m_pNewMailAttLabel);
	CC_SAFE_RELEASE(m_pNav);
}



Mail* Mail::create(NavigationDelegate* pDelegate)
{
	Mail *pRet = new Mail();
	if (pRet && pRet->init(pDelegate))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}


bool Mail::init(NavigationDelegate* pDelegate)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pDelegate = pDelegate;
	m_pMailModel= DM_GET_MAIL_MODEL;


	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_youjian/pl_youjian");

	//m_pMailListView = MailListView::create();
	//m_pMailListView ->retain();
	//m_pContainer->addChild(m_pMailListView);

	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	this->addChild(UserInfoView::sharedUserInfoView());
    m_pCloseButton->getButton()->setTarget(this, menu_selector(Mail::closeCallBack));

	m_pReciveAllItem->setTarget(this, menu_selector(Mail::reciveAllButtonCallBack));

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height + 21, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),12);
	m_pDTabView->retain();
	m_pDTabView->setTopMenuisLeftAlignment(true);
	m_pDTabView->setDelegate(this);

	std::string tabName[mail::kMailTagCount] = {"youjian_btn_txt_jiangli","youjian_btn_txt_shijian"};
	std::string selectImg[2] = {"all_btn_fenye_lv","all_btn_fenye_lan"};
	std::string normalImg[2] = {"all_btn_fenye_lv_up","all_btn_fenye_lan_up"};
	for(unsigned int i=0;i<mail::kMailTagCount;i++)
	{
		MailListView* listView = MailListView::create((mail::MailTag)i,PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
		m_pDTabView->addTab(listView, "", CCString::createWithFormat("PL_ui/all/%s.png",normalImg[i].c_str())->getCString(), 
			CCString::createWithFormat("PL_ui/all/%s.png",selectImg[i].c_str())->getCString(), 
			NULL, CCString::createWithFormat("PL_ui/youjian/%s.png", tabName[i].c_str())->getCString());
	}

	m_pNav->addChild(m_pDTabView);
	return true;
}

void Mail::SelectTabAtIndex(DTabView* tabView, uint32_t idx)
{
	switch (idx)
	{
	case mail::kMailTagGain:
		{
			m_pBtnBox->setVisible(true);
			break;
		}
	case mail::kMailTagEvent:
		{
			m_pBtnBox->setVisible(false);
			break;
		}
	}
}

void Mail::updateView()
{
	//有新邮件时请求邮件列表
	if(!m_pMailModel->isSynced) getNewMails();
	//updateNew();

	onUpdateNewMailCount();
}

void Mail::updateNew()
{
	/*m_pNodeNewFriend->setVisible(m_pMailModel->getFriendNewNum() > 0);
	m_pNodeNewSystem->setVisible(m_pMailModel->getSystemNewNum() > 0);*/

	onUpdateNewMailCount();
}

void Mail::getNewMails()
{
	HTTP_CLIENT->getMails();
}

void Mail::onEnter()
{
	BINDING_EXEC(m_pMailModel, this, Mail::updateView, D_EVENT_MAIL_NEW_COUNT_SYNC);
	BINDING_EXEC(m_pMailModel, this, Mail::updateNew, D_EVENT_MAIL_NEW_COUNT_UPDATE);

	DLayer::onEnter();
}

void Mail::onExit()
{
	//m_pMailModel->setAllMailsReaded();	//视为邮件都已读
	m_pMailModel->setAllFriendMailsReaded();

	UNBINDING(m_pMailModel, this, D_EVENT_MAIL_NEW_COUNT_SYNC);
	UNBINDING(m_pMailModel, this, D_EVENT_MAIL_NEW_COUNT_UPDATE);
	DLayer::onExit();
}

bool Mail::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",CCNode *,	m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReciveAllItem", CCMenuItemImage*, m_pReciveAllItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton*, m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBox" , CCNode*,m_pBtnBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailBg" ,CCNode*, m_pNewMailBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailAttBg" ,CCNode*, m_pNewMailAttBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailLabel" ,CCLabelBMFont*, m_pNewMailLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNewMailAttLabel" ,CCLabelBMFont*, m_pNewMailAttLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNav" ,CCNode*, m_pNav);
	return false;
}

SEL_MenuHandler Mail::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}


void Mail::closeCallBack(cocos2d::CCObject* obj)
{
	 if(m_pDelegate) m_pDelegate->NavigateTo(navigation::kNavHome);
}

void Mail::callBackAll(CCObject *pSender)
{

}

void Mail::callBackSwitch(CCObject *pSender)
{
	/*m_pBtnSystem->unselected();
	m_pBtnUser->unselected();
	CCMenuItem *pItem = static_cast<CCMenuItem *>(pSender);
	pItem->selected();
	m_pMailListView->setType(pItem->getTag());*/
}

void Mail::reciveAllButtonCallBack( CCObject *pSender )
{
	HTTP_CLIENT->getAllMailsItem(this, callfuncJson_selector(Mail::reciveAllRequestCallBack));
}

void Mail::reciveAllRequestCallBack( const Json::Value &requestData, const Json::Value &responseData )
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	//领取成功
	//m_pMailListView->updateMailList();
	//m_pReciveAllItem->setEnabled(false);
}

void Mail::onUpdateNewMailCount()
{
	//显示新邮件数(仅当大于零时显示)
	int newMailsNum = m_pMailModel->getSystemNewNum();
	m_pNewMailBg->setVisible(newMailsNum>0);
	if(newMailsNum>0) m_pNewMailLabel->setString(D_CSTRING_FROM_INT(newMailsNum));

	int newMailsAttNum = m_pMailModel->getNormalAttNum();
	m_pNewMailAttBg->setVisible(newMailsAttNum>0);
	if(newMailsAttNum>0) m_pNewMailAttLabel->setString(D_CSTRING_FROM_INT(newMailsAttNum));
}
