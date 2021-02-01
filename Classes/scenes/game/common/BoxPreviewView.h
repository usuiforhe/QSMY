//
//  BoxPreviewView.h
//  宝箱预览
//
//  Created by Eci on 13-11-15.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BoxPreviewView__
#define __QSMY__BoxPreviewView__

#include "AwardListView.h"

enum BoxViewLabelType
{
	kCheck,
	kCongretulation,
};

class BoxPreviewView : public BaseAwardListView
{

public:

	static BoxPreviewView *create(cocos2d::CCArray * pAwardList);

	BoxPreviewView();

	virtual ~BoxPreviewView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual bool init(cocos2d::CCArray * pAwardList);

	void setAwardName(const char *pName);

	void showLabel(BoxViewLabelType);

protected:

	virtual void addChildrenBeforeList();
	void closeCallBack(cocos2d::CCObject*obj);

	DButton				*m_pButton;
	DButton				*m_pButtonClose;
	cocos2d::CCLabelTTF	*m_pLabelName;
	DButton				*m_pButtonToHide;
	cocos2d::CCNode		*m_pNodeToHide;
	cocos2d::CCNode		*m_pLabelToHide;
	cocos2d::CCNode		*m_pCheck;
	cocos2d::CCNode		*m_pTitleCommon;
};



#endif /* defined(__QSMY__BoxPreviewView__) */