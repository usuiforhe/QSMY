//
//  EquipEnhance.h
//  装备强化
//  Created by cll on 13-4-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__EquipEnhance__
#define __QSMY__EquipEnhance__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../common/common.h"
#include "../../../model/EquipModel.h"
#include "../../../model/BurnModel.h"
#include "DragonBone.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../burn/TreasureSelectView.h"

class FormationDelegate;
class EquipDelegate;
class EnhanceEffect;
class EquipTypeMark;
class EquipEnhance
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public	TreasureSelectDelegate
{

public:
	EquipEnhance();
	~EquipEnhance();

	CREATE_FUNC(EquipEnhance);

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	void setDelegate(EquipDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	//设置阵容信息
	void setFormationInfo(FormationDelegate* pDelegate)
	{
		m_pFormationDelegate = pDelegate;
	}

	void setEquip(equip::EquipCurInfo*);
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	
	virtual void setSelectedTreasureList(cocos2d::CCArray * pList);
	
	void enhanceCallBack(cocos2d::CCObject * obj);

	void playEffect(cocos2d::CCObject* pEffect);

private:
	void touchCallBack(cocos2d::CCObject* obj);
	void loadLayer();
	void updateEquipInfo();
	void doEnhanceEquipCallBack(const Json::Value & requestData,const Json::Value &responseData);
	void doAutoEnhanceCallBack(const Json::Value & requestData,const Json::Value &responseData);
    /*void enhanceCallBack(cocos2d::CCObject * obj);*/
    void autoEnhanceCallBack(cocos2d::CCObject * obj);
    void closeCallBack(cocos2d::CCObject* obj);
	void pauseCallBack(cocos2d::CCObject* obj);
    void binding();
	void treasureBurnCallBack(cocos2d::CCObject * pObj);
	void treasureAutoCallBack(cocos2d::CCObject * pObj);
	void onAutoList();
	//预览闪闪闪
	void updatePreview();
	//数字闪起来
	void doShine(unsigned int levelTo);
	void shining(float val);
private:
    uint32_t m_pCurrentEquipLv;
    uint32_t m_pCurrentCoin;
	uint32_t m_totalLv;
    TreasureSelectView	*m_pTreasurePopup;
	void showTreasureSelectList();
	uint8_t	m_uClickIndex;

private:
	equip::EquipCurInfo *m_pEquipCurInfo;
	EquipDelegate * m_pDelegate;
	FormationDelegate *m_pFormationDelegate;
	EquipModel * m_pEquipModel;
    
private:
    
	EquipBox	*m_pEquipBox[TREASURE_BURN_MAX];
    DButton     *m_pAutoEnhanceButton;
    DButton     *m_pEnhanceButton;
	DButton     *m_pCloseButton;
    
    cocos2d::CCNode         *m_pContainer;
    
    WarriorGradeBg       *m_pGrade;
	cocos2d::CCLabelTTF     *m_pName;
    cocos2d::CCLabelBMFont  *m_pLevel;
	cocos2d::CCNode	*m_pUpLevelNode;
    
    cocos2d::CCLabelBMFont  *m_pProperty;
    cocos2d::CCLabelBMFont  *m_pPropertyResult;
    
    ItemBox                 *m_pItemBox;
    cocos2d::CCLabelTTF     *m_pItemName;
    cocos2d::CCLabelBMFont  *m_pItemNeed;
	cocos2d::CCLabelBMFont  *m_pMyCoin;
    cocos2d::CCLabelBMFont  *m_pItemCount;
    cocos2d::CCLabelBMFont  *m_pVipLvNeed;

	cocos2d::CCNode				*m_pPositionNode;
	cocos2d::CCNode				*m_pTreasureNode;
	cocos2d::CCLabelBMFont	*m_pTreasureCost;
	//cocos2d::CCSprite  *m_pPropertyNode;
	//宝物用
	cocos2d::CCLabelBMFont  *m_pResist;
	cocos2d::CCLabelBMFont  *m_pResistResult;
	EquipTypeMark			*m_pTypeLeft;
	EquipTypeMark			*m_pTypeRight;
	cocos2d::CCLabelTTF     *m_pLevelDesc5;
	cocos2d::CCLabelTTF     *m_pLevelDesc10;
	cocos2d::CCNode         *m_pLevelActive5;
	cocos2d::CCNode         *m_pLevelActive10;
	cocos2d::CCNode         *m_pLevel5;
	cocos2d::CCNode         *m_pLevel10;
	DButton     *m_pButtonLeft;
	DButton     *m_pButtonMiddle;
	DButton     *m_pButtonRight;
	DProgressBar*m_pProgressBar[WARRIOR_GRADE_NUM];

	cocos2d::CCLabelBMFont  *m_pLevelShine;
};

class EnhanceEffect : public DEffectNode
{
public:
    EnhanceEffect();
    ~EnhanceEffect();
    
    static EnhanceEffect* create(uint32_t lv , cocos2d::CCPoint pos , bool isAuto = true);
    virtual bool init(uint32_t lv);

	//默认是不可以点击的！设置后可以点击，区别自动强化和单次强化
	virtual void registerWithTouchDispatcher()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this, iPriority, true );
	}
private:
    sp::Armature* m_pEnhance;
	cocos2d::CCPoint pos;

	uint32_t index;
	int iPriority;
private:
    
    void animateEnd(sp::Armature * target, const char* event,const char* data);
    void showEnhance(sp::Armature * target, const char* event,const char* data);
    
};


class EquipTypeMark
	:public cocos2d::CCNode
	, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	EquipTypeMark();
	~EquipTypeMark();
	CREATE_FUNC( EquipTypeMark ) ;
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable( cocos2d::CCObject *pTarget,const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;
	void setEquipType(equip::EquipPropType val);
	void setEquipType(equip::TreasurePropType val);
private:
	CCNode *m_pMark[equip::kEquipPropTotal];
};

class EquipTypeMarkLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(EquipTypeMarkLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(EquipTypeMark);
};
#endif /* defined(__QSMY__EquipEnhance__) */


