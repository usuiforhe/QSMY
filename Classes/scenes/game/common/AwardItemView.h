#ifndef __QSMY__AwardItemView__
#define __QSMY__AwardItemView__

#include "AppInclude.h"
#include "model/EquipModel.h"
#include "model/ItemModel.h"
#include "model/WarriorModel.h"
#include "model/DropModel.h"
#include "model/AvatarModel.h"
#include "DragonBone.h"
#include "common.h"

class WarriorGradeBg;

class AwardItemView
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	AwardItemView();
	virtual ~AwardItemView();

	virtual bool init(drop::Drop *obj);
	virtual bool setItemInfo(item::ItemInfo *obj);
    virtual bool setAvatarInfo(avatar::AvatarInfo *obj);
	virtual bool setEquipInfo(equip::EquipCurInfo *obj);
	virtual bool setWarriorInfo(warrior::WarriorCurInfo *obj);
	virtual bool setSoulInfo(warrior::SoulInfo *obj);
	virtual bool setPointInfo(uint32_t num);
	virtual bool setCoinInfo(uint32_t num);
	virtual bool setVipInfo(uint32_t level);
	virtual bool setPowerInfo(uint32_t num);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
protected:
	void closeCallBack(cocos2d::CCObject*obj);
	void shareCallBack(cocos2d::CCObject*obj);
    
    virtual sp::Armature * getAnimation(warrior::WarriorCurInfo *obj);
	virtual sp::Armature * getAnimation(equip::EquipCurInfo *obj);
	virtual sp::Armature * getAnimation(warrior::SoulInfo *obj);
    virtual sp::Armature * getAnimation(item::ItemInfo *obj);
private:
	void animateEnd(sp::Armature * target, const char* event,const char* data);
private:
	cocos2d::CCNode				*m_pContainer;
	cocos2d::CCNode             *m_pHunpo;

	cocos2d::CCLabelTTF			*m_pName;
	cocos2d::CCLabelTTF			*m_pDesc;
	cocos2d::CCLabelBMFont		*m_pItemNum;
	DButton						*m_pButton;
	DButton						*m_pCloseButton;
	//----equip
	cocos2d::CCLabelBMFont  *m_pProperty;
	cocos2d::CCSprite       *m_pPropertyType[equip::kEquipPropTotal];
	//----warrior
	cocos2d::CCLabelBMFont	*m_pAttack;
	cocos2d::CCLabelBMFont	*m_pDefence;
	cocos2d::CCLabelBMFont	*m_pSpeed;
	cocos2d::CCLabelBMFont	*m_pHp;

	cocos2d::CCNode			*m_pEffectNode;
	WarriorGradeBg				*m_pGradeBg;
};


#endif /* defined(__QSMY__AwardItemView__) */