//
//  common.h
//  公用组件
//
//  Created by wanghejun on 13-8-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_common_h
#define QSMY_common_h

#include "./gui/SkillBox.h"
#include "./gui/AwardItemSmall.h"
#include "./gui/WarriorHeadMid.h"
#include "./gui/WarriorHeadSmall.h"
#include "./gui/ItemBox.h"
#include "./gui/EquipBox.h"
#include "./gui/WarriorTypeMark.h"
#include "./gui/StoryTalk.h"
#include "./gui/DShareButton.h"
#include "./gui/WarriorGradeBg.h"
#include "./gui/WarriorHeadUpLevel.h"
#include "./gui/WarriorHeadGradeBg.h"

#include "components/components.h"


#ifdef CUSTOM_CCB_LOADER//(____ccNodeLoaderLibrary__)
#undef CUSTOM_CCB_LOADER//(____ccNodeLoaderLibrary__)
#endif

#define CUSTOM_CCB_LOADER(____ccNodeLoaderLibrary__) \
__CUSTOM_CCB_LOADER__(____ccNodeLoaderLibrary__)\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("SkillBox", SkillBoxLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("AwardItemSmall", AwardItemSmallLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorHeadMid", WarriorHeadMidLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorHeadSmall", WarriorHeadSmallLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("ItemBox", ItemBoxLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("EquipBox", EquipBoxLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("StoryTalk",StoryTalkLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DShareButton",DShareButtonLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorGradeBg",WarriorGradeBgLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorHeadUpLevel",WarriorHeadUpLevelLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("WarriorHeadGradeBg",WarriorHeadGradeBgLoader::loader());\




#endif

