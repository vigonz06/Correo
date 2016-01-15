#ifndef GRAPHINTER
#define GRAPHINTER

#include "ContactList.h"
#include "TrayList.h"
#include "UserList.h"
#include "Mail.h"
#include "User.h"

/*----------------------------
Class responsible for interacting with the user
through the console
------------------------------*/

class Session;

class GraphInter
{
private:

	static GraphInter* inter;
	static Mail* errorMail();
	static Mail* error;

	GraphInter() {}

	int update(int key, int elem, int max_elems);
	void updateTray(int key, Session* session);

	int menu(std::string elems[], int max_elems, std::string to_choose);

	void send_to_multiple(Mail* mail, ContactList* contactList);
	void tab_word(std::string word, int pos, int cont);
	std::string tab_word(std::string word);
	std::string pags(Session* session);

public:

	static GraphInter* get();
	static void load();
	static void close();

	int mainMenu();
	void logMenu(std::string &username, std::string &password);
	int sessionMenu(Session* sesion);
	int trayMenu(Session* session, std::string elems[], int max_elems);
	int mailMenu(Session* session);
	int mailMenu(Mail* mail);
	int aliasMenu(Session* session);
	int menumail(Mail* mail, std::string elems[], int max_elems, std::string to_choose);
	int AliasMenu(Session* session);
	int WhatToDelete(Session* session);
	int MailOptions();
	int ChooseTray();
	int Invert();
	int AccountOptions();

	Mail* newMail(const std::string &sender, ContactList* contactList);
	Mail* answerMail(Mail* &originalMail, const std::string &sender);
	Mail* forward(Mail* &originalMail, const std::string &sender, ContactList* contactList);

	Mail* selectMail(Session* sesion);
	std::string selectAlias(Session* session);

	std::string center_word(std::string word, int length, std::string arround);
	void showFastNames(ContactList* contactList);
	int SureToEmpty(Mail* mail);

	void drawMail(const Mail* mail);
	void showTray(Session* session);

	int choosefilter(Session* session);
	int chooseorder(Session* session);
	int filter();

	void pause();
	std::string linea();
	void clearConsole();

	void checkUsername(std::string &password);
	void checkPassword(std::string &password);
	std::string valid_user();

	void enter(std::string &word);
	void enter(int &digit);
	void enter(char* str);

	std::string HideLimitPassword();
	std::string HidePassword();

	void display(std::string error);
	void display(char sign);
};
#endif