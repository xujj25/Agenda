#include "AgendaUI.hpp"
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using std::cout;
using std::cin;
using std::endl;

/*class AgendaUI {

std::string m_userName;
std::string m_userPassword;
AgendaService m_agendaService;
};*/

/*public:*/

int state;
/*state: 
** 0: quit
** 1: start
** 2: login 
** 3: */

int string_to_int(std::string str) {
	std::stringstream int_ss(str);
	int num;
	int_ss >> num;
	return num;
}

AgendaUI::AgendaUI() {
	cout << endl;
	cout << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << "|                                                              |" << endl;
	cout << "|                    Welcome to use Agenda!                    |" << endl;
	cout << "|               SYSU copyright reserved (2016)                 |" << endl;
	cout << "|                                                              |" << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << endl;
	cout << endl;
	state = 1;
	OperationLoop();
}
void AgendaUI::OperationLoop(void) {
	while (true) {
		if (state == 0) {
			return;
		} else if (state == 1) {
			startAgenda();
			while (true) {
				cout << endl;
				cout << "----------------------------------------------------------------" << endl;
				cout << endl;
				cout << "[If you want help, please input \"h\" to get help_page]" << endl;
				cout << "Agenda : ~$ ";
				if (executeOperation(getOperation())) {
					continue;
				} else {
					break;
				}
			}
		} else if (state == 2) {
			cout << endl;
			cout << endl;
			cout << "-----------------------------Agenda-----------------------------" << endl;
			cout << endl;
			cout << "Action :" << endl;
			cout << "o   - log out Agenda" << endl;
			cout << "ci  - change account information" << endl;
			cout << "dc  - delete Agenda account" << endl;
			cout << "lu  - list all Agenda user" << endl;
			cout << "cm  - create a meeting" << endl;
			cout << "la  - list all meetings" << endl;
			cout << "las - list all sponsor meetings" << endl;
			cout << "lap - list all participate meetings" << endl;
			cout << "qm  - query meeting by title" << endl;
			cout << "qt  - query meeting by time interval" << endl;
			cout << "dm  - delete meeting by title" << endl;
			cout << "da  - delete all meetings" << endl;
			while (true) {
				cout << endl;
				cout << "----------------------------------------------------------------" << endl;
				cout << endl;
				cout << "[If you want help, please input \"h\" to get help_page]" << endl;
				cout << "Agenda@" << m_userName << " : # ";
				if (executeOperation(getOperation())) {
					continue;
				} else {
					break;
				}
			}
		}
	}
}

/*private:*/
/**
 * constructor
 */
void AgendaUI::startAgenda(void) {
	cout << endl;
	cout << "-----------------------------Agenda-----------------------------" << endl;
	cout << endl;
	cout << "Action:" << endl;
	cout << "l  - log in Agenda by user name and password" << endl;
	cout << "r  - register an Agenda account" << endl;
	cout << "q  - quit Agenda" << endl;
}

/**
 * catch user's operation
 * @return the operation
 */
std::string AgendaUI::getOperation() {
	std::string operation;
	cin >> operation;
	return operation;
}

/**
 * execute the operation
 * @return if the operationloop continue
 */
bool AgendaUI::executeOperation(std::string t_operation) {
	if (t_operation == "h") {
		return false;
	}
	if (state == 1) {
		if (t_operation == "l") {
			userLogIn();
			return false;
		} else if (t_operation == "r") {
			userRegister();
			return true;
		} else if (t_operation == "q") {
			quitAgenda();
			return false;
		} else {
			cout << endl;
			cout << "No such command!" << endl;
			return true;
		}
	} else if (state == 2) {
		if (t_operation == "o") {
			userLogOut();
			return false;
		} else if (t_operation == "ci") {
			changeUserInfo();
			return true;
		} else if (t_operation == "dc") {
			deleteUser();
			return false;
		} else if (t_operation == "lu") {
			listAllUsers();
			return true;
		} else if (t_operation == "cm") {
			createMeeting();
			return true;
		} else if (t_operation == "la") {
			listAllMeetings();
			return true;
		} else if (t_operation == "las") {
			listAllSponsorMeetings();
			return true;
		} else if (t_operation == "lap") {
			listAllParticipateMeetings();
			return true;
		} else if (t_operation == "qm") {
			queryMeetingByTitle();
			return true;
		} else if (t_operation == "qt") {
			queryMeetingByTimeInterval();
			return true;
		} else if (t_operation == "dm") {
			deleteMeetingByTitle();
			return true;
		} else if (t_operation == "da") {
			deleteAllMeetings();
			return true;
		} else {
			cout << endl;
			cout << "No such command!" << endl;
			return true;
		}
	}
}

/**
 * user Login
 */
void AgendaUI::userLogIn(void) {
	cout << endl;
	cout << "[log in] [user name] [password]" << endl;
	cout << "[log in] ";
	std::string userName, password;
	cin >> userName >> password;
	if (m_agendaService.userLogIn(userName, password)) {
		m_userName = userName;
		m_userPassword = password;
		cout << "[log in] succeed!" << endl;
		state = 2;
	} else {
		cout << "[error] log in fail!" << endl;
	}
}

/**
 * change user information
 */

void AgendaUI::changeUserInfo(void) {
	cout << endl;
	cout << "[change user information]" << endl;
	cout << endl;
	cout << "[information item] [choice number]" << endl;
	cout << "[password] -------------- 1" << endl;
	cout << "[email] ----------------- 2" << endl;
	cout << "[phone] ----------------- 3" << endl;
	cout << endl;
	cout << "[change user information] Please input your choice number:" << endl;
	cout << "[change user information] ";
	std::string choice_str;
	cin >> choice_str;
	int choice = string_to_int(choice_str);
	if (choice < 1 || choice > 3) {
		cout << "[change user information] error!" << endl;
		return;
	}

	cout << "[change user information] Please input your password again:" << endl;
	cout << "[change user information] ";
	std::string t_password;
	cin >> t_password;
	if (!m_agendaService.userLogIn(m_userName, t_password)) {
		cout << "[change user information] wrong password!" << endl;
		return;
	}

	cout << "[change user information] ";
	if (choice == 1) {
		cout << "[new password]" << endl;
	} else if (choice == 2) {
		cout << "[new email]" << endl;
	} else if (choice == 3) {
		cout << "[new phone]" << endl;
	}
	std::string information;
	cout << "[change user information] ";
	cin >> information;
	m_agendaService.changeUserInfo(m_userName, t_password, choice, information);
	cout << "[change user information] succeed!" << endl;
}

/**
 * user regist
 */
void AgendaUI::userRegister(void) {
	cout << endl;
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	std::string userName, password, email, phone;
	cin >> userName >> password >> email >> phone;
	if (m_agendaService.userRegister(userName, password, email, phone)) {
		cout << "[register] succeed!" << endl;
	} else {
		cout << "[error] register fail!" << endl;
	}
}

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda(void) {
	state = 0;
	cout << endl;
	cout << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << "|                                                              |" << endl;
	cout << "|           Thank you for using Agenda, CU next time!          |" << endl;
	cout << "|               SYSU copyright reserved (2016)                 |" << endl;
	cout << "|                                                              |" << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << endl;
	cout << endl;
}

/**
 * user logout
 */
void AgendaUI::userLogOut(void) {
	state = 1;
	m_userName = m_userPassword = "";
}

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser(void) {
	cout << endl;
	cout << "[delete agenda account] ";
	if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
		cout << "succeed!" << endl;
		state = 1;
	} else {
		cout << "error!" << endl;
	}
}

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void) {
	cout << endl;
	cout << "[list all users]" << endl;
	cout << endl;
	cout << std::left << std::setw(15) << "name"
	<< std::left << std::setw(15) << "email"
	<< std::left << std::setw(15) << "phone" << endl;
	for (auto& each : m_agendaService.listAllUsers()) {
		cout << std::left << std::setw(15) << each.getName()
		<< std::left << std::setw(15) << each.getEmail()
		<< std::left << std::setw(15) << each.getPhone() << endl;
	}
}

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting(void) {
	cout << endl;
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	std::string num_str;
	int number;
	std::string pati;
	std::vector<std::string> t_participators;
	cin >> num_str;
	number = string_to_int(num_str);

	if (number <= 0) {
		cout << "[create meeting] error!" << endl;
		return;
	}

	for (int idx = 1; idx <= number; idx ++) {
		cout << "[create meeting] [please enter the participator " << idx << " ]" << endl;
		cout << "[create meeting] ";
		cin >> pati;
		t_participators.push_back(pati);
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[create meeting] ";
	std::string t_title, t_startTime, t_endTime;
	cin >> t_title >> t_startTime >> t_endTime;
	cout << "[create meeting] ";
	if (m_agendaService.createMeeting(m_userName, t_title, t_startTime,t_endTime, t_participators)) {
		cout << "succeed!" << endl;
	} else {
		cout << "error!" << endl;
	}
}

/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings(void) {
	cout << endl;
	cout << "[list all meetings]" << endl;
	cout << endl;
	printMeetings(m_agendaService.listAllMeetings(m_userName));

}

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings(void) {
	cout << endl;
	cout << "[list all sponsor meetings]" << endl;
	cout << endl;
	printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings(void) {
	cout << endl;
	cout << "[list all participator meetings]" << endl;
	cout << endl;
	printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle(void) {
	cout << endl;
	cout << "[query meeting] [title]:" << endl;
	cout << "[query meeting] ";
	std::string t_title;
	cin >> t_title;
	printMeetings(m_agendaService.meetingQuery(m_userName, t_title));
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval(void) {
	cout << endl;
	cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meeting] ";
	std::string t_startTime, t_endTime;
	cin >> t_startTime >> t_endTime;
	printMeetings(m_agendaService.meetingQuery(m_userName, t_startTime, t_endTime));
}

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle(void) {
	cout << endl;
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	std::string t_title;
	cin >> t_title;
	cout << endl;
	if (m_agendaService.deleteMeeting(m_userName, t_title)) {
		cout << "[delete meeting by title] succeed!" << endl;
	} else {
		cout << "[error] delete meeting fail!" << endl;
	}
}

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings(void) {
	cout << endl;
	m_agendaService.deleteAllMeetings(m_userName);
	cout << "[delete all meetings] succeed!" << endl;
}

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
	cout << std::left << std::setw(10) << "title"
	<< std::left << std::setw(10) << "sponsor"
	<< std::left << std::setw(20) << "start time"
	<< std::left << std::setw(20) << "end time"
	<< std::left << std::setw(30) << "participators";
	
	int idx;
	std::string participators;
	for (auto& each : t_meetings) {
		cout << endl;
		for (idx = 0; idx < each.getParticipator().size(); idx ++) {
			participators += (each.getParticipator()[idx]);
			if (idx < each.getParticipator().size() - 1) {
				participators += ',';
			}
		}
		cout << std::left << std::setw(10) << each.getTitle()
		<< std::left << std::setw(10) << each.getSponsor()
		<< std::left << std::setw(20) << Date::dateToString(each.getStartDate())
		<< std::left << std::setw(20) << Date::dateToString(each.getEndDate())
		<< std::left << std::setw(30) << participators;
		participators.clear();
	}
	cout << endl;
}
