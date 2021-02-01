//
//  BoxEffect.h
//  开宝箱动画
//
//  Created by fengkerong.billy on 14-10-15.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#ifndef __QSMY__BoxEffect__
#define __QSMY__BoxEffect__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../common/common.h"
#include "DragonBone.h"

class BoxEffect : public DEffectNode
{
public:
	BoxEffect();
	~BoxEffect();
	static BoxEffect* create(item::ItemInfo *);
	virtual bool init(item::ItemInfo *);
	//注释掉如下代码，使用默认的触摸禁止办法
	//D_TOUCH_REGISTER_DEFAULT( DPOPUP_USER_LEVELUP_PRIORITY );

	item::ItemInfo *getItemInfo(){
		return m_pItemInfo;
	}

private:
	sp::Armature* m_pFlash;
	item::ItemInfo *m_pItemInfo;

private:
	void animateEnd(sp::Armature * target, const char* event,const char* data);
	void play();

};
#endif /* defined(__QSMY__BoxEffect__) */
