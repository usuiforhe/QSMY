//
//  DCCBFileLoader.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DCCBFileLoader.h"
#include "DCCBReader.h"

#define PROPERTY_CCBFILE "ccbFile"
USING_NS_CC;
USING_NS_CC_EXT;

void DCCBFileLoader::onHandlePropTypeCCBFile(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCNode * pCCBFileNode, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_CCBFILE) == 0) {
        ((CCBFile*)pNode)->setCCBFileNode(pCCBFileNode);
    } else {
        CCNodeLoader::onHandlePropTypeCCBFile(pNode, pParent, pPropertyName, pCCBFileNode, pCCBReader);
    }
}


CCNode * DCCBFileLoader::parsePropTypeCCBFile(CCNode * pNode, CCNode * pParent, CCBReader * pReader) {
    
    DCCBReader * pCCBReader = static_cast<DCCBReader*>(pReader);
    
    std::string ccbFileName = pCCBReader->getCCBRootPath() + pCCBReader->readCachedString();
    
    /* Change path extension to .ccbi. */
    std::string ccbFileWithoutPathExtension = CCBReader::deletePathExtension(ccbFileName.c_str());
    ccbFileName = ccbFileWithoutPathExtension + CCB_SUFFIX + SUFFIX;
    
    // Load sub file
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(ccbFileName.c_str());
    unsigned long size = 0;
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &size);
    
    DCCBReader * ccbReader = new DCCBReader(pCCBReader);
    ccbReader->autorelease();
    ccbReader->getAnimationManager()->setRootContainerSize(pParent->getContentSize());
    
    std::string string((char*)pBytes,size);
#ifdef CRYPT
    string = DecodeAES(UI_PASSWORD, string);
#endif

    
    CCData *data = new CCData((unsigned char*)string.c_str(),string.length());
    CC_SAFE_DELETE_ARRAY(pBytes);
    
    data->retain();
    ccbReader->mData = data;
    ccbReader->mBytes = data->getBytes();
    ccbReader->mCurrentByte = 0;
    ccbReader->mCurrentBit = 0;
    CC_SAFE_RETAIN(pCCBReader->mOwner);
    ccbReader->mOwner = pCCBReader->mOwner;
    
    if (NULL != ccbReader->mOwner) {
        //CCLOG("DDD");
    }
    
    ccbReader->getAnimationManager()->mOwner = ccbReader->mOwner;
    
    // The assignments below are done in the CCBReader constructor.
    //     ccbReader->mOwnerOutletNames = pCCBReader->mOwnerOutletNames;
    //     ccbReader->mOwnerOutletNodes = pCCBReader->mOwnerOutletNodes;
    //     ccbReader->mOwnerOutletNodes->retain();
    //     ccbReader->mOwnerCallbackNames = pCCBReader->mOwnerCallbackNames;
    //     ccbReader->mOwnerCallbackNodes = pCCBReader->mOwnerCallbackNodes;
    //     ccbReader->mOwnerCallbackNodes->retain();
    
    data->release();
    
    CCNode * ccbFileNode = ccbReader->readFileWithCleanUp(false, pCCBReader->getAnimationManagers());
    
    if (ccbFileNode && ccbReader->getAnimationManager()->getAutoPlaySequenceId() != -1)
    {
        // Auto play animations
        ccbReader->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(ccbReader->getAnimationManager()->getAutoPlaySequenceId(), 0);
    }
    
    if (ccbReader->isJSControlled() && pCCBReader->isJSControlled() && NULL != ccbReader->mOwner)
    {
        //set variables and callback to owner
        //set callback
        CCArray *ownerCallbackNames = ccbReader->getOwnerCallbackNames();
        CCArray *ownerCallbackNodes = ccbReader->getOwnerCallbackNodes();
        if (NULL != ownerCallbackNames && ownerCallbackNames->count() > 0 &&
            NULL != ownerCallbackNodes && ownerCallbackNodes->count() > 0)
        {
            assert(ownerCallbackNames->count() == ownerCallbackNodes->count());
            int nCount = ownerCallbackNames->count();
            for (int i = 0 ; i < nCount; i++) {
                pCCBReader->addOwnerCallbackName((dynamic_cast<CCString*>(ownerCallbackNames->objectAtIndex(i)))->getCString());
                pCCBReader->addOwnerCallbackNode(dynamic_cast<CCNode*>(ownerCallbackNodes->objectAtIndex(i)) );
            }
        }
        //set variables
        CCArray *ownerOutletNames = ccbReader->getOwnerOutletNames();
        CCArray *ownerOutletNodes = ccbReader->getOwnerOutletNodes();
        if (NULL != ownerOutletNames && ownerOutletNames->count() > 0 &&
            NULL != ownerOutletNodes && ownerOutletNodes->count() > 0)
        {
            assert(ownerOutletNames->count() == ownerOutletNodes->count());
            int nCount = ownerOutletNames->count();
            for (int i = 0 ; i < nCount; i++) {
                pCCBReader->addOwnerOutletName((dynamic_cast<CCString*>(ownerOutletNames->objectAtIndex(i)))->getCString());
                pCCBReader->addOwnerOutletNode(dynamic_cast<CCNode*>(ownerOutletNodes->objectAtIndex(i)) );
            }
        }
    }
    return ccbFileNode;
}
