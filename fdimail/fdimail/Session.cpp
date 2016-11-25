//#include "GraphInter.h"
#include "utilsWin.h"
#include "Session.h"

Session::Session(Manager* manager) :
manager(manager)
{
	int option;

	GraphInter::load();

	do
	{
		GraphInter::get()->clearConsole();
		option = GraphInter::get()->mainMenu();
		GraphInter::get()->clearConsole();

		switch (option)
		{
		case 0:

			user = manager->createAccount();
			if (user != nullptr) launch();
			break;

		case 1:

			user = manager->registerUser();
			if (user != nullptr) launch();
			break;
		}
	} while (option != 2);
}

Session::~Session()
{
	GraphInter::close();
}

void Session::launch()
{
	int opt;

	active_list = false;
	Filter filter = none;
	visible.init(active_tray());

	do
	{
		visible.refresh();
		GraphInter::get()->clearConsole();
		opt = GraphInter::get()->sessionMenu(this);
		GraphInter::get()->clearConsole();

		switch (opt)
		{
		case 0:

			readMail();
			break;

		case 1:

			sendMail();
			break;

		case 2:

			mailOptions();
			break;

		case 3:

			changeTray();
			break;

		case 4:

			AliasOptions();
			break;

		case 5:

			filterOptions(filter);
			break;

		case 6:

			AccountOptions(opt);
			break;

		case 7:

			fastRead();
			break;
		}
	} while (opt != 8);

	visible.close();
	user = nullptr;
}

void Session::forwardMail(Mail* &originalMail)
{
	Mail* forward = GraphInter::get()->forward(originalMail, user->getId(), user->getContactlist());

	if (forward == nullptr)
	{
		message("Mail not sent");
		delete forward;
	}
	else manager->sendMail(user, forward);
}

void Session::answerMail(Mail* &originalMail)
{
	Mail* answer = GraphInter::get()->answerMail(originalMail, user->getId());

	if (answer == nullptr)
	{
		message("Mail not sent");
		delete answer;
	}
	else manager->answer(user, answer);
}

void Session::restoreMail()
{
	int option;

	if (active_tray()->empty()) message("You have no mails to restore");

	else
	{
		do
		{
			visible.refresh();
			GraphInter::get()->clearConsole();

			option = GraphInter::get()->WhatToDelete(this);

			switch (option)
			{
			case 0:
			{
				ElemTray* elem = GraphInter::get()->selectMail(this);

				if (elem != nullptr)
				{
					if (elem->box) user->getInbox()->insert(elem);

					else user->getOutbox()->insert(elem);

					active_tray()->pop(elem->getId());
				}
				break;
			}
			case 1:

				do
				{
					if (active_tray()->operator[](0)->box) user->getInbox()->insert(active_tray()->operator[](0));

					else user->getOutbox()->insert(active_tray()->operator[](0));

					active_tray()->pop(active_tray()->operator[](0)->getId());

				} while (!active_tray()->empty());
				break;
			}
		} while (!active_tray()->empty() && option != 2);
	}
}

void Session::mailOptions()
{
	if (active_tray()->empty()) message("You have no mails to manipulate");

	else
	{
		if (active_tray() == user->getRecycling())
		{
			int option;

			option = GraphInter::get()->MailOptions();

			switch (option)
			{
			case 0:

				deleteMail();
				break;

			case 1:

				restoreMail();
				break;
			}
		}
		else deleteMail();
	}
}

void Session::changeTray()
{
	active_list = GraphInter::get()->chooseTray();

	visible.link(active_tray());
}

void Session::deleteMail()
{
	int option;

	if (visible.empty()) message("You have no mails to delete");

	else
	{
		do
		{
			visible.refresh();
			GraphInter::get()->clearConsole();

			option = GraphInter::get()->WhatToDelete(this);

			switch (option)
			{
			case 0:
			{
				do
				{
					if (active_tray() == user->getRecycling())
					{
						manager->deleteMail(active_tray(), active_tray()->operator[](0));
					}
					else
					{
						user->getRecycling()->insert(active_tray()->operator[](0));
						active_tray()->pop(active_tray()->operator[](0)->getId());
					}
				} while (!active_tray()->empty());
				break;
			}
			case 1:
			{
				ElemTray* elem = GraphInter::get()->selectMail(this);

				if (elem != nullptr)
				{
					if (active_tray() == user->getRecycling())
					{
						manager->deleteMail(active_tray(), elem);
					}
					else
					{
						user->getRecycling()->insert(elem);
						active_tray()->pop(elem->getId());
					}
				}
				break;
			}
			}
		} while (!active_tray()->empty() && option != 2);
	}
}

void Session::readMail()
{
	if (visible.empty()) message("You have no mails to read");

	else
	{
		ElemTray* elem = GraphInter::get()->selectMail(this);

		if (elem != nullptr)
		{
			active_tray()->readMail(elem->getId());

			int option = GraphInter::get()->mailMenu(elem->mail);

			GraphInter::get()->clearConsole();

			switch (option)
			{
			case 0:

				answerMail(elem->mail);
				break;

			case 1:

				forwardMail(elem->mail);
				break;
			}
		}
	}
}

void Session::sendMail()
{
	Mail* mail = GraphInter::get()->newMail(user->getId(), user->getContactlist());

	if (mail == nullptr)
	{
		message("Mail not sent");
		delete mail;
	}
	else manager->sendMail(user, mail);
}

void Session::fastRead()
{
	if (visible.empty()) message("You do not have any mails on your tray");

	else
	{
		visible.filterByRead(false);
		visible.orderBySubject();
		visible.orderByDate();

		for (int i = 0; i < visible.size(); i++)
		{
			GraphInter::get()->drawMail(visible.operator[](i)->mail);
			GraphInter::get()->display(GraphInter::get()->linea());
			GraphInter::get()->pause();

			visible.operator[](i)->read = true;
		}
	}
}

void Session::AccountOptions(int &option)
{
	int menu;
	
	do
	{
		GraphInter::get()->clearConsole();
		menu = GraphInter::get()->AccountOptions();

		switch (menu)
		{
		case 0:

			manager->deleteAccount(user);
			option = 8;
			break;

		case 1:

			changeUsername();
			break;

		case 2:

			changePassword();
			break;
		}
	} while (menu != 3 && option != 8);
}

void Session::AliasOptions()
{
	int option;

	do
	{
		GraphInter::get()->clearConsole();

		option = GraphInter::get()->AliasMenu(this);

		switch (option)
		{
		case 0:

			AddFastName();
			break;


		case 1:
		{
			std::string name = GraphInter::get()->selectAlias(this);

			if (name != user->getId())
			{
				if (name != "")
				{
					std::string alias;

					GraphInter::get()->display("Enter the new alias:");
					GraphInter::get()->enter(alias);

					if (alias != "") user->getContactlist()->get(user->getContactlist()->getAdress(name)->getId())->setAlias(alias);
				}
			}
			else message("You cannot change your self alias");
			break;
		}
		case 2:

			if (user->getContactlist()->size() > 1)
			{
				std::string name = GraphInter::get()->selectAlias(this);

				if (name != "")
				{
					if (user->getContactlist()->get(name)->getId() == "Me") message("You cannot delete your self alias");

					else user->getContactlist()->destroy(name);
				}
			}
			else message("You cannot delete your self alias");
			break;

		case 3:

			if (user->getContactlist()->size() > 1)
			{
				int namelenth = user->getContactlist()->size();

				for (int i = namelenth - 1; i >= 0; i--)
				{
					if (user->getContactlist()->operator[](i)->getId() != "Me") user->getContactlist()->destroy(user->getContactlist()->operator[](i)->getId());
				}
			}
			else message("You cannot delete your self alias");
			break;
		}
	} while (option != 4);
}

void Session::AddFastName()
{
	if (user->getContactlist()->full()) message("You cannot asign more alias");

	else
	{
		bool name_right, alias_right;
		std::string idUser, newId;

		do
		{
			name_right = true;

			GraphInter::get()->clearConsole();
			GraphInter::get()->display("Enter the user id you want to asign:");
			idUser = GraphInter::get()->valid_user();

			if (idUser != "")
			{
				if (manager->getUserList()->get(idUser) == nullptr)
				{
					message("This user does not exist");

					name_right = false;
				}
				if (idUser == user->getId())
				{
					message("There is already an asigned alias for your own username");

					name_right = false;
				}
				else
				{
					for (int i = 0; i < user->getContactlist()->size() && name_right; i++)
					{
						if (idUser == user->getContactlist()->operator[](i)->getAddress())
						{
							message("This username already has an alias asigned");

							name_right = false;
						}
					}
				}
			}
		} while (!name_right);

		if (idUser != "")
		{
			do
			{
				std::ostringstream character;
				alias_right = true;

				GraphInter::get()->clearConsole();
				GraphInter::get()->display("User: " + idUser);
				GraphInter::get()->display("Enter the alias you choose for this user:");
				GraphInter::get()->enter(newId);

				if (newId.size() == 0)
				{
					message("Error, the alias cannot be empty");

					alias_right = false;
				}
				if (newId == "Me")
				{
					message("Error, this is a default alias, you cannot asign it");

					alias_right = false;
				}
				else
				{
					for (char k: newId)
					{
						if (('A' > k || k > 'Z') && (k < 'a' || k > 'z'))
						{
							message("Error, the alias cannot contain symbols or numbers, only leters");

							alias_right = false;
						}
					}

					for (int j = 0; j < user->getContactlist()->size() && alias_right; j++)
					{
						if (newId == user->getContactlist()->operator[](j)->getId())
						{
							message("This alias is already asigned to an user");

							alias_right = false;
						}
					}
				}
			} while (!alias_right);

			Contact* newContact = new Contact(idUser, newId);

			user->getContactlist()->insert(newContact);
		}
	}
}

void Session::filterOptions(Filter filter)
{
	int option;

	option = GraphInter::get()->filter();

	switch (option)
	{
	case 0:

		visible.closeFilter();
		break;

	case 1:

		chooseOrder(filter);
		break;

	case 2:

		chooseFilter(filter);
		break;
	}
}

void Session::chooseFilter(Filter filter)
{
	if (visible.empty()) message("You have no mails to filter");
	
	else
	{
		visible.closeFilter();

		filter = Filter(GraphInter::get()->choosefilter());

		switch (filter)
		{
		case none:
			break;

		case date:
		{
			char* lowdate = new char[256];
			char* update = new char[256];

			do
			{
				GraphInter::get()->display("Enter the lower date");
				GraphInter::get()->enter(lowdate);
				GraphInter::get()->display("Enter the upper date");
				GraphInter::get()->enter(update);

				if (lowdate > update) message("The lower date cannot be higher than the upper one");
				
			} while (lowdate > update);

			visible.setFilterDate(lowdate, update);

			delete lowdate;
			delete update;
			break;
		}
		case read:

			visible.setFilterRead();
			break;

		case unread:

			visible.setFilterUnread();
			break;
		
		default:

			std::string reference;

			GraphInter::get()->display("Enter your reference word");
			GraphInter::get()->enter(reference);

			for (char i: reference)
			{
				i = tolower(i);
			}
			visible.setFilter(reference, filter);
			break;
		}
	}
}

void Session::chooseOrder(Filter filter)
{
	if (visible.empty()) message("You have no mails to filter");
	
	else
	{
		filter = Filter(GraphInter::get()->chooseorder());
		bool invert = GraphInter::get()->Invert();

		visible.setInvert(invert);
		visible.changeOrder(filter);
	}
}

void Session::changeUsername()
{
	bool name_ok;
	std::string data;

	do
	{
		name_ok = true;

		GraphInter::get()->clearConsole();

		data = GraphInter::get()->valid_user();

		if (manager->getUserList()->get(data) != nullptr)
		{
			message("This username already exists");

			name_ok = false;
		}
	} while (!name_ok);

	if (data != "@fdimail.com")
	{
		GraphInter::get()->checkUsername(data);

		if (data != "@fdimail.com")
		{
			std::string oldId = user->getId();

			manager->getUserList()->pop(user->getId());

			user->setId(data);
			manager->getUserList()->insert(user);

			for (int i = 0; i < manager->getUserList()->size(); i++)
			{
				for (int j = 0; j < manager->getUserList()->operator[](i)->getContactlist()->size(); j++)
				{
					if (manager->getUserList()->operator[](i)->getContactlist()->operator[](j)->getAddress() == oldId)
					{
						manager->getUserList()->operator[](i)->getContactlist()->operator[](i)->setAddress(user->getId());
					}
				}
			}
		}
	}
}

void Session::changePassword()
{
	std::string data;

	GraphInter::get()->display("Enter your new password");

	data = GraphInter::get()->HideLimitPassword();

	if (data.size() != 0)
	{
		GraphInter::get()->checkPassword(data);

		user->setPassword(data);
	}
}

TrayList* Session::active_tray()
{
	switch (active_list)
	{
	case 0:

		return user->getInbox();
		break;

	case 1:

		return user->getOutbox();
		break;

	case 2:

		return user->getRecycling();
		break;
	}
}
