#ifndef SESSION
#define SESSION

#include "VisibleTrayList.h"
#include "GraphInter.h"
#include "Manager.h"
#include "User.h"
#include "Mail.h"

/*----------------------------
This class is responsible for the user session, 
for what he can do on it
------------------------------*/

class Session
{
private:

	VisibleTrayList visible;
	Manager* manager;
	User* user;

	int active_list;
	void changeTray();

	void launch();

	void readMail();
	void sendMail();
	void answerMail(Mail* &originalMail);
	void forwardMail(Mail* &originalMail);
	void deleteMail();
	void restoreMail();
	void fastRead();

	void AccountOptions(int &option);
	void AddFastName();
	void AliasOptions();

	void filterOptions(Filter filter);
	void chooseFilter(Filter filter);
	void chooseOrder(Filter filter);

	void changeUsername();
	void changePassword();

public:

	Session(Manager* manager);
	~Session();
	
	VisibleTrayList* get_visible(){ return &visible; }
	int get_active_list(){ return active_list; }
	Manager* getManager(){ return manager; }
	User* getUser(){ return user; }
	TrayList* active_tray();
};
#endif