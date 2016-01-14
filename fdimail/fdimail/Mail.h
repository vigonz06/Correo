#ifndef MAIL
#define MAIL

#include "GlobalConstants.h"
#include "RecipientList.h"
#include <fstream>
#include <string>
#include "Date.h"

/*----------------------------
This class contains the mail datos of a mail
and the neccesary functions to make it work
correctly
------------------------------*/

class ContactList;

class Mail
{
private:

	std::string id;
	int user_count;

	Date date;
	std::string from;
	std::string body;
	std::string subject;
	RecipientList* recipients;

	void send_to_multiple(Mail* mail, ContactList* contactList);

public:

	Mail() : user_count(0) {}

	Date getDate() { return date; }
	std::string getFrom() { return from; }
	std::string getBody() { return body; }
	std::string getSubject() { return subject; }
	RecipientList* getRecipients() { return recipients; }

	const std::string& getId() const { return id; }
	int getCounter() const { return user_count; }
	int lowerCounter(){ return --user_count; }

	void save(std::ofstream &file)const;
	bool load(std::ifstream &file);

	static Mail* errorMail();
	Mail* newMail(const std::string &sender, ContactList* contactList);
	Mail* answerMail(Mail* &originalMail, const std::string &sender);
	Mail* forward(Mail* &originalMail, const std::string &sender, ContactList* contactList);

	const std::string to_string()const;
	const std::string header()const;
};
#endif