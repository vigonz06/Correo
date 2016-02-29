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

	const std::string& getId() const { return id; }

	TrayList* getInbox() { return &inbox; }
	TrayList* getOutbox() { return &outbox; }
	TrayList* getRecycling() { return &recycling; }
	ContactList* getContactlist() { return &contactList; }

	void setId(std::string newId) { id = newId; }
	void setPassword(std::string newPassword) { password = sha1(newPassword); }
	bool checkPassword(const std::string &possible_password) const { return password == sha1(possible_password); }

	void save(std::ofstream &file)const;
	bool load(std::ifstream &file);
};
#endif