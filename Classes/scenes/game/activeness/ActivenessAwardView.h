//
//  ActivenessAwardView.h
//  活跃度领奖列表
//
//  Created by OnePiece on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ActivenessAwardView__
#define __QSMY__ActivenessAwardView__


#include "AppInclude.h"
#include "components/components.h"
#include "model/ActivenessModel.h"
#include "model/ItemModel.h"
#include "../common/AwardListView.h"

struct ActivenessDelegate;

class ActivenessAwardView
:public BaseAwardListView
{
public:
	ActivenessAwardView();
    
	virtual ~ActivenessAwardView();
    
    static ActivenessAwardView * create(activeness::AwardInfo * pAwardInfo);

    virtual void onEnter();
    
    virtual void onExit();
    
	bool init(activeness::AwardInfo * pAwardInfo);
        
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

protected:
    virtual void addChildrenBeforeList();
    
	virtual void updateAwardListView();
	void onDrawClick(cocos2d::CCObject * sender);
	void onCloseClick(cocos2d::CCObject * sender);

protected:
	cocos2d::CCLabelBMFont * m_pPoint;
    cocos2d::CCNode * m_pButtonContainer;
	DButton * m_pButton;
    
	activeness::AwardInfo * m_pAwardInfo;
	CC_SYNTHESIZE(ActivenessDelegate *, m_pDelegate, Delegate);
};


#endif /* defined(__QSMY__ActivenessAwardView__) */
