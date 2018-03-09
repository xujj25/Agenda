#include "Storage.hpp"
#include "Path.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
/*class Storage {
private:

public:

private:
static std::shared_ptr<Storage> m_instance;
std::list<User> m_userList;
std::list<Meeting> m_meetingList;
bool m_dirty;
};
*/

/* private */
/**
*   default constructor
*/
Storage::Storage() {
	m_dirty = true;
	readFromFile();
	m_dirty = false;
}

/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
	if (m_dirty == false) {
		return false;
	}

	std::ifstream user_file(Path::userPath, std::ios::in);
	if (!user_file) return false;

	std::string value;
	std::string t_name, t_password, t_email, t_phone;
	while (!user_file.eof()) {

		std::getline(user_file, value, ',');
		if (value.length() == 0) break;
		t_name = std::string(value, 1, value.length() - 2);
		
		std::getline(user_file, value, ',');
		t_password = std::string(value, 1, value.length() - 2);
		
		std::getline(user_file, value, ',');
		t_email = std::string(value, 1, value.length() - 2);
		
		std::getline(user_file, value);
		t_phone = std::string(value, 1, value.length() - 2);
		
		createUser(User(t_name, t_password, t_email, t_phone));
	}

	std::string pati;
	int index;
	std::ifstream meeting_file(Path::meetingPath);
	if (!meeting_file) return false;
	std::string t_sponsor, t_startDate, t_endDate, t_title;
	std::vector<std::string> t_participators;
	while (!meeting_file.eof()) {
		std::getline(meeting_file, value, ',');
		if (value.length() == 0) break;
		t_sponsor = std::string(value, 1, value.length() - 2);
		
		std::getline(meeting_file, value, ',');
		t_participators.clear();
		for (index = 1; index < value.length(); index ++) {
			if (value[index] == '&' || value[index] == '"') {
				t_participators.push_back(pati);
				pati.clear();
			} else {
				pati.push_back(value[index]);
			}
		}
	
		std::getline(meeting_file, value, ',');
		t_startDate = std::string(value, 1, value.length() - 2);
		
		std::getline(meeting_file, value, ',');
		t_endDate = std::string(value, 1, value.length() - 2);
		
		std::getline(meeting_file, value);
		t_title = std::string(value, 1, value.length() - 2);

		createMeeting(
			Meeting(t_sponsor, t_participators, Date(t_startDate), Date(t_endDate), t_title));
	}

	return true;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/

std::string make_line_of_csv(std::vector<std::string> vec) {
	int index;
	std::string result;
	for (index = 0; index < vec.size() - 1; index ++) {
		result += ('"' + vec[index] + '"' + ',');
	}
	result += ('"' + vec[vec.size() - 1] + '"');
	return result;
}

std::string make_line_of_participators(std::vector<std::string> vec) {
	int index;
	std::string result;
	for (index = 0; index < vec.size() - 1; index ++) {
		result += (vec[index] + '&');
	}
	result += vec[vec.size() - 1];
	return result;
}

bool Storage::writeToFile(void) {
	if (m_dirty == false) {
		return false;
	}
	std::ofstream user_file(Path::userPath, std::ios::out);
	std::ofstream meeting_file(Path::meetingPath);
	std::vector<std::string> vec_of_line;
	for (auto& each : m_userList) {
		vec_of_line.clear();
		vec_of_line.push_back(each.getName());
		vec_of_line.push_back(each.getPassword());
		vec_of_line.push_back(each.getEmail());
		vec_of_line.push_back(each.getPhone());
		user_file << make_line_of_csv(vec_of_line) << std::endl;
	}
	for (auto& each : m_meetingList) {
		vec_of_line.clear();
		vec_of_line.push_back(each.getSponsor());
		vec_of_line.push_back(make_line_of_participators(each.getParticipator()));
		vec_of_line.push_back(Date::dateToString(each.getStartDate()));
		vec_of_line.push_back(Date::dateToString(each.getEndDate()));
		vec_of_line.push_back(each.getTitle());
		meeting_file << make_line_of_csv(vec_of_line) << std::endl;
	}
	m_dirty = false;
	return true;
}



/* public */
/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::m_instance = nullptr;
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr) {
		m_instance = std::shared_ptr<Storage>(new Storage);
	}
	return m_instance;
}

/**
*   destructor
*/
Storage::~Storage() {
	if (m_dirty == true) {
		sync();
	}
	m_dirty = false;
	m_instance = nullptr;
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User & t_user) {
	bool flag = false;
	for (auto& each : m_userList) {
		if (each.getName() == t_user.getName()) {
			flag = true;
			break;
		}
	}
	if (!flag) {
		m_userList.push_back(User(t_user));
		m_dirty = true;
	}
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> result;
	for (auto& each : m_userList) {
		if (filter(each)) {
			result.push_back(each);
		}
	}
	return result;
}

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
         std::function<void(User &)> switcher) {
	int num_of_updated_users = 0;
	for (auto& each : m_userList) {
		if (filter(each)) {
			switcher(each);
			num_of_updated_users ++;
			m_dirty = true;
		}
	}
	return num_of_updated_users;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
	std::list<User> new_userList;
	for (auto& each : m_userList) {
		if (!filter(each)) {
			new_userList.push_back(User(each));
		}
	}
	if (new_userList.size() == m_userList.size()) {
		return 0;
	} else {
		int num = m_userList.size() - new_userList.size();
		m_userList = new_userList;
		m_dirty = true;
		return num;
	}
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(Meeting(t_meeting));
	m_dirty = true;
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting> result;
	for (auto& each : m_meetingList) {
		if (filter(each)) {
			result.push_back(Meeting(each));
		}
	}
	return result;
}

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
            std::function<void(Meeting &)> switcher) {
	int num_of_updated_meetings = 0;
	for (auto& each : m_meetingList) {
		if (filter(each)) {
			switcher(each);
			num_of_updated_meetings ++;
			m_dirty = true;
		}
	}
	return num_of_updated_meetings;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	std::list<Meeting> new_meetingList;
	for (auto& each : m_meetingList) {
		if (!filter(each)) {
			new_meetingList.push_back(Meeting(each));
		}
	}
	if (m_meetingList.size() == new_meetingList.size()) {
		return 0;
	} else {
		int num = (m_meetingList.size() - new_meetingList.size());
		m_meetingList = new_meetingList;
		m_dirty = true;
		return num;
	}
}

/**
* sync with the file
*/
bool Storage::sync(void) {
	return writeToFile();
}