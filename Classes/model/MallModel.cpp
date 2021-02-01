//
//  MallModel.cpp
//  商城
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MallModel.h"
#include "managers/DataManager.h"

using namespace mall;



//是否满足购买条件
bool ProductInfo::isCanBuy()
{
    if(baseInfo->getReqDesc()==BUY_CONDITION_VIP)
    {
        return DM_GET_USER_MODEL->getUserInfo()->getVipLevel()>=baseInfo->getReqNum();
    }
    
    if(baseInfo->getReqDesc()==BUY_CONDITION_NUM_LESS)
    {
        return getNum() < baseInfo->getReqNum();
    }
    
    return true;
}


MallModel::MallModel()
:m_productBaseMap(NULL)
,m_pRedPacketInfoMap(NULL)
,m_productInfoVec(NULL)
,m_nUnenoughItemID(0)
{
	//init();
}

MallModel::~MallModel()
{
	CC_SAFE_RELEASE(m_productBaseMap);
	CC_SAFE_RELEASE(m_pRedPacketInfoMap);
	CC_SAFE_RELEASE(m_productInfoVec);
}


bool MallModel::init()
{
	m_productBaseMap = ProductBaseMap::create();
	m_pRedPacketInfoMap = RedPacketInfoMap::create();
	m_productInfoVec = ProductInfoVec::create();
	
	CC_SAFE_RETAIN(m_productBaseMap);
	CC_SAFE_RETAIN(m_pRedPacketInfoMap);
	CC_SAFE_RETAIN(m_productInfoVec);
	
	loadMallData();
    load200RedPacket();
    
    return Model::init();
}

void MallModel::load200RedPacket()
{
	const CSVDecoder::CSV val =  CSVDecoder::create(RED_PACKET_200_CONFIG_PATH)->getData();
	CCArray* productIds = CCArray::create();
	unsigned int lastId = 0;
	for (uint32_t i=0; i<val.size(); ++i)
	{
		unsigned int packetId = atoi(val[i][0].c_str());
		if (packetId != lastId && productIds->count() >0)
		{
            CCArray* tempArray = CCArray::create();
            CCObject* pObject = NULL;
            CCARRAY_FOREACH(productIds, pObject)
            {
                tempArray->addObject(CCInteger::create( ((CCInteger*)pObject)->getValue()));
            }
			m_pRedPacketInfoMap->setObject(tempArray, packetId);
            
			productIds->removeAllObjects();
		}

		unsigned int mallProductId = atoi(val[i][1].c_str());
		productIds->addObject(CCInteger::create(mallProductId));
		lastId = packetId;
	}
    
    if (productIds->count() > 0) {
        CCArray* tempArray = CCArray::create();
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(productIds, pObject)
        {
            tempArray->addObject(CCInteger::create( ((CCInteger*)pObject)->getValue()));
        }
        
        m_pRedPacketInfoMap->setObject(tempArray, lastId);
    }
}

void MallModel::loadMallData()
{
    parseProductsData( CSVDecoder::create(PRODUCTS_CONFIG_PATH)->getData());
	
}

void MallModel::parseProductsData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		ProductBase *pProduct = ProductBase::create();
		pProduct->pId = atoi(val[i][0].c_str());
		pProduct->type = (ProductType)atoi(val[i][1].c_str());
		pProduct->itemId = atoi(val[i][2].c_str());
		pProduct->name = val[i][3].c_str();
		pProduct->num = atoi(val[i][4].c_str());
		pProduct->req_desc = val[i][5].c_str();
		pProduct->req_num = atoi(val[i][6].c_str());
        pProduct->if_look = val[i][7].compare("1")==0;
        pProduct->old_price = atoi(val[i][8].c_str());
        pProduct->if_time = val[i][9].compare("1")==0;
        pProduct->resId = atoi(val[i][10].c_str());
		m_productBaseMap->setObject(pProduct, pProduct->pId);
        
	}
}

ProductBase * MallModel::getProductBaseByID(int pid)
{
	return (ProductBase*)m_productBaseMap->objectForKey(pid);
}


void  MallModel::getProductsByType(ProductType pType, ProductInfoVec &vec)
{
	vec.removeAllObjects();
    
	CCObject* obj = NULL;
	ProductInfo* pProduct = NULL;
    
	CCARRAY_FOREACH(m_productInfoVec, obj)
	{
		pProduct = (ProductInfo*)obj;
        
		if(pProduct->baseInfo->type == pType)
		{
			vec.addObject(pProduct);
		}
	}
}

void  MallModel::getProductsInMallByType(ProductType pType, ProductInfoVec &vec)
{
	vec.removeAllObjects();
    
    user::UserInfo* pUserInfo =DM_GET_USER_MODEL->getUserInfo();
    
	CCObject* obj = NULL;
	ProductInfo* pProduct = NULL;
    
	CCARRAY_FOREACH(m_productInfoVec, obj)
	{
		pProduct = (ProductInfo*)obj;
        
		if(pProduct->baseInfo && pProduct->baseInfo->type == pType)
		{
            if(!pProduct->baseInfo || !pProduct->baseInfo->if_look) continue;
            if(pProduct->baseInfo->req_desc=="vip" && pUserInfo->hasBoughtVipBag(pProduct->baseInfo->req_num)) continue;
			//限制礼包被查看
			if(pProduct->baseInfo->req_desc=="vip" && pProduct->baseInfo->req_num > 9 ){
				//对于大于9级的vip礼包
				if(pUserInfo->getVipLevel() + 1 < pProduct->baseInfo->req_num){
					continue;
				}				
			}

			vec.addObject(pProduct);
		}
	}
}


bool MallModel::isItemInMall(uint32_t itemId)
{
    CCObject* obj = NULL;
	ProductInfo* pProduct = NULL;
    
	CCARRAY_FOREACH(m_productInfoVec, obj)
	{
		pProduct = (ProductInfo*)obj;
        
        if(!pProduct->baseInfo || !pProduct->baseInfo->if_look) continue;
        if(pProduct->baseInfo->itemId==itemId) return true;
		
	}
    return false;
}

uint32_t MallModel::getProductPriceById(uint32_t id)
{
    mall::ProductInfo* pInfo = getProductById(id);
    CCAssert(pInfo, "");
    return pInfo->price;
}

mall::ProductInfo* MallModel::getProductById(uint32_t id)
{
    CCObject* obj = NULL;
	ProductInfo* pProduct = NULL;
    
	CCARRAY_FOREACH(m_productInfoVec, obj)
	{
		pProduct = (ProductInfo*)obj;
        
		if(pProduct->pId==id)
		{
			return pProduct;
		}
	}
    return NULL;
}

mall::ProductInfo* MallModel::getProductByItemId(uint32_t itemId)
{
	CCObject* obj = NULL;
	ProductInfo* pProduct = NULL;
	CCARRAY_FOREACH(m_productInfoVec, obj)
	{
		pProduct = (ProductInfo*)obj;

		if(pProduct->baseInfo->getItemId() == itemId)
		{
			return pProduct;
		}
	}
	return NULL;
}

void MallModel::parseProducts(const Json::Value &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		parseOneProduct(val[i]);
	}
    
	isSynced = true;
	
	postNotification(D_EVENT_MALLLIST_UPDATE);
}

unsigned int ProductInfo::getNum()
{
    uint32_t num = 0;
    switch (getBaseInfo()->getType()) {
        case kProductTypeItem:
        case kProductTypeVipPk:
            num = DM_GET_ITEM_MODEL->getItemNum(getBaseInfo()->getItemId());
            break;
        case kProductTypeEquip:
            //
            break;
        case kProductTypeAvatar:
            if(DM_GET_AVATAR_MODEL->getAvatarInfoById(getBaseInfo()->getItemId())!=NULL)num =1;
            break;
        default:
            break;
    }
    return num;
}

void MallModel::parseOneProduct(const Json::Value &val)
{
	ProductInfo * pProduct = ProductInfo::create();
    
	DJSON_UINT(pProduct->pId,val,"id");
	DJSON_UINT(pProduct->price,val,"unit_price");
    int pay_type = pProduct->pay_type;
    DJSON_UINT(pay_type,val,"pay_type");
	pProduct->pay_type = static_cast<ProductPayType>(pay_type);
    
	pProduct->baseInfo = (ProductBase*)m_productBaseMap->objectForKey(pProduct->pId);
    
    if(pProduct->baseInfo==NULL)
    {
        CCLOG("%s, %d","error product!!", pProduct->pId);
        return;
    }
    
    switch (pProduct->getBaseInfo()->getType()) {
        case kProductTypeItem:
        case kProductTypeVipPk:
            pProduct->m_sDesc = DM_GET_ITEM_MODEL->getItemBaseByID(pProduct->getBaseInfo()->getItemId())->getItemDesc();
            break;
        case kProductTypeEquip:
            pProduct->m_sDesc = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pProduct->getBaseInfo()->getItemId())->getEquipDesc();
            break;
        case kProductTypeAvatar:
            pProduct->m_sDesc = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(pProduct->getBaseInfo()->getItemId())->getDesc();
            break;
        default:
            break;
    }
    
	m_productInfoVec->addObject(pProduct);
}

void MallModel::reset()
{
    isSynced = false;
    m_productInfoVec->removeAllObjects();
}

CCArray* MallModel::getRedPacketListById( unsigned int id )
{
	return (CCArray*)(m_pRedPacketInfoMap->objectForKey(id));
}


