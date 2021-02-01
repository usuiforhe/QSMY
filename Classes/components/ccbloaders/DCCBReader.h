//
//  DCCBReader.h
//  QSMY
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DCCBReader__
#define __QSMY__DCCBReader__

#include "AppInclude.h"
#include "DCCBFileLoader.h"
#include "DCCScrollViewLoader.h"

#ifdef CRYPT
#include "utils/aes.h"
#endif

class DCCBReader
:public cocos2d::extension::CCBReader
{
    
public:
    DCCBReader(cocos2d::extension::CCNodeLoaderLibrary *pCCNodeLoaderLibrary, cocos2d::extension::CCBMemberVariableAssigner *pCCBMemberVariableAssigner = NULL, cocos2d::extension::CCBSelectorResolver *pCCBSelectorResolver = NULL, cocos2d::extension::CCNodeLoaderListener *pCCNodeLoaderListener = NULL);
    DCCBReader(DCCBReader *pCCBReader);
    DCCBReader();
    virtual ~DCCBReader(){};
    virtual cocos2d::CCNode* readNodeGraphFromFile(const char *pCCBFileName, cocos2d::CCObject *pOwner, const cocos2d::CCSize &parentSize);
  
    friend class DCCBFileLoader;
    friend class DCCScrollViewLoader;
};

#endif /* defined(__QSMY__DCCBReader__) */
