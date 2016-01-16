#include "User.h"

User::User(const std::string idUser, const std::string password):
id(idUser), password(sha1(password)), contactList(idUser) {}

bool User::load(std::ifstream &file)
{
	file >> id;

	if (!file.fail())
	{
		file >> password;

		if (!file.fail())
		{
			contactList.load(file);
			recycling.load(file);
			outbox.load(file);
			inbox.load(file);
			return true;
		}
		else return false;
	}
	else return false;
}

void User::save(std::ofstream &file)const
{
	file << id << std::endl
		<< password << std::endl;


	contactList.save(file);
	recycling.save(file);
	outbox.save(file);
	inbox.save(file);
}