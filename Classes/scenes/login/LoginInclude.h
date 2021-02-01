#ifndef __PILI_LOGIN_INCLUDE__
#define __PILI_LOGIN_INCLUDE__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/GameManager.h"

enum 
{
	kUserRegisterTag,
	kSelectServerListTag,
	kEnterGameTag
};

enum PageFrom{
	kFromNone,
	kFromUpdate
};

enum PopupView{
	kLoginView,
	kLoginPWView,
	KRegisterView,
	kChangePWView,
	kAnnounceView,
	KBindingView,
	kRemoveUserTipsView
};

enum ClearMenuTag{
	kMenuTag0,
	kMenuTag1,
	kMenuTag2,
	kMenuTag3,
	kMenuTag4   //同意条款按钮
};

enum LoginPage{
	kLoginPageUpdate = 0,
	KLoginTypeSelect,
	KUserSelect,
	kLoginPageSelectServer
};

enum LoginType{
	kLogin,
	kQuickLogin,
	kFacebookLogin,
	kGoogelPlusLogin
};

class LoginDelegate
{
public:
	virtual void showInfo(LoginPage, PageFrom from = kFromNone) = 0;
	virtual void selectServer(platform::ServerInfo*) = 0;
	virtual void openPopup(PopupView) = 0;
    virtual void getAnnounce() = 0;
	virtual void loginGame(const char* userName = NULL, const char* password = NULL) = 0;
	virtual void registerUser(const char* userName = NULL, const char* password = NULL, const char* confirmPassword = NULL, const char* email = NULL) = 0;
	virtual void bindAccount(const char* userName = NULL, const char* password = NULL, const char* confirmPassword = NULL, const char* email = NULL) = 0;
	virtual void resetPass(const char* userName = NULL, const char* oldPass = NULL, const char* newPass = NULL, const char* confirmPass = NULL) = 0;
};


#define LOGIN_EDIT_BOX_SIZE CCSizeMake(265, 40)

#endif