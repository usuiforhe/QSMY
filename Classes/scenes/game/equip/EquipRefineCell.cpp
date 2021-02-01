//
//  EquipRefineCell.cpp
//  装备精炼cell
//  Created by mzp on 14-04-09.
//	Modified 
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "EquipRefineCell.h"
#include "../../../model/ItemModel.h"
#include "../../../managers/GameManager.h"


USING_NS_CC;


EquipRefineData::EquipRefineData()
	: m_nId(0)
	, m_name("")
	, m_nCount(0)
	, m_nAdd(0)
{

}

EquipRefineData::~EquipRefineData()
{

}

EquipRefineData* EquipRefineData::create(unsigned int id)
{
	EquipRefineData* pRet = new EquipRefineData();
	if(pRet && pRet->init(id))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool EquipRefineData::init(unsigned int id)
{
	if (!ValueObjcet::init()) 
	{
		return false;
	}

	item::ItemInfo* pItem = DM_GET_ITEM_MODEL->getItemInfoByID(id);
	if (!pItem)
	{
		item::ItemBase* pBase = DM_GET_ITEM_MODEL->getItemBaseByID(id);
		if (pBase)
		{
			this->setName(pBase->getItemName());
		}
		
		this->setID(id);
		this->setCount(0);
	}
	else
	{
		this->setCount(pItem->getNum());
		this->setID(id);
		this->setName(pItem->getBaseInfo()->getItemName());
	}

	return true;
}

void EquipRefineData::setCount(uint32_t val)
{
	m_nCount = val;
	postNotification(D_EVENT_EQUIP_REFINE_UPDATED);
}

int EquipRefineData::getAdded()
{
	return m_nAdd;
}

void EquipRefineData::add()
{
	if (getRemain() > 0)
	{
		m_nAdd++;
		postNotification(D_EVENT_EQUIP_REFINE_ADD);
	}
}

void EquipRefineData::fixAdd(int num)
{
	m_nAdd = num;
	postNotification(D_EVENT_EQUIP_REFINE_UPDATED);
}

int EquipRefineData::getRemain()
{
	return m_nCount - m_nAdd;
}







EquipRefineCell::EquipRefineCell()
	: m_pLabelName(NULL)
	, m_pLabelCount(NULL)
	, m_pItemBox(NULL)
	, m_pMark(NULL)
	, m_pData(NULL)
	, m_bMax(false)
    , m_bIgnoreTouchEnd(false)
{

}

EquipRefineCell::~EquipRefineCell()
{
	CC_SAFE_RELEASE(m_pLabelName);
	CC_SAFE_RELEASE(m_pLabelCount);
	CC_SAFE_RELEASE(m_pItemBox);
	CC_SAFE_RELEASE(m_pMark);
	SAFE_RELEASE_UNBINDING_ALL(m_pData, this);
}

bool EquipRefineCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF*, this->m_pLabelName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNeed", CCLabelBMFont*, this->m_pLabelCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox", ItemBox*, this->m_pItemBox);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMark", extension::CCScale9Sprite*, this->m_pMark);

	return false;
}

bool EquipRefineCell::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches /*= NULL*/)
{
	if (!m_pData)
	{
		return false;
	}
	
	CCPoint pt = pTouch->getLocation();
	CCRect rcBoundingBox = CCRectMake(0, 0, getContentSize().width, getContentSize().height);
	rcBoundingBox = CCRectApplyAffineTransform( rcBoundingBox, nodeToWorldTransform());
	if (!rcBoundingBox.containsPoint(pt))
	{
		return false;
	}

	if (!m_bMax)
	{
		schedule(schedule_selector(EquipRefineCell::update), 0.1f, kCCRepeatForever, 0.5f);
	}

	return true;
}

void EquipRefineCell::dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	DLayer::dTouchEnded(pTouch, pEvent);
	unschedule(schedule_selector(EquipRefineCell::update));

    if(!m_bIgnoreTouchEnd)
    {
        if (!m_bMax)
        {
            update(0.0f);
        }
        else
        {
            Tips_Alert(D_LOCAL_STRING("EquipRefineMax").c_str());
        }
    }
    m_bIgnoreTouchEnd = false;
}

void EquipRefineCell::dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	DLayer::dTouchCancelled(pTouch,pEvent);
	unschedule(schedule_selector(EquipRefineCell::update));
}

void EquipRefineCell::update(float dt)
{
    m_bIgnoreTouchEnd = true;
	if (!m_pData)
	{
		return;
	}
	
	m_pData->add();
}

void EquipRefineCell::setData(EquipRefineData* pData)
{
	if (m_pData == pData) 
	{
		updateView();
		return;
	}

	SAFE_RELEASE_UNBINDING_ALL(m_pData, this);
	CC_SAFE_RETAIN(pData);
	m_pData = pData;
	BINDING_EXEC(m_pData, this, EquipRefineCell::updateView, D_EVENT_EQUIP_REFINE_UPDATED);
	BINDING_EXEC(m_pData, this, EquipRefineCell::updateView, D_EVENT_EQUIP_REFINE_ADD);
}

void EquipRefineCell::updateView()
{
	if (m_pData)
	{
		m_pLabelName->setString(m_pData->getName().c_str());
		m_pLabelCount->setString(CCString::createWithFormat("x%i", m_pData->getRemain())->getCString());
		m_pItemBox->getContainer()->removeAllChildren();
		m_pItemBox->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(m_pData->getID()));
		m_pMark->setVisible(m_pData->getCount() > 0);
	}
	else
	{
		m_pLabelName->setString("");
		m_pLabelCount->setString("");
		m_pItemBox->getContainer()->removeAllChildren();
		m_pMark->setVisible(false);
	}
}

void EquipRefineCell::setRefineMax(bool bMax)
{
	m_bMax = bMax;

	if (m_bMax)
	{
		unschedule(schedule_selector(EquipRefineCell::update));
	}
}
