//
//  StepEquip.cpp
//  QSMY
//
//  Created by wanghejun on 13-12-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "StepEquip.h"
#include "../../Game.h"
#include "../../formation/Formation.h"

void StepEquip::trigger()
{
    if(dynamic_cast<Formation*>(Game::sharedGame()->getFocusLayer())==NULL)
    {
        Game::sharedGame()->NavigateTo(navigation::kNavChangeFormation);
    }
    StepEnForce::trigger();
}