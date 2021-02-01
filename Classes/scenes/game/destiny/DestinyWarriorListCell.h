//
//  DestinyWarriorListCell.h
//  弟子条目
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyWarriorListCell__
#define __QSMY__DestinyWarriorListCell__

#include "AppInclude.h"
#include "DestinyWarriorHead.h"

class DestinyWarriorListCell
:public cocos2d::extension::CCTableViewCell
{
#pragma mark - - init etc.
public:
    DestinyWarriorListCell();
    ~DestinyWarriorListCell();
    bool init(cocos2d::extension::CCData*);
    static DestinyWarriorListCell * create(cocos2d::extension::CCData*);
    
    DestinyWarriorHead* getDestinyWarriorHead()
    {
        return m_pWarriorHead;
    }

private:
    DestinyWarriorHead  *m_pWarriorHead;
};

#endif /* defined(__QSMY__DestinyWarriorListCell__) */
