#include "GraphInter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Mail.h"
#include "Date.h"
#include <ctime>

void Mail::save(std::ofstream &file) const
{
	file << id << std::endl << user_count << std::endl 
		<< date << std::endl << from << std::endl;

	recipients->save(file);

	file << subject << std::endl 
		<< body << "#" << std::endl;
}

bool Mail::load(std::ifstream &file)
{
	file >> id;

	if (id != "XXX" && !file.fail())
	{
		file >> user_count;

		if (!file.fail())
		{
			file >> date;

			if (!file.fail())
			{
				file >> from;

				if (!file.fail())
				{
					if (recipients->load(file))
					{
						if (!file.fail())
						{
							file.ignore();
							std::getline(file, subject);

							if (!file.fail())
							{
								std::getline(file, body, '#');

								if (!file.fail()) return true;
								else return false;
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

Mail* Mail::errorMail()
{
	std::ostringstream ID;
	Mail* error = new Mail;
	std::string* recipient = new std::string;

	error->from = "Tecnical Service";
	error->date = time(0);
	error->user_count = 2;

	ID << "fdimail" << "_" << error->date;
	error->id = ID.str();

	*recipient = "fdimail";
	error->recipients->insert(recipient);

	error->subject = "Mail error";

	error->body = "It seems that this mail does not exist";

	return error;
}

Mail* Mail::newMail(const std::string &sender, ContactList* contactList)
{
	std::ostringstream ID;
	Mail* newMail = new Mail;

	newMail->from = sender;
	newMail->date = time(0);
	newMail->user_count = 2;

	ID << sender << "_" << newMail->date;
	newMail->id = ID.str();

	GraphInter::get()->display("From: " + sender);

	send_to_multiple(newMail, contactList);

	if (newMail->recipients->empty()) return nullptr;
	
	else
	{
		GraphInter::get()->display("Subject: ");
		GraphInter::get()->enter(newMail->subject);

		if (newMail->subject == "")
		{
			int choose;

			do
			{
				GraphInter::get()->clearConsole();

				choose = GraphInter::get()->SureToEmpty(newMail);

				switch (choose)
				{
				case 0:

					GraphInter::get()->display("From: " + newMail->from);

					for (int k = 0; k < newMail->recipients->length(); k++)
					{
						if (k == 0)
						{
							GraphInter::get()->display("To: " + *newMail->recipients->operator[](k));
						}
						else
						{
							GraphInter::get()->display("CC: " + *newMail->recipients->operator[](k));
						}
					}
					GraphInter::get()->display("Subject: ");
					GraphInter::get()->enter(newMail->subject);
					break;

				case 1:

					subject = "No subject";
					GraphInter::get()->display("From: " + newMail->from);

					for (int k = 0; k < newMail->recipients->length(); k++)
					{
						if (k == 0)
						{
							GraphInter::get()->display("To: " + *newMail->recipients->operator[](k));
						}
						else
						{
							GraphInter::get()->display("CC: " + *newMail->recipients->operator[](k));
						}
					}
					GraphInter::get()->display("Subject: " + newMail->subject);
					break;
				}
			} while (choose == 0 && newMail->subject == "");
		}

		GraphInter::get()->display("Body (enter twice (ENTER) to end the body): ");

		std::string line;
		newMail->body = "";
		do
		{
			GraphInter::get()->enter(line);
			newMail->body += line + "\n";
		} while (line != "");

		if (newMail->body == "" || newMail->body == "\n") return nullptr;

		else return newMail;
	}
}

Mail* Mail::answerMail(Mail* &originalMail, const std::string &sender)
{
	std::ostringstream ID, BODY, SUBJECT;
	std::string* recipient = new std::string;
	Mail* newMail = new Mail;
	std::string WhatToSay;

	SUBJECT << "Re: " << originalMail->subject;

	newMail->from = sender;
	newMail->date = time(0);

	*recipient = originalMail->from;
	newMail->recipients->insert(recipient);

	newMail->subject = SUBJECT.str();

	ID << sender << "_" << newMail->date;
	newMail->id = ID.str();
	GraphInter::get()->display(GraphInter::get()->center_word("Answered mail", HORIZONTAL, " "));

	GraphInter::get()->display("");

	GraphInter::get()->display("From: " + sender);

	GraphInter::get()->display("To: " + originalMail->from);

	GraphInter::get()->display("Subject: " + newMail->subject);

	GraphInter::get()->display("Body (enter twice (ENTER) to end the body): ");

	std::string line;
	WhatToSay = "";
	do
	{
		GraphInter::get()->enter(line);
		WhatToSay += line + "\n";
	} while (line != "");

	if (WhatToSay == "" || WhatToSay == "\n")
	{
		WhatToSay = "No body";
	}

	BODY << WhatToSay << std::endl << GraphInter::get()->linea()
		<< std::endl << originalMail->to_string();

	newMail->body = BODY.str();

	return newMail;
}

Mail* Mail::forward(Mail* &originalMail, const std::string &sender, ContactList* contactList)
{
	std::ostringstream ID, BODY, SUBJECT;
	std::string WhatToSay;
	Mail* newMail = new Mail;

	SUBJECT << "Re: " << originalMail->subject;

	newMail->from = sender;
	newMail->date = time(0);
	newMail->subject = SUBJECT.str();

	ID << sender << "_" << newMail->date;
	newMail->id = ID.str();

	GraphInter::get()->display(GraphInter::get()->center_word("Forwarded mail", HORIZONTAL, " "));

	GraphInter::get()->display("");

	GraphInter::get()->display("From: " + sender);

	send_to_multiple(newMail, contactList);

	if (newMail->recipients->empty()) return nullptr;

	else
	{
		GraphInter::get()->display("Subject: " + newMail->subject);

		GraphInter::get()->display("Body (enter twice (ENTER) to end the body): ");

		std::string line;
		WhatToSay = "";
		do
		{
			GraphInter::get()->enter(line);
			WhatToSay += line + "\n";
		} while (line != "");

		if (WhatToSay == "" || WhatToSay == "\n")
		{
			WhatToSay = "No body";
		}

		BODY << WhatToSay << std::endl << GraphInter::get()->linea()
			<< std::endl << originalMail->to_string();

		newMail->body = BODY.str();

		return newMail;
	}
}

void Mail::send_to_multiple(Mail* mail, ContactList* contactList)
{
	int i;

	for (i = 0; i < MAX_RECIPIENTS && *mail->recipients->operator[](i) != "@fdimail.com"; i++)
	{
		GraphInter::get()->clearConsole();

		GraphInter::get()->display("From: " + mail->from);

		if (i > 0)
		{
			for (int j = 0; j < i; j++)
			{
				if (j == 0)
				{
					GraphInter::get()->display("To: " + *mail->recipients->operator[](j));
				}
				else
				{
					GraphInter::get()->display("CC: " + *mail->recipients->operator[](j));
				}
			}
			GraphInter::get()->display(GraphInter::get()->linea());
		}
		std::string* recipient = new std::string;

		if (i == 0)
		{
			GraphInter::get()->display("To (enter (ENTER) to end the recipients):");
		}
		else
		{
			GraphInter::get()->display("CC (enter (ENTER) to end the recipients):");
		}
		GraphInter::get()->enter(*recipient);

		*recipient = contactList->SearchFastName(*recipient);
		mail->recipients->insert(recipient);

		if (*mail->recipients->operator[](i) == "@fdimail.com")
		{
			mail->recipients->destroy(*mail->recipients->operator[](i));
			i--;
		}
		else
		{
			bool repeat = false;

			for (int j = 0; j < i && !repeat; j++)
			{
				if (i != j && *mail->recipients->operator[](j) == *mail->recipients->operator[](i))
				{
					GraphInter::get()->display("You have already choose this destinatary, you cannot choose it again");
					GraphInter::get()->pause();

					mail->recipients->destroy(*mail->recipients->operator[](i));
					i--;
					repeat = true;
				}
			}
		}
		GraphInter::get()->clearConsole();
	}

	mail->user_count = mail->recipients->length() + 1;

	GraphInter::get()->display("From: " + mail->from);

	if (!mail->recipients->empty())
	{
		for (int k = 0; k < mail->recipients->length(); k++)
		{
			if (k == 0)
			{
				GraphInter::get()->display("To: " + *mail->recipients->operator[](k));
			}
			else
			{
				GraphInter::get()->display("CC: " + *mail->recipients->operator[](k));
			}
		}
	}
}

const std::string Mail::to_string() const
{
	std::ostringstream flow;

	flow << "From: " << from << std::setw(55) << showDate(date) << std::endl;

	for (int i = 0; i < recipients->length(); i++)
	{
		flow << "To: " << recipients->operator[](i) << std::endl;
	}
	flow << std::endl << "Subject: " << subject << std::endl
		<< std::endl << body;

	return flow.str();
}

const std::string Mail::header()const
{
	std::ostringstream lowFlow;

	lowFlow << std::setw(30) << std::left << from << std::setw(34) << std::left << subject << showDay(date);

	return lowFlow.str();
}