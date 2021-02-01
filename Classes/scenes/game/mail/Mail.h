//
//  Mail.h
//  邮件模块
//
//  Created by cll on 13-7-18.

//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__Mail__
#define __QSMY__Mail__


#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/MailModel.h"

class NavigationDelegate;

class MailListView;


class Mail
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public DTabViewDelegate
{

public:
	Mail();
	~Mail();

	virtual void onEnter();
	virtual void onExit();

	static Mail* create(NavigationDelegate* pDelegate);
	virtual bool init(NavigationDelegate* pDelegate);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual void SelectTabAtIndex(DTabView* tabView, uint32_t idx);

private:

	void updateView();
	void updateNew();
	void getNewMails();
	void closeCallBack(cocos2d::CCObject*);
	void reciveAllButtonCallBack(CCObject *pSender);
	void reciveAllRequestCallBack(const Json::Value &requestData, const Json::Value &responseData);
	void onUpdateNewMailCount();

private:
	cocos2d::CCNode				*m_pContainer;
	cocos2d::CCNode   *m_pNewMailBg;    //事件角标
	cocos2d::CCNode   *m_pNewMailAttBg; //附件角标
	cocos2d::CCLabelBMFont *m_pNewMailLabel;
	cocos2d::CCLabelBMFont *m_pNewMailAttLabel;

	cocos2d::CCNode *m_pNav;

	void callBackAll(CCObject *pSender);

	void callBackSwitch(CCObject *pSender);
private:
	MailModel					*m_pMailModel;
	NavigationDelegate			*m_pDelegate;
	MailListView				*m_pMailListView;

	cocos2d::CCMenuItemImage				*m_pReciveAllItem;
    DButton                                 *m_pCloseButton;
	//MailAwardListView *m_pMailAwardListView;
	//uint32_t interval_id;
	cocos2d::CCNode* m_pBtnBox;
	DTabView* m_pDTabView;
};

#endif /* defined(__QSMY__Mail__) */
