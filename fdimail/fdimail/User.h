#ifndef USER
#define USER

#include "ContactList.h"
#include "TrayList.h"
#include "Encript.h"

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

	User() : contactList(){}
	User(const std::string idUser, const std::string password);

	std::string getId() { return id; }
	TrayList* getInbox() { return &inbox; }
	TrayList* getOutbox() { return &outbox; }
	TrayList* getRecycling() { return &recycling; }
	ContactList* getContactlist() { return &contactList; }

	void setId(std::string newId) { id = newId; }
	void setPassword(std::string newPassword) { password = sha1(newPassword); }
	bool checkPassword(const std::string &possiblePassword) const { return password == sha1(possiblePassword); }

	bool load(std::ifstream &file);
	void save(std::ofstream &file)const;
};
#endif
