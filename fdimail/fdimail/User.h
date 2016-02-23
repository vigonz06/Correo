#ifndef USER
#define USER

#include "ContactList.h"
#include "TrayList.h"
#include "Encript.h"
#include <fstream>
#include <string>

/*----------------------------
Contains the user variables, and the
functions necessary for working with it
------------------------------*/

class User
{
private:

	std::string id;
	std::string password;

	TrayList inbox;
	TrayList outbox;
	TrayList recycling;
	ContactList contactList;

public:

	User(const std::string idUser, const std::string password);
	User() : contactList(){}

	std::string& setId(std::string newId) { return id = newId; }
	std::string& setPassword(std::string newPassword) { return password = sha1(newPassword); }
	bool checkPassword(const std::string &possible_password) const { return password == sha1(possible_password); }

	const std::string& getId() const { return id; }

	TrayList* getInbox() { return &inbox; }
	TrayList* getOutbox() { return &outbox; }
	TrayList* getRecycling() { return &recycling; }
	ContactList* getContactlist() { return &contactList; }

	void save(std::ofstream &file)const;
	bool load(std::ifstream &file);
};
#endif