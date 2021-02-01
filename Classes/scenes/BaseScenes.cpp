//
//  BaseScenes.cpp
//  QSMY
//
//  Created by wanghejun on 13-12-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BaseScenes.h"
#include "utils/SystemUtil.h"


BaseScenes::BaseScenes()
{
    
}

BaseScenes::~BaseScenes()
{
    
}

bool BaseScenes::init()
{
    if(!DRoot::init()) return false;
    setKeypadEnabled(true);
    return true;
}

void BaseScenes::keyBackClicked()
{
    DRoot::keyBackClicked();
    SystemUtil::quitGame();
}