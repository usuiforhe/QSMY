//
//  EquipDelegate.h
//  QSMY
//
//  Created by wanghejun on 13-10-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EquipDelegate__
#define __QSMY__EquipDelegate__

#include "model/EquipModel.h"

enum EquipViewType {
    kEquipNone,
    kEquipInfo,
    kEquipEnhance,
	kEquipRefine,
	kEquipDecompose,
	kEquipDeletePopup,
};

class EquipDelegate {
public:
    EquipDelegate()
    :m_pEquip(NULL)
    ,m_pParam(NULL)
    ,m_type(kEquipNone)
    {
        
    }
    
    virtual ~EquipDelegate()
    {
        CC_SAFE_RELEASE(m_pEquip);
        CC_SAFE_RELEASE(m_pParam);
    }
    
	virtual void closeInfoView(EquipViewType type)
    {
        m_type = kEquipNone;
        setEquip(NULL);
        setParam(NULL);
    }
    
	virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL)
    {
        m_type = type;
        setEquip(pEquip);
        setParam(param);
    }

	virtual void onGradeSelect(std::vector<equip::EquipGrade> v)
	{
		//m_pEquipMultiSelect->onGradeSelect(v);
	}
    
protected:
    
    void setEquip(equip::EquipCurInfo* pInfo)
    {
        CC_SAFE_RETAIN(pInfo);
        CC_SAFE_RELEASE(m_pEquip);
        m_pEquip = pInfo;
    }
    
    void setParam(cocos2d::CCObject * pParam)
    {
        CC_SAFE_RETAIN(pParam);
        CC_SAFE_RELEASE(m_pParam);
        m_pParam = pParam;
    }
    
    equip::EquipCurInfo         *m_pEquip;
    cocos2d::CCObject           *m_pParam;
    EquipViewType               m_type;
};


#endif /* defined(__QSMY__EquipDelegate__) */
