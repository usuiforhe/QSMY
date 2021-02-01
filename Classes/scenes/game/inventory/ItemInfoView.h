//
//  ItemInfoView.h
//  道具展示
//  Created by cll on 13-4-15.
//  Copyright (c) 2013, thedream. All rights reserved.
//

#ifndef __QSMY__ITEMINFOVIEW__
#define __QSMY__ITEMINFOVIEW__

#include "AppInclude.h"
#include "components/components.h"
#include "model/ItemModel.h"
#include "../common/AwardListView.h"
#include "../pve/Pve.h"

class InventoryDelegate;
class PkgInfoView;

class ItemInfoView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
	
public:
	ItemInfoView();
	~ItemInfoView();

	CREATE_FUNC(ItemInfoView);

	virtual bool init();
    
	void setDelegate(InventoryDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	void setPveDelegate(PveDelegate* pDelegate)
	{
		m_pveDelegate = pDelegate;
	}

	void setItemInfo(item::ItemInfo * pItemInfo);
    void setItemInfo(item::ItemBase * pItemInfo);
	void setAvatarInfo(avatar::AvatarBaseInfo* pInfo);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    virtual void onEnter();
    virtual void onExit();

private:
	void useCallback(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject * obj);
    void updateView();
    void binding();
private:
	InventoryDelegate               *m_pDelegate;
	PveDelegate                     *m_pveDelegate;
	ItemModel                       *m_pItemModel;
    item::ItemInfo                  *m_pItemInfo;
private:
    DButton                         *m_pUseMenuItem;
    cocos2d::CCLabelTTF             *m_pName;
    cocos2d::CCLabelTTF             *m_pDesc;
	cocos2d::CCNode                 *m_pContainer;
    cocos2d::CCSprite               *m_pUse;
    cocos2d::CCSprite               *m_pReturn;
    
    cocos2d::CCLabelBMFont		*m_pAmount;
    cocos2d::CCNode                 *m_pAmountNode;
	DButton								*m_pCloseButton;
	WarriorGradeBg					*m_pGrade;
	cocos2d::CCNode                 *m_pCountFontNode;
	cocos2d::CCNode                 *m_pDressFontNode;
    
private:
    cocos2d::CCNode                 *m_pItemInfoView;
    PkgInfoView                     *m_pVipPkgInfoView;
    bool                            m_bUseAble;
};


//MARK:  PkgInfoView

class PkgInfoView
:public BaseAwardListView
{
    
    CC_SYNTHESIZE_READONLY(DButton*, m_pButton, Button);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pUse, Use);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pOK, OK);
    CC_SYNTHESIZE_READONLY(DButton*, m_pCloseButton, CloseButton);
	CC_SYNTHESIZE_READONLY(DButton*, m_pConfirmButton, ConfirmButton);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pAmount, Amount);

private:
    PkgInfoView();
    virtual ~PkgInfoView();
    friend class ItemInfoView;
    
    CREATE_FUNC(PkgInfoView);
	
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

    void setItemInfo(item::ItemBase * pItemInfo);
public:
    D_INIT_DEFAULT(BaseAwardListView);
    
protected:
    cocos2d::CCLabelTTF     *m_pName;
    
protected:
    virtual void addChildrenBeforeList();

private:
	cocos2d::CCNode		*m_pTitleCommon;
	cocos2d::CCNode		*m_pTitlePackage;
};


#endif /* defined(__QSMY__ITEMINFOVIEW__) */
