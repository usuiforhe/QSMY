//
//  EquipBox.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "EquipBox.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "model/WarriorModel.h"
#include "model/EquipModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

EquipBox::EquipBox()
:m_pGrade(NULL)
,m_pContainer(NULL)
,m_pTouchEvent(NULL)
,m_pName(NULL)
,m_pLevel(NULL)
,m_pNameBg(NULL)
,m_pEquipGrade(NULL)
,m_pUpLevel(NULL)
,m_pEquipInfo(NULL)
,m_pCanEnhance(NULL)
,m_pCanAdd(NULL)
//,m_pCanRefine(NULL)
,m_bShowCanEnhance(true)
,m_pNoEquip(NULL)
,m_pLevelContainer(NULL)
,m_pEquipType(equip::kEquipTypeALL)
,m_pEquipOnMenu(NULL)
,m_pChangeAble(false)
,m_pEquipGradeBg(NULL)
,m_nTypeNum(-1)
,m_pFateNode(NULL)
,m_pProperty(NULL)
,m_pEmpty(NULL)
,m_pBottom(NULL)
,m_pTxtQianghua(NULL)
,m_pTxtJinjie(NULL)
{
	D_RESET_C_ARRAY(m_vBgs,EQUIP_BOX_BG_NUM);
	//D_RESET_C_ARRAY(m_pMark, equip::kEquipPropTotal);
}

EquipBox::~EquipBox()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTouchEvent);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pEquipInfo);
    CC_SAFE_RELEASE(m_pNameBg);
	CC_SAFE_RELEASE(m_pCanEnhance);
	//CC_SAFE_RELEASE(m_pCanRefine);
	CC_SAFE_RELEASE(m_pCanAdd);
	CC_SAFE_RELEASE(m_pEquipGrade);
	CC_SAFE_RELEASE(m_pUpLevel);
	CC_SAFE_RELEASE(m_pNoEquip);
	CC_SAFE_RELEASE(m_pLevelContainer);
	CC_SAFE_RELEASE(m_pEquipOnMenu);
	CC_SAFE_RELEASE(m_pEquipGradeBg);
	D_SAFE_RELEASE_C_ARRAY(m_vBgs,EQUIP_BOX_BG_NUM);
	CC_SAFE_RELEASE(m_pFateNode);
	//D_SAFE_RELEASE_C_ARRAY(m_pMark, equip::kEquipPropTotal);
	CC_SAFE_RELEASE(m_pProperty);
	CC_SAFE_RELEASE(m_pEmpty);
	CC_SAFE_RELEASE(m_pBottom);
	CC_SAFE_RELEASE(m_pTxtQianghua);
	CC_SAFE_RELEASE(m_pTxtJinjie);
}

bool EquipBox::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  CCNodeRGBA*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer",  CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchEvent",  CCMenuItem*, this->m_pTouchEvent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName",  CCLabelTTF*, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel",  CCLabelBMFont*, this->m_pLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameBg", CCNode *, this->m_pNameBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanEnhance", CCNode *, this->m_pCanEnhance);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanAdd", CCNode *, this->m_pCanAdd);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanAdd", CCNode *, this->m_pCanRefine);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipGrade",  CCNodeRGBA*, this->m_pEquipGrade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipUpLevel", CCNode*, this->m_pUpLevel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNoEquip", CCNode*, this->m_pNoEquip);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevelContainer",  CCNode*, this->m_pLevelContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipOnMenu",  CCMenuItem*, this->m_pEquipOnMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEquipGradeBg",  WarriorHeadGradeBg*, this->m_pEquipGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg1", cocos2d::CCNode*, m_vBgs[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg2", cocos2d::CCNode*, m_vBgs[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg3", cocos2d::CCNode*, m_vBgs[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBg4", cocos2d::CCNode*, m_vBgs[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFateNode", CCNode*, m_pFateNode);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty",  CCLabelBMFont*, this->m_pProperty);
	//D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pEquipType", CCNode*, m_pMark, equip::kEquipPropTotal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEmpty", CCNode*, m_pEmpty);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode*, m_pBottom);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtQianghua", CCNode*, m_pTxtQianghua);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtJinjie", CCNode*, m_pTxtJinjie);
    return false;
}

bool EquipBox::init()
{
	if(!CCNode::init()) return false;

	

	return true;
}

void EquipBox::onEnter()
{
	CCNode::onEnter();
}

void EquipBox::reset()
{
    CC_SAFE_RELEASE_NULL(m_pEquipInfo);
    if(m_pGrade) m_pGrade->setVisible(false);
    if(m_pContainer) m_pContainer->setVisible(false);
    if(m_pName) m_pName->setVisible(false);
    if(m_pLevel) m_pLevel->setVisible(false);
    if(m_pNameBg) m_pNameBg->setVisible(false);
	if(m_pCanEnhance) m_pCanEnhance->setVisible(false);
	if(m_pCanAdd) m_pCanAdd->setVisible(false);
	//if(m_pCanRefine) m_pCanRefine->setVisible(false);
	
	if(m_pEquipGrade) m_pEquipGrade->setVisible(false);
	if(m_pUpLevel) m_pUpLevel->setVisible(false);
	if(m_pNoEquip) m_pNoEquip->setVisible(true);
	if(m_pLevelContainer) m_pLevelContainer->setVisible(false);
	m_pEquipGradeBg->setWarriorGrade(warrior::WarriorGrade(0));
	if(m_vBgs)
	{
		if(m_vBgs[0])m_vBgs[0]->setVisible(false);
		if(m_vBgs[1])m_vBgs[1]->setVisible(false);
		if(m_vBgs[2])m_vBgs[2]->setVisible(false);
		if(m_vBgs[3])m_vBgs[3]->setVisible(false);
		if(m_nTypeNum>=0)
		{
			if(m_vBgs[m_nTypeNum])m_vBgs[m_nTypeNum]->setVisible(true);
			if(m_pNoEquip) m_pNoEquip->setVisible(false);
		}
	}
	if(m_pFateNode) m_pFateNode->setVisible(false);
}

void EquipBox::setTreasureInfo(equip::EquipCurInfo* pInfo)
{
	bool isEmpty = pInfo == NULL;
	if(pInfo == NULL)
	{
		//空了
		reset();
	}
	else
	{
		CC_SAFE_RELEASE(m_pEquipInfo);
		CC_SAFE_RETAIN(pInfo);
		m_pEquipInfo = pInfo;

		setEquipInfo(pInfo->getBaseInfo());

		//level
		if(m_pLevelContainer)
		{
			m_pLevelContainer->setVisible(true);
			m_pLevel->setVisible(true);
			m_pLevel->setString(CCString::createWithFormat("%d",pInfo->getLevel())->getCString());
		}
		//exp
		m_pProperty->setString(CCString::createWithFormat("%d",DM_GET_BURN_MODEL->getTreasureExpAdd(pInfo))->getCString());
	}
	//额外处理
	m_pEmpty->setVisible(isEmpty);
	m_pCanAdd->setVisible(isEmpty);
	m_pBottom->setVisible(!isEmpty);
}

void EquipBox::setEquipInfo(equip::EquipBaseInfo* pInfo)
{
    if(m_pContainer) m_pContainer->removeAllChildren();
    
    if(pInfo==NULL)
    {
        reset();
        return;
    }
    if(m_pNoEquip) m_pNoEquip->setVisible(false);
    if(m_pNameBg) m_pNameBg->setVisible(true);
    
	if(m_pTxtJinjie) m_pTxtJinjie->setVisible(pInfo->getEquipType() == equip::kEquipTypeTreasure);
	if(m_pTxtQianghua) m_pTxtQianghua->setVisible(pInfo->getEquipType() != equip::kEquipTypeTreasure);
    if(m_pGrade)
    {
        m_pGrade->setVisible(true);
        m_pGrade->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(pInfo->getEquipGrade()));
    }
    
    if(m_pContainer)
    {
        m_pContainer->setVisible(true);
        m_pContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon30P(pInfo->getEquipId()));
    }
    
    if(m_pName)
    {
        m_pName->setVisible(true);
        m_pName->setString(pInfo->getEquipName().c_str());
    }
    
    if(m_pLevelContainer)
    {
		m_pLevelContainer->setVisible(true);
        m_pLevel->setVisible(true);
        m_pLevel->setString("0");
    }
    
	if (m_pEquipGrade)
	{
		m_pEquipGrade->setVisible(true);
		m_pEquipGrade->setColor(DM_GET_EQUIP_MODEL->getEquipColorByGrade(pInfo->getEquipGrade()));
	}
	
	/*if (m_pUpLevel)
	{
		m_pUpLevel->setVisible(true);
		m_pUpLevel->removeAllChildren();
		m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(0));
	}*/
	if(m_pCanAdd) m_pCanAdd->setVisible(m_pAddAble);

	m_pEquipGradeBg->setWarriorGrade(warrior::WarriorGrade(pInfo->getEquipGrade()));
	if(m_vBgs)
	{
		if(m_vBgs[0])m_vBgs[0]->setVisible(false);
		if(m_vBgs[1])m_vBgs[1]->setVisible(false);
		if(m_vBgs[2])m_vBgs[2]->setVisible(false);
		if(m_vBgs[3])m_vBgs[3]->setVisible(false);
	}
	if(m_pFateNode) m_pFateNode->setVisible(false);
}

void EquipBox::setEquipInfo(equip::EquipCurInfo* pInfo,  uint32_t wid, equip::EquipType pEquipType)
{
    CC_SAFE_RELEASE(m_pEquipInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pEquipInfo = pInfo;
	m_pEquipType = pEquipType;
	m_pChangeAble = false;
	m_pFateAble = false;
	m_pAddAble = false;


	cocos2d::CCArray* pList = CCArray::create();
	DM_GET_EQUIP_MODEL->getEquipNotEquipedListByType(pEquipType,*pList);
	uint32_t pEquipProperty = 0;
	if(pInfo)
	{
		std::string temp;
		pEquipProperty = DM_GET_EQUIP_MODEL->computeEquipInfo(pInfo,DM_GET_EQUIP_MODEL->getEquipPropType(pInfo, temp));
	}
	CCObject* obj = NULL;
	CCARRAY_FOREACH(pList, obj)
	{
		equip::EquipCurInfo* pEquipCurInfo = (equip::EquipCurInfo*)obj;
		std::string temp;
		uint32_t pTempProperty = DM_GET_EQUIP_MODEL->computeEquipInfo(pEquipCurInfo,DM_GET_EQUIP_MODEL->getEquipPropType(pEquipCurInfo, temp));
		if(pTempProperty > pEquipProperty)
		{
			m_pChangeAble = true;
			break;
		}
	}
	if(pList->count() > 0 && pEquipProperty == 0)
	{
		m_pAddAble = true;
	}
	
    if(pInfo==NULL)
    {
        reset();
		m_pCanAdd->setVisible(m_pAddAble);
        return;
    }


    setEquipInfo(pInfo->getBaseInfo());
    
    
    if(m_pLevelContainer)
    {
		m_pLevelContainer->setVisible(true);
        m_pLevel->setVisible(true);
        m_pLevel->setString(CCString::createWithFormat("%d",pInfo->getLevel())->getCString());
    }

	if (m_bShowCanEnhance)
	{
		// 先判断是否有缘，再判断是否可以强化，再判断是否可以精炼
		cocos2d::CCArray* pFatesEuqip = cocos2d::CCArray::create();
		DM_GET_WARRIOR_MODEL->getWarriorFatesByWID(wid, pFatesEuqip);
		cocos2d::CCObject* obj = NULL;
		warrior::FateInfo* fateInfo = NULL;
		m_pFateAble = false;
		CCARRAY_FOREACH(pFatesEuqip, obj)
		{
			fateInfo = (warrior::FateInfo*)obj;
			if (fateInfo && fateInfo->equip_id != pInfo->getEquipId())
			{
				cocos2d::CCArray* pEquipList = DM_GET_EQUIP_MODEL->getEquipCurInfosByEquipID(fateInfo->equip_id);
				cocos2d::CCObject* equipObj = NULL;
				CCARRAY_FOREACH(pEquipList, equipObj)
				{
					equip::EquipCurInfo* pEquipCurInfo = (equip::EquipCurInfo*)equipObj;
					equip::EquipType equipType = pInfo->getBaseInfo()->getEquipType();
					if (pEquipCurInfo &&
						!pEquipCurInfo->isEquiped() &&
						pEquipCurInfo->getBaseInfo()->getEquipType() == equipType && 
						pEquipCurInfo->getBaseInfo()->getEquipGrade() >= pInfo->getBaseInfo()->getEquipGrade())
					{
						m_pFateAble = true;
						break;
					}
				}
			}

			if (m_pFateAble)
			{
				break;
			}
		}

		if (m_pAddAble)
		{
			m_pCanAdd->setVisible(true);
			m_pCanEnhance->setVisible(false);
			//m_pCanRefine->setVisible(false);
			//m_pLevelContainer->setVisible(false);
		}  
		else
		{
			m_pCanAdd->setVisible(false);
			m_pCanEnhance->setVisible(false);
			m_pFateNode->setVisible(false);
			if(m_pFateAble)
			{
				m_pFateNode->setVisible(true);
			}

			if (pInfo->isCanEnhance())
			{
				m_pCanEnhance->setVisible(true);
			}
			if (pInfo->isCanRefine())
			{
				//m_pCanRefine->setVisible(true);
			}
		}
	}
	else
	{
		m_pCanAdd->setVisible(false);
		m_pCanEnhance->setVisible(false);
		//m_pCanRefine->setVisible(false);
	}
	
	/*if (m_pUpLevel)
	{
		m_pUpLevel->setVisible(true);
		m_pUpLevel->removeAllChildren();
		m_pUpLevel->addChild(ResourceManager::sharedResourceManager()->getUpLevel(pInfo->getRefineLevel()));
	}*/
}

bool EquipBox::canChange()
{
	return m_pChangeAble || m_pFateAble;
}



