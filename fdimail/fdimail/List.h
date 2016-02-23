#ifndef LIST
#define LIST

#include "GlobalConstants.h"
#include <string>
#include <vector>

/*----------------------------
This is a base class for all the lists this program has
We have used a template to be able to work whith diferent
types of arguments.

The list is dynamic and resizes automatically when inserting elements.
On destruction deletes every dynamic object held.
To prevent this (ie elements held should not be destroyed), call erase in child destructor.

By default, the list orders itself according to the valor returned by getId method of
elements inserted. If you choose to override insert to prevent order, make sure you override
search as well (it is a binary search)
------------------------------*/

template <class T>
class List
{
protected:

	std::vector<T*>* list;

public:

	List() : list(nullptr) {}
	~List() { release(); }

	void insert(T* elem);
	bool destroy(const std::string &id);
	bool pop(const std::string &id);

	void erase();

	bool search(const std::string &id, int &pos, int &left_key, int &right_key) const;
	T* get(const std::string &id);

	void save(const std::string &name);
	bool load(const std::string &name);
};

template<class T>
void List<T>::insert(T* elem)
{
	if (full()) resize(dim + 1);

	int pos;
	int left_key = 0, right_key = counter - 1;

	search(elem->getId(), pos, left_key, right_key);
	list->insert(pos, elem);
}

template<class T>
bool List<T>::destroy(const std::string &id)
{
	int pos;
	int left_key = 0, right_key = counter - 1;
	if (search(id, pos, left_key, right_key))
	{
		list->erase(pos);
		return true;
	}
	else return false;
}

template<class T>
bool List<T>::pop(const std::string &id)
{
	int pos;
	int left_key = 0, right_key = counter - 1;
	if (search(id, pos, left_key, right_key))
	{
		list[pos] = nullptr;
		shiftLeft(pos);
		return true;
	}
	else return false;
}

template<class T>
void List<T>::erase()
{
	for (int i = 0; i < counter; i++)
	{
		list[i] = nullptr;
	}
	counter = 0;
}

template<class T>
bool List<T>::search(const std::string &id, int &pos, int &left_key, int &right_key) const
{
	if (left_key <= right_key)
	{
		pos = (left_key + right_key) / 2;

		if (list[pos]->getId() == id) return true;
		
		if (list[pos]->getId() < id) left_key = pos + 1;

		if (list[pos]->getId() > id) right_key = pos - 1;

		return search(id, pos, left_key, right_key);
	}
	else
	{
		pos = left_key;
		return false;
	}
}

template<class T>
T* List<T>::get(const std::string &id)
{
	int pos = 0;
	int ini = 0, fin = counter - 1;

	return search(id, pos, ini, fin) ? list[pos] : nullptr;
}

template<class T>
void List<T>::save(const std::string &name)
{
	std::ofstream file;

	file.open(name);

	for (int i = 0; i < counter; i++)
	{
		list[i]->save(file);
	}

	file << "XXX";

	file.close();
}

template<class T>
bool List<T>::load(const std::string &name)
{
	std::ifstream file;
	bool right;
	T* elem;

	file.open(name);

	if (file.is_open())
	{
		right = true;

		for (int i = 0; right; i++)
		{
			elem = new T;

			if (!elem->load(file))
			{
				delete elem;
				right = false;
			}
			else insert(elem);
		}

		file.close();

		return true;
	}
	else return false;
}

template<class T>
void List<T>::release()
{
	if (dim != 0)
	{
		for (int i = 0; i < counter; i++)
		{
			delete list[i];
			list[i] = nullptr;
		}
		delete[] list;
		list = nullptr;
		counter = 0;
		dim = 0;
	}
}

template<class T>
void List<T>::resize(int newdim)
{
	if (newdim > dim)
	{
		T** newlist = new T*[newdim];

		for (int i = 0; i < counter; i++)
		{
			newlist[i] = list[i];
		}
		for (int i = counter; i < newdim; i++)
		{
			newlist[i] = nullptr;
		}
		delete[] list;

		list = newlist;
		dim = newdim;
	}
}
#endif
