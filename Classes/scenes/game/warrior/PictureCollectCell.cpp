#include "PictureCollectCell.h"
#include "WarriorDelegate.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "WarriorInfo.h"
USING_NS_CC;
USING_NS_CC_EXT;
extern bool m_bIsfromCollect;
PictureCollectCell::PictureCollectCell()
:m_pBg_dis(NULL)
,m_pBg_up(NULL)
,m_pWarriorGradeBg(NULL)
,m_pWarriorTypeMark(NULL)
,m_pName(NULL)
,m_pZhizi(NULL)
,m_pZizhiNum(NULL)
,m_pNew(NULL)
,m_pNumber(NULL)
,m_pArea(0)
,m_nID(0)
,m_pWarriorDelegate_t(NULL)
,m_pMenuButton(NULL)
,m_pWarriorSprite(NULL)
,m_bLighted(false)
{
	 D_RESET_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}

PictureCollectCell::~PictureCollectCell()
{
	CC_SAFE_RELEASE(m_pBg_dis);
	CC_SAFE_RELEASE(m_pBg_up);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	CC_SAFE_RELEASE(m_pWarriorTypeMark);
	CC_SAFE_RELEASE(m_pName);
	CC_SAFE_RELEASE(m_pZhizi);
	CC_SAFE_RELEASE(m_pZizhiNum);
	CC_SAFE_RELEASE(m_pNew);
	CC_SAFE_RELEASE(m_pNumber);
	CC_SAFE_RELEASE(m_pWarriorSprite);
	D_SAFE_RELEASE_C_ARRAY(m_pProgressBar, WARRIOR_GRADE_NUM);
}
void PictureCollectCell::onEnter()
{
	CCNode::onEnter();

}
void PictureCollectCell::onExit()
{
	CCNode::onExit();
}
bool PictureCollectCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg_dis",  CCSprite*, this->m_pBg_dis);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg_up",  CCSprite*, this->m_pBg_up);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg*, this->m_pWarriorGradeBg);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGreen",  CCSprite*, this->m_pProgressBar[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBlue",  CCSprite*, this->m_pProgressBar[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPurple",  CCSprite*, this->m_pProgressBar[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGold",  CCSprite*, this->m_pProgressBar[3]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark",  WarriorTypeMark*, this->m_pWarriorTypeMark);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName",  CCLabelTTF*, this->m_pName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZhizi",  CCSprite*, this->m_pZhizi);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNew", CCSprite*, this->m_pNew);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pZizhiNum", CCLabelBMFont*, this->m_pZizhiNum);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNumber", CCLabelBMFont*, this->m_pNumber);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorSprite", CCNode*, this->m_pWarriorSprite);
	return false;
}
SEL_MenuHandler PictureCollectCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this,"chooseMenuCallBack",PictureCollectCell::chooseMenuCallBack);
	return NULL;
}

void PictureCollectCell::setRankNum(int num)
{
	this->m_pNumber->setString(CCString::createWithFormat("%d",num)->getCString());
}

void PictureCollectCell::setPictureCell(warrior::WarriorHandBookInfo* pInfo)
{
	whetherShow(true);
	
	this->m_pName->setString(CCString::createWithFormat("%s",pInfo->getWarrior_name().c_str())->getCString());
	this->m_pZizhiNum->setString(CCString::createWithFormat("%d",pInfo->getQuality())->getCString());
	if(pInfo->getNew()==0)
	{
		this->m_pNew->setVisible(false);
	}
	if(pInfo->getNew()==1)
	{
		this->m_pNew->setVisible(true);
	}

	for(uint8_t i = 0 ; i < WARRIOR_GRADE_NUM ; i++){
		int color = pInfo->getColor_grade();
		m_pProgressBar[i]->setVisible( color == i+1);
	}

	this->m_pArea=pInfo->getArea();
	this->m_pWarriorGradeBg->setWarriorGrade(static_cast<warrior::WarriorGrade>(pInfo->getColor_grade()));
	this->m_pWarriorTypeMark->setWarriorInfo(DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pInfo->getWarrior_id()));
	this->m_pWarriorSprite->removeAllChildren();
	
	this->m_pWarriorSprite->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon((warrior::WarriorCurInfo::create(this->m_nID))->getAvatarId(), 0.43f),0,999);
	if(pInfo->getIsLighted()==false)
	{
		this->m_pWarriorGradeBg->setVisible(false);
		this->m_pWarriorSprite->getChildByTag(999)->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureGray));
	}
	if(pInfo->getIsLighted()==true)
	{
		this->m_pWarriorGradeBg->setVisible(true);
		this->m_pWarriorSprite->getChildByTag(999)->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
	}
}

void PictureCollectCell::whetherShow(bool m_bBool)
{
	if(m_bBool)
	{
		this->setVisible(m_bBool);
	}
	else
	{
		this->setVisible(m_bBool);
	}
}
void PictureCollectCell::setDelegate_t(WarriorDelegate* m_pWarriorDelegate)
{
	m_pWarriorDelegate_t=m_pWarriorDelegate;
}
void PictureCollectCell::chooseMenuCallBack(cocos2d::CCObject* obj)
{
	m_bIsfromCollect_t=true;
	CCMenuItem* pMenuItem=dynamic_cast<CCMenuItem*>(obj);
	if (pMenuItem)
	{
		if((pMenuItem->getTag())==9)
		{
			m_pWarriorDelegate_t->showInfoView(kWarriorInfo, warrior::WarriorCurInfo::create(this->m_nID));
		}
	}
}