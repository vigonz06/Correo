#include "GraphInter.h"
#include "utilsWin.h"
#include "Manager.h"
#include <sstream>

Manager* Manager::manager = nullptr;

Manager::Manager(const std::string &new_domain) :
domain(new_domain)
{
	if (manager == nullptr)
	{
		manager = this;
		bootUp();
	}
	else message("Manager cannot be instantiated twice!");
}

Manager::~Manager()
{
	shutDown();
}

void Manager::bootUp()
{
	std::ostringstream mail_file, user_file;
	mail_file << domain << "_mails.txt";
	user_file << domain << "_users.txt";

	if (!userList.load(mail_file.str())) message("Could not load userList");
	if (!mailList.load(user_file.str())) message("Could not load mailList");
}

void Manager::shutDown()
{
	std::ostringstream mail_file, user_file;
	mail_file << domain << "_mails.txt";
	user_file << domain << "_users.txt";

	userList.save(user_file.str());
	mailList.save(mail_file.str());
}

User* Manager::registerUser()
{
	std::string idUser;
	std::string last_password;

	GraphInter::get()->logMenu(idUser, last_password);

	if (idUser != "@fdimail.com" && last_password != "")
	{
		User* user = (userList.get(idUser));

		if (user != nullptr)
		{
			if (user->checkPassword(last_password)) return user;

			else
			{
				message("Wrong password");
				return nullptr;
			}
		}
		else
		{
			message("User does not exist");
			return nullptr;
		}
	}
	else return nullptr;
}

User* Manager::createAccount()
{
	std::string idUser;
	std::string last_password;

	GraphInter::get()->logMenu(idUser, last_password);
	
	if (idUser != "@fdimail.com" && last_password != "")
	{
		if (userList.get(idUser) == nullptr)
		{
			GraphInter::get()->checkPassword(last_password);
			User* user = new User(idUser, last_password);
			userList.insert(user);
			return user;
		}
		else
		{
			message("This username already exists");
			return nullptr;
		}
	}
	else return nullptr;
}

void Manager::deleteAccount(User* user)
{
	while (!user->getRecycling()->empty())
	{
		deleteMail(user->getRecycling(), user->getRecycling()->operator[](0));
	}
	while (!user->getOutbox()->empty())
	{
		deleteMail(user->getOutbox(), user->getOutbox()->operator[](0));
	}
	while (!user->getInbox()->empty())
	{
		deleteMail(user->getInbox(), user->getInbox()->operator[](0));
	}
	userList.destroy(user->getId());
}

void Manager::sendMail(User* user, Mail* mail)
{
	mailList.insert(mail);

	tElemTray* elem = new tElemTray(mail, Outbox, true);

	user->getOutbox()->insert(elem);

	for (auto j: mail->getRecipients())
	{
		if (userList.get(j) != nullptr) userList.get(j)->getInbox()->insert(new tElemTray(mail, Inbox, false));

		else
		{
			GraphInter::get()->display("Destinatary" + j + " not found");
			GraphInter::get()->display("He was not send the mail");
			GraphInter::get()->pause();

			mail->lowerCounter();
		}
	}
}

bool Manager::answer(User* user, Mail* mail)
{
	if (userList.get(mail->getRecipients()[0]) != nullptr)
	{
		mailList.insert(mail);

		user->getOutbox()->insert(new tElemTray(mail, Outbox, true));

		userList.get(mail->getRecipients()[0])->getInbox()->insert(new tElemTray(mail, Inbox, false));

		return true;
	}
	else return false;
}

void Manager::deleteMail(TrayList* box, const tElemTray* elem)
{
	mailList.delete_mail(elem->mail->getId());

	box->destroy(elem->getId());
}