//
//  TransForce.h
//  弟子传功界面
//  Created by cll on 13-3-28.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__TRANSFORCE__
#define __QSMY__TRANSFORCE__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "TransForceHead.h"
#include "../common/common.h"
#include "WarriorListView.h"
#include "DragonBone.h"

#define USE_ITEM_TYPES 2

class WarriorDelegate;

class TransForceWarriorFilter
: public WarriorFilterDelegate
{
public:
    TransForceWarriorFilter(uint32_t ignoreWarriorId):WarriorFilterDelegate(), m_ignoreWarriorId(ignoreWarriorId){};
    virtual ~TransForceWarriorFilter(){};
    virtual bool operator()(warrior::WarriorCurInfo * pWarrior);
    
private:
    uint32_t m_ignoreWarriorId;
};

class TransForceView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public WarriorSelectDelegate
{

public:
	TransForceView();
	virtual ~TransForceView();

	CREATE_FUNC(TransForceView);

	virtual bool init();

	virtual void onEnter(); 

	virtual void onExit();

	void setWarrior(warrior::WarriorCurInfo * pWarrior);

	void setFromWarriorID(const uint32_t &wid);

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    
    virtual void setSelectedWarrior(warrior::WarriorCurInfo * pWarrior)
    {
        if(pWarrior != NULL)
        {
            setFromWarriorID(pWarrior->getWid());
        }
    }
	virtual void setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType){}
    
private:

	void binding();
	void updateTransForce();
    void updateTransForceFrom();
    //void updateButtonStatus();

    
	void submitCallBack(const Json::Value &requestData,const Json::Value &responseData);
	void selectCallBack(cocos2d::CCObject * obj);
    void submitCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
private:
	WarriorDelegate * m_pDelegate;
    warrior::WarriorCurInfo * m_pWarriorInfo;
	warrior::WarriorCurInfo * m_pWarriorInfoFrom;
	WarriorModel * m_pWarriorModel;
private:
    sp::Armature                *m_pSelectFromEffect;
    cocos2d::CCNode             *m_pSelectFrom;
    DButton                     *m_pSubmitButton[USE_ITEM_TYPES];
    cocos2d::CCLabelBMFont      *m_pItemCount[USE_ITEM_TYPES];
    ItemBox                     *m_pItemBox[USE_ITEM_TYPES];
    TransForceHead              *m_pFrom;
    TransForceHead              *m_pTo;

	void updateTransForceExp();
	cocos2d::CCLabelBMFont		*m_pLabelExpNormal;
	cocos2d::CCLabelBMFont		*m_pLabelLvNormal;
	cocos2d::CCLabelBMFont		*m_pLabelSkillNormal;
	cocos2d::CCLabelBMFont		*m_pLabelNumNormal;
	cocos2d::CCLabelBMFont		*m_pLabelExpGreat;
	cocos2d::CCLabelBMFont		*m_pLabelLvGreat;
	cocos2d::CCLabelBMFont		*m_pLabelSkillGreat;
	cocos2d::CCLabelBMFont		*m_pLabelNumGreat;

	uint32_t items[USE_ITEM_TYPES];
	uint32_t types[USE_ITEM_TYPES];
};



#endif /* defined(__QSMY__WARRIORINFO__) */
