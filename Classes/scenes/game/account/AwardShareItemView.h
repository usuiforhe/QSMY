//
//  AwardShareItemView.h
//  分享掉落显示
//
//  Created by mzp on 14-04-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__AwardShareItemView__
#define __QSMY__AwardShareItemView__

#include "AppInclude.h"
#include "model/EquipModel.h"
#include "model/ItemModel.h"
#include "model/WarriorModel.h"
#include "model/DropModel.h"
#include "model/AvatarModel.h"
#include "DragonBone.h"

class AwardShareItemView
:public DPopup
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	AwardShareItemView();
	virtual ~AwardShareItemView();

	static AwardShareItemView* create(drop::Drop* obj);

	virtual bool init(drop::Drop *obj);
	virtual bool setPointInfo(uint32_t num);
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
	cocos2d::CCLabelTTF			*m_pName;
	cocos2d::CCLabelTTF			*m_pDesc;
	cocos2d::CCLabelBMFont		*m_pItemNum;
	DButton						*m_pButton;
	cocos2d::CCNode				*m_pEffectNode;		

};


#endif /* defined(__QSMY__AwardShareItemView__) */
