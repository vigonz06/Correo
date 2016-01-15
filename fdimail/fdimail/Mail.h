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
	RecipientList recipients;

public:

	Mail() : user_count(0) {}

	static Mail* errorMail();

	const std::string& getId() const { return id; }
	int getCounter() const { return user_count; }
	int lowerCounter(){ return --user_count; }

	Date getDate() { return date; }
	std::string getFrom() { return from; }
	std::string getBody() { return body; }
	std::string getSubject() { return subject; }
	RecipientList getRecipients() { return recipients; }

	std::string& setId(std::string newId) { return id = newId; }
	int& setCounter(int newCounter) { return user_count = newCounter; }

	Date& setDate(Date newDate) { return date = newDate; }
	std::string& setFrom(std::string newFrom) { return from = newFrom; }
	std::string& setBody(std::string newBody) { return body = newBody; }
	std::string& setSubject(std::string newSubject) { return subject = newSubject; }

	void save(std::ofstream &file)const;
	bool load(std::ifstream &file);

	const std::string to_string()const;
	const std::string header()const;
};
#endif