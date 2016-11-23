#ifndef USER
#define USER

#include "ContactList.h"
#include "TrayList.h"
#include "Encript.h"
#include <fstream>
#include <string>

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

	std::string getId() { return id; }
	TrayList* getInbox() { return &inbox; }
	TrayList* getOutbox() { return &outbox; }
	TrayList* getRecycling() { return &recycling; }
	ContactList* getContactlist() { return &contactList; }

	void setId(std::string newId) { id = newId; }
	void setPassword(std::string newPassword) { password = sha1(newPassword); }
	bool checkPassword(const std::string &possible_password) const { return password == sha1(possible_password); }

	bool load(std::ifstream &file);
	void save(std::ofstream &file)const;
};
#endif
