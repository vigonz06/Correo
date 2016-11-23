#ifndef GRAPHINTER
#define GRAPHINTER

#include "ContactList.h"
#include "TrayList.h"
#include "UserList.h"
#include "Mail.h"
#include "User.h"

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

	bool mayus(std::string word);
	bool digit(std::string word);
	bool symbl(std::string word);
	std::string HidePassword();

	int menu(std::vector<std::string> elems);

	void send_to_multiple(Mail* mail, ContactList* contactList);
	void tab_word(std::string word, int pos, int cont);
	std::string tab_word(std::string word);
	std::string pags(Session* session);
	void showRecipients(Mail* mail);

public:

	static GraphInter* get();
	static void close();
	static void load();

	int mainMenu();
	int trayMenu(Session* session, std::vector<std::string> elems);
	void logMenu(std::string &username, std::string &password);
	int menumail(Mail* mail, std::vector<std::string> elems);
	int WhatToDelete(Session* session);
	int sessionMenu(Session* sesion);
	int aliasMenu(Session* session);
	int AliasMenu(Session* session);
	int mailMenu(Session* session);
	int mailMenu(Mail* mail);
	
	int AccountOptions();
	int MailOptions();
	int chooseTray();
	int Invert();

	Mail* forward(Mail* &originalMail, const std::string &sender, ContactList* contactList);
	Mail* newMail(const std::string &sender, ContactList* contactList);
	Mail* answerMail(Mail* &originalMail, const std::string &sender);

	std::string selectAlias(Session* session);
	ElemTray* selectMail(Session* sesion);

	std::string center_word(std::string word, int length, std::string arround);
	void showFastNames(ContactList* contactList);
	int SureToEmpty(Mail* mail);

	void drawMail(const Mail* mail);
	void showTray(Session* session);

	int choosefilter();
	int chooseorder();
	int filter();

	std::string linea();
	void clearConsole();
	void pause();

	void checkUsername(std::string &password);
	void checkPassword(std::string &password);
	std::string HideLimitPassword();
	std::string valid_user();

	void display(std::string error);
	void display(char sign);

	void enter(std::string &word);
	void enter(int &digit);
	void enter(char* str);
};
#endif