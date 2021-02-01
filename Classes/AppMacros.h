#ifndef __APPMACROS_H__
#define __APPMACROS_H__


#define MACRO_TO_STRING(x) TO_STRING(x)
#define TO_STRING(x) #x


#define PRINT_MACRO(_x_) CCLOG(MACRO_TO_STRING(_x_))


#define D_ASSERT_POINTER( x ) assert( ( x ) != NULL )


/*x1000 fix跟弟子id的冲突*/
#define D_FIX_DRESS_ID(x) (x*1000)


#define D_DOUBLE_CLICK_DELTA 0.4f

//anchor point
#define D_ANCHOR_POINT_CENTER ccp( 0.5f, 0.5f )
#define D_ANCHOR_POINT_LEFTBOTTOM ccp( 0.0f, 0.0f )
#define D_ANCHOR_POINT_RIGHTBOTTOM ccp( 1.0f, 0.0f )
#define D_ANCHOR_POINT_RIGHTTOP ccp( 1.0f, 1.0f )
#define D_ANCHOR_POINT_LEFTTOP ccp( 0.0f, 1.0f )
#define D_ANCHOR_POINT_TOP ccp( 0.5f, 1.0f )
#define D_ANCHOR_POINT_BOTTOM ccp( 0.5f, 0.0f )
#define D_ANCHOR_POINT_LEFT ccp( 0.0f, 0.5f )
#define D_ANCHOR_POINT_RIGHT ccp( 1.0f, 0.5f )

//size

#define D_WIN_SIZE_WIDTH ( cocos2d::CCDirector::sharedDirector()->getWinSize().width )
#define D_WIN_SIZE_HEIGHT ( cocos2d::CCDirector::sharedDirector()->getWinSize().height )

#define D_WIN_SIZE_WIDTH_HALF ( cocos2d::CCDirector::sharedDirector()->getWinSize().width * 0.5f )
#define D_WIN_SIZE_HEIGHT_HALF ( cocos2d::CCDirector::sharedDirector()->getWinSize().height * 0.5f )

#define D_WIN_SIZE (cocos2d::CCDirector::sharedDirector()->getWinSize())

#define D_WIN_CENTER_POINT ccp( D_WIN_SIZE_WIDTH_HALF, D_WIN_SIZE_HEIGHT_HALF )


#define D_NODE_WIDTH( n ) ( ( n )->getContentSize().width )
#define D_NODE_HEIGHT( n ) ( ( n )->getContentSize().height )

#define D_NODE_WIDTH_HALF( n ) ( ( n )->getContentSize().width * 0.5f )
#define D_NODE_HEIGHT_HALF( n ) ( ( n )->getContentSize().height * 0.5f )

#define D_APP_CENTER_POINT  ccpSub(VisibleRect::center(), CCPointMake(DESIGN_SIZE.width*0.5f, DESIGN_SIZE.height*0.5f))
#define D_NODE_X_CENTER( n, y)	\
	( n )->setAnchorPoint(ccp(0.5f,0.5f));	\
    ( n )->setPosition( ccp(D_DESIGN_SIZE_WIDTH_HALF, DESIGN_SIZE.height - D_NODE_HEIGHT( n )*0.5f - ( y )) )


#define D_DESIGN_SIZE_WIDTH (DESIGN_SIZE.width)
#define D_DESIGN_SIZE_HEIGHT (DESIGN_SIZE.height)

#define D_DESIGN_SIZE_WIDTH_HALF (DESIGN_SIZE.width*0.5f)
#define D_DESIGN_SIZE_HEIGHT_HALF (DESIGN_SIZE.height*0.5f)


#define D_DESIGN_POINT_CENTER ccp( D_DESIGN_SIZE_WIDTH_HALF, D_DESIGN_SIZE_HEIGHT_HALF )
#define D_DESIGN_POINT_LEFTBOTTOM ccp( 0.0f, 0.0f )
#define D_DESIGN_POINT_RIGHTBOTTOM ccp( D_DESIGN_SIZE_WIDTH, 0.0f )
#define D_DESIGN_POINT_RIGHTTOP ccp( D_DESIGN_SIZE_WIDTH, D_DESIGN_SIZE_HEIGHT )
#define D_DESIGN_POINT_LEFTTOP ccp( 0.0f, D_DESIGN_SIZE_HEIGHT )
#define D_DESIGN_POINT_TOP ccp( D_DESIGN_SIZE_WIDTH_HALF, D_DESIGN_SIZE_HEIGHT )
#define D_DESIGN_POINT_BOTTOM ccp( D_DESIGN_SIZE_WIDTH_HALF, 0.0f )
#define D_DESIGN_POINT_LEFT ccp( 0.0f, D_DESIGN_SIZE_HEIGHT_HALF )
#define D_DESIGN_POINT_RIGHT ccp( D_DESIGN_SIZE_WIDTH, D_DESIGN_SIZE_HEIGHT_HALF )


//menu items

#define D_MENU_ITEM_COMMON_SPACING 20

//functions
#define D_GET_DIRECTOR() cocos2d::CCDirector::sharedDirector()
#define D_GET_BROTHER( brotherClassName, brotherTag ) ( ( brotherClassName* )getParent()->getChildByTag( ( brotherTag ) ) )
#define D_GET_CHILD( childClassName, childTag ) ( ( childClassName* )getChildByTag( ( childTag ) ) )

#define D_MESSAGE_CONSTRUCTOR( ownerClassName ) \
ownerClassName() : DTouchDelegate( this ){}


#define D_MESSAGE_BRIDGE()                                             \
virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)             \
{                                                                       \
return dTouchBegan( pTouch, pEvent );                                  \
}                                                                       \
virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)             \
{                                                                       \
dTouchMoved( pTouch, pEvent );                                         \
}                                                                       \
virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)             \
{                                                                       \
dTouchEnded( pTouch, pEvent );                                         \
}                                                                       \
virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)         \
{                                                                       \
dTouchCancelled( pTouch, pEvent );                                     \
}

#define D_REMOVE_FROM_PARENT(pNode) \
pNode->removeFromParent(); \
pNode = NULL; \


#define D_INIT_FUNC()              \
virtual bool init()                                     \
{                                                       \
    return true;                                            \
}

#define D_INIT_DEFAULT( parentClassName )              \
virtual bool init()                                     \
{                                                       \
if( !parentClassName::init() )                          \
{                                                       \
return false;                                           \
}                                                       \
\
return true;                                            \
}

#define D_TOUCH_REGISTER_DEFAULT( iPriority )          \
virtual void registerWithTouchDispatcher( void  )       \
{                                                       \
cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this, iPriority, true );                            \
}

#define D_TOUCH_REGISTER_EMPTY()                       \
virtual void registerWithTouchDispatcher( void  ){}


#define D_FONT_DEFAULT "Arial"
#define D_FONT_HELVETICA "Helvetica"

#define D_FONT_WARRIOR_VALUE "fonts/warrior_value.fnt"
#define D_FONT_BATTLE_DMG "fonts/battle_dmg.fnt"
#define D_FONT_NUMBER "fonts/number.fnt"
#define D_FONT_BATTLE_HEAL "fonts/battle_heal.fnt"
#define D_FONT_SKILL_NAME  "fonts/skill.fnt"
#define D_FONT_SUPPER_SKILL_NAME  "fonts/dajineng50.fnt"

#define D_FONT_GREEN_HERO_NAME  "fonts/lvseyxmc24.fnt"
#define D_FONT_BLUE_HERO_NAME  "fonts/lanseyxmc24.fnt"
#define D_FONT_PURPLE_HERO_NAME  "fonts/zsyxmc24.fnt"
#define D_FONT_ORANGE_HERO_NAME  "fonts/csyxmc24.fnt"


// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define D_FONT_SIZE(__SIZE__)\
cocos2d::CCDirector::sharedDirector()->getWinSize().width > cocos2d::CCDirector::sharedDirector()->getWinSize().height?(int)(cocos2d::CCDirector::sharedDirector()->getWinSize().height / 320.0f * __SIZE__):(int)(cocos2d::CCDirector::sharedDirector()->getWinSize().width / 320.0f * __SIZE__)



#define D_FONT_SIZE_18 (D_FONT_SIZE(18))
#define D_FONT_SIZE_14 (D_FONT_SIZE(14))
#define D_FONT_SIZE_12 (D_FONT_SIZE(12))
#define D_FONT_SIZE_11 (D_FONT_SIZE(11))
#define D_FONT_SIZE_10 (D_FONT_SIZE(10))
#define D_FONT_SIZE_24 (D_FONT_SIZE(24))
#define D_FONT_SIZE_DEFAULT (D_FONT_SIZE(20))



#define D_RESET_C_ARRAY(array,len)                          \
for (uint32_t i = 0; i<len; ++i) {                            \
    array[i] = NULL;                                              \
}                                                             

#define D_SAFE_RELEASE_C_ARRAY(array,len)                          \
for (uint32_t i = 0; i<len; ++i) {                            \
    CC_SAFE_RELEASE(array[i]);                                              \
}

#define D_SAFE_DELETE_C_ARRAY(array,len)                          \
for (uint32_t i = 0; i<len; ++i) {                            \
CC_SAFE_DELETE(array[i]);                                              \
}

#define D_MILLISECOND_TO_SECOND(val) ((float)val/1000)



#define D_CCCA(x)   (x->copy()->autorelease())

#define D_LOCAL_STRING(x, ...) Lang::sharedLang()->getLocalString(x,##__VA_ARGS__)

///////// json ///////////


#define DJSON_STRING(_target_,_val_,_key_)  if(_val_.isMember(_key_)) _target_ = _val_[_key_].asString()
#define DJSON_CSTRING(_target_,_val_,_key_) if(_val_.isMember(_key_)) _target_ = _val_[_key_].asCString()
#define DJSON_BOOL(_target_,_val_,_key_) if(_val_.isMember(_key_)) _target_ = _val_[_key_].asBool()
#define DJSON_INT(_target_,_val_,_key_) if(_val_.isMember(_key_)) _target_ = _val_[_key_].asInt()
#define DJSON_UINT(_target_,_val_,_key_) if(_val_.isMember(_key_)) _target_ = _val_[_key_].asUInt()
#define DJSON_DOUBLE(_target_,_val_,_key_) if(_val_.isMember(_key_)) _target_ = _val_[_key_].asDouble()



#define DJSON_STRING2(_target_,_val_,_key_)  if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asString()
#define DJSON_CSTRING2(_target_,_val_,_key_) if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asCString()
#define DJSON_BOOL2(_target_,_val_,_key_) if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asBool()
#define DJSON_INT2(_target_,_val_,_key_) if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asInt()
#define DJSON_UINT2(_target_,_val_,_key_) if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asUInt()
#define DJSON_DOUBLE2(_target_,_val_,_key_) if(_val_.isMember(#_key_)) _target_->_key_ = _val_[#_key_].asDouble()


#define D_ACTION_CALL_FUNCN(__target__ , __selector__) \
CCCallFuncN::create(__target__,callfuncN_selector(__selector__)) \


#define D_FLOAT_EQUALS(float1,float2) (fabs(float1-float2)<FLT_EPSILON)

/*
virtual void registerWithTouchDispatcher( void  )       \
{                                                       \
cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this, iPriority, true );                            \
}
*/

#define D_CCCA(x)   (x->copy()->autorelease())


#pragma mark - shader -

#define D_SHADER_MASK_KEY "shader_mask"
#define D_SHADER_MASK_GRAY_KEY "shader_mask_gray"
#define D_SHADER_GRAY_KEY "shader_gray"

#define D_SHADER_MASK_GRAY CCShaderCache::sharedShaderCache()->programForKey(D_SHADER_MASK_GRAY_KEY)
#define D_SHADER_MASK CCShaderCache::sharedShaderCache()->programForKey(D_SHADER_MASK_KEY)
#define D_SHADER_GARY CCShaderCache::sharedShaderCache()->programForKey(D_SHADER_GRAY_KEY)
#define D_SHADER_NORMAL CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor)


#pragma mark - ccbassign -

#define D_NAME_WITH_INDEX(_name_, _index_)	\
	cocos2d::CCString::createWithFormat("%s%d", _name_, _index_)->getCString()

#define D_CCB_ASSIGN_ARRAY(_name_, _type_, _array_, _count_)	\
	for (int i = 0; i < _count_; i++)	\
	{	\
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, D_NAME_WITH_INDEX(_name_, i), _type_, _array_[i]);	\
	}

#define D_CCB_ASSIGN_ARRAY_FROM_ONE(_name_, _type_, _array_, _count_)	\
	for (int i = 0; i < _count_; i++)	\
	{	\
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, D_NAME_WITH_INDEX(_name_, i + 1), _type_, _array_[i]);	\
	}

#define D_CSTRING_FROM_UINT(_num_)	\
	(cocos2d::CCString::createWithFormat("%u", _num_)->getCString())

#define D_CSTRING_FROM_INT(_num_)	\
	(cocos2d::CCString::createWithFormat("%d", _num_)->getCString())

#define D_CSTRING_TIME_FROM_UINT(_num_) \
	(cocos2d::CCString::createWithFormat("%.2u:%.2u:%.2u", _num_ / 3600, _num_ / 60 % 60, _num_ % 60)->getCString())

#define D_CSTRING_WITH10K_FROM_UINT(_num_) \
	(_num_ > 10000 ? cocos2d::CCString::createWithFormat("%u%s", (unsigned int)(_num_ / 10000), D_LOCAL_STRING("TenThousand").c_str())->getCString() : cocos2d::CCString::createWithFormat("%u", _num_)->getCString())

#define D_CONFIG_FOR_KEY(_key_) (DM_GET_CONFIG_MODEL->getConfigByKey(_key_))
#define D_CONFIG_INT_FOR_KEY(_key_) (D_CONFIG_FOR_KEY(_key_)->intValue())
#define D_CONFIG_UINT_FOR_KEY(_key_) (D_CONFIG_FOR_KEY(_key_)->uintValue())
#define D_CONFIG_FLOAT_FOR_KEY(_key_) (D_CONFIG_FOR_KEY(_key_)->floatValue())
#define D_CONFIG_CSTRING_FOR_KEY(_key_) (D_CONFIG_FOR_KEY(_key_)->getCString())


#define PL_GET_HEIGHT_DIFF	(DESIGN_SIZE.height - CCEGLView::sharedOpenGLView()->getFrameSize().height / CCEGLView::sharedOpenGLView()->getScaleY())
#define PL_GET_WIDTH_DIFF (DESIGN_SIZE.width -  CCEGLView::sharedOpenGLView()->getFrameSize().width / CCEGLView::sharedOpenGLView()->getScaleX())

#define PL_MOVE_WITH_RESOLUTION_DIFF(_node_, _rate_)	\
{\
	_node_->setPositionY(_node_->getPositionY() + PL_GET_HEIGHT_DIFF * _rate_);\
}

#define PL_MOVE_NODE_ABOVE_CENTER_Y(_node_)	\
{\
	PL_MOVE_WITH_RESOLUTION_DIFF(_node_, -0.5);	\
}

#define PL_MOVE_NODE_BELOW_CENTER_Y(_node_)	\
{\
	PL_MOVE_WITH_RESOLUTION_DIFF(_node_, 0.5);	\
}

#define PL_MIN_RESOLUTION_HEIGHT    960

#define PL_MAX_HEIGHT_DIFF (DESIGN_SIZE.height - PL_MIN_RESOLUTION_HEIGHT)

#define PL_MOVE_WITH_FLEXIBLE_HEIGHT(_node_, _height_)	\
{\
_node_->setPositionY(_node_->getPositionY() + PL_GET_HEIGHT_DIFF / PL_MAX_HEIGHT_DIFF * _height_);\
}

#define PL_RESIZE_WITH_FLEXIBLE_HEIGHT(_scale9_, _height_)    \
{\
cocos2d::CCSize sz = _scale9_->getContentSize();\
sz.height -= PL_GET_HEIGHT_DIFF / PL_MAX_HEIGHT_DIFF * _height_;\
_scale9_->setPreferredSize(sz);\
}

#define PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(_posy_, _height_) \
	(_posy_ + PL_GET_HEIGHT_DIFF / PL_MAX_HEIGHT_DIFF * _height_)

#define PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(_size_, _height_) \
	(CCSizeMake(_size_.width, _size_.height + PL_GET_HEIGHT_DIFF / PL_MAX_HEIGHT_DIFF * _height_))

#define PL_CAL_POINT_WIHT_FLEXIBLE_HEIGHT(_point_, _height_) \
	(ccp(_point_.x, _point_.y + PL_GET_HEIGHT_DIFF / PL_MAX_HEIGHT_DIFF * _height_))

#define FOOT_NAVIGATION_HEIGHT 128
#define PL_WITHNAV_GET_CONTEENT_SIZE(_node_, _endnode_) CCSizeMake(640, (_node_->getPositionY() - _endnode_->getPositionY()))
#define PL_NONAV_GET_CONTEENT_SIZE(_node_) CCSizeMake(640, _node_->getPositionY())

#endif /* __APPMACROS_H__ */

