#ifndef TRAYLIST
#define TRAYLIST

#include "GlobalConstants.h"
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

	tElemTray* get(const tElemTray* id);
	bool search(const tElemTray* id, int &pos);

	tElemTray* get(const std::string id);
	bool search(const std::string id, int &pos);

	void insert(tElemTray * const elem);
	bool destroy(const std::string id);

	void save(std::ofstream &file)const;
	void load(std::ifstream &file);

	bool readMail(const tElemTray* idMail);
};
#endif