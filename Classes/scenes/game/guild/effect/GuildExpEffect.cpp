//
//  GuildExpEffect.h
//  公会动画
//
//  Created by Eci on 13-10-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//



#include "GuildExpEffect.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;

//void GuildExpEffect::armatureCallBack(Armature * target, const char* event,const char* data)
//{
//	int tag = target->getTag();
//	target->removeFromParent();
//	if(tag > 0)
//	{
//		sp::Armature *pArmature = GuildExpEffect::getContribute(tag);
//		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildExpEffect::armatureCallBack), COMPLETE);
//		addChild(pArmature);
//	}
//	else{
//		removeFromParent();
//	}
//}
//
//void GuildExpEffect::playEffect(int exp , int cont)
//{
//	sp::Armature *pArmature = GuildExpEffect::getExperience(exp);
//	pArmature->setTag(cont);
//	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildExpEffect::armatureCallBack), COMPLETE);
//	addChild(pArmature);
//}

sp::Armature * GuildExpEffect::getArmature(uint32_t exp, uint32_t cont)
{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("jumpGuild",sp::ZORDER);
	pArmature->setPosition(D_DESIGN_POINT_CENTER);

	std::string contName[3] = {"shuzi1_1","shuzi1_2","shuzi1_3"};
	std::string expName[3] = {"shuzi2_1","shuzi2_2","shuzi2_3"};
	for( int i = 0 ; i < 3 ; i++ )
	{
		SPContainer* pContainer = SPContainer::create(sp::ZORDER);
		CCLabelBMFont* pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",exp)->getCString(), "fonts/number1.fnt");
		pNode->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(pNode);
		pNode->setScale(1.6f);
		pArmature->getBone(expName[i].c_str())->replaceDisplayAt(pContainer, 0);

		pContainer = SPContainer::create(sp::ZORDER);
		pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",cont)->getCString(), "fonts/number1.fnt");
		pNode->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(pNode);
		pNode->setScale(1.6f);
		pArmature->getBone(contName[i].c_str())->replaceDisplayAt(pContainer, 0);
	}
	return pArmature;
}

//sp::Armature * GuildExpEffect::getArmature(std::string name , uint32_t num , CCPoint pos)
//{
//
//	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate(name.c_str(),sp::ZORDER);
//	pArmature->setPosition(pos);
//
//	std::string boneName[3] = {"shuzi1","shuzi2","shuzi3"};
//	for( int i = 0 ; i < 3 ; i++ )
//	{
//		SPContainer* pContainer = SPContainer::create(sp::ZORDER);
//		CCLabelBMFont* pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",num)->getCString(), "fonts/number1.fnt");
//		pNode->setAnchorPoint(ccp(0,0.5));
//		pContainer->addChild(pNode);
//		pArmature->getBone(boneName[i].c_str())->replaceDisplayAt(pContainer, 0);
//	}
//	
//	return pArmature;
//}
//
//sp::Armature * GuildExpEffect::getExperience(uint32_t num)
//{
//	return GuildExpEffect::getArmature(std::string("guildExperience") , num , D_DESIGN_POINT_CENTER);
//}
//
//sp::Armature * GuildExpEffect::getContribute(uint32_t num)
//{
//	return GuildExpEffect::getArmature(std::string("guildContribute") , num , D_DESIGN_POINT_CENTER);
//}