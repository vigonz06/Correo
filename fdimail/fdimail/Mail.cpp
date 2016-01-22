#include <iostream>
#include <sstream>
#include <iomanip>
#include "Mail.h"
#include "Date.h"

bool Mail::load(std::ifstream &file)
{
	std::string recipient;

	file >> id;

	if (id != "XXX" && !file.fail())
	{
		file >> user_count;

		if (!file.fail())
		{
			file >> date;

			if (!file.fail())
			{
				file >> from;

				if (!file.fail())
				{
					for (int i = 0; i < user_count - 1; i++)
					{
						file >> recipient;

						recipients.push_back(recipient);
					}
					if (!file.fail())
					{
						file.ignore();
						std::getline(file, subject);

						if (!file.fail())
						{
							std::getline(file, body, '#');

							if (!file.fail()) return true;
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

void Mail::save(std::ofstream &file) const
{
	file << id << std::endl << user_count << std::endl
		<< date << std::endl << from << std::endl;

	for (int i = 0; i < recipients.size(); i++)
	{
		file << recipients[i] << std::endl;
	}

	file << subject << std::endl
		<< body << "#" << std::endl;
}

const std::string Mail::to_string() const
{
	std::ostringstream flow;

	flow << "From: " << from << std::setw(55) << showDate(date) << std::endl;

	for (int i = 0; i < recipients.size(); i++)
	{
		flow << "To: " << recipients.operator[](i) << std::endl;
	}
	flow << std::endl << "Subject: " << subject << std::endl
		<< std::endl << body;

	return flow.str();
}

const std::string Mail::header()const
{
	std::ostringstream lowFlow;

	lowFlow << std::setw(30) << std::left << from << std::setw(34) << std::left << subject << showDay(date);

	return lowFlow.str();
}