//
//  GuildExpEffect.h
//  公会动画
//
//  Created by Eci on 13-10-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__GuildExpEffect__
#define __QSMY__GuildExpEffect__

#include "AppInclude.h"
#include "components/components.h"
#include "model/GuildModel.h"
#include "DragonBone.h"

class GuildExpEffect	:	public cocos2d::CCNode
{
public:
	CREATE_FUNC(GuildExpEffect);
	D_INIT_DEFAULT(cocos2d::CCNode);
	static sp::Armature* getArmature(uint32_t exp, uint32_t cont);
//	static sp::Armature* getExperience(uint32_t num);
//	static sp::Armature* getContribute(uint32_t num);
//	void playEffect(int exp , int cont);
//private:
//	static sp::Armature* getArmature(std::string name, uint32_t num , cocos2d::CCPoint pos);
//	void armatureCallBack(sp::Armature * target, const char* event,const char* data);
	
};


#endif /* defined(__QSMY__ExploreResultEffect__) */
