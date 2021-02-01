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
#include "DEditBoxImplIOS.h"
#import "sys/utsname.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define kLabelZOrder  9999

#include "DEditBox.h"
#import "EAGLView.h"

#define getDEditBoxImplIOS() ((cocos2d::extension::DEditBoxImplIOS*)editBox_)

static const int CC_EDIT_BOX_PADDING = 0;

@implementation CustomUITextView
- (CGRect)textRectForBounds:(CGRect)bounds {
    float padding = CC_EDIT_BOX_PADDING * cocos2d::CCEGLView::sharedOpenGLView()->getScaleX() / [[EAGLView sharedEGLView] contentScaleFactor ];
    return CGRectMake(bounds.origin.x + padding, bounds.origin.y + padding,
                      bounds.size.width - padding*2, bounds.size.height - padding*2);
}
- (CGRect)editingRectForBounds:(CGRect)bounds {
    return [self textRectForBounds:bounds];
}
@end


@implementation DEditBoxImplOCIOS

@synthesize textField = textField_;
@synthesize editState = editState_;
@synthesize editBox = editBox_;

- (void)dealloc
{
    [textField_ resignFirstResponder];
    [textField_ removeFromSuperview];
    self.textField = NULL;
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox
{
    self = [super init];
    
    do
    {
        if (self == nil) break;
        editState_ = NO;
        self.textField = [[[CustomUITextView alloc] initWithFrame: frameRect] autorelease];
        if (!textField_) break;
        [textField_ setTextColor:[UIColor whiteColor]];
        textField_.font = [UIFont systemFontOfSize:frameRect.size.height*2/3]; //TODO need to delete hard code here.
		textField_.textAlignment = NSTextAlignmentLeft;
        textField_.backgroundColor = [UIColor clearColor];
        //textField_.borderStyle = UITextBorderStyleNone;
        if([self isIos7])
        {
            [textField_ setTextContainerInset:UIEdgeInsetsMake(0,-5,0,10)];
        }else
        {
            [textField_ setContentInset:UIEdgeInsetsMake(-8,-8,0,0)];
        }
        
        textField_.scrollEnabled = NO;
        textField_.delegate = self;
        textField_.returnKeyType = UIReturnKeyDone;
        //[textField_ addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        self.editBox = editBox;
        
        [[EAGLView sharedEGLView] addSubview:textField_];
		textField_.hidden = YES;
        
        return self;
    }while(0);
    
    return nil;
}

-(bool) isIos7
{
    return [[[UIDevice currentDevice]systemVersion]floatValue]>=7.0f;
}

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance
{
    id eglView = [EAGLView sharedEGLView];
    [eglView doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
}

-(void) setPosition:(CGPoint) pos
{
    CGRect frame = [textField_ frame];
    frame.origin = pos;
    [textField_ setFrame:frame];
}

-(void) setContentSize:(CGSize) size
{
    CGRect frame = [textField_ frame];
    frame.size = size;
    [textField_ setFrame:frame];
}

-(void) visit
{
    
}

-(void) openKeyboard
{
    [textField_ becomeFirstResponder];
}

-(void) closeKeyboard
{
    [textField_ resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextView *)sender
{
    if (sender == textField_) {
        [sender resignFirstResponder];
    }
    return NO;
}

-(void)animationSelector
{
    id eglView = [EAGLView sharedEGLView];
    [eglView doAnimationWhenAnotherEditBeClicked];
}

- (BOOL)textViewShouldBeginEditing:(UITextView *)sender        // return NO to disallow editing.
{
    CCLOG("textFieldShouldBeginEditing...");
    editState_ = YES;
    id eglView = [EAGLView sharedEGLView];
    if ([eglView isKeyboardShown])
    {
        [self performSelector:@selector(animationSelector) withObject:nil afterDelay:0.0f];
    }
    cocos2d::extension::DEditBoxDelegate* pDelegate = getDEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidBegin(getDEditBoxImplIOS()->getDEditBox());
    }
    
    cocos2d::extension::DEditBox*  pEditBox= getDEditBoxImplIOS()->getDEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
    }
    return YES;
}


- (BOOL)textViewShouldEndEditing:(UITextView *)sender
{
    CCLOG("textFieldShouldEndEditing...");
    editState_ = NO;
    //getDEditBoxImplIOS()->setText(getDEditBoxImplIOS()->getText());
    getDEditBoxImplIOS()->refreshInactiveText();
    
    cocos2d::extension::DEditBoxDelegate* pDelegate = getDEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidEnd(getDEditBoxImplIOS()->getDEditBox());
        pDelegate->editBoxReturn(getDEditBoxImplIOS()->getDEditBox());
    }
    
    cocos2d::extension::DEditBox*  pEditBox= getDEditBoxImplIOS()->getDEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "ended",pEditBox);
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "return",pEditBox);
    }
	
	if(editBox_ != nil)
	{
		getDEditBoxImplIOS()->onEndEditing();
	}
    return YES;
}



/**
 * Delegate method called before the text has been changed.
 * @param textField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)textView:(UITextView*) textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)string
{
    if([string isEqualToString:@"\n"])
    {
        [textView resignFirstResponder];
        return NO;
    }
    
    if (getDEditBoxImplIOS()->getMaxLength() < 0)
    {
        return YES;
    }
    
    NSUInteger oldLength = [textView.text length];
    NSUInteger replacementLength = [string length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
    if(newLength<oldLength) return YES;
    
    return newLength <= getDEditBoxImplIOS()->getMaxLength();
}



/**
 * Called each time when the text field's text has changed.
 */
- (void) textViewDidChange:(UITextView *)textView
{
    // NSLog(@"text is %@", self.textField.text);
    cocos2d::extension::DEditBoxDelegate* pDelegate = getDEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxTextChanged(getDEditBoxImplIOS()->getDEditBox(), getDEditBoxImplIOS()->getText());
    }
    
    cocos2d::extension::DEditBox*  pEditBox= getDEditBoxImplIOS()->getDEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
    }

}


@end


NS_CC_EXT_BEGIN

DEditBoxImpl* __createSystemEditBox(DEditBox* pEditBox)
{
    return new DEditBoxImplIOS(pEditBox);
}

DEditBoxImplIOS::DEditBoxImplIOS(DEditBox* pEditText)
: DEditBoxImpl(pEditText)
, m_pLabel(NULL)
, m_pLabelPlaceHolder(NULL)
, m_systemControl(NULL)
, m_obAnchorPoint(ccp(0.5f, 0.5f))
, m_nMaxTextLength(-1)
{
    m_bInRetinaMode = [[EAGLView sharedEGLView] contentScaleFactor] == 2.0f ? true : false;
}

DEditBoxImplIOS::~DEditBoxImplIOS()
{
    [m_systemControl release];
}

void DEditBoxImplIOS::doAnimationWhenKeyboardMove(float duration, float distance)
{
    if ([m_systemControl isEditState] || distance < 0.0f)
    {
        [m_systemControl doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
    }
}

bool DEditBoxImplIOS::initWithSize(const CCSize& size)
{
    do 
    {
        CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();

        CGRect rect = CGRectMake(0, 0, size.width * eglView->getScaleX(),size.height * eglView->getScaleY());

        if (m_bInRetinaMode)
        {
            rect.size.width /= 2.0f;
            rect.size.height /= 2.0f;
        }
        
        m_systemControl = [[DEditBoxImplOCIOS alloc] initWithFrame:rect editBox:this];
        if (!m_systemControl) break;
        
		initInactiveLabels(size);
        setContentSize(size);
		
        return true;
    }while (0);
    
    return false;
}

void DEditBoxImplIOS::initInactiveLabels(const CCSize& size)
{
	const char* pDefaultFontName = [[m_systemControl.textField.font fontName] UTF8String];

	m_pLabel = CCLabelTTF::create("", "", 0.0f,CCSize(size.width-30,size.height),kCCTextAlignmentLeft);
    m_pLabel->setAnchorPoint(ccp(0, 1.f));
    m_pLabel->setFontFillColor(ccWHITE);
    m_pLabel->setVisible(false);
    m_pEditBox->addChild(m_pLabel, kLabelZOrder);
	
    m_pLabelPlaceHolder = CCLabelTTF::create("", "", 0.0f,size,kCCTextAlignmentLeft);
	// align the text vertically center
    m_pLabelPlaceHolder->setAnchorPoint(ccp(0, 1.f));
    m_pLabelPlaceHolder->setFontFillColor(ccGRAY);
    m_pEditBox->addChild(m_pLabelPlaceHolder, kLabelZOrder);
    
    setFont(pDefaultFontName, size.height*2/3);
    setPlaceholderFont(pDefaultFontName, size.height*2/3);
}

void DEditBoxImplIOS::placeInactiveLabels() {
    m_pLabel->setPosition(ccp(CC_EDIT_BOX_PADDING, m_pEditBox->getContentSize().height-CC_EDIT_BOX_PADDING));
    m_pLabelPlaceHolder->setPosition(ccp(CC_EDIT_BOX_PADDING, m_pEditBox->getContentSize().height-CC_EDIT_BOX_PADDING));
}

void DEditBoxImplIOS::setInactiveText(const char* pText)
{
	if(m_systemControl.textField.secureTextEntry == YES)
	{
		std::string passwordString;
		for(int i = 0; i < strlen(pText); ++i)
			passwordString.append("\u25CF");
		m_pLabel->setString(passwordString.c_str());
	}
	else
		m_pLabel->setString(getText());
	
	// Clip the text width to fit to the text box
	float fMaxWidth = m_pEditBox->getContentSize().width - CC_EDIT_BOX_PADDING * 2;
	CCRect clippingRect = m_pLabel->getTextureRect();
	if(clippingRect.size.width > fMaxWidth) {
		clippingRect.size.width = fMaxWidth;
		m_pLabel->setTextureRect(clippingRect);
	}
}

void DEditBoxImplIOS::setFont(const char* pFontName, int fontSize)
{
    bool isValidFontName = true;
	if(pFontName == NULL || strlen(pFontName) == 0) {
        isValidFontName = false;
    }

    float retinaFactor = m_bInRetinaMode ? 2.0f : 1.0f;
	NSString * fntName = [NSString stringWithUTF8String:pFontName];
    float scaleFactor = CCEGLView::sharedOpenGLView()->getScaleX();
    UIFont *textFont = nil;
    if (isValidFontName) {
        textFont = [UIFont fontWithName:fntName size:fontSize * scaleFactor / retinaFactor];
    }
    
    if (!isValidFontName || textFont == nil){
        textFont = [UIFont systemFontOfSize:fontSize * scaleFactor / retinaFactor];
    }

	if(textFont != nil) {
		[m_systemControl.textField setFont:textFont];
    }

	m_pLabel->setFontName(pFontName);
	m_pLabel->setFontSize(fontSize);
	m_pLabelPlaceHolder->setFontName(pFontName);
	m_pLabelPlaceHolder->setFontSize(fontSize);
}

void DEditBoxImplIOS::setFontColor(const ccColor3B& color)
{
    m_systemControl.textField.textColor = [UIColor colorWithRed:color.r / 255.0f green:color.g / 255.0f blue:color.b / 255.0f alpha:1.0f];
	m_pLabel->setFontFillColor(color);
    m_pLabelPlaceHolder->setFontFillColor(color);
}

void DEditBoxImplIOS::setPlaceholderFont(const char* pFontName, int fontSize)
{
	// TODO need to be implemented.
}

void DEditBoxImplIOS::setPlaceholderFontColor(const ccColor3B& color)
{
	m_pLabelPlaceHolder->setFontFillColor(color);
}

void DEditBoxImplIOS::setInputMode(EditBoxInputMode inputMode)
{
    switch (inputMode)
    {
        case kEditBoxInputModeEmailAddr:
            m_systemControl.textField.keyboardType = UIKeyboardTypeEmailAddress;
            break;
        case kEditBoxInputModeNumeric:
            m_systemControl.textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
            break;
        case kEditBoxInputModePhoneNumber:
            m_systemControl.textField.keyboardType = UIKeyboardTypePhonePad;
            break;
        case kEditBoxInputModeUrl:
            m_systemControl.textField.keyboardType = UIKeyboardTypeURL;
            break;
        case kEditBoxInputModeDecimal:
            m_systemControl.textField.keyboardType = UIKeyboardTypeDecimalPad;
            break;
        case kEditBoxInputModeSingleLine:
            m_systemControl.textField.keyboardType = UIKeyboardTypeDefault;
            break;
        default:
            m_systemControl.textField.keyboardType = UIKeyboardTypeDefault;
            break;
    }
}

void DEditBoxImplIOS::setMaxLength(int maxLength)
{
    m_nMaxTextLength = maxLength;
}

int DEditBoxImplIOS::getMaxLength()
{
    return m_nMaxTextLength;
}

void DEditBoxImplIOS::setInputFlag(EditBoxInputFlag inputFlag)
{
    switch (inputFlag)
    {
        case kEditBoxInputFlagPassword:
            m_systemControl.textField.secureTextEntry = YES;
            break;
        case kEditBoxInputFlagInitialCapsWord:
            m_systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
            break;
        case kEditBoxInputFlagInitialCapsSentence:
            m_systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
            break;
        case kEditBoxInputFlagInitialCapsAllCharacters:
            m_systemControl.textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
            break;
        case kEditBoxInputFlagSensitive:
            m_systemControl.textField.autocorrectionType = UITextAutocorrectionTypeNo;
            break;
        default:
            break;
    }
}

void DEditBoxImplIOS::setReturnType(KeyboardReturnType returnType)
{
    switch (returnType) {
        case kKeyboardReturnTypeDefault:
            m_systemControl.textField.returnKeyType = UIReturnKeyDefault;
            break;
        case kKeyboardReturnTypeDone:
            m_systemControl.textField.returnKeyType = UIReturnKeyDone;
            break;
        case kKeyboardReturnTypeSend:
            m_systemControl.textField.returnKeyType = UIReturnKeySend;
            break;
        case kKeyboardReturnTypeSearch:
            m_systemControl.textField.returnKeyType = UIReturnKeySearch;
            break;
        case kKeyboardReturnTypeGo:
            m_systemControl.textField.returnKeyType = UIReturnKeyGo;
            break;
        default:
            m_systemControl.textField.returnKeyType = UIReturnKeyDefault;
            break;
    }
}

bool DEditBoxImplIOS::isEditing()
{
    return [m_systemControl isEditState] ? true : false;
}

void DEditBoxImplIOS::refreshInactiveText()
{
    const char* text = getText();
	if(m_systemControl.textField.hidden == YES) {
		setInactiveText(text);
		if(strlen(text) == 0)
		{
			m_pLabel->setVisible(false);
            // m_systemControl.textField.hidden = YES;
			m_pLabelPlaceHolder->setVisible(true);
		}
		else
		{
			m_pLabel->setVisible(true);
            //m_systemControl.textField.hidden = NO;
			m_pLabelPlaceHolder->setVisible(false);
		}
	}
}

void DEditBoxImplIOS::setText(const char *pText)
{
    NSString * nsText = [NSString stringWithUTF8String:pText];
    if([nsText compare:m_systemControl.textField.text] !=NSOrderedSame)
    {
        m_systemControl.textField.text = nsText;
    }
    refreshInactiveText();
}

NSString* DEditBoxImplIOS::removeSiriString(NSString* str)
{
    NSString* siriString = @"\xef\xbf\xbc";
    return [str stringByReplacingOccurrencesOfString:siriString withString:@""];
}

//void DEditBoxImplIOS::setText(const char* pText)
//{
//    m_systemControl.textField.text = [NSString stringWithUTF8String:pText];
//	if(m_systemControl.textField.hidden == YES) {
//		setInactiveText(pText);
//		if(strlen(pText) == 0)
//		{
//			m_pLabel->setVisible(false);
//           // m_systemControl.textField.hidden = YES;
//			m_pLabelPlaceHolder->setVisible(true);
//		}
//		else
//		{
//			m_pLabel->setVisible(true);
//            //m_systemControl.textField.hidden = NO;
//			m_pLabelPlaceHolder->setVisible(false);
//		}
//	}
//}

const char*  DEditBoxImplIOS::getText(void)
{
    return [removeSiriString(m_systemControl.textField.text) UTF8String];
    //return [m_systemControl.textField.text UTF8String];
}

void DEditBoxImplIOS::setPlaceHolder(const char* pText)
{
    //m_systemControl.textField.placeholder = [NSString stringWithUTF8String:pText];
	m_pLabelPlaceHolder->setString(pText);
}

static CGPoint convertDesignCoordToScreenCoord(const CCPoint& designCoord, bool bInRetinaMode)
{
    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
    float viewH = (float)[[EAGLView sharedEGLView] getHeight];
    
    CCPoint visiblePos = ccp(designCoord.x * eglView->getScaleX(), designCoord.y * eglView->getScaleY());
    CCPoint screenGLPos = ccpAdd(visiblePos, eglView->getViewPortRect().origin);
    
    CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
    
    if (bInRetinaMode)
    {
        screenPos.x = screenPos.x / 2.0f;
        screenPos.y = screenPos.y / 2.0f;
    }
    CCLOG("[EditBox] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
    return screenPos;
}

void DEditBoxImplIOS::setPosition(const CCPoint& pos)
{
	m_obPosition = pos;
	adjustTextFieldPosition();
}

void DEditBoxImplIOS::setVisible(bool visible)
{
    m_systemControl.textField.hidden = !visible;
}

void DEditBoxImplIOS::setContentSize(const CCSize& size)
{
    m_tContentSize = size;
    CCLOG("[Edit text] content size = (%f, %f)", size.width, size.height);
    placeInactiveLabels();
    CCEGLViewProtocol* eglView = CCEGLView::sharedOpenGLView();
    CGSize controlSize = CGSizeMake(size.width * eglView->getScaleX(),size.height * eglView->getScaleY());
    
    if (m_bInRetinaMode)
    {
        controlSize.width /= 2.0f;
        controlSize.height /= 2.0f;
    }
    [m_systemControl setContentSize:controlSize];
}

void DEditBoxImplIOS::setAnchorPoint(const CCPoint& anchorPoint)
{
    CCLOG("[Edit text] anchor point = (%f, %f)", anchorPoint.x, anchorPoint.y);
	m_obAnchorPoint = anchorPoint;
	setPosition(m_obPosition);
}

void DEditBoxImplIOS::visit(void)
{
    
}

void DEditBoxImplIOS::onEnter(void)
{
    adjustTextFieldPosition();
    const char* pText = getText();
    if (pText) {
        setInactiveText(pText);
    }
}

void DEditBoxImplIOS::adjustTextFieldPosition()
{
	CCSize contentSize = m_pEditBox->getContentSize();
	CCRect rect = CCRectMake(0, 0, contentSize.width, contentSize.height);
    rect = CCRectApplyAffineTransform(rect, m_pEditBox->nodeToWorldTransform());
	
	CCPoint designCoord = ccp(rect.origin.x, rect.origin.y + rect.size.height);
    [m_systemControl setPosition:convertDesignCoordToScreenCoord(designCoord, m_bInRetinaMode)];
}

void DEditBoxImplIOS::openKeyboard()
{
	m_pLabel->setVisible(false);
    m_systemControl.textField.hidden = NO;
	m_pLabelPlaceHolder->setVisible(false);
    adjustTextFieldPosition();
    [m_systemControl openKeyboard];
}

void DEditBoxImplIOS::closeKeyboard()
{
    [m_systemControl closeKeyboard];
}

void DEditBoxImplIOS::onEndEditing()
{
	m_systemControl.textField.hidden = YES;
	if(strlen(getText()) == 0)
	{
		m_pLabel->setVisible(false);
		m_pLabelPlaceHolder->setVisible(true);
	}
	else
	{
		m_pLabel->setVisible(true);
		m_pLabelPlaceHolder->setVisible(false);
		setInactiveText(getText());
	}
}

NS_CC_EXT_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */


