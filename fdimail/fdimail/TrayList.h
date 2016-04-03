#ifndef TRAYLIST
#define TRAYLIST

#include "tElemTray.h"
#include <fstream>
#include "List.h"

/*----------------------------
Derived class from list, responsible
for work whith the id of the mails, 
and its read status
------------------------------*/

class TrayList: public List<tElemTray>
{
public:

	bool search(const std::string &id, int &pos);
	tElemTray* get(const std::string &id);
	void insert(tElemTray * const elem);
	bool destroy(const std::string &id);
	bool pop(const std::string id);

	void save(std::ofstream &file)const;
	void load(std::ifstream &file);

	bool readMail(const std::string &idMail);
};
#endif