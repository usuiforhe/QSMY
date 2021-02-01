extern bool m_bIsfromCollect_t;
#ifndef __QSMY__PICTURECOLLECTCELL__
#define __QSMY__PICTURECOLLECTCELL__
#include "AppInclude.h"
#include "../../../model/WarriorModel.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../common/gui/WarriorTypeMark.h"
//#include "../../../components/components.h"
using namespace warrior;
class WarriorCurInfo;
class WarriorDelegate;
class PictureCollectCell 
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver	
{
	friend class PictureCollect;
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pBg_dis, m_pBg_dis);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pBg_up, m_pBg_up);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, m_pName);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pZhizi, m_pZhizi);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pZizhiNum, m_pZizhiNum);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pNew, m_pNew);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pNumber, m_pNumber);
	CC_SYNTHESIZE_READONLY(int,m_pArea,m_pArea);
	CC_SYNTHESIZE_PASS_BY_REF(int,m_nID,m_nID);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*,m_pWarriorSprite,m_pWarriorSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*,m_pMenuButton,m_pMenuButton);
	CC_SYNTHESIZE_READONLY(bool,m_bLighted,m_bLighted);
	//CC_SYNTHESIZE_PASS_BY_REF(bool,m_bFromHere2HeroInfo,m_bFromHere2HeroInfo);
public:
	static int instances;
	enum Taglist
	{
		kChooseMenuCallBack=9
	};
	CREATE_FUNC(PictureCollectCell);
	PictureCollectCell();
	~PictureCollectCell();
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	void chooseMenuCallBack(cocos2d::CCObject* obj);
	void setPictureCell(warrior::WarriorHandBookInfo* pInfo);
	
	virtual void onEnter();
	virtual void onExit();
	void setDelegate_t(WarriorDelegate* m_pWarriorDelegate);
	void whetherShow(bool m_bBool);
	void setRankNum(int num);
protected:
	WarriorDelegate* m_pWarriorDelegate_t;
	cocos2d::CCSprite *m_pProgressBar[WARRIOR_GRADE_NUM];
private:
	WarriorGradeBg* m_pWarriorGradeBg;
	WarriorTypeMark* m_pWarriorTypeMark;
};

class PictureCollectCellLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PictureCollectCellLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(PictureCollectCell);
};

#endif