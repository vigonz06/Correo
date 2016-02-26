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

	void init(int dim);
	void release();

public:

	List() : list(nullptr) { init(START_ELEM); }
	~List() { release(); }

	//inline bool full()  const { return counter == dim; }
	inline bool empty() const { return list->empty(); }
	inline int length() const { return list->size(); }

	T* operator [](int i) const { return list[i]; }

	bool destroy(const std::string &id);
	bool pop(const std::string &id);
	void insert(T* elem);

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
	int left_key = 0, right_key = list->size() - 1;

	search(elem->getId(), pos, left_key, right_key);
	list->insert(pos, elem);
}

template<class T>
bool List<T>::destroy(const std::string &id)
{
	int pos;
	int left_key = 0, right_key = list->size() - 1;
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
	int left_key = 0, right_key = list->size() - 1;
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
	for (auto i: list)
	{
		i = nullptr;
	}
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
	int ini = 0, fin = list->size() - 1;

	return search(id, pos, ini, fin) ? list[pos] : nullptr;
}

template<class T>
void List<T>::save(const std::string &name)
{
	std::ofstream file;

	file.open(name);

	for (auto i: list)
	{
		i->save(file);
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
void List<T>::init(int newdim)
{
	
	list = new std::vector<T>(newdim);

	for (auto i: lista)
	{
		i = nullptr;
	}
}

template<class T>
void List<T>::release()
{
	list->erase();
}
#endif
