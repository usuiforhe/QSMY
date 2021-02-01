/****************************************************************************
Copyright (c) 2013 Shawn Clovie

http://mcspot.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCBProxy.h"
#include "tolua++.h"
#include "../scenes/game/common/common.h"

CCBProxy::CCBProxy():_selectorHandler(0){
	_memVars = CCDictionary::create();
	_memVars->retain();
    
    _selectorVars = CCDictionary::create();
    _selectorVars->retain();
    
	_handlers = CCArray::create();
	_handlers->retain();
    
}
CCBProxy::~CCBProxy(){
	CC_SAFE_RELEASE(_memVars);
    CC_SAFE_RELEASE(_selectorVars);
	CC_SAFE_RELEASE(_handlers);
	CC_SAFE_RELEASE(_selectorHandler);
}
void CCBProxy::releaseMembers(){
/*
	CCDictElement *e;
	CCNode *n;
	CCDICT_FOREACH(_memVars, e){
		n = (CCNode *)e->getObject();
		n->removeFromParentAndCleanup(true);
		n->autorelease();
	}
	CCObject *o;
	CCARRAY_FOREACH(_handlers, o){
		o->autorelease();
	}
//*/
    _selectorVars->removeAllObjects();
	_memVars->removeAllObjects();
	_handlers->removeAllObjects();
}

CCBProxy * CCBProxy::initProxy(lua_State *l){
	_lua = l;
	return this;
}

SEL_MenuHandler CCBProxy::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName){
	return NULL;
}

SEL_CCControlHandler CCBProxy::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName){
	return NULL;
}


bool CCBProxy::onAssignCCBMemberVariable(CCObject * t, const char * v, CCNode * n){
	if(n && v && strlen(v) > 0){
		_memVars->setObject(n, v);
	}else{
		printf("CCBProxy assigned node (%s:%p) not saved\n", v, n);
	}
	return true;
}

void CCBProxy::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader){
}

CCBSelectorResolver * CCBProxy::createNew(){
	CCBProxy *p = new CCBProxy();
	return dynamic_cast<CCBSelectorResolver *>(p);
}

void CCBProxy::handleEvent(CCControl *n, const int handler, bool multiTouches, CCControlEvent e){
//#if COCOS2D_VERSION > 0x00020100
//	n->addHandleOfControlEvent(handler, e);
//#else
	LuaEventHandler *h = getHandler(handler);
	if(!h){
		h = addHandler(handler, multiTouches)
			->setTypename("CCControlButton");
	}
	n->addTargetWithActionForControlEvents(h, cccontrol_selector(LuaEventHandler::controlAction), e);
//#endif
}

#ifdef LUAPROXY_CCEDITBOX_ENABLED
void CCBProxy::handleEvent(CCEditBox *n, const int handler){
	LuaEventHandler *h = addHandler(handler, false)
		->setTypename("CCEditBox");
	n->setDelegate(h);
}
#endif

void CCBProxy::handleEvent(CCBAnimationManager *m, const int handler){
	addHandler(handler)->handle(m);
}

void CCBProxy::handleKeypad(const int handler){
	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(addHandler(handler));
}

LuaEventHandler * CCBProxy::addHandler(const int handler, bool multiTouches){
	LuaEventHandler *h = LuaEventHandler::create(_lua)
		->handle(handler, multiTouches, 0, false);
	_handlers->addObject(h);
	return h;
}

LuaEventHandler * CCBProxy::getHandler(const int handler){
	CCObject *o;
	LuaEventHandler *h;
	CCARRAY_FOREACH(_handlers, o){
		h = (LuaEventHandler *)o;
		if(h->getHandler() == handler){
			return h;
		}
	}
	return NULL;
}

LuaEventHandler * CCBProxy::removeHandler(LuaEventHandler *h){
	if(h)_handlers->removeObject(h);
	return h;
}

LuaEventHandler * CCBProxy::removeFunction(int handler){
	return removeHandler(getHandler(handler));
}

LuaEventHandler * CCBProxy::removeKeypadHandler(int handler){
	LuaEventHandler *h = removeFunction(handler);
	if(h)CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(h);
	return h;
}

void CCBProxy::setSelectorHandler(LuaEventHandler *h){
    CC_SAFE_RETAIN(h);
	CC_SAFE_RELEASE(_selectorHandler);
	_selectorHandler = h;
}

LuaEventHandler * CCBProxy::handleSelector(const int handler){
	LuaEventHandler *h = NULL;
	if(handler > 0){
		h = LuaEventHandler::create(_lua)->handle(handler);
	}
	setSelectorHandler(h);
	return h;
}

void CCBProxy::menuItemCallback(CCObject *pSender) {
	if(_selectorHandler){
		_selectorHandler->action(pSender);
	}
}

void CCBProxy::controlCallback(CCObject *pSender, CCControlEvent event) {
	if(_selectorHandler){
		_selectorHandler->controlAction(pSender, event);
	}
}

CCDictionary * CCBProxy::getMemberVariables(){return _memVars;}

const char * CCBProxy::getMemberName(CCObject *n){
	CCDictElement *e;
	CCDICT_FOREACH(_memVars, e){
		if(e->getObject() == n)
			return e->getStrKey();
	}
	return "";
}

CCNode * CCBProxy::getNode(const char *n){
	return (CCNode *)_memVars->objectForKey(n);
}

CCArray * CCBProxy::getSelecorOwner(const char *n){
	return (CCArray *)_selectorVars->objectForKey(n);
}



void CCBProxy::nodeToTypeForLua(lua_State *l, CCObject *o, const char *t){
	if(strcmp("CCSprite", t) == 0)				tolua_pushusertype(l, dynamic_cast<CCSprite *>(o), t);
	else if(strcmp("CCControlButton", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCControlButton *>(o), t);
	else if(strcmp("CCLayer", t) == 0)			tolua_pushusertype(l, dynamic_cast<CCLayer *>(o), t);
	else if(strcmp("CCLayerColor", t) == 0)		tolua_pushusertype(l, dynamic_cast<CCLayerColor *>(o), t);
	else if(strcmp("CCLayerGradient", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCLayerGradient *>(o), t);
	else if(strcmp("CCScrollView", t) == 0)		tolua_pushusertype(l, dynamic_cast<CCScrollView *>(o), t);
	else if(strcmp("CCScale9Sprite", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCScale9Sprite *>(o), t);
	else if(strcmp("CCLabelTTF", t) == 0)		tolua_pushusertype(l, dynamic_cast<CCLabelTTF *>(o), t);
	else if(strcmp("CCLabelBMFont", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCLabelBMFont *>(o), t);
	else if(strcmp("CCMenu", t) == 0)			tolua_pushusertype(l, dynamic_cast<CCMenu *>(o), t);
	else if(strcmp("CCMenuItemImage", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCMenuItemImage *>(o), t);
    else if(strcmp("CCMenuItem", t) == 0)	tolua_pushusertype(l, dynamic_cast<CCMenuItem *>(o), t);
	else if(strcmp("CCString", t) == 0)			tolua_pushusertype(l, dynamic_cast<CCString *>(o), t);
	else if(strcmp("CCParticleSystemQuad", t) == 0)tolua_pushusertype(l, dynamic_cast<CCParticleSystemQuad *>(o), t);
	else if(strcmp("CCBFile", t) == 0)			tolua_pushusertype(l, dynamic_cast<CCBFile *>(o), t);
    else if(strcmp("DProgressBar", t) == 0)		tolua_pushusertype(l, dynamic_cast<DProgressBar *>(o), t);
    else if(strcmp("DSprite", t) == 0)			tolua_pushusertype(l, dynamic_cast<DSprite *>(o), t);
    else if(strcmp("DButton", t) == 0)			tolua_pushusertype(l, dynamic_cast<DButton *>(o), t);
    else if(strcmp("CCNodeRGBA", t) == 0)			tolua_pushusertype(l, dynamic_cast<CCNodeRGBA *>(o), t);
	else tolua_pushusertype(l, dynamic_cast<CCNode *>(o), "CCNode");
}

CCNode * CCBProxy::readCCBFromFile(const char *f){
	//assert(f && strlen(f) > 0, "File name must not be null or empty string.");
    cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(ccNodeLoaderLibrary);
	CCBReader * reader = new DCCBReader(ccNodeLoaderLibrary);
#if COCOS2D_VERSION < 0x00020100
	reader->hasScriptingOwner = true;
#endif
	CCNode *node = reader->readNodeGraphFromFile(f, this);
	CCBAnimationManager *m = reader->getAnimationManager();
    
    CCArray* pNames = reader->getOwnerOutletNames();
    CCArray* pMembers = reader->getOwnerOutletNodes();

    for (uint32_t i=0; i<pNames->count(); ++i) {
        CCString* pName = static_cast<CCString*>(pNames->objectAtIndex(i));
        if(strlen(pName->getCString())==0) continue;
        _memVars->setObject(static_cast<CCObject*>(pMembers->objectAtIndex(i)), pName->getCString());
    }
    
    
    pNames =reader->getOwnerCallbackNames();
    pMembers = reader->getOwnerCallbackNodes();
    
    for (uint32_t i=0; i<pNames->count(); ++i) {
        CCString* pName = static_cast<CCString*>(pNames->objectAtIndex(i));
        if(strlen(pName->getCString())==0) continue;
        CCArray* pArr = static_cast<CCArray*>(_selectorVars->objectForKey(pName->getCString()));
        if(pArr==NULL)
        {
            pArr = CCArray::create();
            _selectorVars->setObject(pArr, pName->getCString());
        }
        pArr->addObject(static_cast<CCObject*>(pMembers->objectAtIndex(i)));
    }
	
	node->setUserObject(m);
    reader->release();
	return node;
}
