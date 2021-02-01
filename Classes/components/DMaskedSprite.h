//
//  DMaskedSprite.h
//  QSMY
//
//  Created by wanghejun on 13-5-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DMaskedSprite__
#define __QSMY__DMaskedSprite__

#include "AppInclude.h"

class DMaskedSprite:public cocos2d::CCSprite {
    
public:
    
    DMaskedSprite():m_pMaskTexture(NULL)
    {
        
    }
    
    virtual ~DMaskedSprite()
    {
        CC_SAFE_RELEASE(m_pMaskTexture);
    }
    
    CREATE_FUNC(DMaskedSprite);
    
    virtual bool init();
    
    static DMaskedSprite* create(const char *pszFileName,const char *pszMaskFileName);
	static DMaskedSprite* create(const char *pszFileName,const char *pszMaskFileName, const cocos2d::CCRect& pszRect);
    static DMaskedSprite* create(const char *pszFileName, cocos2d::CCTexture2D* maskTexture);
    static DMaskedSprite* create(const char *pszFileName);
    static DMaskedSprite* create(const char *pszFileName,const cocos2d::CCRect& pszRect);

    virtual void draw(void);
    
    virtual void setMaskPosition(const cocos2d::CCPoint& pos);
    
    virtual void setMask(const char *pszMaskFileName);
    virtual void setMask(cocos2d::CCSprite *pMask);
    virtual void setMask(cocos2d::CCTexture2D *pMaskTexture);

protected:

	virtual bool initWithFile(const char *pszFilename,const char *pszMaskFileName);
	virtual bool initWithFile(const char *pszFilename,const char *pszMaskFileName, const cocos2d::CCRect& pszRect);
    virtual bool initWithFileAndMaskTexture(const char *pszFilename,cocos2d::CCTexture2D *pMaskTexture, const cocos2d::CCRect& pszRect);
    virtual bool initWithTextureAndMaskTexture(cocos2d::CCTexture2D*,cocos2d::CCTexture2D *pMaskTexture, const cocos2d::CCRect& pszRect);
    
protected:
    cocos2d::CCTexture2D *m_pMaskTexture;
    GLuint m_uMaskLocation;
    GLuint m_uTextureLocation;
    GLuint m_uOffsetLocation;
    cocos2d::CCPoint m_obOffsetWithPosition;
};

#endif /* defined(__QSMY__DMaskedSprite__) */
