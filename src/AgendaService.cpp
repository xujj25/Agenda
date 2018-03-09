#include "AgendaService.hpp"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
/*class AgendaService {
 private:
    std::shared_ptr<Storage> m_storage;
};*/

/**
* constructor
*/


AgendaService::AgendaService() {
    startAgenda();
}

/**
* destructor
*/
AgendaService::~AgendaService() {
    quitAgenda();
}

/**
* check if the username match password
* @param userName the username want to login
* @param password the password user enter
* @return if success, true will be returned
*/
bool AgendaService::userLogIn(const std::string userName, const std::string password) {
    if ((m_storage -> queryUser([&](const User &each) -> bool {
            return (userName == each.getName() && password == each.getPassword());
        })).size() == 0) {
        return false;
    } else {
        return true;
    }
}

/**
* regist a user
* @param userName new user's username
* @param password new user's password
* @param email new user's email
* @param phone new user's phone
* @return if success, true will be returned
*/
bool AgendaService::userRegister(const std::string userName, const std::string password,
          const std::string email, const std::string phone) {
    if ((m_storage -> queryUser([&](const User &each) -> bool {
            return (userName == each.getName());
        })).size() == 0) {
        m_storage -> createUser(User(userName, password, email, phone));
        return true;
    } else {
        return false;
    }
}

/**
* change the infomation of user
* @param userName user's username
* @param password user's password
* @param operation the item that want to operate on(1-password, 2-email, 3-phone)
* @param info the infomation that want to change for
* @return if success, true will be returned
*/
bool AgendaService::changeUserInfo(const std::string userName, const std::string password,
                    const int operation, const std::string info) {
    if (operation < 1 || operation > 3) return false;
    return (m_storage -> updateUser([&](const User& each) -> bool {
            return (each.getName() == userName && each.getPassword() == password);
        }, 
        [&](User& each) -> void {
            if (operation == 1) {
                each.setPassword(info);
            } else if (operation == 2) {
                each.setEmail(info);
            } else if (operation == 3) {
                each.setPhone(info);
            }
        }) > 0);
}

/**
* delete a user
* @param userName user's username
* @param password user's password
* @return if success, true will be returned
*/
bool AgendaService::deleteUser(const std::string userName, const std::string password) {
    int num = (m_storage -> deleteUser([&](const User& each) -> bool {
        return (userName == each.getName() && password == each.getPassword());
        }));
    if (num > 0) {
        m_storage -> deleteMeeting([&](const Meeting& each) -> bool {
            if (userName == each.getSponsor()) {
                return true;
            } else {
                bool flag = false;
                for (auto& element : each.getParticipator()) {
                    if (element == userName) {
                        flag = true;
                        break;
                    }
                }
                return flag;
            }
        });
        return true;
    } else {
        return false;
    }
}

/**
* list all users from storage
* @return a user list result
*/
std::list<User> AgendaService::listAllUsers(void) const {
    return (m_storage -> queryUser([](const User& each) -> bool {return true;}));
}

/**
* create a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @param startData the meeting's start date
* @param endData the meeting's end date
* @return if success, true will be returned
*/
bool AgendaService::createMeeting(const std::string userName, const std::string title,
           const std::string startDate, const std::string endDate,
           const std::vector<std::string> participator) {
    if (participator.size() == 0) return false;
    if (Date::dateToString(Date(startDate)) == "0000-00-00/00:00" ||
        Date::dateToString(Date(endDate)) == "0000-00-00/00:00") {

        return false;
    }
    
    if (Date(startDate) >= Date(endDate)) return false;
    
    if (m_storage -> queryUser([&](const User& each) -> bool {
        return each.getName() == userName;
    }).size() == 0) {

        return false;
    }

    for (auto& each_person : participator) {
        if (m_storage -> queryUser([&](const User& each) -> bool {
            return each.getName() == each_person;
        }).size() == 0) {

            return false;
        }
    }

    for (auto& each_meeting : (m_storage -> queryMeeting([](const Meeting& each) -> bool {
        return true;
    }))) {
        if (each_meeting.getTitle() == title) return false;
    }

    for (auto& each : listAllMeetings(userName)) {
        if (each.getEndDate() > Date::stringToDate(startDate) &&
            each.getStartDate() < Date::stringToDate(endDate)) {
            return false;
        }
    }
    for (auto& each_person : participator) {
        for (auto& each : listAllMeetings(each_person)) {
            if (each.getEndDate() > Date::stringToDate(startDate) &&
                each.getStartDate() < Date::stringToDate(endDate)) {
                return false;
            }
        }
    }

    m_storage -> createMeeting(Meeting(userName, participator,
            Date::stringToDate(startDate), Date::stringToDate(endDate), title));

    return true;
}

/**
* search meetings by username and title (user as sponsor or participator)
* @param uesrName the user's userName
* @param title the meeting's title
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                        const std::string title) const {
    std::list<Meeting> result;
    for (auto& each : listAllMeetings(userName)) {
        if (each.getTitle() == title) {
            result.push_back(each);
        }
    }
    return result;
}
/**
* search a meeting by username, time interval (user as sponsor or participator)
* @param uesrName the user's userName
* @param startDate time interval's start date
* @param endDate time interval's end date
* @return a meeting list result
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                        const std::string startDate,
                        const std::string endDate) const {
    std::list<Meeting> result;

    if (Date::dateToString(Date(startDate)) == "0000-00-00/00:00" ||
        Date::dateToString(Date(endDate)) == "0000-00-00/00:00") {

        return result;
    }
    
    if (Date(startDate) > Date(endDate)) return result;

    for (auto& each : listAllMeetings(userName)) {
        if (each.getEndDate() >= Date::stringToDate(startDate) &&
            each.getStartDate() <= Date::stringToDate(endDate)) {
            result.push_back(each);
        }
    }
    
    return result;
}

/**
* list all meetings the user take part in
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllMeetings(
    const std::string userName) const {
    return (m_storage -> queryMeeting([&](const Meeting& each) -> bool {
        if (userName == each.getSponsor()) {
            return true;
        } else {
            bool flag = false;
            for (auto& element : each.getParticipator()) {
                if (element == userName) {
                    flag = true;
                    break;
                }
            }
            return flag;
        }
    }));
}

/**
* list all meetings the user sponsor
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllSponsorMeetings(
    const std::string userName) const {
    return (m_storage -> queryMeeting([&](const Meeting& each) -> bool {
        return (userName == each.getSponsor());
    }));
}

/**
* list all meetings the user take part in and sponsor by other
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string userName) const {
    return (m_storage -> queryMeeting([&](const Meeting& each) -> bool {
        bool flag = false;
        for (auto& element : each.getParticipator()) {
            if (element == userName) {
                flag = true;
                break;
            }
        }
        return flag;
    }));
}

/**
* delete a meeting by title and its sponsor
* @param userName sponsor's username
* @param title meeting's title
* @return if success, true will be returned
*/
bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
    return (m_storage -> deleteMeeting([&](const Meeting& each) -> bool {
        return (userName == each.getSponsor() && title == each.getTitle());
    }) > 0);
}

/**
* delete all meetings by sponsor
* @param userName sponsor's username
* @return if success, true will be returned
*/
bool AgendaService::deleteAllMeetings(const std::string userName) {
    return (m_storage -> deleteMeeting([&](const Meeting& each) -> bool {
        return userName == each.getSponsor();
    }) > 0);
}

/**
* start Agenda service and connect to storage
*/
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

/**
* quit Agenda service
*/
void AgendaService::quitAgenda(void) {
    m_storage -> sync();
}
