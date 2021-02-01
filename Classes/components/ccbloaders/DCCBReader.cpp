//
//  DCCBReader.cpp
//  QSMY
//
//  Created by wanghejun on 13-9-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DCCBReader.h"

USING_NS_CC;
USING_NS_CC_EXT;

DCCBReader::DCCBReader(CCNodeLoaderLibrary * pCCNodeLoaderLibrary, CCBMemberVariableAssigner * pCCBMemberVariableAssigner, CCBSelectorResolver * pCCBSelectorResolver, CCNodeLoaderListener * pCCNodeLoaderListener)
:CCBReader(pCCNodeLoaderLibrary,pCCBMemberVariableAssigner,pCCBSelectorResolver,pCCNodeLoaderListener)
{
    
}

DCCBReader::DCCBReader(DCCBReader * pCCBReader)
:CCBReader(pCCBReader)
{
    
}

DCCBReader::DCCBReader()
:CCBReader()
{
    
}

CCNode* DCCBReader::readNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner, const CCSize &parentSize)
{
    if (NULL == pCCBFileName || strlen(pCCBFileName) == 0)
    {
        return NULL;
    }
    
    std::string strCCBFileName(pCCBFileName);
    std::string strSuffix(CCB_SUFFIX);

    // Add ccbi suffix
    if (!CCBReader::endsWith(strCCBFileName.c_str(), strSuffix.c_str()))
    {
        strCCBFileName += strSuffix;
    }
    
    strCCBFileName += SUFFIX;
    
    std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(strCCBFileName.c_str());
    unsigned long size = 0;
    
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rb", &size);
    
    std::string string((char*)pBytes,size);
#ifdef CRYPT
    string = DecodeAES(UI_PASSWORD, string);
#endif
    CCData *data = new CCData((unsigned char *)string.c_str(), string.length());
    CC_SAFE_DELETE_ARRAY(pBytes);
    CCLog("%s", string.c_str());
    CCNode *ret =  this->readNodeGraphFromData(data, pOwner, parentSize);
    
    data->release();
    
    return ret;
}