#include "GlobalConstants.h"
#include "GraphInter.h"
#include "utilsWin.h"
#include "Session.h"
#include <iostream>
#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <vector>

GraphInter* GraphInter::inter = nullptr;
Mail* GraphInter::error = nullptr;

Mail* GraphInter::errorMail()
{
	Mail* error = new Mail;
	std::ostringstream ID;
	std::string recipient;

	error->setFrom("Tecnical Service");
	error->setDate(time(0));
	error->setCounter(2);

	ID << "fdimail" << "_" << error->getDate();
	error->setId(ID.str());

	recipient = "fdimail";
	error->setRecipient(recipient);

	error->setSubject("Mail error");

	error->setBody("It seems that this mail does not exist");

	return error;
}

int GraphInter::update(int key, int elem, int max_elems)
{
	if (key == UP) elem--;
	if (key == DOWN) elem++;

	if (elem < 0) elem = max_elems - 1;
	if (elem >= max_elems) elem = 0;

	if (key == ESCAPE) elem = max_elems - 1;
	return elem;
}

void GraphInter::updateTray(int key, Session* session)
{
	if (key == RIGHT) session->getVisible()->increasePage();

	if (key == LEFT) session->getVisible()->decreasePage();
}

bool GraphInter::mayus(std::string word)
{
	for (char i : word)
	{
		if (i >= 65 && i <= 90) return true;
	}
	return false;
}

bool GraphInter::digit(std::string word)
{
	for (char i : word)
	{
		if (isdigit(i)) return true;
	}
	return false;
}

bool GraphInter::symbl(std::string word)
{
	for (char i : word)
	{
		if (i < 48 || (i > 57 && i < 65) || (i > 90 && i < 97) || i > 122) return true;
	}
	return false;
}

std::string GraphInter::HidePassword()
{
	int i = 0;
	char word[50];

	do
	{
		word[i] = unsigned char(_getch());
		std::cout.flush();

		if (word[i] != 13)
		{
			if (word[i] != 8)
			{
				display('*');
				i++;
			}
			else if (i > 0)
			{
				display(char(8));
				display(char(32));
				display(char(8));
				i--;
			}
		}
	} while (word[i] != 13);

	word[i] = NULL;
	display("");

	return word;
}

int GraphInter::menu(std::vector<std::string> elems)
{
	int key = UP, elem = 0;

	do
	{
		display("Which one do you choose?: ");

		for (int i = 0; i < elems.size(); i++)
		{
			tab_word(elems[i], i, elem);
		}

		key = getKey();
		elem = update(key, elem, elems.size());

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

void GraphInter::send_to_multiple(Mail* mail, ContactList* contactList)
{
	bool repeat;
	std::string recipient;

	do
	{
		clearConsole();
		repeat = false;

		showRecipients(mail);
		display(linea());

		if (mail->getRecipients().empty()) display("To (enter (ENTER) to end the recipients):");

		else display("CC (enter (ENTER) to end the recipients):");

		enter(recipient);
		recipient = contactList->SearchFastName(recipient);

		if (!recipient.empty())
		{
			for (std::string j : mail->getRecipients())
			{
				if (j == recipient)
				{
					display("You have already choose this destinatary, you cannot choose it again");
					pause();

					repeat = true;
				}
			}
			if (!repeat) mail->setRecipient(recipient);
		}
	} while (mail->getRecipients().size() <= MAX_ELEMS && !recipient.empty());

	mail->setCounter(mail->getRecipients().size() + 1);

	clearConsole();

	showRecipients(mail);
}

void GraphInter::tab_word(std::string word, int pos, int cont)
{
	if (pos == cont) word = "->" + word;

	else word = tab_word(word);

	display(word);
}

std::string GraphInter::tab_word(std::string word)
{
	std::ostringstream tab;

	tab << std::setw(word.size() + 2) << word;

	return tab.str();
}

std::string GraphInter::pags(Session* session)
{
	std::ostringstream pags;

	if (session->getVisible()->getLastPage() == 0) pags << "              ";

	else
	{
		if (session->getVisible()->getPage() > 0) pags << "<- (prev page)";

		else pags << "<- (last page)";
	}

	pags << center_word(std::to_string(session->getVisible()->getPage() + 1) + "/" + std::to_string(session->getVisible()->getLastPage() + 1), HORIZONTAL - 28, " ");

	if (session->getVisible()->getLastPage() == 0) pags << "              ";

	else
	{
		if (session->getVisible()->LastPage()) pags << "(first page)->";

		else pags << "(next page) ->";
	}
	return pags.str();
}

void GraphInter::showRecipients(Mail* mail)
{
	bool repeat = false;

	display("From: " + mail->getFrom());

	if (!mail->getRecipients().empty())
	{
		for (std::string i : mail->getRecipients())
		{
			if (!repeat)
			{
				display("To: " + i);
				repeat = true;
			}
			else display("CC: " + i);
		}
	}
}

GraphInter* GraphInter::get() {	return inter; }

void GraphInter::close()
{
	if (inter != nullptr)
	{
		delete inter;
		delete error;
	}
}

void GraphInter::load()
{
	if (inter == nullptr)
	{
		inter = new GraphInter;
		error = errorMail();
	}
}

int GraphInter::mainMenu()
{
	std::vector<std::string> elems;

	elems.push_back("Sign up");
	elems.push_back("Sign in");
	elems.push_back("Exit");

	return menu(elems);
}

int GraphInter::trayMenu(Session* session, std::vector<std::string> elems)
{
	int key = UP, elem = 0;

	do
	{
		display("Mail of " + session->getUser()->getId());

		session->getVisible()->refresh();
		showTray(session);

		display(linea());

		display("Choose your desired option: ");

		for (int i = 0; i < elems.size(); i++)
		{
			tab_word(elems[i], i, elem);
		}

		key = getKey();
		elem = update(key, elem, elems.size());
		updateTray(key, session);

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

void GraphInter::logMenu(std::string &username, std::string &password)
{
	username = valid_user();

	if (username != "@fdimail.com")
	{
		display("Enter your password");

		password = HideLimitPassword();
	}
	else password = "";
}

int GraphInter::menumail(Mail* mail, std::vector<std::string> elems)
{
	int key = UP, elem = 0;

	do
	{
		drawMail(mail);

		display("Which one do you choose?: ");

		for (int i = 0; i < elems.size(); i++)
		{
			tab_word(elems[i], i, elem);
		}

		key = getKey();
		elem = update(key, elem, elems.size());

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

int GraphInter::WhatToDelete(Session* session)
{
	std::vector<std::string> elems;

	elems.push_back("Choose mail");
	elems.push_back("All mails");
	elems.push_back("Exit to session menu");

	return trayMenu(session, elems);
}

int GraphInter::sessionMenu(Session* session)
{
	std::vector<std::string> elems;

	elems.push_back("Read mail");
	elems.push_back("Send mail");
	elems.push_back("Delete mail");
	elems.push_back("Change Tray");
	elems.push_back("Fast read of unread mails");
	elems.push_back("Account options");
	elems.push_back("Alias options");
	elems.push_back("Filter options");
	elems.push_back("Sign out");

	return trayMenu(session, elems);
}

int GraphInter::aliasMenu(Session* session)
{
	int key = UP, elem = 0;

	do
	{
		session->getVisible()->refresh();

		display("Choose your desired alias: ");

		for (int i = 0; i < session->getUser()->getContactlist()->length(); i++)
		{
			tab_word(session->getUser()->getContactlist()->operator[](i)->header(), i, elem);
		}
		tab_word("Back", session->getVisible()->length(), elem);

		display(linea());

		key = getKey();
		elem = update(key, elem, session->getUser()->getContactlist()->length() + 1);

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

int GraphInter::AliasMenu(Session* session)
{
	int key = UP, elem = 0;

	do
	{
		showFastNames(session->getUser()->getContactlist());

		tab_word("Add an alias", 0, elem);
		tab_word("Delete an alias", 1, elem);
		tab_word("Delete all alias", 2, elem);
		tab_word("Exit to session menu", 3, elem);

		key = getKey();
		elem = update(key, elem, 4);

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

int GraphInter::mailMenu(Session* session)
{
	int key = UP, elem = 0;

	do
	{
		session->getVisible()->refresh();

		display("Choose your desired mail: ");

		for (int i = 0; i < session->getVisible()->length(); i++)
		{
			std::ostringstream mail;

			if (!session->getVisible()->operator[](i)->read) mail << "* ";

			else mail << "  ";

			mail << session->getVisible()->operator[](i)->mail->header();

			tab_word(mail.str(), i, elem);
		}
		tab_word("  Back", session->getVisible()->length(), elem);

		display(linea());
		display(pags(session));
		display(linea());

		key = getKey();
		elem = update(key, elem, session->getVisible()->length() + 1);
		updateTray(key, session);

		clearConsole();

	} while (key != ENTER && key != ESCAPE);

	return elem;
}

int GraphInter::mailMenu(Mail* mail)
{
	std::vector<std::string> elems;

	elems.push_back("Answer");
	elems.push_back("Forward");
	elems.push_back("Exit to session menu");

	return menumail(mail, elems);
}

int GraphInter::AccountOptions()
{
	std::vector<std::string> elems;

	elems.push_back("Change username");
	elems.push_back("Change password");
	elems.push_back("Delete account");
	elems.push_back("Exit to session menu");

	return menu(elems);
}

int GraphInter::MailOptions()
{
	std::vector<std::string> elems;

	elems.push_back("Delete mail");
	elems.push_back("Restore mail");
	elems.push_back("Back");

	return menu(elems);
}

int GraphInter::chooseTray()
{
	std::vector<std::string> elems;

	elems.push_back("Inbox");
	elems.push_back("Outbox");
	elems.push_back("Paper Bin");

	return menu(elems);
}

int GraphInter::Invert()
{
	std::vector<std::string> elems;

	elems.push_back("Order list");
	elems.push_back("Invert list");

	return menu(elems);
}

Mail* GraphInter::forward(Mail* &originalMail, const std::string &sender, ContactList* contactList)
{
	std::ostringstream ID, BODY, SUBJECT;
	Mail* newMail = new Mail;
	std::string WhatToSay;

	SUBJECT << "Re: " << originalMail->getSubject();

	newMail->setFrom(sender);
	newMail->setDate(time(0));
	newMail->setSubject(SUBJECT.str());

	ID << sender << "_" << newMail->getDate();
	newMail->setId(ID.str());

	display(center_word("Forwarded mail", HORIZONTAL, " "));

	display("");

	display("From: " + sender);

	send_to_multiple(newMail, contactList);

	if (newMail->getRecipients().empty()) return nullptr;

	else
	{
		display("Subject: " + newMail->getSubject());

		display("Body (enter twice (ENTER) to end the body): ");

		std::string line;
		WhatToSay = "";
		do
		{
			enter(line);
			WhatToSay += line + "\n";
		} while (!line.empty());

		if (WhatToSay.empty() || WhatToSay == "\n") WhatToSay = "No body";

		BODY << WhatToSay << std::endl << linea()
			<< std::endl << originalMail->to_string();

		newMail->setBody(BODY.str());

		return newMail;
	}
}

Mail* GraphInter::newMail(const std::string &sender, ContactList* contactList)
{
	Mail* newMail = new Mail;
	std::ostringstream ID;
	std::string subject;

	newMail->setFrom(sender);
	newMail->setDate(time(0));
	newMail->setCounter(2);

	ID << sender << "_" << newMail->getDate();
	newMail->setId(ID.str());

	display("From: " + sender);

	send_to_multiple(newMail, contactList);

	if (newMail->getRecipients().empty()) return nullptr;

	else
	{
		display("Subject: ");
		enter(subject);
		newMail->setSubject(subject);

		if (newMail->getSubject() == "")
		{
			int choose;

			do
			{
				clearConsole();

				choose = SureToEmpty(newMail);

				switch (choose)
				{
				case 0:

					showRecipients(newMail);
					display("Subject: ");
					enter(subject);
					newMail->setSubject(subject);
					break;

				case 1:

					subject = "No subject";
					showRecipients(newMail);
					display("Subject: " + newMail->getSubject());
					break;
				}
			} while (choose == 0 && newMail->getSubject() == "");
		}

		display("Body (enter twice (ENTER) to end the body): ");

		std::string line;
		std::string text = "";
		do
		{
			enter(line);
			text += line + "\n";
		} while (!line.empty());

		newMail->setBody(text);

		if (newMail->getBody().empty() || newMail->getBody() == "\n") return nullptr;

		else return newMail;
	}
}

Mail* GraphInter::answerMail(Mail* &originalMail, const std::string &sender)
{
	std::ostringstream ID, BODY, SUBJECT;
	Mail* newMail = new Mail;
	std::string recipient;
	std::string WhatToSay;

	SUBJECT << "Re: " << originalMail->getSubject();

	newMail->setFrom(sender);
	newMail->setDate(time(0));

	newMail->setRecipient(originalMail->getFrom());
	newMail->setCounter(2);

	newMail->setSubject(SUBJECT.str());

	ID << sender << "_" << newMail->getDate();
	newMail->setId(ID.str());

	display(center_word("Answered mail", HORIZONTAL, " "));

	display("");

	display("From: " + sender);

	display("To: " + newMail->getRecipients()[0]);

	display("Subject: " + newMail->getSubject());

	display("Body (enter twice (ENTER) to end the body): ");

	std::string line;
	WhatToSay = "";
	do
	{
		enter(line);
		WhatToSay += line + "\n";
	} while (!line.empty());

	if (WhatToSay.empty() || WhatToSay == "\n") WhatToSay = "No body";

	BODY << WhatToSay << std::endl << linea()
		<< std::endl << originalMail->to_string();

	newMail->setBody(BODY.str());

	return newMail;
}

std::string GraphInter::selectAlias(Session* session)
{
	int number;

	number = aliasMenu(session);

	if (number < session->getUser()->getContactlist()->length()) return session->getUser()->getContactlist()->operator[](session->getVisible()->length() - number + 1)->getAddress();

	else return "";
}

ElemTray* GraphInter::selectMail(Session* session)
{
	int number;

	number = mailMenu(session);

	if (number != session->getVisible()->length()) return session->getVisible()->operator[](number);

	else return nullptr;
}

std::string GraphInter::center_word(std::string word, int length, std::string arround)
{
	if (word.size() != length)
	{
		for (int i = word.size(); i < length; i++)
		{
			if (word.size() % 2) word += arround;

			else word = arround + word;
		}
	}
	return word;
}

void GraphInter::showFastNames(ContactList* contactList)
{
	if (contactList->length() != 0)
	{
		std::ostringstream alias;

		alias << std::setw(10) << "Username" << std::setw(33) << "Alias";

		display(alias.str());

		display(linea());

		for (int i = 0; i < contactList->length(); i++)
		{
			display(tab_word(contactList->operator[](i)->header()));
		}
		display(linea());
	}
}

int GraphInter::SureToEmpty(Mail* mail)
{
	int key = UP, elem = 0;

	do
	{
		showRecipients(mail);
		display(linea());
		display("Are you sure you do not want this mail to have subject?");

		tab_word("No, I want to enter the subject", 0, elem);
		tab_word("Yes, I do not want subject for this mail", 1, elem);

		key = getKey();
		elem = update(key, elem, 2);

		clearConsole();

	} while (key != ENTER);

	return elem;
}

void GraphInter::drawMail(const Mail* mail)
{
	std::cout << mail->to_string() << std::endl;
	linea();
}

void GraphInter::showTray(Session* session)
{
	std::string title, thisMail;
	std::ostringstream menu;

	if (session->get_active_list() == 0)
	{
		title = center_word("Inbox", HORIZONTAL, "-");
	}
	if (session->get_active_list() == 1)
	{
		title = center_word("Outbox", HORIZONTAL, "-");
	}
	if (session->get_active_list() == 2)
	{
		title = center_word("Paper Bin", HORIZONTAL, "-");
	}

	display(linea());

	display(title);

	if (session->getVisible()->empty())
	{
		display(linea());

		display(center_word("You have no mails", HORIZONTAL, " "));
	}
	else
	{
		menu << std::endl << " R N" << std::setw(7) << "FROM" 
			<< std::setw(33) << "SUBJECT" << std::setw(30) << "DATE";

		display(menu.str());

		display(linea());

		for (int i = 0; i < session->getVisible()->length(); i++)
		{
			std::ostringstream show;

			if (session->getVisible()->operator[](i)->read) show << "  ";

			else show << " *";

			Mail* mail = session->getVisible()->operator[](i)->mail;

			if (mail == nullptr)
			{
				mail = error;
			}

			thisMail = mail->header();

			show << std::setw(2) << (i + 1 + (MAILS_X_PAGE*session->getVisible()->getPage())) << " - " << thisMail;
			display(show.str());
		}
	}
	display(linea());
	display(pags(session));
	display(linea());
}

int GraphInter::choosefilter()
{
	std::vector<std::string> elems;

	elems.push_back("Subject");
	elems.push_back("Date");
	elems.push_back("Emissor");
	elems.push_back("Recipients");
	elems.push_back("Body");
	elems.push_back("Read");
	elems.push_back("Unread");
	elems.push_back("Exit to session menu");

	return menu(elems);
}

int GraphInter::chooseorder()
{
	std::vector<std::string> elems;

	elems.push_back("Subject");
	elems.push_back("Date");

	return menu(elems);
}

int GraphInter::filter()
{
	std::vector<std::string> elems;

	elems.push_back("Change order");
	elems.push_back("Change filter");
	elems.push_back("Quit filter");
	elems.push_back("Exit to session menu");

	return menu(elems);
}

std::string GraphInter::linea()
{
	std::ostringstream line;

	line << std::setfill('-')
		<< std::setw(HORIZONTAL) << '-'
		<< std::setfill(' ');

	return line.str();
}

void GraphInter::clearConsole() { system("cls"); }

void GraphInter::pause()
{
	std::cin.sync();
	std::cin.get();
}

void GraphInter::checkUsername(std::string &username)
{
	std::string newUsername;

	display("Confirm your new username: ");

	enter(newUsername);

	newUsername += "@fdimail.com";

	if (newUsername != "@fdimail.com")
	{
		while (newUsername != username)
		{
			display("Error, usernames are not the same");
			pause();

			username = valid_user();

			if (username != "@fdimail.com")
			{
				display("Confirm your username:");

				enter(newUsername);

				newUsername += "@fdimail.com";
			}
			else newUsername = "@fdimail.com";
		}
	}
	else username = "@fdimail.com";
}

void GraphInter::checkPassword(std::string &password)
{
	std::string newPassword;

	display("Confirm your password: ");

	newPassword = HidePassword();

	while (newPassword != password)
	{
		display("Error, passwords are not the same");
		display("Enter your new passwords:");

		password = HideLimitPassword();

		display("Confirm your passwords:");

		newPassword = HidePassword();
	}
}

std::string GraphInter::HideLimitPassword()
{
	std::string word;
	bool correct;
	int security;

	do
	{
		security = 0;
		correct = true;
		word = HidePassword();

		if (word.size() != 0)
		{
			if (mayus(word)) security += 2;

			if (digit(word)) security += 2;

			if (symbl(word)) security += 2;

			if (word.size() >= PASSWORD_MIN_LENGTH) security++;

			if (word.size() >= PASSWORD_NORMAL_LENGTH) security++;

			if (security < SECURITY)
			{
				display("Your password is not secure enought");
				display("Enter your password");
			}
		}
	} while (word.size() != 0 && security < SECURITY);

	return word;
}

std::string GraphInter::valid_user()
{
	std::ostringstream character;
	std::string id;
	bool id_right;

	do
	{
		clearConsole();
		id_right = true;

		display("Enter your id: ");
		enter(id);

		if (id.size() > 15)
		{
			message("Error, your id cannot be longer than 15 characters ");

			id_right = false;
		}
		else
		{
			for (char i: id)
			{
				if ('A' > i || i > 'Z' && i < 'a' || i > 'z')
				{
					message("Error, your id cannot contain symbols or numbers, only leters");

					id_right = false;
				}
				else i = tolower(i);
			}
		}
	} while (!id_right);

	id = id + "@fdimail.com";

	return id;
}

void GraphInter::display(std::string error)
{
	std::cout << error << std::endl;
}

void GraphInter::display(char sign)
{
	std::cout << sign;
}

void GraphInter::enter(std::string &word)
{
	std::cin.sync();
	std::getline(std::cin, word);
	std::cin.clear();
}

void GraphInter::enter(int &digit)
{
	std::cin.sync();
	std::cin >> digit;
	std::cin.clear();
}

void GraphInter::enter(char* str)
{
	std::cin.sync();
	std::cin >> str;
	std::cin.clear();
}