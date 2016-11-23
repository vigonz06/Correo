/*-------------------------------
Fdimail
=======

Authors:
Victor Gonzalez del Hierro.

Date:
2015/5

Version:
10.0

Features:
Apart from the order in the enunciate of the practice, we have implemented:
	 1. A paper bin where the mails are inserted when you delete then, so that you can recover them
	 2. A sistem to interact with the console usig the arrows, and the (ENTER) and (ESCAPE) keys.
	 3. A submenu to order or filter the active tray by the paramenter you choose.
	 4. A submenu to change your username, password or delete your account.
	 5. A sistem of pages for show the active tray, with 5 mails per page.
	 6. An option to send, or forward a mail to more than one destinatary.
	 7. A subprogram to encript the user password on the savefile.
	 8. Detection and correction of errors in data entry console.
	 9. A submenu to asign alias to other users, or delete them.
	10. A subprogram to hide the entry console password.
	11. An option to forward a mail.
	 

---------------------------------*/

#include "Session.h"

void main() 
{
	Manager manager("fdimail.com");
	Session session(&manager);
}