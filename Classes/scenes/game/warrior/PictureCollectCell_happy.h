#ifndef __QSMY__PICTURECOLLECTCELLHAPPY__
#define __QSMY__PICTURECOLLECTCELLHAPPY__
#include "AppInclude.h"
//#include "../../../model/WarriorModel.h"
#include "../common/gui/WarriorGradeBg.h"
#include "../common/gui/WarriorTypeMark.h"
#include "PictureCollectCell.h"
#include "../common/common.h"
#define NUM_OF_CELLS 6
class PictureCollectCell;
class WarriorDelegate;
using namespace warrior;
class PictureCollectCell_happy
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver	
{
public:
	static PictureCollectCell_happy * create(cocos2d::extension::CCData * pData,uint32_t pid);

	PictureCollectCell_happy();

	~PictureCollectCell_happy();

	virtual bool init(cocos2d::extension::CCData * pData,uint32_t pid);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    void fillAll(std::vector<warrior::WarriorHandBookInfo*> m_Vector);

	void chooseMenuCallBack(cocos2d::CCObject* obj);

	void setDelegate_happy(WarriorDelegate* m_pWarriorDelegate_happy);

public:
	uint32_t getPid();
	PictureCollectCell* getCell(int id);
public:
	/*PictureCollectCell* m_pHero1;
	PictureCollectCell* m_pHero2;
	PictureCollectCell* m_pHero3;
	PictureCollectCell* m_pHero4;
	PictureCollectCell* m_pHero5;
	PictureCollectCell* m_pHero6;*/
	std::vector<PictureCollectCell*> m_Vector;
	WarriorDelegate* m_pWarriorDelegate_happy;
	static std::vector<cocos2d::CCSprite*>  m_Vector_sprite_t;
private:
	uint32_t m_nPid;
};


#endif