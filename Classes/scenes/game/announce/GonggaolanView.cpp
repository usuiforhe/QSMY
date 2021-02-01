//公告栏 游戏内

#include "GonggaolanView.h"
#include "managers/GameManager.h"
#include "utils/StringUtil.h"
using std::string;
GonggaolanView::GonggaolanView()
	:m_pContainer(NULL)
	,m_pData(NULL)
	,m_pTableView(NULL)
	,m_pInfo(NULL)
	,m_pScrollView(NULL)
	,m_pCloseButton(NULL)
	,m_pCell(NULL)
	,m_pCurInfo(NULL)
	,m_pArrow(NULL)
{

}

GonggaolanView::~GonggaolanView()
{
	CC_SAFE_RELEASE_NULL(m_pCurInfo);
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pData);
	CC_SAFE_RELEASE(m_pTableView);
	CC_SAFE_RELEASE(m_pInfo);
	CC_SAFE_RELEASE(m_pScrollView);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pArrow);
}

bool GonggaolanView::init()
{
	if(!DPopup::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_gonggaolan/pl_gonggaolan");

	m_pData = readCCData("PL_ui/ccb/pl_gonggaolan/pl_gonggaolan_cell");
	CC_SAFE_RETAIN(m_pData);

	m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setPosition(ccp(10,0));
	m_pContainer->addChild(m_pTableView);
	CC_SAFE_RETAIN(m_pTableView);

	m_pCloseButton->getButton()->setTarget(this, menu_selector(GonggaolanView::closeCallBack));
	//m_pInfo->setString("");
	
	if(DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->count()>0)
	{
		m_pCurInfo = static_cast<announce::AnnounceInfo *>(DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->objectAtIndex(0));
		CC_SAFE_RETAIN(m_pCurInfo);
	}
	m_pTableView->reloadData();
	m_pArrow->setVisible(false);
	return true;
}

bool GonggaolanView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", cocos2d::CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInfo" , CCRichLabelTTF* , m_pInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pScrollView" , CCScrollView*, m_pScrollView);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton" ,DButton* , m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pArrow" ,CCNode*,m_pArrow);
	return false;
}

cocos2d::SEL_MenuHandler GonggaolanView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GonggaolanView::closeCallBack);
	return NULL;
}

cocos2d::CCSize GonggaolanView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	return CCSizeMake(183,64);
}

void GonggaolanView::tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell)
{
	int idx = cell->getIdx();
	announce::AnnounceInfo * pAnnounce = static_cast<announce::AnnounceInfo *>(DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->objectAtIndex(idx));
	if(m_pCurInfo!=NULL)
	{
		if(m_pCurInfo == pAnnounce)
		{
			return;
		}
		CC_SAFE_RELEASE_NULL(m_pCurInfo);
	}

	m_pCurInfo = pAnnounce;
	CC_SAFE_RETAIN(m_pCurInfo);

	m_pTableView->reloadData();
}

cocos2d::extension::CCTableViewCell * GonggaolanView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	GonggaolanCell * pCell = static_cast<GonggaolanCell *>(table->dequeueCell());

	if(pCell == NULL)
	{
		pCell = GonggaolanCell::create(m_pData);
	}

	announce::AnnounceInfo * pAnnounce = static_cast<announce::AnnounceInfo *>(DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->objectAtIndex(idx));
	pCell->setAnnounce(pAnnounce);

	if(pAnnounce == m_pCurInfo)
	{
		pCell->setSelected(true);
		updateInfo(pAnnounce);
	}
	else
	{
		pCell->setSelected(false);
	}

	return pCell;
}

unsigned int GonggaolanView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return DM_GET_ANNOUNCE_MODEL->getAnnounceNewList()->count();
}

void GonggaolanView::setSelectedCell(GonggaolanCell* pCell)
{
	if(m_pCell == pCell)
	{
		return;
	}

	if(m_pCell != NULL)
	{
		m_pCell->setSelected(false);
		m_pCell->release();
	}

	m_pCell = pCell;
	if(m_pCell != NULL)
	{
		m_pCell->setSelected(true);
		m_pCell->retain();
	}
}

void GonggaolanView::closeCallBack(cocos2d::CCObject *sender)
{
	closePopup();
}

void GonggaolanView::updateInfo(announce::AnnounceInfo * pAnnounce)
{
	m_pInfo->setString(pAnnounce->getContent().c_str());
	
	//m_pInfo->setDimensions(CCSizeMake(m_pInfo->getContentSize().width,m_pInfo->getContentSize().height));
	//m_pScrollView->setContentSize(CCSizeMake(m_pScrollView->getContentSize().width,m_pInfo->getDimensions().height));
	//CCLOG(CCString::createWithFormat("m_pInfo dw:%d,dh:%d",m_pInfo->getContentSize().width,m_pInfo->getDimensions().height)->getCString());
	m_pScrollView->setContentOffset(m_pScrollView->minContainerOffset());
}

/**-----------GonggaolanCell--------------**/

GonggaolanCell::GonggaolanCell()
	:m_pBtn(NULL)
	,m_pTxt(NULL)
{

}

GonggaolanCell::~GonggaolanCell()
{
	CC_SAFE_RELEASE(m_pBtn);
	CC_SAFE_RELEASE(m_pTxt);
}

GonggaolanCell * GonggaolanCell::create(cocos2d::extension::CCData *pData)
{
	GonggaolanCell * pRet = new GonggaolanCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		pRet = NULL;
	}

	return pRet;
}

bool GonggaolanCell::init(cocos2d::extension::CCData *pData)
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_DATA_AND_ADDCHILD(pData);
	m_pBtn->setEnabled(false);
	return true;
}

bool GonggaolanCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtn" , CCMenuItemImage*,m_pBtn);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxt" , CCLabelTTF*, m_pTxt);
	return false;
}


cocos2d::SEL_MenuHandler GonggaolanCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "navCallBack", AnnounceCell::navCallBack);
	return NULL;
}

void GonggaolanCell::setAnnounce(announce::AnnounceInfo *pAnnounce)
{
	m_pTxt->setString(pAnnounce->getTitle().c_str());
	setSelected(false);
}

void GonggaolanCell::setSelected(bool isSelect)
{
	if(isSelect)
	{
		m_pBtn->selected();
	}
	else
	{
		m_pBtn->unselected();
	}
}