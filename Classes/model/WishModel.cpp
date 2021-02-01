//
//  WishModel.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-8-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WishModel.h"
#include "../managers/GameManager.h"
#include "../managers/TipsManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace wish;

bool WishItem::isUnlocked()
{
    uint8_t vip = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
    return vip >= m_nVipLevel;
}

bool WishModel::init()
{
    m_pWishes = CCDictionary::create();
    CC_SAFE_RETAIN(m_pWishes);
    
    WishItem * pWishItem = WishItem::create();
    pWishItem->setIndex(0);
    pWishItem->setVipLevel(0);
    m_pWishes->setObject(pWishItem, pWishItem->getIndex());

    pWishItem = WishItem::create();
    pWishItem->setIndex(1);
    pWishItem->setVipLevel(0);
    m_pWishes->setObject(pWishItem, pWishItem->getIndex());
    
    pWishItem = WishItem::create();
    pWishItem->setIndex(2);
    pWishItem->setVipLevel(3);
    m_pWishes->setObject(pWishItem, pWishItem->getIndex());
    
    pWishItem = WishItem::create();
    pWishItem->setIndex(3);
    pWishItem->setVipLevel(7);
    m_pWishes->setObject(pWishItem, pWishItem->getIndex());
    
    pWishItem = WishItem::create();
    pWishItem->setIndex(4);
    pWishItem->setVipLevel(9);
    m_pWishes->setObject(pWishItem, pWishItem->getIndex());
    
    m_pWishRes = CCArray::create();
    CC_SAFE_RETAIN(m_pWishRes);
    
    return Model::init();
}

void WishModel::syncFromServer()
{
    if(!isSynced)
    {
        HTTP_CLIENT->getWishesInfo();
    }
}

WishItem * WishModel::getWishItem(uint32_t index)
{
    return dynamic_cast<WishItem *>(m_pWishes->objectForKey(index));
}

void WishModel::wishesGet(const Json::Value &requestData, const Json::Value &responseData)
{
    const Json::Value & jsonWishes = responseData["wish"];
    
    WarriorModel * pWarriorModel = DM_GET_WARRIOR_MODEL;
    for(uint32_t i(0); i < jsonWishes.size() && i < UNLOCKABLE_WISH_NUM; ++i)
    {
        WishItem * pWishItem = (WishItem *)m_pWishes->objectForKey(i);
        uint32_t warriorId = jsonWishes[i].asUInt();
        if(pWarriorModel->getWarriorCurInfoByWID(warriorId) != NULL)
        {
            pWishItem->setWarriorId(warriorId);
        }
        else
        {
            pWishItem->setWarriorId(0);
        }
    }
    
    isSynced = true;
    postNotification(D_EVENT_WISHES_GET);
}

bool WishModel::isWarriorWished(uint32_t warriorId)
{
    CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_pWishes, pElement)
    {
        WishItem * pWishItem = dynamic_cast<WishItem *>(pElement->getObject());
        if(pWishItem->getWarriorId() == warriorId)
        {
            return true;
        }
    }
    
    return false;
}

void WishModel::setWish(uint32_t index, warrior::WarriorCurInfo *pWarrior)
{
    WishItem * pWishItem = (WishItem *) m_pWishes->objectForKey(index);
    pWishItem->setWarriorId(pWarrior->getWid());
    postNotification(D_EVENT_WISH_UPDATE);
}

void WishModel::setWarriorChanged()
{
	m_bWarriorsChanged = true;
}

void WishModel::doWish(bool isTen)
{
    uint32_t itemNum = DM_GET_ITEM_MODEL->getItemNum(SKY_LIGHT_ID);
    if(itemNum < K_WISH_SKY_LIGHT_NUM_PER_TIME)
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(SKY_LIGHT_ID));
        return;
    }
    
    CCArray * pWishes = CCArray::create();
    
    CCDictElement * pElement(NULL);
    CCDICT_FOREACH(m_pWishes, pElement)
    {
        WishItem * pWishItem = dynamic_cast<WishItem *>(pElement->getObject());
        
        if(pWishItem->isUnlocked())
        {
            if(pWishItem->getWarriorId() > 0)
            {
                warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pWishItem->getWarriorId());
                if(pWarrior != NULL)
                {
                    pWishes->addObject(CCInteger::create(pWarrior->getWid()));
                    continue;
                }
                else
                {
                    pWishItem->setWarriorId(0);
                }
            }
            
            
            TIPSMANAGER->warning(D_LOCAL_STRING("WishNotFilled"));            
            return;
        }
    }
    
    HTTP_CLIENT->wish(pWishes, isTen);
}

void WishModel::doWishComplete(const Json::Value &requestData, const Json::Value &responseData)
{
    m_pWishRes->removeAllObjects();
    const Json::Value & res = responseData["wishRes"];
    
    for(Json::Value::iterator iter(res.begin()); iter != res.end(); ++ iter)
    {
        m_pWishRes->addObject(CCInteger::create((*iter).asUInt()));
    }
    
    DM_GET_ITEM_MODEL->changeItemNumByID(SKY_LIGHT_ID, -K_WISH_SKY_LIGHT_NUM_PER_TIME * res.size());
    
	m_bWarriorsChanged = false;

    postNotification(D_EVENT_WISH_RES_GET);
}

void WishModel::saveWishWarriors()
{
	if (m_bWarriorsChanged)
	{
		CCArray * pWishes = CCArray::create();

		CCDictElement * pElement(NULL);
		CCDICT_FOREACH(m_pWishes, pElement)
		{
			WishItem * pWishItem = dynamic_cast<WishItem *>(pElement->getObject());

			if(pWishItem->isUnlocked())
			{
				uint32_t wid = pWishItem->getWarriorId();
				if(wid > 0)
				{
					warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
					if(!pWarrior)
					{
						wid = 0;
					}
				}
				pWishes->addObject(CCInteger::create(wid));
			}
		}

		HTTP_CLIENT->wishChangeWarrior(pWishes);
	}
	
}