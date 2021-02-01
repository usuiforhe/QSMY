//
//  ItemModel.cpp
//  QSMY
//
//  Created by cll on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ItemModel.h"
#include "managers/GameManager.h"
#include "managers/AnalyticsManager.h"
#include "../utils/StringUtil.h"

using namespace item;
USING_NS_CC;

ItemInfo* ItemInfo::create(ItemBase* pItem)
{
    return ItemInfo::create(pItem->getItemId(), 0);
}

ItemInfo* ItemInfo::create(unsigned int id,unsigned int num)
{
    ItemInfo* pRet = ItemInfo::create();
    if(pRet)
    {
        pRet->itemId = id;
        pRet->num = num;
        pRet->baseInfo = DM_GET_ITEM_MODEL->getItemBaseByID(id);
        return pRet;
    }
    return NULL;
}

bool ItemInfo::isItemForRename()
{
	return itemId == ITEM_CHANGE_NAME_ID;
}

#pragma mark - ItemModel -


ItemModel::ItemModel()
:m_itemBaseMap(NULL)
,m_itemInfoVec(NULL)
,m_pVipPkgBaseMap(NULL)
,m_pDictUnenough(NULL)
,m_ItemColorMap(NULL)
{
	
}

ItemModel::~ItemModel()
{
	CC_SAFE_RELEASE(m_itemBaseMap);
	CC_SAFE_RELEASE(m_itemInfoVec);
    CC_SAFE_RELEASE(m_pVipPkgBaseMap);
	CC_SAFE_RELEASE(m_pDictUnenough);
    CC_SAFE_RELEASE(m_ItemColorMap);
}


bool ItemModel::init()
{
	m_itemBaseMap       = CCDictionary::create();
    m_pVipPkgBaseMap    = CCDictionary::create();
	m_itemInfoVec       = ItemInfoVec::create();
	m_pDictUnenough		= CCDictionary::create();
    m_ItemColorMap      = CCDictionary::create();

	CC_SAFE_RETAIN(m_itemBaseMap);
	CC_SAFE_RETAIN(m_itemInfoVec);
    CC_SAFE_RETAIN(m_pVipPkgBaseMap);
	CC_SAFE_RETAIN(m_pDictUnenough);
    CC_SAFE_RETAIN(m_ItemColorMap);

	loadItemBaseData();
    return Model::init();
}

void ItemModel::reset()
{
    if(m_itemInfoVec) m_itemInfoVec->removeAllObjects();
    Model::reset();
}

void ItemModel::loadItemBaseData()
{
    loadColorConfig( CSVDecoder::create(ITEM_COLOR_CONFIG_PATH)->getData());
    parseItemBaseData( CSVDecoder::create(ITEMS_CONFIG_PATH)->getData());
    parseVipPkgData( CSVDecoder::create(VIP_BOX_CONFIG_PATH)->getData());
	loadUnenoughData( CSVDecoder::create(ITEM_UNENOUGH_CONFIG_PATH)->getData());
}

void ItemModel::loadColorConfig(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        ItemColorConfig *pItemColorConfig = ItemColorConfig::create();
        ItemGrade grade = (ItemGrade)atoi(val[i][0].c_str());
		uint32_t color = 0;
        sscanf(val[i][1].c_str(), "%i",&color);
        pItemColorConfig->color = cocos2d::ccc3(color>>16&0xFF, color>>8&0xFF, color>>0&0xFF);
        m_ItemColorMap->setObject(pItemColorConfig, grade);
	}
}

void ItemModel::parseVipPkgData(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        uint32_t id = atoi(val[i][0].c_str());
		VipPkg *pItem = static_cast<VipPkg *>(m_pVipPkgBaseMap->objectForKey(id));
        if(pItem==NULL)
        {
           pItem= VipPkg::create();
           m_pVipPkgBaseMap->setObject(pItem, id);
        }
		pItem->id = id;
        VipPkg::VipPkgItem item =
        {
            (drop::DropType)atoi(val[i][1].c_str()),
            atoi(val[i][2].c_str()),
            atoi(val[i][3].c_str())
        };
        
        pItem->items.push_back(item);
		
	}
}

void ItemModel::parseItemBaseData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i) 
	{
		ItemBase *pItem = ItemBase::create();
		pItem->item_id = atoi(val[i][0].c_str());
		pItem->item_type = (ItemType)atoi(val[i][1].c_str());
		pItem->item_name = val[i][2].c_str();
		pItem->item_desc = val[i][3].c_str();
		pItem->can_be_used = val[i][4].compare("1")==0;
		pItem->use_item_id = atoi(val[i][5].c_str());
		pItem->use_item_num = atoi(val[i][6].c_str());
		pItem->order_priority = atoi(val[i][7].c_str());
        pItem->m_uResID = atoi(val[i][8].c_str());
        pItem->m_uSheet = atoi(val[i][9].c_str());
        pItem->item_grade = (ItemGrade)atoi(val[i][10].c_str());
		pItem->m_bShow = (bool)atoi(val[i][11].c_str());
        m_Sheets.insert(pItem->m_uSheet);
        replace(pItem->item_desc,"\\n","\n");
		m_itemBaseMap->setObject(pItem, pItem->item_id);

	}
}


ItemInfo* ItemModel::getItemInfoByID(unsigned int sid)
{
	
	CCObject *obj = NULL;
	ItemInfo * pItemInfo = NULL;

	CCARRAY_FOREACH(m_itemInfoVec, obj)
	{
		pItemInfo = (ItemInfo*) obj;

		if(pItemInfo->itemId == sid)
		{
			return pItemInfo;
		}
	}
	return NULL;
}

ItemBase * ItemModel::getItemBaseByID(unsigned int sid)
{
    return (ItemBase*)m_itemBaseMap->objectForKey(sid);
}

uint32_t ItemModel::getResIDByID(uint32_t sid)
{
	ItemBase* pBaseInfo = getItemBaseByID(sid);
	if (pBaseInfo == NULL)
	{
		CCLog("error item id: %d.", sid);
		return 0;
	}

	return  pBaseInfo->m_uResID;	
}

void ItemModel::changeItemNumByID(unsigned int sid,int num)
{
    if(num>0)
    {
        addItemNumByID(sid,num);
    }else
    {
        reduceItemNumByID(sid,abs(num));
    }
}


/*减少道具数目*/
void ItemModel::reduceItemNumByID(unsigned int sid, unsigned int num)
{
	
	ItemInfo * useItem = getItemInfoByID(sid);

	if(!useItem)
	{
		CCLOG("%s:%d","找不到道具信息",sid);
		return;
	}
	
	//更新使用条件道具数目
	if(useItem->baseInfo && useItem->baseInfo->use_item_id > 0)
	{
        if(useItem->getItemId()==useItem->baseInfo->use_item_id)
        {
            num = useItem->baseInfo->use_item_num; //如果是本身需要多少个才能使用 则更正使用数量
        }else
        {
            reduceItemNumByID(useItem->baseInfo->use_item_id, useItem->baseInfo->use_item_num*num);
        }
	}

	CCObject* obj = NULL;
	ItemInfo* pItemInfo = NULL;

	CCARRAY_FOREACH(m_itemInfoVec, obj)
	{
		pItemInfo = (ItemInfo*) obj;

		if(pItemInfo->itemId == sid)
		{
			if (num > pItemInfo->num)
				pItemInfo->num = 0;
			else
				pItemInfo->num -= num;

			pItemInfo->postNotification(D_EVENT_ITEMINFO_UPDATE);

			if (pItemInfo->num == 0)
			{
				m_itemInfoVec->removeObject(pItemInfo);
                postNotification(D_EVENT_ITEMLIST_UPDATE);
			}
		}
	}

}


/*增加道具数目*/
void ItemModel::addItemNumByID(unsigned int sid, unsigned int num)
{
	CCObject* obj = NULL;
	ItemInfo* pItemInfo = NULL;

	CCARRAY_FOREACH(m_itemInfoVec, obj)
	{
		pItemInfo = (ItemInfo*) obj;

		if(pItemInfo->itemId == sid)
		{
			pItemInfo->num += num;
			pItemInfo->postNotification(D_EVENT_ITEMINFO_UPDATE);
			//postNotification(D_EVENT_ITEMLIST_UPDATE);
			return;
		}
	}
	pItemInfo = ItemInfo::create();
	pItemInfo->itemId = sid;
	pItemInfo->num = num;
	pItemInfo->baseInfo = (ItemBase*)m_itemBaseMap->objectForKey(pItemInfo->itemId);
    addItemInorder(pItemInfo);
	pItemInfo->postNotification(D_EVENT_ITEMINFO_UPDATE);
	postNotification(D_EVENT_ITEMLIST_UPDATE);
}


void ItemModel::parseItems(const Json::Value &val)
{
	for (uint32_t i=0; i<val.size(); ++i) 
	{
		parseOneItem(val[i]);
	}

	isSynced = true;

	postNotification(D_EVENT_ITEMLIST_UPDATE);
}

void ItemModel::parseOneItem(const Json::Value &val)
{
	ItemInfo *pItemInfo = getItemInfoByID(val["itemId"].asUInt());
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	int oldNum = 0;
	if (NULL != pItemInfo)
	{
		oldNum = pItemInfo->getNum();
	}
/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/	
	if(!pItemInfo)
	{
		pItemInfo = parseItem(val);
		if (pItemInfo)
		{
			
            pItemInfo->isSynced = true;
            addItemInorder(pItemInfo);
            postNotification(D_EVENT_ITEMLIST_UPDATE);
		}
	}
	else
	{
		parseItem(val, pItemInfo);
		pItemInfo->isSynced = true;
		pItemInfo->postNotification(D_EVENT_ITEMINFO_UPDATE);
        if(pItemInfo->num==0)
        {
            m_itemInfoVec->removeObject(pItemInfo);
            postNotification(D_EVENT_ITEMLIST_UPDATE);
			pItemInfo = NULL;
        }
	}
/************************************************************************/
/* for analytics begin                                                  */
/************************************************************************/
	int newNum = 0;
	if (NULL != pItemInfo)
	{
		newNum = pItemInfo->getNum();
	}
	else
	{
		return;
	}
	AnalyticsManager *pAnalytics = AnalyticsManager::sharedAnalyticsManager();
	if (newNum > oldNum)
	{
		pAnalytics->dropItem(pItemInfo->getBaseInfo()->getItemName().c_str(), newNum - oldNum);
	}
	else if (newNum < oldNum)
	{
		pAnalytics->useItem(pItemInfo->getBaseInfo()->getItemName().c_str(), oldNum - newNum);
	}

/************************************************************************/
/*  for analytics end                                                   */
/************************************************************************/		
	
}



ItemInfo* ItemModel::parseItem(const Json::Value &val, ItemInfo *pItemInfo)
{
	if(pItemInfo == NULL)
	{
		pItemInfo = ItemInfo::create();
	}

	DJSON_UINT(pItemInfo->itemId,val,"itemId");
	DJSON_UINT(pItemInfo->num,val,"num");

	pItemInfo->baseInfo = (ItemBase*)m_itemBaseMap->objectForKey(pItemInfo->itemId);

	//错误数据
	if (!pItemInfo->baseInfo)
	{
		CCLOG("item info error");
		pItemInfo = NULL;
	}
	
	return pItemInfo;
}

/* 體力類道具使用返回提示*/
void ItemModel::addPow(item::ItemInfo* pInfo , bool bAllStarAward)
{
	//暫不讀取返回結果 直接語言包返回提示

	//弹出提示
	if(bAllStarAward)
	{
		Tips_Alert(D_LOCAL_STRING("$quansanxingjiangli").c_str());
	}
	else if(pInfo){
		//大丹小丹提示语句不同
		unsigned int count = 0;
		if(pInfo->getBaseInfo()->getItemId() == 10021) count = 20;
		if(pInfo->getBaseInfo()->getItemId() == 10149) count = 5;		
		Tips_Alert(D_LOCAL_STRING("$usetips_10021" , count).c_str());
	}
	
	postNotification(D_EVENT_ADD_POW);
}

/* 耐力類道具使用返回提示*/
void ItemModel::addEndu(item::ItemInfo* pInfo)
{
	//暫不讀取返回結果 直接語言包返回提示
	unsigned int count = 0;
	if(pInfo && pInfo->getBaseInfo()->getItemId() == 10022) count = 20;
	if(pInfo && pInfo->getBaseInfo()->getItemId() == 10150) count = 2;
	Tips_Alert(D_LOCAL_STRING("$usetips_10022" , count).c_str());
	postNotification(D_EVENT_ADD_ENDU);
}

void ItemModel::addItemInorder(item::ItemInfo *pItem)
{
    item::ItemInfo * pItemInfo = NULL;
    uint32_t i = 0;
    for (; i<m_itemInfoVec->count(); ++i)
    {
        
        pItemInfo = (item::ItemInfo *)m_itemInfoVec->objectAtIndex(i);
        
        if (pItemInfo->baseInfo->item_grade < pItem->baseInfo->item_grade )
        {
            break;
        }else if( pItemInfo->baseInfo->item_grade > pItem->baseInfo->item_grade )
        {
            continue;
        }//相等比下一项
        
        if (pItemInfo->baseInfo->order_priority < pItem->baseInfo->order_priority)
        {
            break;
        }else if (pItemInfo->baseInfo->order_priority > pItem->baseInfo->order_priority)
		{
			continue;
		}
        
        if (pItemInfo->baseInfo->item_id < pItem->baseInfo->item_id)
        {
            break;
        }else if (pItemInfo->baseInfo->item_id > pItem->baseInfo->item_id)
        {
            continue;
        }
        
        break;//相等退出
    }
    
    m_itemInfoVec->insertObject(pItem,i);
}

cocos2d::CCArray* ItemModel::getVipPkgInfo(uint32_t itemId)
{
    VipPkg* pInfo = static_cast<VipPkg*>(m_pVipPkgBaseMap->objectForKey(itemId));
    if(pInfo==NULL) return cocos2d::CCArray::create();
    std::vector<VipPkg::VipPkgItem> &items = pInfo->getItems();
    cocos2d::CCArray* pRet = cocos2d::CCArray::createWithCapacity(items.size());
    std::vector<VipPkg::VipPkgItem>::iterator itr;
    Json::Value root;
    for (itr=items.begin(); itr!=items.end(); ++itr)
    {
        root["type"] = (*itr).type;
        switch ((*itr).type)
        {
            case drop::kDropTypeItem:
                root["item"]["itemId"] =(*itr).id;
                root["item"]["num"] =(*itr).count;
                break;
            case drop::kDropTypePkg:
                root["item"]["itemId"] =(*itr).id;
                root["item"]["num"] =(*itr).count;
                break;
            case drop::kDropTypeEquip:
                root["equip"]["eid"] =(*itr).id;
                root["equip"]["num"] =(*itr).count;
                break;
            case drop::kDropTypeSoul:
                root["soul"]["wid"] =(*itr).id;
                root["soul"]["num"] =(*itr).count;
                break;
            case drop::kDropTypeWarrior:
                root["warrior"]["wid"] =(*itr).id;
                root["warrior"]["num"] =(*itr).count;
                break;
            case drop::kDropTypeCoin:
                root["coin"] =(*itr).count;
                break;
            case drop::kDropTypePoint:
                root["point"] =(*itr).count;
                break;
            default:
                break;
        }
        drop::Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(root);
        if(pDrop) pRet->addObject(pDrop);
    }
    return pRet;
}

bool ItemModel::isMyItem(item::ItemInfo* pItem)
{
    return m_itemInfoVec->indexOfObject(pItem)!= CC_INVALID_INDEX;
}

void ItemModel::useItem(unsigned int sid)
{
	useItem(getItemInfoByID(sid));
}

void ItemModel::useItem(item::ItemInfo * pItemInfo,uint32_t num)
{
	if(pItemInfo==NULL) return;
	/*判断使用条件是否符合*/
	if (pItemInfo->getBaseInfo()->getUseItemId() > 0)
	{
		item::ItemInfo *pItem = DM_GET_ITEM_MODEL->getItemInfoByID(pItemInfo->getBaseInfo()->getUseItemId());
		if(!pItem || pItemInfo->getBaseInfo()->getUseItemNum() > pItem->getNum())
		{
			NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS,CCInteger::create(pItemInfo->getBaseInfo()->getUseItemId()));
		}
		else
			HTTP_CLIENT->useItem(pItemInfo->getItemId(),num);
	}
	else
	{
		HTTP_CLIENT->useItem(pItemInfo->getItemId(),num);
	}
	
}

unsigned int  ItemModel::getAvailableEquipNum()
{
	unsigned int num = 0;

	CCObject* obj = NULL;
	ItemInfo* pItem = NULL;
	CCARRAY_FOREACH(m_itemInfoVec, obj)
	{
		pItem = static_cast<ItemInfo*>(obj);
		if(pItem && pItem->baseInfo && pItem->baseInfo->getItemType() == item::kItemTypeFragment)
		{
			//碎片
			if(pItem->getNum() >= pItem->getBaseInfo()->getUseItemNum()) num++;
		}
	}

	return num;
}

void ItemModel::loadUnenoughData(const CSVDecoder::CSV &val)
{

	for (uint32_t i=0; i<val.size(); ++i) 
	{
		ItemUnenoughInfo *pInfo = ItemUnenoughInfo::create();
		pInfo->m_nItemID = atoi(val[i][0].c_str());
		pInfo->m_eAlertType = (UnenoughAlertType)atoi(val[i][1].c_str());
		pInfo->m_sContent = val[i][2];
		//pInfo->m_nProductID = atoi(val[i][3].c_str());
		m_pDictUnenough->setObject(pInfo, pInfo->m_nItemID);
	}
}

ItemUnenoughInfo *ItemModel::getUnenoughInfo(uint32_t itemID)
{
	ItemUnenoughInfo *pInfo = (ItemUnenoughInfo *)m_pDictUnenough->objectForKey(itemID);
	if (!pInfo)
	{
		pInfo = (ItemUnenoughInfo *)m_pDictUnenough->objectForKey(0);
	}
	return pInfo;
}

void ItemModel::getItemsBySheet(uint32_t sheet,cocos2d::CCArray* array)
{
    if(array==NULL) return;
    array->removeAllObjects();
    CCObject* obj = NULL;
    ItemInfo* pItem = NULL;
    CCARRAY_FOREACH(m_itemInfoVec, obj)
    {
        pItem = static_cast<ItemInfo*>(obj);
        if(pItem && pItem->baseInfo && pItem->baseInfo->getSheet()==sheet && pItem->baseInfo->m_bShow)
        {
            array->addObject(pItem);
        }
    }
}

cocos2d::ccColor3B ItemModel::getItemColorByGrade(item::ItemGrade grade)
{
    item::ItemColorConfig* pItemColorConfig = (item::ItemColorConfig*)m_ItemColorMap->objectForKey(grade);
    return pItemColorConfig?pItemColorConfig->color:cocos2d::ccc3(0,0,0);
}

