#include "GraphInter.h"
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
	else GraphInter::get()->display("Manager cannot be instantiated twice!");
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

	load(mail_file.str(), mailList.load);
	load(user_file.str(), userList.load);
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
				GraphInter::get()->display("Wrong password");
				GraphInter::get()->pause();
				return nullptr;
			}
		}
		else
		{
			GraphInter::get()->display("User does not exist");
			GraphInter::get()->pause();
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
			GraphInter::get()->display("This username already exists");
			GraphInter::get()->pause();
			return nullptr;
		}
	}
	else return nullptr;
}

void Manager::deleteAccount(User* user)
{
	while (!user->getRecycling()->empty())
	{
		deleteMail(user->getRecycling(), user->getRecycling()->operator[](0)->getId());
	}
	while (user->getOutbox()->empty())
	{
		deleteMail(user->getOutbox(), user->getOutbox()->operator[](0)->getId());
	}
	while (!user->getInbox()->empty())
	{
		deleteMail(user->getInbox(), user->getInbox()->operator[](0)->getId());
	}
	userList.destroy(user->getId());
}

void Manager::sendMail(User* user, Mail* mail)
{
	if (mailList.insert(mail))
	{
		tElemTray* elem = new tElemTray(mail);
		elem->read = true;

		user->getOutbox()->insert(elem);

		for (int j = 0; j < mail->getRecipients().length(); j++)
		{
			if (userList.get(*mail->getRecipients().operator[](j)) != nullptr)
			{
				userList.get(*mail->getRecipients().operator[](j))->getInbox()->insert(new tElemTray(mail));
			}
			else
			{
				mail->lowerCounter();
				GraphInter::get()->display("Destinatary " + *mail->getRecipients().operator[](j) + " not found");
				GraphInter::get()->display("He was not sent the mail");
				GraphInter::get()->pause();
			}
		}
	}
	else
	{
		GraphInter::get()->display("MailList full");
		GraphInter::get()->pause();
	}
}

bool Manager::answer(User* user, Mail* mail)
{
	if (userList.get(*mail->getRecipients().operator[](0)) != nullptr)
	{
		mailList.insert(mail);

		user->getOutbox()->insert(new tElemTray(mail));
		user->getOutbox()->get(mail->getId())->read = true;

		userList.get(*mail->getRecipients().operator[](0))->getInbox()->insert(new tElemTray(mail));

		return true;
	}
	else return false;
}

void Manager::deleteMail(TrayList* box, const std::string &idMail)
{
	box->destroy(idMail);

	mailList.delete_mail(idMail);
}

void Manager::popMail(TrayList* box, const std::string &idMail)
{
	box->pop(box->get(idMail));
}

void Manager::load(std::string name, bool(*funct)(std::string))
{
	while (name != "" && funct(name))
	{
		GraphInter::get()->display("Could not load mailList");
		GraphInter::get()->display("Enter the file url ((ENTER) for continue)");
		GraphInter::get()->enter(name);
	}
}