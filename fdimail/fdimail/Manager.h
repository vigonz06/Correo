#ifndef MANAGER
#define MANAGER

#include "MailList.h"
#include "UserList.h"
#include <string>

class Manager
{
private:

	static Manager* manager;
	std::string domain;
	MailList mailList;
	UserList userList;

	void deleteAlias(User* user);
	void shutDown();
	void bootUp();

public:

	Manager(const std::string &new_domain);
	~Manager();

	static Manager* getManager() { return manager; }
	MailList* getMailList() { return &mailList; }
	UserList* getUserList() { return &userList; }

	void deleteMail(TrayList* box, const ElemTray* elem);
	void sendMail(User* user, Mail* mail);
	bool answer(User* user, Mail* mail);
	void deleteAccount(User* user);

	User* createAccount();
	User* registerUser();
};
#endif