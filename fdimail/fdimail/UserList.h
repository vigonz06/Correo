#ifndef USERLIST
#define USERLIST

#include "GlobalConstants.h"
#include <string>
#include "List.h"
#include "User.h"

/*----------------------------
Derived class from list, responsible
for holding the user database
------------------------------*/

class UserList: public List<User> {};

#endif