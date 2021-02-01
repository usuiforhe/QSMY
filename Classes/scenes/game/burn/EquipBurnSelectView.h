//
//  EquipBurnSelectView.h
//  宝物选择
//
//

#ifndef __QSMY__EquipBurnSelectView__
#define __QSMY__EquipBurnSelectView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../equip/EquipMultiSelectView.h"

struct EquipBurnDelegate
{
	virtual void onSelectConfirm(){};
};

class EquipBurnSelectView
: public DLayer
, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    EquipBurnSelectView();    
    ~EquipBurnSelectView();   
    static EquipBurnSelectView * create();   
    virtual bool init();    
    virtual void onEnter();    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	inline void setDelegate(EquipBurnDelegate * pDelegate)
	{
		m_pDelegate = pDelegate;
	}
	//当前要升级的宝物ID（避免被选择）
	CC_SYNTHESIZE(unsigned int,m_uCurrentID,CurrentID);

protected:
    void onConfirmClick(cocos2d::CCObject * sender);
    void onBackClick(cocos2d::CCObject * sender);
    
protected:

    cocos2d::CCNode * m_pContainer;
	cocos2d::CCNode * m_pTabContainer;
    DButton * m_pConfirmButton;
    DButton * m_pBackButton;
	cocos2d::CCNode	*m_pTitleTreasure;
	cocos2d::CCNode	*m_pTitleHero;
	cocos2d::CCNode	*m_pTitleEquip;

	cocos2d::extension::CCData* m_pData;

	EquipBurnDelegate *m_pDelegate;
private:
	DTabView                *m_pDTabView;
};

#endif /* defined(__QSMY__EquipBurnSelectView__) */
