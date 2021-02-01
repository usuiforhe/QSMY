/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
 http://www.cocos2d-x.org
 
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

#include "DEditBoxImplAndroid.h"
#include "AppMacros.h"
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>
#include <string>
#define  LOG_TAG    "DEditBoxImplAndroid.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",__VA_ARGS__)

#define  CLASS_NAME "org/cocos2dx/lib/QsmyHelper"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "DEditBox.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.h"
//#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

static EditTextCallback s_pfEditTextCallback = NULL;
static void* s_ctx = NULL;
static EditTextCallbackHide s_pfEditTextCallbackHide = NULL;

NS_CC_EXT_BEGIN

	extern "C" {

		JNIEXPORT void JNICALL Java_org_cocos2dx_lib_QsmyHelper_nativeSetMultiEditTextDialogResult(JNIEnv * env, jobject obj, jbyteArray text) {
			jsize  size = env->GetArrayLength(text);

			if (size > 0) {
				jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0);
				char* pBuf = (char*)malloc(size+1);
				if (pBuf != NULL) {
					memcpy(pBuf, data, size);
					pBuf[size] = '\0';
					// pass data to edittext's delegate
					if (s_pfEditTextCallback) s_pfEditTextCallback(pBuf, s_ctx);
					free(pBuf);
				}
				env->ReleaseByteArrayElements(text, data, 0);
			} else {
				if (s_pfEditTextCallback) s_pfEditTextCallback("", s_ctx);
			}
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_lib_QsmyHelper_nativeHideEditMultiText(JNIEnv * env, jobject obj) {
				if (s_pfEditTextCallbackHide){
					s_pfEditTextCallbackHide(s_ctx);
				}
		}

}

DEditBoxImpl* __createSystemEditBox(DEditBox* pEditBox)
{
    return new DEditBoxImplAndroid(pEditBox);
}

DEditBoxImplAndroid::DEditBoxImplAndroid(DEditBox* pEditText)
: DEditBoxImpl(pEditText)
, m_pLabel(NULL)
, m_pLabelPlaceHolder(NULL)
, m_eEditBoxInputMode(kEditBoxInputModeSingleLine)
, m_eEditBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, m_eKeyboardReturnType(kKeyboardReturnTypeDefault)
, m_colText(ccWHITE)
, m_colPlaceHolder(ccGRAY)
, m_nMaxLength(-1)
, m_obAnchorPoint(ccp(0.5f, 0.5f))
{
    
}

DEditBoxImplAndroid::~DEditBoxImplAndroid()
{
	
}

void DEditBoxImplAndroid::doAnimationWhenKeyboardMove(float duration, float distance)
{ // don't need to be implemented on android platform.
	
}

static const int CC_EDIT_BOX_PADDING = 5;

bool DEditBoxImplAndroid::initWithSize(const CCSize& size)
{
    int fontSize = getFontSizeAccordingHeightJni(size.height-12);
    m_pLabel = CCLabelTTF::create("", "", size.height-12,CCSize(size.width-30,size.height),kCCTextAlignmentLeft);
	// align the text vertically center
    m_pLabel->setAnchorPoint(ccp(0, 1.f));
    m_pLabel->setPosition(ccp(CC_EDIT_BOX_PADDING, size.height-CC_EDIT_BOX_PADDING));
    m_pLabel->setColor(m_colText);
    m_pEditBox->addChild(m_pLabel);
	
    m_pLabelPlaceHolder = CCLabelTTF::create("", "", size.height-12);
	// align the text vertically center
    m_pLabelPlaceHolder->setAnchorPoint(ccp(0, 1.f));
    m_pLabelPlaceHolder->setPosition(ccp(CC_EDIT_BOX_PADDING, size.height-CC_EDIT_BOX_PADDING));
    m_pLabelPlaceHolder->setVisible(false);
    m_pLabelPlaceHolder->setColor(m_colPlaceHolder);
    m_pEditBox->addChild(m_pLabelPlaceHolder);
    
    m_EditSize = size;
	setContentSize(size);
    return true;
}

void DEditBoxImplAndroid::setFont(const char* pFontName, int fontSize)
{
	if(m_pLabel != NULL) {
		m_pLabel->setFontName(pFontName);
		m_pLabel->setFontSize(fontSize);
	}
	
	if(m_pLabelPlaceHolder != NULL) {
		m_pLabelPlaceHolder->setFontName(pFontName);
		m_pLabelPlaceHolder->setFontSize(fontSize);
	}
}

void DEditBoxImplAndroid::setFontColor(const ccColor3B& color)
{
    m_colText = color;
    m_pLabel->setColor(color);
}

void DEditBoxImplAndroid::setPlaceholderFont(const char* pFontName, int fontSize)
{
	if(m_pLabelPlaceHolder != NULL) {
		m_pLabelPlaceHolder->setFontName(pFontName);
		m_pLabelPlaceHolder->setFontSize(fontSize);
	}
}

void DEditBoxImplAndroid::setPlaceholderFontColor(const ccColor3B& color)
{
    m_colPlaceHolder = color;
    m_pLabelPlaceHolder->setColor(color);
}

void DEditBoxImplAndroid::setInputMode(EditBoxInputMode inputMode)
{
    m_eEditBoxInputMode = inputMode;
}

void DEditBoxImplAndroid::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
}

int DEditBoxImplAndroid::getMaxLength()
{
    return m_nMaxLength;
}

void DEditBoxImplAndroid::setInputFlag(EditBoxInputFlag inputFlag)
{
    m_eEditBoxInputFlag = inputFlag;
}

void DEditBoxImplAndroid::setReturnType(KeyboardReturnType returnType)
{
    m_eKeyboardReturnType = returnType;
}

bool DEditBoxImplAndroid::isEditing()
{
    return false;
}

void DEditBoxImplAndroid::setText(const char* pText)
{
    if (pText != NULL)
    {
        m_strText = pText;
		
        if (m_strText.length() > 0)
        {
            m_pLabelPlaceHolder->setVisible(false);
			
            std::string strToShow;
			
            if (kEditBoxInputFlagPassword == m_eEditBoxInputFlag)
            {
                long length = cc_utf8_strlen(m_strText.c_str(), -1);
                for (long i = 0; i < length; i++)
                {
                    strToShow.append("\u25CF");
                }
            }
            else
            {
                strToShow = m_strText;
            }

			m_pLabel->setString(strToShow.c_str());

			// Clip the text width to fit to the text box
			float fMaxWidth = m_EditSize.width - CC_EDIT_BOX_PADDING * 2;
			CCRect clippingRect = m_pLabel->getTextureRect();
			if(clippingRect.size.width > fMaxWidth) {
				clippingRect.size.width = fMaxWidth;
				m_pLabel->setTextureRect(clippingRect);
			}

        }
        else
        {
            m_pLabelPlaceHolder->setVisible(true);
            m_pLabel->setString("");
        }
		
    }
}

const char*  DEditBoxImplAndroid::getText(void)
{
    return m_strText.c_str();
}

void DEditBoxImplAndroid::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        m_strPlaceHolder = pText;
        if (m_strPlaceHolder.length() > 0 && m_strText.length() == 0)
        {
            m_pLabelPlaceHolder->setVisible(true);
        }
		
        m_pLabelPlaceHolder->setString(m_strPlaceHolder.c_str());
    }
}


void DEditBoxImplAndroid::setPosition(const CCPoint& pos)
{ // don't need to be implemented on android platform.
	m_obPosition = pos;
}

void DEditBoxImplAndroid::setVisible(bool visible)
{ // don't need to be implemented on android platform.

}

void DEditBoxImplAndroid::setContentSize(const CCSize& size)
{ // don't need to be implemented on android platform.
	m_tContentSize = size;
}

void DEditBoxImplAndroid::setAnchorPoint(const CCPoint& anchorPoint)
{ // don't need to be implemented on android platform.
	m_obAnchorPoint = anchorPoint;
	setPosition(m_obPosition);
}

void DEditBoxImplAndroid::visit(void)
{ // don't need to be implemented on android platform.
    
}

void DEditBoxImplAndroid::onEnter(void)
{ // don't need to be implemented on android platform.
    
}

static void editBoxCallbackFunc(const char* pText, void* ctx)
{
    DEditBoxImplAndroid* thiz = (DEditBoxImplAndroid*)ctx;
    thiz->setText(pText);
	
    if (thiz->getDelegate() != NULL)
    {
        thiz->getDelegate()->editBoxTextChanged(thiz->getDEditBox(), thiz->getText());
        thiz->getDelegate()->editBoxEditingDidEnd(thiz->getDEditBox());
        thiz->getDelegate()->editBoxReturn(thiz->getDEditBox());
    }
    
    DEditBox* pEditBox = thiz->getDEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "ended",pEditBox);
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "return",pEditBox);
    }

	thiz->onEndEditing();
}

static void editBoxCallbackFuncHide(void* stx)
{
	DEditBoxImplAndroid* thiz = (DEditBoxImplAndroid*)stx;
	thiz->hideText();
}

void DEditBoxImplAndroid::hideText()
{
	m_pLabel->setVisible(false);
	m_pLabelPlaceHolder->setVisible(false);
}

void DEditBoxImplAndroid::onEndEditing()
{
	m_pLabel->setVisible(true);
	if(strlen(getText()) == 0)
	{
        m_pLabel->setString("");
		m_pLabelPlaceHolder->setVisible(true);
	}
	else
	{
		m_pLabelPlaceHolder->setVisible(false);
	}
}

void DEditBoxImplAndroid::openKeyboard()
{
    if (m_pDelegate != NULL)
    {
        m_pDelegate->editBoxEditingDidBegin(m_pEditBox);
    }
    DEditBox* pEditBox = this->getDEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
    }
	
    showEditTextDialogJNI(true,
						  m_strText.c_str(),
						  m_eEditBoxInputMode,
						  m_eEditBoxInputFlag,
						  m_eKeyboardReturnType,
						  m_nMaxLength,
						  m_pLabel->getFontSize()*cocos2d::CCEGLView::sharedOpenGLView()->getScaleX(),
						  editBoxCallbackFunc,
						  editBoxCallbackFuncHide,
						  (void*)this  );
	
}

void DEditBoxImplAndroid::showEditTextDialogJNI(bool isMulti,const char* pszMessage, int nInputMode, int nInputFlag, int nReturnType, int nMaxLength, int nFontSize, EditTextCallback pfEditTextCallback,EditTextCallbackHide pfEditTextCallbackHide, void* ctx) {
	
	CCSize contentSize = m_pEditBox->getContentSize();
	CCRect rect = CCRectMake(0, 0, contentSize.width, contentSize.height);
    rect = CCRectApplyAffineTransform(rect, m_pEditBox->nodeToWorldTransform());	
	CCPoint designCoord = ccp(rect.origin.x, rect.origin.y + rect.size.height);
	CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView(); 
    CCPoint visiblePos = ccp(designCoord.x * eglView->getScaleX(), designCoord.y * eglView->getScaleY());
    CCPoint screenGLPos = ccpAdd(visiblePos, eglView->getViewPortRect().origin);  
	CCSize controlSize = CCSizeMake(m_tContentSize.width * eglView->getScaleX(),m_tContentSize.height * eglView->getScaleY());

	
	int fontColorR = m_colText.r;
	int fontColorG = m_colText.g;
	int fontColorB = m_colText.b;

	s_pfEditTextCallback = pfEditTextCallback;
	s_pfEditTextCallbackHide = pfEditTextCallbackHide;
	s_ctx = ctx;

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showEditTextDialog", "(Ljava/lang/String;ZIIIIIIIIIIII)V")) {

		jstring stringArg1 = t.env->NewStringUTF(pszMessage);

		jint jLeft = (int)screenGLPos.x;
		jint jTop = (int)screenGLPos.y;
		jint jWidth = (int)controlSize.width;
		jint jHeight = (int)controlSize.height;
		
		t.env->CallStaticVoidMethod(t.classID, t.methodID,stringArg1,isMulti, nInputMode, nInputFlag, nReturnType, nMaxLength,jLeft,jTop,jWidth,jHeight,nFontSize,fontColorR,fontColorG,fontColorB);

		t.env->DeleteLocalRef(stringArg1);
		t.env->DeleteLocalRef(t.classID);
	}
}

void DEditBoxImplAndroid::closeKeyboard()
{

}

NS_CC_EXT_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */

