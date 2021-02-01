#include "RecoverByItem.h"
#include "Pve.h"

USING_NS_CC;

static uint32_t MIN = 1;

RecoverByItem::RecoverByItem()
	:m_pTodayTotal0(NULL)
	,m_pCount(NULL)
	,m_pCloseButton(NULL)
	,m_pCannelButton(NULL)
	,m_pSubmitButton(NULL)
	,m_pSubButton(NULL)
	,m_pAddButton(NULL)
	,m_pSubMinButton(NULL)
	,m_pAddMaxButton(NULL)
	,m_nCount(MIN)
	,m_pPower(NULL)
{

}

RecoverByItem::~RecoverByItem()
{
	CC_SAFE_RELEASE(m_pTodayTotal0);
	CC_SAFE_RELEASE(m_pCount);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pCannelButton);
	CC_SAFE_RELEASE(m_pSubmitButton);
	CC_SAFE_RELEASE(m_pSubButton);
	CC_SAFE_RELEASE(m_pAddButton);
	CC_SAFE_RELEASE(m_pSubMinButton);
	CC_SAFE_RELEASE(m_pAddMaxButton);
	CC_SAFE_RELEASE(m_pPower);
}

bool RecoverByItem::init()
{
	if(!CCLayer::init()) return false;

	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beibao/PL_beibao_daojuhuifu");

	m_pPower = DM_GET_ITEM_MODEL->getItemInfoByID(D_CONFIG_UINT_FOR_KEY(ITEM_CHICKEN));
	CC_SAFE_RETAIN(m_pPower);
	m_pCloseButton->getButton()->setTarget(this, menu_selector(RecoverByItem::closeCallBack));
	m_pCannelButton->getButton()->setTarget(this,menu_selector(RecoverByItem::closeCallBack));
	m_pAddButton->getButton()->setTarget(this, menu_selector(RecoverByItem::addCallBack));
	m_pAddMaxButton->getButton()->setTarget(this, menu_selector(RecoverByItem::addMaxCallBack));
	m_pSubButton->getButton()->setTarget(this, menu_selector(RecoverByItem::subCallBack));
	m_pSubmitButton->getButton()->setTarget(this, menu_selector(RecoverByItem::submitCallBack));

	updateView();
	return true;
}

void RecoverByItem::onEnter()
{
	m_nCount = MIN;
	CCLayer::onEnter();
	BINDING_ONLY(m_pPower,this,RecoverByItem::buyHandler,D_EVENT_ITEMINFO_UPDATE);
}

void RecoverByItem::onExit()
{
	CCLayer::onExit();
	UNBINDING_ALL(m_pPower, this);
}

bool RecoverByItem::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayTotal0" , CCLabelTTF* , m_pTodayTotal0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount" , CCLabelBMFont* , m_pCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton" ,DButton* , m_pCloseButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCannelButton" ,DButton* , m_pCannelButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton" ,DButton* , m_pSubmitButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubButton" , DButton* ,m_pSubButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddButton" , DButton* ,m_pAddButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAddMaxButton" , DButton* , m_pAddMaxButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubMinButton" , CCMenuItem* , m_pSubMinButton);

	return false;
}

SEL_MenuHandler RecoverByItem::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this,"subMinCallBack",RecoverByItem::subMinCallBack);
	return NULL;
}

SEL_CCControlHandler RecoverByItem::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void RecoverByItem::closeCallBack(CCObject*)
{
	if(m_Delegate)
	{
		m_Delegate->closePopup();
	}
	else
	{
		removeFromParent();
	}
}

void RecoverByItem::addCallBack(CCObject*)
{
	m_nCount++;
	m_nCount = MIN(m_nCount,m_pPower->getNum());
	updateInfo();

}

void RecoverByItem::subCallBack(CCObject*)
{
	m_nCount--;
	m_nCount = MAX(MIN,m_nCount);
	updateInfo();
}

void RecoverByItem::addMaxCallBack(cocos2d::CCObject*)
{
	m_nCount = m_pPower->getNum();
	updateInfo();
}

void RecoverByItem::subMinCallBack(cocos2d::CCObject*)
{
	m_nCount = MIN;
	updateInfo();
}

void RecoverByItem::submitCallBack(cocos2d::CCObject*)
{
	DM_GET_ITEM_MODEL->useItem(m_pPower,m_nCount);
}

void RecoverByItem::updateView()
{
	updateInfo();
}

void RecoverByItem::updateInfo()
{
	m_pCount->setString(CCString::createWithFormat("%d",m_nCount)->getCString());
	m_pTodayTotal0->setString(CCString::createWithFormat("%d",m_pPower->getNum())->getCString());

	m_pSubButton->getButton()->setEnabled(MIN<m_nCount);
	m_pAddButton->getButton()->setEnabled(m_nCount < m_pPower->getNum());

	m_pSubMinButton->setEnabled(m_pSubButton->getButton()->isEnabled());
	m_pAddMaxButton->getButton()->setEnabled(m_pAddButton->getButton()->isEnabled());
}

void RecoverByItem::buyHandler()
{
	closeCallBack();
}