//
//  DMaskedSprite.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DMaskedSprite.h"

USING_NS_CC;

#define  CCRectZero		CCRectMake(0,0,0,0)

DMaskedSprite* DMaskedSprite::create(const char *pszFileName,const char *pszMaskFileName)
{
    DMaskedSprite *pobSprite = new DMaskedSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName,pszMaskFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}



DMaskedSprite* DMaskedSprite::create(const char *pszFileName,const char *pszMaskFileName, const CCRect& pszRect)
{
	DMaskedSprite *pobSprite = new DMaskedSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName,pszMaskFileName,pszRect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

DMaskedSprite* DMaskedSprite::create(const char *pszFileName, cocos2d::CCTexture2D *maskTexture)
{
    DMaskedSprite* pobSprite = new DMaskedSprite();
    if (pobSprite && pobSprite->initWithFileAndMaskTexture(pszFileName, maskTexture, CCRectMake(0, 0, 0, 0))) {
        pobSprite->autorelease();
        return pobSprite;
    }
    return NULL;
}

DMaskedSprite* DMaskedSprite::create(const char *pszFileName)
{
    DMaskedSprite *pobSprite = new DMaskedSprite();
    if (pobSprite && pobSprite->initWithFileAndMaskTexture(pszFileName,NULL,CCRectZero))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DMaskedSprite* DMaskedSprite::create(const char *pszFileName, const CCRect& pszRect)
{
	DMaskedSprite *pobSprite = new DMaskedSprite();
	if (pobSprite && pobSprite->initWithFileAndMaskTexture(pszFileName,NULL,pszRect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}


bool DMaskedSprite::init(void)
{
    return initWithTextureAndMaskTexture(NULL,NULL, CCRectZero);
}


bool DMaskedSprite::initWithFile(const char *pszFilename,const char *pszMaskFileName)
{
    return initWithFile(pszFilename,pszMaskFileName,CCRectZero);
}

bool DMaskedSprite::initWithFile(const char *pszFilename,const char *pszMaskFileName, const CCRect& pszRect)
{
    CCTexture2DPixelFormat oldPixelFormat = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_A8);
	CCTexture2D * pMaskTexture = CCTextureCache::sharedTextureCache()->addImage(pszMaskFileName);
    CCTexture2D::setDefaultAlphaPixelFormat(oldPixelFormat);
	return initWithFileAndMaskTexture(pszFilename,pMaskTexture,pszRect);
}




bool DMaskedSprite::initWithFileAndMaskTexture(const char *pszFilename,CCTexture2D *pMaskTexture, const CCRect& pszRect)
{
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
    if (pTexture)
    {
        return initWithTextureAndMaskTexture(pTexture, pMaskTexture,pszRect);
    }
    return false;
}

bool DMaskedSprite::initWithTextureAndMaskTexture(cocos2d::CCTexture2D*pTexture,cocos2d::CCTexture2D *pMaskTexture, const cocos2d::CCRect& pszRect)
{
	
	
	if (pszRect.equals(CCRectZero) && pTexture!=NULL)
	{
		if (!CCSprite::initWithTexture(pTexture)) return false;
	}
	else
	{
		if (!CCSprite::initWithTexture(pTexture,pszRect)) return false;
	}
    
    // 1
    setMask(pMaskTexture);
    // 2
    
    setShaderProgram(D_SHADER_MASK);
        
    m_uTextureLocation    = glGetUniformLocation( getShaderProgram()->getProgram(), "u_texture");
    m_uMaskLocation       = glGetUniformLocation( getShaderProgram()->getProgram(), "u_mask");
    m_uOffsetLocation     = glGetUniformLocation( getShaderProgram()->getProgram(), "v_offset");
    CHECK_GL_ERROR_DEBUG();
    
    ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
    setBlendFunc(blendFunc);
    
    return true;

}


void DMaskedSprite::setMaskPosition(const CCPoint& pos)
{
    if(!m_pMaskTexture) return;
    m_obOffsetWithPosition.x = pos.x * CC_CONTENT_SCALE_FACTOR()/m_pMaskTexture->getPixelsWide();
    m_obOffsetWithPosition.y = pos.y* CC_CONTENT_SCALE_FACTOR()/m_pMaskTexture->getPixelsHigh();
}

void DMaskedSprite::draw(void)
{
    CC_NODE_DRAW_SETUP();
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    if(getTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, getTexture()->getName());
        glUniform1i(m_uTextureLocation, 0);
    }
    
    if(m_pMaskTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, m_pMaskTexture->getName() );
        glUniform1i(m_uMaskLocation, 1);
        glUniform2f(m_uOffsetLocation, m_obOffsetWithPosition.x, m_obOffsetWithPosition.y);
    }
    
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    ccGLBindTexture2D(0);
    
    CHECK_GL_ERROR_DEBUG();
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

void DMaskedSprite::setMask(cocos2d::CCTexture2D *pMaskTexture)
{
    CC_SAFE_RELEASE(m_pMaskTexture);
    CC_SAFE_RETAIN(pMaskTexture);
    m_pMaskTexture=pMaskTexture;
    if(m_pMaskTexture) m_pMaskTexture->setAliasTexParameters();
}

void DMaskedSprite::setMask(const char *pszMaskFileName)
{
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_A8);
	CCTexture2D * pMaskTexture = CCTextureCache::sharedTextureCache()->addImage(pszMaskFileName);
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
    CC_SAFE_RELEASE(m_pMaskTexture);
    CC_SAFE_RETAIN(pMaskTexture);
    m_pMaskTexture=pMaskTexture;
    if(m_pMaskTexture) m_pMaskTexture->setAliasTexParameters();
}

void DMaskedSprite::setMask(cocos2d::CCSprite *pMask)
{
    CCTexture2D * pMaskTexture = pMask->getTexture();
    setMask(pMaskTexture);
}

