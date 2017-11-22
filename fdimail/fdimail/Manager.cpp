#include "GraphInter.h"
#include "utilsWin.h"
#include "Manager.h"

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

void Manager::deleteAlias(User* user)
{
	for (User i: userList; i++)
	{
		for (Contact j: i->getContactlist(); j++)
		{
			if (i->j->getAddress() == user->getId())
			{
				i->getContactlist()->destroy(i->j->getId());
			}
		}
	}
}

void Manager::shutDown()
{
	std::ostringstream mail_file, user_file;
	mail_file << domain << "_mails.txt";
	user_file << domain << "_users.txt";

	userList.save(user_file.str());
	mailList.save(mail_file.str());
}

void Manager::bootUp()
{
	std::ostringstream mail_file, user_file;
	mail_file << domain << "_mails.txt";
	user_file << domain << "_users.txt";

	if (!mailList.load(mail_file.str())) message("Could not load Maillist");
	if (!userList.load(user_file.str())) message("Could not load Userlist");
}

void Manager::deleteAccount(User* user)
{
	while (!user->getRecycling()->empty())
	{
		deleteMail(user->getRecycling(), user->getRecycling()[0]);
	}
	while (!user->getOutbox()->empty())
	{
		deleteMail(user->getOutbox(), user->getOutbox()[0]);
	}
	while (!user->getInbox()->empty())
	{
		deleteMail(user->getInbox(), user->getInbox()[0]);
	}
	deleteAlias(user);
	userList.destroy(user->getId());
}

void Manager::sendMail(User* user, Mail* mail)
{
	bool fail = false;

	mailList.insert(mail);

	ElemTray* elem = new ElemTray(mail, false, true);

	user->getAll()->insert(elem);
	user->getOutbox()->insert(elem);

	for (std::string j: mail->getRecipients())
	{
		if (userList.get(j) != nullptr)
		{
			userList.get(j)->getAll()->insert(new ElemTray(mail, true, false));
			userList.get(j)->getInbox()->insert(new ElemTray(mail, true, false));
		}
		else
		{
			mail->lowerCounter();
			fail = true;
		}
	}
	if (fail) message("Some recipients could not be sent this mail");
}

bool Manager::answer(User* user, Mail* mail)
{
	if (userList.get(mail->getRecipients()[0]) != nullptr)
	{
		mailList.insert(mail);

		user->getOutbox()->insert(new ElemTray(mail, false, true));

		userList.get(mail->getRecipients()[0])->getInbox()->insert(new ElemTray(mail, true, false));

		return true;
	}
	else return false;
}

void Manager::deleteMail(TrayList* box, const ElemTray* elem)
{
	mailList.delete_mail(elem->mail->getId());

	box->destroy(elem->getId());
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
