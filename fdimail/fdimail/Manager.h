#ifndef MANAGER
#define MANAGER

#include "MailList.h"
#include "UserList.h"
#include <string>

/*----------------------------
Manager holds the mail and user databases
He is responsible for loading and saving them to text files
It acts as the backend server, providing service to Sessions requesting GETS and POSTS
------------------------------*/

class Manager
{
private:

	static Manager* manager;

	std::string domain;
	MailList mailList;
	UserList userList;

	void loadUsers(std::string &name);
	void loadMails(std::string &name);

public:

	Manager(const std::string &new_domain);
	~Manager();

	static Manager* getManager(){ return manager; }

	MailList* getMailList(){ return &mailList; }
	UserList* getUserList(){ return &userList; }

	void bootUp();
	void shutDown();

	User* registerUser();
	User* createAccount();

	void deleteAccount(User* user);
	bool answer(User* user, Mail* mail);
	void sendMail(User* user, Mail* mail);
	void deleteMail(TrayList* box, const tElemTray* elem);
};
#endif