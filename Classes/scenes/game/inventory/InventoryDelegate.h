//
//  InventoryDelegate.h
//  QSMY
//
//  Created by wanghejun on 13-11-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__InventoryDelegate__
#define __QSMY__InventoryDelegate__

#include "model/ItemModel.h"

enum InventoryType {
    kItemNone,
    kItemInfo,
};

class InventoryDelegate {
public:
    InventoryDelegate()
    :m_pItem(NULL)
    ,m_pParam(NULL)
    ,m_type(kItemNone)
    {
        
    }
    
    virtual ~InventoryDelegate()
    {
        CC_SAFE_RELEASE(m_pItem);
        CC_SAFE_RELEASE(m_pParam);
    }
    
	virtual void closeInfoView(InventoryType type)
    {
        m_type = kItemNone;
        setItem(NULL);
        setParam(NULL);
    }
    
	virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL)
    {
        m_type = type;
        setItem(pItem);
        setParam(param);
    }

	virtual void useItem(item::ItemInfo *pItem)
	{
	}
    
protected:
    
    void setItem(item::ItemInfo* pInfo)
    {
        CC_SAFE_RETAIN(pInfo);
        CC_SAFE_RELEASE(m_pItem);
        m_pItem = pInfo;
    }
    
    void setParam(cocos2d::CCObject * pParam)
    {
        CC_SAFE_RETAIN(pParam);
        CC_SAFE_RELEASE(m_pParam);
        m_pParam = pParam;
    }
    
    item::ItemInfo              *m_pItem;
    cocos2d::CCObject           *m_pParam;
    InventoryType               m_type;
};

#endif /* defined(__QSMY__InventoryDelegate__) */
