//
//  MailListView.h
//  邮件列表
//
//  Created by cll on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MailList__
#define __QSMY__MailList__


#define BTN_TAG_SYS	0
#define BTN_TAG_USER	1
#define BTN_TAG_PVE	2

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/MailModel.h"

class MailListViewCell;

class MailListViewCellDelegate
{
public:
	virtual void MailListViewCellPickUp(MailListViewCell *pCell) = 0;
	virtual void DeleteMail(MailListViewCell *pCell) = 0;
};

class MailListView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public MailListViewCellDelegate
{
    
public:
	MailListView();
	~MailListView();
    
	static MailListView* create(mail::MailTag type, const cocos2d::CCSize& size);
	virtual bool init(mail::MailTag type, const cocos2d::CCSize& size);

	virtual void onEnter();
	virtual void onExit();
    
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	virtual void MailListViewCellPickUp(MailListViewCell *pCell);
	virtual void DeleteMail(MailListViewCell *pCell);
	void setType(uint8_t uType);
	void updateMailList();
private:

	
	void onAwardBut(cocos2d::CCObject*obj);
	

private:
   
	MailModel					*m_pMailModel;
	MailModel::MailList			*m_pMailList;
    
    DTableView                  *m_pListView;
    cocos2d::extension::CCData  *m_pCCData;		//条目ccb 信息
	cocos2d::extension::CCData	*m_pAttachmentCCData;
	cocos2d::extension::CCData	*m_pItemCCData;
    cocos2d::CCNode             *m_pContainer;

	uint8_t m_uType;
	std::map<unsigned int,CCSize>  m_sizeMap;
	mail::MailTag m_uTag;
};




class MailListViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{

	CC_SYNTHESIZE(float, m_fHeight, Height);
	CC_SYNTHESIZE(MailListViewCellDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(mail::MailInfo *, m_pMailInfo, MailInfo);
public:
    
	MailListViewCell();
    ~MailListViewCell();

	static MailListViewCell* create(cocos2d::extension::CCData* pData, cocos2d::extension::CCData* pItemCCData);
	virtual bool init(cocos2d::extension::CCData* pData, cocos2d::extension::CCData* pItemCCData);
	void setMailInfo(mail::MailInfo* pMailInfo);
	void setButtonTargetAndTag(CCObject* target, SEL_MenuHandler _selector, unsigned int tag);
    
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	//DTable
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	void menuCallBack(CCObject* pSender);
private:
    
	void updateCellView();
    
public:
    cocos2d::CCMenuItem    *m_pMenuItemAward;		//领奖
    cocos2d::extension::CCScale9Sprite*	m_pFateListBg;
	cocos2d::extension::CCScale9Sprite*	m_pContentBg;
	cocos2d::CCNode*									m_pTitleNode;

private:
	cocos2d::CCLabelTTF     *m_pContentTTF;			//内容
  
 //   cocos2d::CCNode         *m_pFromUser;           //是否玩家邮件
 //   cocos2d::CCNode         *m_pFromSystem;           //是否玩家邮件
	//cocos2d::CCNode			*m_pFromPve;

	/*cocos2d::CCNode			*m_pNodeAttach;
	cocos2d::CCNode			*m_pNodeAgreed;
	cocos2d::CCNode			*m_pNodeRemoved;
	cocos2d::CCNode			*m_pNodeReply;*/

	//cocos2d::extension::CCScale9Sprite *m_pS9SBack;
	//cocos2d::CCMenuItem		*m_pBtnAttach;
	//cocos2d::CCNode			*m_pNodeBottom;

	cocos2d::CCNode			*m_pNode;
	cocos2d::extension::CCData		*m_pItemCCBData;
	DButton*		m_pAttachButton;
	DButton*		m_pConfirmButton;
	cocos2d::CCLabelTTF*	m_pTimeLabel;
	cocos2d::CCNode*			m_pTableContainer;
	DTableView*					m_pItemTable;

	void attachButtonCallBack(CCObject* pSender);
	void confirmButtonCallBack(CCObject* pSender);

	/*void callBackCenter(CCObject *pSender);
	void callBackLeft(CCObject *pSender);
	void callBackRight(CCObject *pSender);*/
};


class AttachmentCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
public:
	AttachmentCell();
	~AttachmentCell();

	static AttachmentCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	void setCount(std::string countString);
	void setIcon(CCSprite* iconSprite);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
private:
	cocos2d::CCNode*			m_pNode;
	cocos2d::CCLabelTTF*	m_pCountLabel;
	cocos2d::CCNode*			m_pIconContainer;
	cocos2d::CCNode*			m_pCountNode;
};

#endif /* defined(__QSMY__MailList__) */
