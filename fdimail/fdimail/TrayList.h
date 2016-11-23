#ifndef TRAYLIST
#define TRAYLIST

#include "ElemTray.h"
#include <fstream>
#include "List.h"

class TrayList: public List<ElemTray>
{
public:

	bool search(const std::string &id, int &pos);
	ElemTray* get(const std::string &id);
	void insert(ElemTray * const elem);
	bool destroy(const std::string &id);
	bool pop(const std::string id);

	void save(std::ofstream &file)const;
	void load(std::ifstream &file);

	bool readMail(const std::string &idMail);
};
#endif