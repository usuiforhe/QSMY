//
//  AvatarModel.cpp
//  QSMY
//
//  Created by heven on 14-3-7.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "AvatarModel.h"
#include "managers/DataManager.h"
#include <algorithm>

using namespace avatar;
USING_NS_CC;

#pragma mark - AvatarBaseInfo -

static int sortAvatarByIsOwn(const CCObject* p1, const CCObject* p2)
{
	return ((avatar::AvatarBaseInfo*)p1)->getIsOwn()>((avatar::AvatarBaseInfo*)p2)->getIsOwn();
};

AvatarPropertyInfo::AvatarPropertyInfo()
	:propertyType(0),
	propertyValue(0)
{
	
}

AvatarPropertyInfo::~AvatarPropertyInfo()
{

}

AvatarBaseInfo::AvatarBaseInfo()
{
    
}

AvatarBaseInfo::~AvatarBaseInfo()
{
    
}

#pragma mark - AvatarInfo -
AvatarInfo::AvatarInfo()
:m_nId(0)
,m_nLv(1)
,m_pBaseInfo(NULL)
{
    
}

AvatarInfo::~AvatarInfo()
{
    
}

AvatarInfo* AvatarInfo::create(unsigned int id)
{
    AvatarInfo* pRet = AvatarInfo::create();
    if(pRet)
    {
        pRet->m_nId = id;
        pRet->m_pBaseInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(id);
        return pRet;
    }
    return NULL;
}

void AvatarInfo::parseInfo(const Json::Value &val)
{
    DJSON_UINT(m_nId,val,"id");
    DJSON_UINT(m_nLv,val,"lv");
    m_pBaseInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(m_nId);
}

#pragma mark - AvatarModel -

AvatarModel::AvatarModel()
:m_pAvatarMap(NULL)
,m_pAvatarBaseInfoMap(NULL)
{
    
}

AvatarModel::~AvatarModel()
{
    CC_SAFE_RELEASE(m_pAvatarMap);
    CC_SAFE_RELEASE(m_pAvatarBaseInfoMap);
}

bool AvatarModel::init()
{
    m_pAvatarMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pAvatarMap);
    
    m_pAvatarBaseInfoMap = CCDictionary::create();
    CC_SAFE_RETAIN(m_pAvatarBaseInfoMap);
    
    loadItemBaseData();
    
    return Model::init();
}

void AvatarModel::loadItemBaseData()
{
    parseItemBaseData(CSVDecoder::create("data/dress/dress")->getData());
}


void AvatarModel::parseItemBaseData(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
		AvatarBaseInfo *pAvatarBaseInfo = AvatarBaseInfo::create();
        pAvatarBaseInfo->id = atoi(val[i][0].c_str());
        pAvatarBaseInfo->name = val[i][1].c_str();
        pAvatarBaseInfo->warrior_id = atoi(val[i][2].c_str());
		pAvatarBaseInfo->dressType = atoi(val[i][3].c_str());
		pAvatarBaseInfo->dressValue = atoi(val[i][4].c_str());
		//5678是备用的属性和值
		pAvatarBaseInfo->desc = val[i][9].c_str();
		pAvatarBaseInfo->dressMark = atoi(val[i][10].c_str());		
        m_pAvatarBaseInfoMap->setObject(pAvatarBaseInfo, pAvatarBaseInfo->id);
	}
}

cocos2d::CCArray* AvatarModel::findAvatarInfoForWarrior(unsigned int wid)
{
	cocos2d::CCArray* pRet = cocos2d::CCArray::create();
    
    AvatarBaseInfo* pItem = NULL;
    CCDictElement* obj = NULL;
    CCDICT_FOREACH(m_pAvatarBaseInfoMap, obj)
    {
        pItem = static_cast<AvatarBaseInfo*>(obj->getObject());
        if(pItem &&  pItem->getWarriorId()==wid)
		{
            mall::ProductInfo *  productInfo = DM_GET_MALL_MODEL->getProductByItemId(pItem->getId());
            if(productInfo)
			{
				avatar::AvatarInfo* info = getAvatarInfoById(pItem->getId());
				pItem->setIsOwn(info!=NULL ? true : false);
				pRet->addObject(pItem);
			}
		}
    }
    
   std::sort(pRet->data->arr, pRet->data->arr + pRet->data->num, sortAvatarByIsOwn);
    
    return pRet;
}


cocos2d::CCArray* AvatarModel::getPropertyList(unsigned int wid)
{
	cocos2d::CCArray* pRet = cocos2d::CCArray::create();

	cocos2d::CCArray* pAvatarList= findAvatarInfoForWarrior(wid);

	CCObject* pObj = NULL;
	AvatarBaseInfo* pItem = NULL;
	CCARRAY_FOREACH(pAvatarList, pObj)
	{
		pItem = (AvatarBaseInfo*)pObj;
		if(pItem->getIsOwn() == false)continue;

		bool bAdded = false;
		CCObject* pObjIn = NULL;
		CCARRAY_FOREACH(pRet, pObjIn)
		{
			AvatarPropertyInfo* pProperty = (AvatarPropertyInfo*)pObjIn;
			if(pProperty->propertyType == pItem->getDressType())
			{
				bAdded = true;
				pProperty->propertyValue += pItem->getDressValue();
				break;
			}
		}
		if(!bAdded)
		{
			//add to pRet
			AvatarPropertyInfo* pProperty = AvatarPropertyInfo::create();
			pProperty->propertyType = pItem->getDressType();
			pProperty->propertyValue = pItem->getDressValue();
			pRet->addObject(pProperty);
		}
		
	}

	return pRet;
}


avatar::AvatarBaseInfo* AvatarModel::getAvatarBaseInfoById(unsigned int id)
{
    return static_cast<avatar::AvatarBaseInfo*>(m_pAvatarBaseInfoMap->objectForKey(id));
}

avatar::AvatarInfo* AvatarModel::getAvatarInfoById(unsigned int id)
{
    return static_cast<avatar::AvatarInfo*>(m_pAvatarMap->objectForKey(id));
}

void AvatarModel::parseDresses(const Json::Value& val)
{
    m_pAvatarMap->removeAllObjects();
    for (uint32_t i=0; i<val.size(); ++i)
	{
		avatar::AvatarInfo* pItem = parseDress(val[i]);
        if(pItem) m_pAvatarMap->setObject(pItem, pItem->getId());
	}
    
	isSynced = true;
	postNotification(D_EVENT_AVATAR_LIST_UPDATE);
}

avatar::AvatarInfo* AvatarModel::parseDress(const Json::Value& val,avatar::AvatarInfo* pAvatar)
{
	if(pAvatar == NULL)
	{
		pAvatar = AvatarInfo::create();
	}
    
    pAvatar->parseInfo(val);
	return pAvatar;
}

void AvatarModel::addAvatar(avatar::AvatarInfo* pAvatarInfo)
{
    if(pAvatarInfo==NULL) return;
    if(m_pAvatarMap->objectForKey(pAvatarInfo->getId())!=NULL) return;
    m_pAvatarMap->setObject(pAvatarInfo, pAvatarInfo->getId());
}


avatar::AvatarInfo* AvatarModel::findAvatarForId(unsigned int aid)
{
	cocos2d::CCArray* pRet = cocos2d::CCArray::create();
    
    AvatarInfo* pItem = NULL;
    CCDictElement* obj = NULL;
    CCDICT_FOREACH(m_pAvatarMap, obj)
    {
        pItem = static_cast<AvatarInfo*>(obj->getObject());
        if(pItem && pItem->getBaseInfo() && pItem->getBaseInfo()->getId()==aid)
		{
			return pItem;
		}
    }
    
    return NULL;
}


cocos2d::CCArray* AvatarModel::findAvatarForWarrior(unsigned int wid)
{
	cocos2d::CCArray* pRet = cocos2d::CCArray::create();

    AvatarInfo* pItem = NULL;
    CCDictElement* obj = NULL;
    CCDICT_FOREACH(m_pAvatarMap, obj)
    {
        pItem = static_cast<AvatarInfo*>(obj->getObject());
        if(pItem && pItem->getBaseInfo() && pItem->getBaseInfo()->getWarriorId()==wid)
		{
			pRet->addObject(pItem);
		}
    }

    return pRet;
}

bool AvatarModel::hasAvatarOfWarrior(unsigned int wid)
{
    return findAvatarForWarrior(wid)->count() != 0;
}


