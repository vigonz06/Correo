#ifndef LIST
#define LIST

#include "GlobalConstants.h"
#include <vector>

template <class T>
class List
{
protected:

	int counter, dim;
	std::vector<T*> list;

	bool search(const std::string &id, int &pos, int &left_key, int &right_key) const;

public:

	List() {}
	~List() {}

	inline bool full()  const { return counter == dim; }
	inline bool empty() const { return counter == 0; }
	inline int length() const { return counter; }

	T* operator [](int i) const { return list[i]; }
	T* get(const std::string &id);

	bool destroy(const std::string &id);
	bool pop(const std::string &id);
	void insert(T* elem);
	void erase();

	void save(const std::string &name);
	bool load(const std::string &name);
};

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
bool List<T>::destroy(const std::string &id)
{
	int pos;
	int left_key = 0, right_key = counter - 1;
	if (search(id, pos, left_key, right_key))
	{
		delete list[pos];
		shiftLeft(pos);
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
void List<T>::insert(T* elem)
{
	if (full()) resize(dim + 1);

	int pos;
	int left_key = 0, right_key = counter - 1;

	search(elem->getId(), pos, left_key, right_key);
	shiftRight(pos);
	list[pos] = elem;
	counter++;
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

		while (right)
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
#endif
