//
//  MailListView.cpp
//  邮件列表
//
//  Created by cll on 13-7-18
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MailListView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "model/DropModel.h"
#include "../common/AwardListView.h"
#include "../Game.h"

USING_NS_CC;
USING_NS_CC_EXT;


#define  MAIL_LIST_SIZE    (CCSizeMake(622, 743))
#define  MAIL_CELL_SIZE    (CCSizeMake(612, 176))
#define  MAIL_ATTACHMENT_CELL_SIZE	(CCSizeMake(612, 266))
#define	 MAIL_CELL_MENU_HEIGHT	60
#define  MAIL_CELL_BOTTOM_HEIGHT	60
#define	 MAIL_CELL_SPACE_HEIGHT	10
//带附件的邮件框原始大小
#define  CELL_SIZE    (CCSizeMake(612, 266))
//文本原始大小
#define  CONTENT_SIZE    (CCSizeMake(390, 75))
//文本背景原始大小
#define  CONTENT_BG_SIZE    (CCSizeMake(419, 91))

//无附件的邮件框原始大小
#define  CELL_SIZE_SMALL    (CCSizeMake(612, 176))
//文本原始大小
#define  CONTENT_SIZE_SMALL    (CCSizeMake(490, 75))
//文本背景原始大小
#define  CONTENT_BG_SIZE_SMALL    (CCSizeMake(539, 91))

/************************ mail select class begin*******************/



MailListView::MailListView()
:m_pListView(NULL)
,m_pMailModel(NULL)
,m_pMailList(NULL)
,m_pCCData(NULL)
,m_pAttachmentCCData(NULL)
,m_pItemCCData(NULL)
,m_pContainer(NULL)
,m_uType(BTN_TAG_SYS)
{
};

MailListView::~MailListView()
{
    CC_SAFE_RELEASE(m_pCCData);
	CC_SAFE_RELEASE(m_pAttachmentCCData);
	CC_SAFE_RELEASE(m_pItemCCData);
	//CC_SAFE_RELEASE(m_pContainer);
}

MailListView* MailListView::create(mail::MailTag type, const cocos2d::CCSize& size)
{
	MailListView* pRet = new MailListView();

	if (pRet && pRet->init(type, size))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool MailListView::init(mail::MailTag type, const cocos2d::CCSize& size)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pMailModel = DM_GET_MAIL_MODEL;
    m_uTag = type;
    m_pCCData = readCCData("PL_ui/ccb/pl_youjian/pl_youjian_shijian");
	m_pAttachmentCCData = readCCData("PL_ui/ccb/pl_youjian/pl_youjian_jiangli");
	m_pItemCCData = readCCData("PL_ui/ccb/pl_common/ItemBox99p");

    CC_SAFE_RETAIN(m_pCCData);
    CC_SAFE_RETAIN(m_pAttachmentCCData);
	CC_SAFE_RETAIN(m_pItemCCData);
    m_pContainer = CCNode::create();
	addChild(m_pContainer);

	//m_pListView = DTableView::create(this, MAIL_LIST_SIZE);
	m_pListView = DTableView::create(this, size);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);


	m_pContainer->addChild(m_pListView);
    
	return true;
    
}

void MailListView::onEnter()
{
    DLayer::onEnter();
	BINDING_EXEC(m_pMailModel,this,MailListView::updateMailList,D_EVENT_MAILLIST_UPDATE);
}

void MailListView::onExit()
{
	UNBINDING(m_pMailModel,this, D_EVENT_MAILLIST_UPDATE);
	DLayer::onExit();
}


void MailListView::updateMailList()
{
    if(!m_pMailModel->isSynced) return;

	//m_pMailList = m_pMailModel->getAllMailList();
	//m_pMailList = m_pMailModel->getUserMailList();

	/*switch (m_uType)
	{
	case BTN_TAG_USER:
		m_pMailList = m_pMailModel->getUserMailList();
		break;
	case BTN_TAG_SYS:
	default:
		m_pMailList = m_pMailModel->getSysMailList();
		break;
	}*/
	m_pMailList = m_pMailModel->getMaillGain();
	switch(m_uTag)
	{
	case mail::kMailTagGain:
		m_pMailList = m_pMailModel->getMaillGain();
		break;
	case mail::kMailTagEvent:
		m_pMailModel->deleteEventMail();
		m_pMailList = m_pMailModel->getMaillEvent();
		break;
	default:
		break;
	}

	m_pListView->reloadData();
	m_pListView->setContentOffset(m_pListView->minContainerOffset());
}


void MailListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}



CCSize MailListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	mail::MailInfo *pMail =  (mail::MailInfo*)(m_pMailList->objectAtIndex(idx));

	//计算cellsize
	CCLabelTTF* pLabel = CCLabelTTF::create();
	pLabel->setFontSize(18);
	pLabel->setFontName(D_FONT_HELVETICA);

	if (pMail && pMail->checkHasAtt() == true)
	{
		//return MAIL_ATTACHMENT_CELL_SIZE;
		//计算cellsize
		pLabel->setDimensions(CCSizeMake(CONTENT_SIZE.width, 0));
		pLabel->setString(pMail->content.c_str());
		float delta = pLabel->getContentSize().height - CONTENT_SIZE.height;
		pMail->setHeight(MAX(delta , 0.0f));

		return CCSizeMake(CELL_SIZE.width , CELL_SIZE.height + pMail->getHeight() + MAIL_CELL_SPACE_HEIGHT);
	}
	else
	{
		//return MAIL_CELL_SIZE;
		//计算cellsize
		pLabel->setDimensions(CCSizeMake(CONTENT_SIZE_SMALL.width, 0));
		pLabel->setString(pMail->content.c_str());
		float delta = pLabel->getContentSize().height - CONTENT_SIZE_SMALL.height;
		pMail->setHeight(MAX(delta , 0.0f));

		return CCSizeMake(CELL_SIZE_SMALL.width , CELL_SIZE_SMALL.height + pMail->getHeight() + MAIL_CELL_SPACE_HEIGHT);
	}

	/*if(m_sizeMap.find(pMail->id)==m_sizeMap.end())
	{
		MailListViewCell * pCell = MailListViewCell::create(m_pCCData);
		pCell->setMailInfo(pMail);
		m_sizeMap.insert(std::make_pair(pMail->id, CCSizeMake(MAIL_CELL_SIZE.width, pCell->getHeight() + MAIL_CELL_SPACE_HEIGHT)));
	}
	return m_sizeMap[pMail->id];*/
}

CCTableViewCell* MailListView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	
	MailListViewCell *cell = (MailListViewCell*)table->dequeueCell();
	mail::MailInfo *pMail =  (mail::MailInfo*)(m_pMailList->objectAtIndex(idx));
	if (cell)
	{
		if (cell->getMailInfo() == NULL || cell->getMailInfo()->checkHasAtt() != pMail->checkHasAtt())
		{
			CC_SAFE_RELEASE_NULL(cell);
		}
	}
	
	if (!cell)
	{
		if (pMail->checkHasAtt() == true)
		{
			cell = MailListViewCell::create(m_pAttachmentCCData, m_pItemCCData);
		}
		else
		{
			cell = MailListViewCell::create(m_pCCData, m_pItemCCData);
		}
	}

	cell->setMailInfo(pMail);
	cell->setDelegate(this);
	return cell;
}

uint32_t MailListView::numberOfCellsInTableView(CCTableView *table)
{
	if (m_pMailList == NULL)
	{
		return 0;
	}
	
	return m_pMailList?m_pMailList->count():0;
}

void MailListView::onAwardBut(cocos2d::CCObject *sender)
{
    CCMenuItem * pMenuItem = (CCMenuItem*) sender;
	mail::MailInfo *pMail =  dynamic_cast<mail::MailInfo*>(m_pMailList->objectAtIndex(pMenuItem->getTag()));

	if (pMail)
	{
		HTTP_CLIENT->pickUpMail(pMail->id);
	}
	    
}

void MailListView::setType(uint8_t uType)
{
	if (m_uType == uType)
	{
		return;
	}
	
	m_uType = uType;
	m_sizeMap.clear();
	updateMailList();
}

void MailListView::MailListViewCellPickUp(MailListViewCell *pCell)
{
	HTTP_CLIENT->pickUpMail(pCell->getMailInfo()->id);
}

void MailListView::DeleteMail( MailListViewCell *pCell )
{
	CCArray* pMails = CCArray::create();
	pMails->addObject(CCInteger::create(pCell->getMailInfo()->id));
	HTTP_CLIENT->deleteMail(pMails);
}



/************************ mail select class end*******************/




/************************ mail cell class begin**********************/

MailListViewCell::MailListViewCell()
:m_pMailInfo(NULL)
,m_pMenuItemAward(NULL)
,m_pContentTTF(NULL)
,m_pItemCCBData(NULL)
//,m_pFromUser(NULL)
//,m_pFromSystem(NULL)
//,m_pFromPve(NULL)
//,m_pNodeAttach(NULL)
//,m_pNodeAgreed(NULL)
//,m_pNodeRemoved(NULL)
//,m_pNodeReply(NULL)
//,m_pS9SBack(NULL)
//,m_pBtnAttach(NULL)
//,m_pNodeBottom(NULL)
,m_pAttachButton(NULL)
,m_pConfirmButton(NULL)
,m_pTimeLabel(NULL)
,m_pTableContainer(NULL)
,m_pNode(NULL)
,m_pItemTable(NULL)
,m_fHeight(0.f)
,m_pFateListBg(NULL)
,m_pContentBg(NULL)
,m_pTitleNode(NULL)
{
    
}


MailListViewCell::~MailListViewCell()
{
	//SAFE_RELEASE_UNBINDING_ALL(m_pMailInfo, this);
	UNBINDING(m_pMailInfo, this, D_EVENT_MAIL_UPDATE);
    CC_SAFE_RELEASE(m_pMenuItemAward);
	CC_SAFE_RELEASE(m_pContentTTF);
	CC_SAFE_RELEASE(m_pAttachButton);
	CC_SAFE_RELEASE(m_pConfirmButton);
	CC_SAFE_RELEASE(m_pTimeLabel);
	CC_SAFE_RELEASE(m_pTableContainer);
	CC_SAFE_RELEASE(m_pFateListBg);
	CC_SAFE_RELEASE(m_pContentBg);
	CC_SAFE_RELEASE(m_pTitleNode);
    /*CC_SAFE_RELEASE(m_pFromUser);
    CC_SAFE_RELEASE(m_pFromSystem);
	CC_SAFE_RELEASE(m_pFromPve);
	CC_SAFE_RELEASE(m_pNodeAttach);
	CC_SAFE_RELEASE(m_pNodeAgreed);
	CC_SAFE_RELEASE(m_pNodeRemoved);
	CC_SAFE_RELEASE(m_pNodeReply);
	CC_SAFE_RELEASE(m_pS9SBack);
	CC_SAFE_RELEASE(m_pBtnAttach);
	CC_SAFE_RELEASE(m_pNodeBottom);*/
}


MailListViewCell* MailListViewCell::create(cocos2d::extension::CCData* pData, cocos2d::extension::CCData* pItemCCData)
{
    MailListViewCell* pRet = new MailListViewCell();
    if(pRet && pRet->init(pData, pItemCCData))
    {
        pRet->autorelease();
    }else
    {        
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool MailListViewCell::init(cocos2d::extension::CCData* pData, cocos2d::extension::CCData* pItemCCData)
{
	if (!CCTableViewCell::init())
	{
		return false;
	}
	
	m_pNode = NULL;
	CCB_READER_DATA(pData, m_pNode);
	if(m_pNode) addChild(m_pNode);

	if (m_pTableContainer)
	{
		m_pItemTable = DTableView::create(this, m_pTableContainer->getContentSize());
		m_pItemTable->setDirection(kCCScrollViewDirectionHorizontal);
		m_pItemTable->setDelegate(this);
		m_pItemTable->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_pItemTable->setAnchorPoint(CCPointZero);
		m_pTableContainer->addChild(m_pItemTable);
	}
	
	m_pItemCCBData = pItemCCData;

	if (m_pAttachButton)
	{
		m_pAttachButton->getButton()->setTarget(this, menu_selector(MailListViewCell::menuCallBack));
	}

	if (m_pConfirmButton)
	{
		m_pConfirmButton->getButton()->setTarget(this, menu_selector(MailListViewCell::menuCallBack));
	}
	
	return true;
}

void MailListViewCell::setMailInfo(mail::MailInfo* pMailInfo)
{
	if(pMailInfo == m_pMailInfo) return;
	//SAFE_RELEASE_UNBINDING_ALL(m_pMailInfo, this);
	UNBINDING(m_pMailInfo, this, D_EVENT_MAIL_UPDATE);
	m_pMailInfo = pMailInfo;
   BINDING_EXEC(m_pMailInfo, this, MailListViewCell::updateCellView, D_EVENT_MAIL_UPDATE);
}

bool MailListViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItemAward", CCMenuItem*, this->m_pMenuItemAward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFContent", CCLabelTTF*, this->m_pContentTTF);
	/* CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeUser", CCNode*, this->m_pFromUser);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSystem", CCNode*, this->m_pFromSystem);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePve", CCNode*, this->m_pFromPve);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAttach", CCNode *, m_pNodeAttach);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAgreed", CCNode *, m_pNodeAgreed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRemoved", CCNode *, m_pNodeRemoved);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeReply", CCNode *, m_pNodeReply);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pS9SBack", CCScale9Sprite *, m_pS9SBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAttach", CCMenuItem *, m_pBtnAttach);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBottom", CCNode *, m_pNodeBottom);*/
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttachButton", DButton*, this->m_pAttachButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pConfirmButton", DButton*, this->m_pConfirmButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimeLabel", CCLabelTTF*, this->m_pTimeLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableContainer", CCNode*, this->m_pTableContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateListBg", CCScale9Sprite*, this->m_pFateListBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContentBg", CCScale9Sprite*, this->m_pContentBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitleNode", CCNode*, this->m_pTitleNode);
    return false;
}



SEL_MenuHandler MailListViewCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	/*CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackCenter", MailListViewCell::callBackCenter);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackLeft", MailListViewCell::callBackLeft);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRight", MailListViewCell::callBackRight);*/
	return NULL;
}

void MailListViewCell::updateCellView()
{
	bool bFromUser = m_pMailInfo->mail_type == mail::kMailTypeFromUser || m_pMailInfo->mail_type == mail::kMailTypeFromFriend;
	/* m_pFromUser->setVisible(bFromUser);
	m_pFromSystem->setVisible(!m_pFromUser->isVisible());
	m_pFromPve->setVisible(false);*/
	//领取按钮是否显示
	//m_pMenuItemAward->setEnabled(!m_pMailInfo->checkAttAccepted());

	/*bool hasBottom = m_pMailInfo->checkHasAtt() || bFromUser;

	if (m_pMailInfo->checkHasAtt())
	{
	m_pNodeAttach->setVisible(true);
	m_pNodeAgreed->setVisible(false);
	m_pNodeRemoved->setVisible(false);
	m_pNodeReply->setVisible(false);
	m_pBtnAttach->setEnabled(!m_pMailInfo->checkAttAccepted());
	}
	else
	{
	m_pNodeAttach->setVisible(false);
	m_pNodeAgreed->setVisible(false);
	m_pNodeRemoved->setVisible(false);
	m_pNodeReply->setVisible(bFromUser);
	}*/

	//m_pTimeLabel->setString(TimeUtil::getCompleteTimeString(m_pMailInfo->time));

	m_pContentTTF->setDimensions(CCSizeMake(m_pContentTTF->getDimensions().width, 0));
	m_pContentTTF->setString(m_pMailInfo->content.c_str());
	m_pTimeLabel->setString(TimeUtil::getCompleteTimeString(m_pMailInfo->mail_ts).c_str());

	float contentHeight = m_pContentTTF->getContentSize().height + MAIL_CELL_BOTTOM_HEIGHT;

	//判断附件
	CCSize contentsize = m_pMailInfo->checkHasAtt() ? CONTENT_SIZE : CONTENT_SIZE_SMALL;
	CCSize cellsize = m_pMailInfo->checkHasAtt() ? CELL_SIZE : CELL_SIZE_SMALL;
	CCSize contentbgsize = m_pMailInfo->checkHasAtt() ? CONTENT_BG_SIZE : CONTENT_BG_SIZE_SMALL;

	if(m_pContentTTF->getContentSize().height > contentsize.height)
	{
		//处理需要自适应的情况
		float delta = m_pContentTTF->getContentSize().height - contentsize.height;
		m_pMailInfo->setHeight(delta);
		m_pFateListBg->setPreferredSize(CCSizeMake(cellsize.width , cellsize.height + delta));
		m_pContentBg->setPreferredSize(CCSizeMake(contentbgsize.width , contentbgsize.height + delta));
		m_pTitleNode->setPositionY(delta);
	}
	else{
		m_pContentTTF->setDimensions(contentsize);
		m_pMailInfo->setHeight(0.0f);
		m_pFateListBg->setPreferredSize(cellsize);
		m_pContentBg->setPreferredSize(contentbgsize);
		m_pTitleNode->setPositionY(0.0f);
	}
	
	
	//heightOfCell = 266+m_pContentTTF->getContentSize().height;

	/*if (hasBottom)
	{
	contentHeight += MAIL_CELL_MENU_HEIGHT;
	}*/
	

	/*CCSize sz = m_pS9SBack->getContentSize();
	sz.height = contentHeight;
	m_pS9SBack->setPreferredSize(sz);*/

	/*float yPos =  MAIL_CELL_SIZE.height - contentHeight;
	m_pNodeBottom->setPositionY(yPos);

	m_fHeight = contentHeight;
	m_pNode->setPositionY(-yPos);*/
	if (m_pItemTable)
	{
		m_pItemTable->reloadData();
	}
	
	if (m_pMailInfo && m_pMailInfo->checkHasAtt() && m_pMailInfo->checkAttAccepted())
	{
		if (m_pAttachButton)
		{
			m_pAttachButton->getButton()->setEnabled(false);
		}
	}
	else
	{
		if (m_pAttachButton)
		{
			m_pAttachButton->getButton()->setEnabled(true);
		}
	}

	if (m_pConfirmButton)
	{
		m_pConfirmButton->getButton()->setEnabled(!(m_pMailInfo && m_pMailInfo->is_read));
	}
}

//void MailListViewCell::callBackCenter(CCObject *pSender)
//{
//	if (m_pDelegate)
//	{
//		if (m_pMailInfo->checkHasAtt())
//		{
//			m_pDelegate->MailListViewCellPickUp(this);
//		}
//	}
//	
//}
//void MailListViewCell::callBackLeft(CCObject *pSender)
//{
//	if (m_pMailInfo->mail_type == mail::kMailTypeFromFriend)
//	{
//		HTTP_CLIENT->friendDeleteMail(m_pMailInfo->id);
//	}
//	else if (m_pMailInfo->mail_type == mail::kMailTypeFromUser)
//	{
//		HTTP_CLIENT->deleteMail(m_pMailInfo->id);
//	}
//	
//}
//void MailListViewCell::callBackRight(CCObject *pSender)
//{
//	bool bFromUser = m_pMailInfo->mail_type == mail::kMailTypeFromUser || m_pMailInfo->mail_type == mail::kMailTypeFromFriend;
//	if (bFromUser)
//	{
//		CCDictionary *pDict = CCDictionary::create();
//		pDict->setObject(ccs(m_pMailInfo->send_user), "name");
//		pDict->setObject(CCInteger::create(m_pMailInfo->sender_id), "id");
//		pDict->setObject(CCInteger::create(m_pMailInfo->mail_type), "type");
//		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_EDIT_MESSAGE, pDict);
//	}
//}

void MailListViewCell::attachButtonCallBack( CCObject* pSender )
{
		if (m_pDelegate)
		{
			if (m_pMailInfo->checkHasAtt())
			{
				m_pDelegate->MailListViewCellPickUp(this);
			}
		}
}

void MailListViewCell::confirmButtonCallBack( CCObject* pSender )
{

}

cocos2d::CCSize MailListViewCell::tableCellSizeForIndex( cocos2d::extension::CCTableView *table, unsigned int idx )
{
	return CCSizeMake(120, 80);
}

void MailListViewCell::tableCellTouched( cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell )
{

}

cocos2d::extension::CCTableViewCell* MailListViewCell::tableCellAtIndex( cocos2d::extension::CCTableView *table, uint32_t idx )
{
	AttachmentCell* pCell = (AttachmentCell*)table->dequeueCell();
	if (!pCell)
	{
		pCell = AttachmentCell::create(m_pItemCCBData);
		pCell->setAnchorPoint(CCPointZero);
		pCell->setPosition(CCPointZero);
	}
	
	drop::Drop* pDrop = (drop::Drop*)(m_pMailInfo->arAtt->objectAtIndex(idx));
	if (pDrop && DropModel::getDropCount(pDrop) != 0)
	{
		pCell->setCount(CCString::createWithFormat("x%d", DropModel::getDropCount(pDrop))->getCString());
	}
	else
	{
		pCell->setCount("");
	}
	
	pCell->setIcon(DropModel::getDropIconSprite(pDrop));
	
	return pCell;
}

uint32_t MailListViewCell::numberOfCellsInTableView( cocos2d::extension::CCTableView *table )
{
	if (m_pMailInfo == NULL)
	{
		return 0;
	}
	
	return m_pMailInfo->checkHasAtt() ? m_pMailInfo->arAtt->count() : 0;
}

void MailListViewCell::setButtonTargetAndTag(CCObject* target, SEL_MenuHandler _selector, unsigned int tag )
{
	if (m_pMailInfo)
	{
		if (m_pMailInfo->checkHasAtt() && m_pAttachButton)
		{
			m_pAttachButton->getButton()->setTarget(target, _selector);
			m_pAttachButton->getButton()->setTag(tag);
		}
		else if (m_pConfirmButton)
		{
			m_pConfirmButton->getButton()->setTarget(target, _selector);
			m_pConfirmButton->getButton()->setTag(tag);
		}
	}
}

void MailListViewCell::menuCallBack( CCObject* pSender )
{
	if (m_pMailInfo && m_pDelegate)
	{
		if (m_pMailInfo->checkHasAtt())
		{
			m_pDelegate->MailListViewCellPickUp(this);
		}
		else
		{
			m_pDelegate->DeleteMail(this);			
			//getMailInfo()->is_read = true;
			//getMailInfo()->postNotification(D_EVENT_MAIL_UPDATE);
			//DM_GET_MAIL_MODEL->readSystemMail(getMailInfo());
		}
	}
}









/************************ mail cell class end**********************/





AttachmentCell::AttachmentCell()
	:m_pIconContainer(NULL)
	,m_pCountLabel(NULL)
	,m_pCountNode(NULL)
	,m_pNode(NULL)
{

}

AttachmentCell::~AttachmentCell()
{
	CC_SAFE_RELEASE(m_pIconContainer);
	CC_SAFE_RELEASE(m_pCountLabel);
	CC_SAFE_RELEASE(m_pCountNode);
}

AttachmentCell* AttachmentCell::create( cocos2d::extension::CCData* pData )
{
	AttachmentCell* pRet = new AttachmentCell();
	if (pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool AttachmentCell::init( cocos2d::extension::CCData* pData )
{
	m_pNode = NULL;
	CCB_READER_DATA(pData, m_pNode);
	if(m_pNode)
		addChild(m_pNode);

	if (m_pCountNode)
	{
		m_pCountNode->setVisible(true);
	}
	
	setScale(0.8f);
	return true;
}

void AttachmentCell::setCount( std::string countString )
{
	if (m_pCountLabel)
	{
		m_pCountLabel->setString(countString.c_str());
	}
}

void AttachmentCell::setIcon( CCSprite* iconSprite )
{
	if (iconSprite && m_pIconContainer)
	{
		m_pIconContainer->removeAllChildren();
		m_pIconContainer->addChild(iconSprite);
	}
}

bool AttachmentCell::onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode*, this->m_pIconContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountLabel", CCLabelTTF*, this->m_pCountLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCountNode", CCNode*, m_pCountNode);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchMenu", CCControlButton*, m_pTouchMenu);

	return false;
}

cocos2d::SEL_MenuHandler AttachmentCell::onResolveCCBCCMenuItemSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	return NULL;
}
