//
//  FirstRecharge.h
//  Ê×³äµ¯¿ò
//
//  Created by fengkerong on 14-7-9.
//  Copyright (c) 2013Äê thedream. All rights reserved.
//

#ifndef __QSMY__FIRSTRECHARGE__
#define __QSMY__FIRSTRECHARGE__

#include "AppInclude.h"
#include "model/MallModel.h"
#include "Account.h"
#include "../common/common.h"
#include "../inventory/InventoryDelegate.h"
#include "../equip/EquipDelegate.h"

#define PRESENT_COUNT 4

class EquipInfoView;
class ItemInfoView;
class FirstRecharge
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public InventoryDelegate
	,public EquipDelegate
{

	CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);

public:
	FirstRecharge();
	virtual ~FirstRecharge();

	CREATE_FUNC(FirstRecharge);

	virtual bool init();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual void closeInfoView(InventoryType type);
	virtual void closeInfoView(EquipViewType type);
	virtual void showInfoView(InventoryType type,item::ItemInfo* pItem,cocos2d::CCObject* param = NULL);
	virtual void showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param = NULL);

protected:
	void rechargeCallBack(cocos2d::CCObject * obj);
	void closeCallBack(cocos2d::CCObject *obj);
	void onHeroClick(cocos2d::CCObject *obj);
	void onItemTouch(cocos2d::CCObject *obj);

	void readItemsInfo(cocos2d::CCString *str);

private:

	cocos2d::CCNode		*m_pHeroContainer;
	cocos2d::CCNode		*m_pGradeContainer;
	cocos2d::CCNode     *m_pPopupLayer;
	cocos2d::CCSprite	*m_pGrade1;
	cocos2d::CCSprite	*m_pGrade2;
	cocos2d::CCSprite	*m_pGrade3;
	cocos2d::CCSprite	*m_pGrade4;
	cocos2d::CCLabelBMFont	*m_pTime;
	ItemInfoView            *m_pItemInfoView;
	cocos2d::CCArray        *m_pArr;

	warrior::WarriorBaseInfo * pWarriorBase;

	ItemBox		*m_pItemBox[PRESENT_COUNT];

	DButton		*m_pCloseButton;
};

#endif /* defined(__QSMY__FIRSTRECHARGE__) */
