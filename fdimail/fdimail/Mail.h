#ifndef MAIL
#define MAIL

#include "GlobalConstants.h"
#include <fstream>
#include <string>
#include <vector>
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
	std::vector<std::string> recipients;

public:

	Mail() : user_count(0) {}

	const std::string& getId() const { return id; }
	int getCounter() const { return user_count; }
	int lowerCounter(){ return --user_count; }

	Date getDate() { return date; }
	std::string getFrom() { return from; }
	std::string getBody() { return body; }
	std::string getSubject() { return subject; }
	std::vector<std::string> getRecipients() { return recipients; }

	std::string& setId(std::string newId) { return id = newId; }
	int& setCounter(int newCounter) { return user_count = newCounter; }

	Date& setDate(Date newDate) { return date = newDate; }
	std::string& setFrom(std::string newFrom) { return from = newFrom; }
	std::string& setBody(std::string newBody) { return body = newBody; }
	std::string& setSubject(std::string newSubject) { return subject = newSubject; }
	void setRecipient(std::string newRecipient) { recipients.push_back(newRecipient); }

	bool load(std::ifstream &file);
	void save(std::ofstream &file)const;

	const std::string to_string() const;
	const std::string header() const;
};
#endif