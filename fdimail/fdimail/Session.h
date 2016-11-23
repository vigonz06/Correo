#ifndef SESSION
#define SESSION

#include "VisibleTrayList.h"
#include "Manager.h"
#include "User.h"
#include "Mail.h"

class Session
{
private:

	VisibleTrayList visible;
	Manager* manager;
	int active_list;
	User* user;

	void launch();

	void forwardMail(Mail* &originalMail);
	void answerMail(Mail* &originalMail);
	void restoreMail();
	void mailOptions();
	void changeTray();
	void deleteMail();
	void readMail();
	void sendMail();	
	void fastRead();

	void AccountOptions(int &option);
	void AliasOptions();
	void AddFastName();

	void filterOptions(Filter filter);
	void chooseFilter(Filter filter);
	void chooseOrder(Filter filter);

	void changeUsername();
	void changePassword();

public:

	Session(Manager* manager);
	~Session();
	
	VisibleTrayList* getVisible() { return &visible; }
	int get_active_list() { return active_list; }
	Manager* getManager() { return manager; }
	User* getUser() { return user; }
	TrayList* active_tray();
};
#endif
