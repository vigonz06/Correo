#ifndef MAIL
#define MAIL

#include <fstream>
#include <string>
#include <vector>
#include "Date.h"

class ContactList;

class Mail
{
private:

	int counter;
	std::string id;

	Date date;
	std::string from;
	std::string body;
	std::string subject;
	std::vector<std::string> recipients;

public:

	Mail() : counter(0) {}

	void lowerCounter() { counter--; }
	int getCounter() const { return counter; }
	const std::string getId() const { return id; }

	Date getDate() { return date; }
	std::string getFrom() { return from; }
	std::string getBody() { return body; }
	std::string getSubject() { return subject; }
	std::vector<std::string> getRecipients() { return recipients; }

	void setId(std::string newId) { id = newId; }
	void setCounter(int newCounter) { counter = newCounter; }

	void setDate(Date newDate) { date = newDate; }
	void setFrom(std::string newFrom) { from = newFrom; }
	void setBody(std::string newBody) { body = newBody; }
	void setSubject(std::string newSubject) { subject = newSubject; }
	void setRecipient(std::string newRecipient) { recipients.push_back(newRecipient); }

	bool load(std::ifstream &file);
	void save(std::ofstream &file)const;

	const std::string to_string() const;
	const std::string header() const;
	const std::string subSubject();
};
#endif
