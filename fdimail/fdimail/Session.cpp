#include "GraphInter.h"
#include "Session.h"
#include "Mail.h"

Session::Session(Manager* manager) :
manager(manager)
{
	GraphInter::load();

	int option;
	do
	{
		GraphInter::get()->clearConsole();
		option = GraphInter::get()->mainMenu();
		GraphInter::get()->clearConsole();

		if (option == 0)
		{
			
			user = manager->createAccount();
		}
		if (option == 1)
		{
			user = manager->registerUser();
		}
		if (user != nullptr && option != 2)
		{
			launch();
		}
	} while (option != 2);
}

Session::~Session()
{
	GraphInter::close();
}

/****************************************************************/
/*                            MENUS                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void Session::launch()
{
	active_list = false;
	visible.init(active_tray());
	Filter filter = none;
	int opt;

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
			MailOptions();
			break;
		case 3:
			changeTray();
			break;
		case 4:
			fastRead();
			break;
		case 5:
			AccountOptions(opt);
			break;
		case 6:
			AliasOptions();
			break;
		case 7:
			filterOptions(filter);
			break;
		}
	} while (opt != 8);
	visible.closeFilter();
	visible.refresh();
	visible.erase();
	visible.link(nullptr);
	user = nullptr;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                 MAIN MENU OPTIONS                                 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void Session::readMail()
{
	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to read");
		GraphInter::get()->pause();
	}
	else
	{
		Mail* mail = GraphInter::get()->selectMail(this);

		if (mail != nullptr)
		{
			active_tray()->readMail(mail->getId());

			int option = GraphInter::get()->mailMenu(mail);

			GraphInter::get()->clearConsole();

			if (option == 0)
			{
				answerMail(mail);
			}
			else if (option == 1)
			{
				forwardMail(mail);
			}
		}
	}
}

void Session::sendMail()
{
	Mail* mail = GraphInter::get()->newMail(user->getId(), user->getContactlist());

	if (mail == nullptr)
	{
		GraphInter::get()->display("Mail not sent");
		GraphInter::get()->pause();
		delete mail;
	}
	else
	{
		manager->sendMail(user, mail);
	}
}

void Session::answerMail(Mail* &originalMail)
{
	Mail* answer = GraphInter::get()->answerMail(originalMail, user->getId());

	if (answer == nullptr)
	{
		GraphInter::get()->display("Mail not sent");
		GraphInter::get()->pause();
		delete answer;
	}
	else
	{
		manager->answer(user, answer);
	}
	GraphInter::get()->pause();
}

void Session::forwardMail(Mail* &originalMail)
{
	Mail* forward = GraphInter::get()->forward(originalMail, user->getId(), user->getContactlist());

	if (forward == nullptr)
	{
		GraphInter::get()->display("Mail not sent");
		GraphInter::get()->pause();
		delete forward;
	}
	else
	{
		manager->sendMail(user, forward);
	}
	GraphInter::get()->pause();
}

void Session::deleteMail()
{
	int option;

	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to delete");
		GraphInter::get()->pause();
	}
	else
	{
		do
		{
			visible.refresh();
			GraphInter::get()->clearConsole();

			option = GraphInter::get()->WhatToDelete(this);

			if (option == 0)
			{
				Mail* mail = GraphInter::get()->selectMail(this);

				if (mail != nullptr)
				{
					if (active_tray() == user->getRecycling())
					{
						manager->deleteMail(active_tray(), mail->getId());
					}
					else
					{
						user->getRecycling()->insert(active_tray()->get(mail->getId()));
						manager->popMail(active_tray(), mail->getId());
					}
				}
			}
			else if (option == 1)
			{
				do
				{
					std::string newId = active_tray()->operator[](0)->getId();

					if (active_tray() == user->getRecycling())
					{
						manager->deleteMail(active_tray(), newId);
					}
					else
					{
						user->getRecycling()->insert(active_tray()->get(newId));
						manager->popMail(active_tray(), newId);
					}
				} while (!active_tray()->empty());
			}
		} while (!visible.empty() && option != 2);
	}
}

void Session::restoreMail()
{
	int option;

	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to delete");
		GraphInter::get()->pause();
	}
	else
	{
		do
		{
			visible.refresh();
			GraphInter::get()->clearConsole();

			option = GraphInter::get()->WhatToDelete(this);

			if (option == 0)
			{
				Mail* mail = GraphInter::get()->selectMail(this);

				if (mail != nullptr)
				{
					if (active_tray()->get(mail->getId())->box)
					{
						user->getInbox()->insert(active_tray()->get(mail->getId()));
						manager->popMail(active_tray(), mail->getId());
					}
					else
					{
						user->getOutbox()->insert(active_tray()->get(mail->getId()));
						manager->popMail(active_tray(), mail->getId());
					}
				}
			}
			if (option == 1)
			{
				for (int j = 0; j < active_tray()->length(); j++)
				{
					if (active_tray()->operator[](0)->box)
					{
						user->getInbox()->insert(active_tray()->operator[](0));
						manager->popMail(active_tray(), active_tray()->operator[](0)->getId());
					}
					else
					{
						user->getOutbox()->insert(active_tray()->operator[](0));
						manager->popMail(active_tray(), active_tray()->operator[](0)->getId());
					}
				}
			}
		} while (!visible.empty() && option != 2);
	}
}

void Session::changeTray()
{
	active_list = GraphInter::get()->ChooseTray();

	visible.link(active_tray());
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

void Session::fastRead()
{
	if (visible.empty())
	{
		GraphInter::get()->display("You do not have any mails on your tray");
		GraphInter::get()->pause();
	}
	else
	{
		visible.filterByRead(false);
		visible.orderByDate();
		visible.orderBySubject();

		for (int i = 0; i < visible.length(); i++)
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

		if (menu == 0)
		{
			changeUsername();
		}
		else if (menu == 1)
		{
			changePassword();
		}
		else if (menu == 2)
		{
			manager->deleteAccount(user);
			option = 8;
		}
	} while (menu != 3 && option != 8);
}

void Session::AddFastName()
{
	if (user->getContactlist()->full())
	{
		GraphInter::get()->display("You cannot asign more alias");
		GraphInter::get()->pause();
	}
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
					GraphInter::get()->display("This user does not exist");
					GraphInter::get()->pause();

					name_right = false;
				}
				else if (idUser == user->getId())
				{
					GraphInter::get()->display("There is already an asigned alias for your own username");
					GraphInter::get()->pause();

					name_right = false;
				}
				else
				{
					for (int i = 0; i < user->getContactlist()->length() && name_right; i++)
					{
						if (idUser == user->getContactlist()->operator[](i)->user)
						{
							GraphInter::get()->display("This username already has an alias asigned");
							GraphInter::get()->pause();

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
					GraphInter::get()->display("Error, the alias cannot be empty");
					GraphInter::get()->pause();

					alias_right = false;
				}
				else if (newId == "Me")
				{
					GraphInter::get()->display("Error, this is a default alias, you cannot asign it");
					GraphInter::get()->pause();

					alias_right = false;
				}
				else
				{
					for (int k = 0; k < int(newId.size()) && alias_right; k++)
					{
						if (('A' > newId[k] || newId[k] > 'Z') && (newId[k] < 'a' || newId[k] > 'z'))
						{
							character << "(" << char(newId[k]) << ")";

							GraphInter::get()->display("Error, your id cannot contain the character " + character.str());
							GraphInter::get()->pause();

							alias_right = false;
						}
					}

					for (int j = 0; j < user->getContactlist()->length() && alias_right; j++)
					{
						if (newId == user->getContactlist()->operator[](j)->getId())
						{
							GraphInter::get()->display("This alias is already asigned to an user");
							GraphInter::get()->pause();

							alias_right = false;
						}
					}
				}
			} while (!alias_right);

			tContact* newContact = new tContact(idUser, newId);

			user->getContactlist()->insert(newContact);
		}
	}
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

			if (user->getContactlist()->length() > 1)
			{
				std::string name = GraphInter::get()->selectAlias(this);

				if (name != "")
				{
					if (user->getContactlist()->get(name)->alias == "Me")
					{
						GraphInter::get()->display("You cannot delete your self alias");
						GraphInter::get()->pause();
					}
					else
					{
						user->getContactlist()->destroy(name);
					}
				}
			}
			else
			{
				GraphInter::get()->display("You cannot delete your self alias");
				GraphInter::get()->pause();
			}
			break;

		case 2:

			if (user->getContactlist()->length() > 1)
			{
				int namelenth = user->getContactlist()->length();

				for (int i = namelenth - 1; i >= 0; i--)
				{
					if (user->getContactlist()->operator[](i)->alias != "Me")
					{
						user->getContactlist()->destroy(user->getContactlist()->operator[](i)->getId());
					}
				}
			}
			else
			{
				GraphInter::get()->display("You cannot delete your self alias");
				GraphInter::get()->pause();
			}
			break;
		}
	} while (option != 3);
}

void Session::MailOptions()
{
	int opt;

	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to manipulate");
		GraphInter::get()->pause();
	}
	else
	{
		if (active_tray() != user->getRecycling())
		{
			deleteMail();
		}
		else
		{
			opt = GraphInter::get()->MailOptions();

			switch (opt)
			{
			case 0:

				deleteMail();
				break;

			case 1:

				restoreMail();
				break;
			}
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

		chooseOrder(filter);
		break;

	case 1:

		chooseFilter(filter);
		break;

	case 2:

		visible.closeFilter();
		break;
	}
}

void Session::chooseFilter(Filter filter)
{
	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to filter");
		GraphInter::get()->pause();
	}
	else
	{
		visible.closeFilter();

		filter = Filter(GraphInter::get()->choosefilter());

		if (filter != none)
		{
			if (filter == date)
			{
				char* lowdate = new char[256];
				char* update = new char[256];

				do
				{
					GraphInter::get()->display("Enter the lower date");
					GraphInter::get()->enter(lowdate);
					GraphInter::get()->display("Enter the upper date");
					GraphInter::get()->enter(update);

					if (lowdate > update)
					{
						GraphInter::get()->display("The lower date cannot be higher than the upper one");
					}
				} while (lowdate > update);

				visible.setFilterDate(lowdate, update);

				delete lowdate;
				delete update;
			}
			else if (filter == read)
			{
				visible.setFilterRead();
			}
			else if (filter == unread)
			{
				visible.setFilterUnread();
			}
			else
			{
				std::string reference;

				GraphInter::get()->display("Enter your reference word");
				GraphInter::get()->enter(reference);

				for (int i = 0; i < int(reference.size()); i++)
				{
					reference[i] = tolower(reference[i]);
				}
				visible.setFilter(reference, filter);
			}
		}
	}
}

void Session::chooseOrder(Filter filter)
{
	if (visible.empty())
	{
		GraphInter::get()->display("You have no mails to filter");
		GraphInter::get()->pause();
	}
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
			GraphInter::get()->display("This username already exists");
			GraphInter::get()->pause();

			name_ok = false;
		}
	} while (!name_ok);

	if (data != "@fdimail.com")
	{
		GraphInter::get()->checkUsername(data);

		manager->getUserList()->pop(user);

		user->setId(data);
		user->getContactlist()->changeMe(data);
		manager->getUserList()->insert(user);
	}
}

void Session::changePassword()
{
	std::string data;

	GraphInter::get()->display("Enter your new password");

	data = GraphInter::get()->HideLimitPassword();

	if (data.size() != 0)
	{
		GraphInter::get()->display("");
		GraphInter::get()->checkPassword(data);

		user->setPassword(data);
	}
}