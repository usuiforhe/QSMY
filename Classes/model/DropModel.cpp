//
//  DropModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DropModel.h"

#include "EquipModel.h"
#include "ItemModel.h"
#include "WarriorModel.h"

#include "../managers/GameManager.h"
#include <algorithm>
using namespace drop;
using namespace item;
using namespace equip;
using namespace warrior;
USING_NS_CC;

DropModel::DropModel()
:m_pDropsMap(NULL)
{
    
}

DropModel::~DropModel()
{
	CC_SAFE_RELEASE(m_pDropsMap);
}

std::string DropModel::dropContent(drop::Drop* pDrop)
{
	std::string ret = "";
	if (!pDrop)
	{
		return ret;
	}

	switch(pDrop->getType())
	{
	case kDropTypeItem:
	case kDropTypePkg:
		ret = cocos2d::CCString::createWithFormat("%sx%d", pDrop->getItem()->getBaseInfo()->getItemName().c_str(), pDrop->getItem()->getNum())->getCString();
		break;
	case kDropTypeEquip:
		ret = cocos2d::CCString::createWithFormat("%sx1", pDrop->getEquip()->getBaseInfo()->getEquipName().c_str())->getCString();
		break;
	case kDropTypeWarrior:
		ret = cocos2d::CCString::createWithFormat("%sx1", pDrop->getWarrior()->getBaseInfo()->getWarriorName().c_str())->getCString();
		break;
	case kDropTypeSoul:
		ret = cocos2d::CCString::createWithFormat("%sx1", D_LOCAL_STRING("%sSoul", pDrop->getWarrior()->getBaseInfo()->getWarriorName().c_str()).c_str())->getCString();
		break;
	case kDropTypeCoin:
		ret = cocos2d::CCString::createWithFormat("%sx%d", D_LOCAL_STRING("Coin").c_str(), pDrop->getCoin())->getCString();
		break;
	case kDropTypePoint:
		ret = cocos2d::CCString::createWithFormat("%sx%d", D_LOCAL_STRING("Point").c_str(), pDrop->getPoint())->getCString();
		break;
	case kDropTypeVip:
		ret = cocos2d::CCString::createWithFormat("VIP%d", pDrop->getVip())->getCString();
		break;
	case kDropTypePower:
		ret = cocos2d::CCString::createWithFormat("%sx%d", D_LOCAL_STRING("Power").c_str(), pDrop->getPower())->getCString();
		break;
	case kDropTypeAvatar:
		ret = cocos2d::CCString::createWithFormat("%sx1", pDrop->getAvatarInfo()->getBaseInfo()->getName().c_str())->getCString();
		break;
	/*case kDropTypeAlchemy:
		ret = cocos2d::CCString::createWithFormat("%sx%d", pDrop->getAlchemy()->getBaseInfo()->getAlchemyName().c_str(), pDrop->getAlchemy()->getNum())->getCString();
		break;
	case kDropTypeChiNum:
		ret = cocos2d::CCString::createWithFormat("%sx%d", D_LOCAL_STRING("ChiNum").c_str(), pDrop->getChiNum())->getCString();
		break;

	case kDropTypeGundam :
		ret = cocos2d::CCString::createWithFormat("%sx1", pDrop->getGundam()->getBaseInfo()->getGName().c_str())->getCString();
		break;

	case kDropTypeGundamFrag :
		ret = cocos2d::CCString::createWithFormat("%s%sx1", pDrop->getGundamFrag()->getBaseInfo()->getGName().c_str(), D_LOCAL_STRING("Frag").c_str())->getCString();
		break;*/
	}

	return ret;
}

bool DropModel::init()
{
	m_pDropsMap = cocos2d::CCDictionary::create();
	CC_SAFE_RETAIN(m_pDropsMap);
    return Model::init();
}


void DropModel::addDrops(const Json::Value &val,uint32_t id , bool noSort)
{
    CCArray* pArray = resetDrop(id);
	for (uint32_t i=0; i<val.size();i++)
	{
		Drop* pDrop = parseDrop(val[i]);
		inDrops(pDrop,pArray , noSort);
	}
}

void DropModel::addDrop(const Json::Value &val, uint32_t id)
{
    CCArray* pArray = resetDrop(id);
    Drop* pDrop = parseDrop(val);
    pArray->addObject(pDrop);
}

cocos2d::CCArray* DropModel::resetDrop(uint32_t id)
{
    cocos2d::CCArray* pArray = cocos2d::CCArray::create();
    m_pDropsMap->setObject(pArray, id);
    return pArray;
}
bool DropModel::dropSort(CCObject* o1, CCObject* o2)
{
	drop::Drop* d1 = (drop::Drop*)o1;
	drop::Drop* d2 = (drop::Drop*)o2;
	int grade1 = getGrade(d1);
	int grade2 = getGrade(d2);
	return (grade1 > grade2);
}
int DropModel::getGrade(drop::Drop* pDropItem)
{
	int itemGrade = 0;
	switch (pDropItem->getType()) {
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			itemGrade = (int)pDropItem->getItem()->getBaseInfo()->getItemGrade();
			break;
		}
	case drop::kDropTypeEquip:
		{
			itemGrade = (int)pDropItem->getEquip()->getBaseInfo()->getEquipGrade();
			break;
		}
	case drop::kDropTypeSoul:
		{
			itemGrade = (int)pDropItem->getSoul()->getBaseInfo()->getRealGrade();
			break;
		}
	case drop::kDropTypeWarrior:
		{
			itemGrade = (int)pDropItem->getWarrior()->getBaseInfo()->getRealGrade();
			break;
		}
	}
	return itemGrade;
}
cocos2d::CCArray* DropModel::getDropArray(uint32_t id)
{
    cocos2d::CCArray* pArray = (cocos2d::CCArray*)m_pDropsMap->objectForKey(id);
    if(!pArray) return NULL;

	//std::vector<CCObject*> vec;
	//CCObject *iter = NULL;
	//CCARRAY_FOREACH(pArray , iter)
	//{
	//	vec.push_back(iter);
	//}
	//sort(vec.begin() , vec.end() , DropModel::dropSort);
	////(drop::Drop*)
	//pArray->removeAllObjects();
	//std::vector<CCObject*>::iterator i;
	//for(i = vec.begin();i!= vec.end();i++)
	//{
	//	pArray->addObject(*i);
	//}

	std::sort(pArray->data->arr , pArray->data->arr+pArray->data->num , DropModel::dropSort);

    pArray->retain();
    pArray->autorelease();
    m_pDropsMap->removeObjectForKey(id);
    return pArray;
}

void DropModel::removeDrop(unsigned int id)
{
    m_pDropsMap->removeObjectForKey(id);
}

Drop* DropModel::parseDrop(const Json::Value &val)
{
    uint8_t type = 0;
    DJSON_UINT(type,val,"type");
    
    Drop * pDrop(NULL);
	switch(type)
	{
        case kDropTypeItem:
        case kDropTypePkg:
            pDrop = Drop::create(DM_GET_ITEM_MODEL->parseItem(val["item"]));
            break;
        case kDropTypeEquip:
            pDrop = Drop::create(DM_GET_EQUIP_MODEL->parseEquip(val["equip"]));
            break;
        case kDropTypeWarrior:
            pDrop = Drop::create(DM_GET_WARRIOR_MODEL->parseWarrior(val["warrior"]));
            break;
        case kDropTypeSoul:
            pDrop = Drop::create(DM_GET_WARRIOR_MODEL->parseSoul(val["soul"]));
            break;
        case kDropTypeCoin:
            pDrop = Drop::createCoin(val["coin"].asUInt());
            break;
        case kDropTypePoint:
            pDrop = Drop::createPoint(val["point"].asUInt());
            break;
        case kDropTypeVip:
            pDrop = Drop::createVip(val["vip"].asUInt());
            break;
        case kDropTypePower:
            pDrop = Drop::createPower(val["power"].asUInt());
            break;
        case kDropTypeAvatar:
            pDrop = Drop::create(DM_GET_AVATAR_MODEL->parseDress(val["dress"]));
			break;
		case kDropTypeJJCScore:
			pDrop = Drop::createJJCScore(val["point"].asUInt());
            break;
	}

	return pDrop;
}

void DropModel::inDrops(drop::Drop *pDrop,cocos2d::CCArray *pArray , bool noSort)
{
	//不合并了！
	bool inDropArray = false;

	//需要合并，就合并
	if(!noSort)
	{
		Drop *tmpDrop = NULL;
		Drop *newDrop = NULL;
		for(uint32_t j = 0; j < pArray->count(); ++j)
		{
			tmpDrop = (Drop *)pArray->objectAtIndex(j);
			if(tmpDrop->getType() == pDrop->getType())
			{
				switch(tmpDrop->getType())
				{
				case drop::kDropTypeCoin:
					newDrop = Drop::createCoin(tmpDrop->getCoin()+pDrop->getCoin());
					inDropArray = true;
					break;
				case drop::kDropTypePoint:
					newDrop = Drop::createPoint(tmpDrop->getPoint()+pDrop->getPoint());
					inDropArray = true;
					break;
				case drop::kDropTypeVip:
					break;
				case drop::kDropTypeItem:
				case drop::kDropTypePkg:
					if(tmpDrop->getItem()->getItemId() == pDrop->getItem()->getItemId())
					{
						ItemInfo * newItem = ItemInfo::create(tmpDrop->getItem()->getItemId(), tmpDrop->getItem()->getNum() + pDrop->getItem()->getNum());
						newDrop = Drop::create(newItem);
						CCLOG("item------%d",newDrop->getItem()->getNum());
						inDropArray = true;
					}
					break;
				case drop::kDropTypeSoul:

					if(tmpDrop->getSoul()->getWid() == pDrop->getSoul()->getWid())
					{
						warrior::SoulInfo * newSoul = SoulInfo::create(tmpDrop->getSoul()->getWid(), tmpDrop->getSoul()->getNum() + pDrop->getSoul()->getNum());
						newDrop = Drop::create(newSoul);
						CCLOG("soul------%d",newDrop->getSoul()->getNum());
						inDropArray = true;
					}						
					break;
				case drop::kDropTypePower:
					newDrop = Drop::createPower(tmpDrop->getPower()+pDrop->getPower());
					inDropArray = true;
					break;
				default:
					break;
				}
				if(inDropArray)
				{
					pArray->replaceObjectAtIndex(j,newDrop);				
					break;
				}
			}
		}
	}
	
	if(!inDropArray)
	{
		pArray->addObject(pDrop);
	}
}

unsigned int DropModel::getDropCount( drop::Drop* pDropItem )
{
	if (pDropItem == NULL)
	{
		return 0;
	}

	unsigned int num = 0;
	switch (pDropItem->getType()) {
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			num = pDropItem->getItem()->getNum();
			break;
		}
	case drop::kDropTypeEquip:
		{
			num = 1;
			break;
		}
	case drop::kDropTypeSoul:
		{
			num = pDropItem->getSoul()->getNum();
			break;
		}
	case drop::kDropTypeAvatar:
		{
			num = 1;
			break;
		}
	case drop::kDropTypeWarrior:
		{
			num = 1;
			break;
		}
	case drop::kDropTypeCoin:
		{
			num = pDropItem->getCoin();
			break;
		}
	case drop::kDropTypePoint:
		{
			num = pDropItem->getPoint();
			break;
		}
	case drop::kDropTypeVip:
		{
			break;
		}
	case drop::kDropTypePower:
		{
			num = pDropItem->getPower();
			break;
		}
	default:
		{
			break;
		}
	}

	return num;
}

cocos2d::CCSprite* DropModel::getDropIconSprite( drop::Drop* pDropItem )
{
	if (pDropItem == NULL)
	{
		return NULL;
	}

	CCSprite* pIcon = NULL;
	switch (pDropItem->getType()) {
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			if (pDropItem->getItem()->getBaseInfo()->getItemType() == kItemTypeFragment)
			{
				pIcon = ResourceManager::sharedResourceManager()->getEquipIcon40P(pDropItem->getItem()->getBaseInfo()->getResID());
			}
			else
			{
				pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(pDropItem->getItem()->getItemId());
			}
			break;
		}
	case drop::kDropTypeEquip:
		{
			pIcon = ResourceManager::sharedResourceManager()->getEquipIcon40P(pDropItem->getEquip()->getEquipId());
			break;
		}
	case drop::kDropTypeSoul:
		{
			pIcon =  ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pDropItem->getSoul()->getWid());
			break;
		}
	case drop::kDropTypeAvatar:
		{
			pIcon =  ResourceManager::sharedResourceManager()->getWarriorIconForAward(pDropItem->getAvatarInfo()->getBaseInfo()->getResId());
			break;
		}
	case drop::kDropTypeWarrior:
		{
			pIcon =  ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pDropItem->getWarrior()->getWid());
			break;
		}
	case drop::kDropTypeCoin:
		{
			pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(COIN_ID);
			break;
		}
	case drop::kDropTypePoint:
		{
			pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(POINT_ID);
			break;
		}
	case drop::kDropTypeVip:
		{
			user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(pDropItem->getVip());
			item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
			pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(pItemBase->getItemId());

			break;
		}
	case drop::kDropTypePower:
		{
			pIcon = ResourceManager::sharedResourceManager()->getItemIcon40P(POWER_ID);
			break;
		}
	default:
		{
			break;
		}
	}

	return pIcon;
}


Drop::~Drop()
{
    CC_SAFE_RELEASE(m_pEquip);
    CC_SAFE_RELEASE(m_pItem);
    CC_SAFE_RELEASE(m_pWarrior);
    CC_SAFE_RELEASE(m_pSoul);
};


Drop * Drop::create(equip::EquipCurInfo * pEquip)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->init(pEquip))
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

Drop * Drop::create(item::ItemInfo * pItem)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->init(pItem))
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

Drop * Drop::create(warrior::WarriorCurInfo * pWarrior)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->init(pWarrior))
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

Drop * Drop::create(warrior::SoulInfo * pSoul)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->init(pSoul))
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

Drop * Drop::create(avatar::AvatarInfo * pAvatarInfo)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->init(pAvatarInfo))
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

Drop * Drop::createCoin(uint32_t coin)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->initCoin(coin))
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

Drop * Drop::createPoint(uint32_t Point)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->initPoint(Point))
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

Drop * Drop::createVip(uint32_t vip)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->initVip(vip))
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

Drop * Drop::createPower(uint32_t power)
{
    Drop * pRet = new Drop();
    if(pRet && pRet->initPower(power))
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

Drop * drop::Drop::createJJCScore( uint32_t score )
{
	Drop * pRet = new Drop();
	if(pRet && pRet->initJJCScore(score))
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


Drop * Drop::create(DropType eType, unsigned int dropID, unsigned int dropNum /* = 1 */)
{
	Drop *pDrop = NULL;
	switch (eType)
	{
	case kDropTypeItem:
	case kDropTypePkg:
		pDrop = Drop::create(item::ItemInfo::create(dropID, dropNum));
		break;
	case kDropTypeEquip:
		pDrop = Drop::create(equip::EquipCurInfo::create(dropID));
		break;
	case kDropTypeSoul:
		pDrop = Drop::create(warrior::SoulInfo::create(dropID, dropNum));
		break;
	case kDropTypeWarrior:
		pDrop = Drop::create(warrior::WarriorCurInfo::create(dropID));
		break;
	case kDropTypeCoin:
		pDrop = Drop::createCoin(dropNum);
		break;
	case kDropTypePoint:
		pDrop = Drop::createPoint(dropNum);
		break;
	case kDropTypeVip:
		pDrop = Drop::createVip(dropNum);
		break;
	case kDropTypePower:
		pDrop = Drop::createPower(dropNum);
		break;
	case kDropTypeAvatar:
		pDrop = Drop::create(avatar::AvatarInfo::create(dropID));
		break;
	case kDropTypeJJCScore:
		pDrop = Drop::createJJCScore(dropNum);
		break;
	case kDropTypeNone:
	default:;
	}
	return pDrop;
}


bool Drop::init(avatar::AvatarInfo * pAvatarInfo)
{
    if(pAvatarInfo == NULL)
    {
        return false;
    }
    CC_SAFE_RETAIN(pAvatarInfo);
    CC_SAFE_RELEASE(m_pAvatarInfo);
    m_eType = kDropTypeAvatar;
    m_pAvatarInfo = pAvatarInfo;
    
    return init();
};


bool Drop::init(equip::EquipCurInfo * pEquip)
{
    if(pEquip == NULL)
    {
        return false;
    }
    CC_SAFE_RETAIN(pEquip);
    CC_SAFE_RELEASE(m_pEquip);
    m_eType = kDropTypeEquip;
    m_pEquip = pEquip;
    
    return init();
};

bool Drop::init(item::ItemInfo * pItem)
{
    if(pItem == NULL)
    {
        return false;
    }
    
    CC_SAFE_RETAIN(pItem);
    CC_SAFE_RELEASE(m_pItem);
    m_eType = kDropTypeItem;
    m_pItem = pItem;
    return init();
};

bool Drop::init(warrior::WarriorCurInfo * pWarrior)
{
    if(pWarrior == NULL)
    {
        return false;
    }
    
    CC_SAFE_RETAIN(pWarrior);
    CC_SAFE_RELEASE(m_pWarrior);
    m_eType = kDropTypeWarrior;
    m_pWarrior = pWarrior;
    return init();
};

bool Drop::init(warrior::SoulInfo * pSoul)
{
    if(pSoul == NULL)
    {
        return false;
    }
    
    CC_SAFE_RETAIN(pSoul);
    CC_SAFE_RELEASE(m_pSoul);
    m_eType = kDropTypeSoul;
    m_pSoul = pSoul;
    
    return init();
};

bool Drop::initCoin(uint32_t coin)
{
    if(coin == 0)
    {
        return false;
    }
    m_eType = kDropTypeCoin;
    m_nCoin = coin;
    return init();
}

bool Drop::initPoint(uint32_t point)
{
    if(point == 0)
    {
        return false;
    }
    m_eType = kDropTypePoint;
    m_nPoint = point;
    return init();
}

bool Drop::initVip(uint32_t vip)
{
    if(vip == 0)
    {
        return false;
    }
    m_eType = kDropTypeVip;
    m_nVip = vip;
    
    return init();
}


bool Drop::initPower(uint32_t power)
{
    if(power == 0)
    {
        return false;
    }
    m_eType = kDropTypePower;
    m_nPower = power;
    
    return init();
}

bool drop::Drop::initJJCScore( uint32_t score )
{
	if (score == 0)
	{
		return false;
	}

	m_eType = kDropTypeJJCScore;
	m_nScore = score;
	return init();
}