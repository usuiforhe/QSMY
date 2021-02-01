//
//  DressChange.h
//  时装模块
//  Created by mzp on 14-5-15.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Dress_Change__
#define __QSMY_Dress_Change__


#include "AppInclude.h"
#include "components/components.h"
#include "model/WarriorModel.h"
#include "../equip/EquipEnhance.h"

class EquipPropertyType;
class WarriorDelegate;


class DressChange
	: public DLayer
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
{
public:
	DressChange();
	~DressChange();

	CREATE_FUNC(DressChange);
	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell);

	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView* table);

	void setWarriorInfo(warrior::WarriorCurInfo* pWarriorCurInfo);

	void setDelegate(WarriorDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

private:
	void closeCallback(cocos2d::CCObject* obj);
	void onSelectClick(cocos2d::CCObject* sender);
	void onTouchFlash(cocos2d::CCObject* sender);
	void updateView();

private:
	warrior::WarriorCurInfo* m_pWarriorInfo;
	cocos2d::CCArray* m_pDressList;
	cocos2d::CCArray* m_pPropertyList;
	WarriorDelegate* m_pDelegate;
    
    void BuyDress(MessageType type,DMessageBox* target);

    void buyCallback(const Json::Value & requestData,const Json::Value &responseData);

    
    
private:
	DButton* m_pConfirmButton;
	DButton* m_pBtnOk;
	DButton* m_pSelectButton;
	cocos2d::CCMenuItem* m_pPrev;
	cocos2d::CCMenuItem* m_pNext;
	cocos2d::CCMenuItem* m_pMenuTouch;
	cocos2d::CCNode* m_pAvatarContainer;
	cocos2d::CCNode* m_pTableContainer;
	cocos2d::CCNode* m_pPropertyTable;
	WarriorGradeBg*	m_pWarriorGradeBg;
	cocos2d::CCLabelTTF* m_pCount;
    
    cocos2d::CCNode* m_pShowDress;
    
	sp::Armature*	m_pAvatarAni;
    avatar::AvatarBaseInfo* m_pAvatar;
	DTableView* m_pListView;
	DTableView* m_pPropertyView;
	cocos2d::extension::CCData* m_pData;
	cocos2d::extension::CCData* m_pPropertyData;
};

#define SpecialDressMark	5

class DressViewCell
	: public cocos2d::extension::CCTableViewCell
	, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	DressViewCell();
	virtual ~DressViewCell();

	static DressViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setAvatarInfo(avatar::AvatarBaseInfo* pAvatarInfo);

private:
	cocos2d::CCNode* m_pContainer;
	cocos2d::CCNode* m_pBoard;
	cocos2d::CCNode* m_pShow;
	cocos2d::CCNode* m_pWillBuy;
	cocos2d::CCNode* m_pHave;
	cocos2d::CCNode* m_pShining;
	// 属性加成
	EquipTypeMark* m_pMark;
	cocos2d::CCLabelBMFont* m_pValue;

	//cocos2d::CCNode* m_pPropertyGroup;	
	//cocos2d::CCNode* m_pPropertyGroup1;
	//EquipTypeMark* m_pPropertyType1;
	//cocos2d::CCLabelBMFont* m_pProperty1;
    
	// 名字
	cocos2d::CCLabelTTF* m_pName;
    
    cocos2d::CCLabelBMFont* m_pPriceNow;
    cocos2d::CCNode*		m_pPriceNode;

    cocos2d::CCNode *m_pPrice1;
    cocos2d::CCNode *m_pOnSale;
    cocos2d::CCNode *m_pPrice;
    cocos2d::CCLabelBMFont* m_pPriceBmTf;
    cocos2d::CCLabelBMFont* m_pPriceBmTf1;

	cocos2d::CCNode	*m_pSpecialDressMark[SpecialDressMark];
};

#define DressMarkCount	10

class DressPropertyCell
	: public cocos2d::extension::CCTableViewCell
	, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	DressPropertyCell();
	virtual ~DressPropertyCell();
	static DressPropertyCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void setPropertyInfo(avatar::AvatarPropertyInfo* pInfo);
private:
	// 属性加成
	//EquipTypeMark* m_pMark;
	cocos2d::CCNode			*m_pMarkFont[DressMarkCount];
	cocos2d::CCLabelBMFont	* m_pValue;
};



#endif	// __QSMY_Dress_Change__