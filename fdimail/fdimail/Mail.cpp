#include <iostream>
#include <sstream>
#include <iomanip>
#include "Mail.h"
#include "Date.h"
#include <ctime>

Mail* Mail::errorMail()
{
	std::ostringstream ID;
	Mail* error = new Mail;
	std::string* recipient = new std::string;

	error->from = "Tecnical Service";
	error->date = time(0);
	error->user_count = 2;

	ID << "fdimail" << "_" << error->date;
	error->id = ID.str();

	*recipient = "fdimail";
	error->recipients.insert(recipient);

	error->subject = "Mail error";

	error->body = "It seems that this mail does not exist";

	return error;
}

void Mail::save(std::ofstream &file) const
{
	file << id << std::endl << user_count << std::endl 
		<< date << std::endl << from << std::endl;

	recipients.save(file);

	file << subject << std::endl 
		<< body << "#" << std::endl;
}

bool Mail::load(std::ifstream &file)
{
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
					if (recipients.load(file))
					{
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
	else return false;
}

const std::string Mail::to_string() const
{
	std::ostringstream flow;

	flow << "From: " << from << std::setw(55) << showDate(date) << std::endl;

	for (int i = 0; i < recipients.length(); i++)
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