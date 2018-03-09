#include "Date.hpp"
#include <sstream>
#include <string>
/*class Date {
private:
int m_year;
int m_month;
int m_day;
int m_hour;
int m_minute;
};*/

/**
* @brief default constructor
*/
Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {}

/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) : m_year(t_year),
    m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

/**
* @brief constructor with a string
* yyyy-mm-dd/hh:mm
*/

bool dateString_isValid(std::string dateString) {
    bool flag = true;;
    if (dateString.length() == 16) {
        if (!(dateString[4] == '-' && dateString[7] == '-' &&
                    dateString[10] == '/' && dateString[13] == ':')) {
            flag = false;
        }
        for (int index = 0; index < 16; index ++) {
            if (index != 4 && index != 7 && index != 10 && index != 13) {
                if (dateString[index] < '0' || dateString[index] > '9') flag = false;
            }
        }
    } else {
        flag = false;
    }
    return flag;
}


int string_to_int(std::string str, int pos, int length) {
    std::stringstream strings(str.substr(pos, length));
    int num;
    strings >> num;
    return num;
}

Date::Date(std::string dateString) {
    if (!dateString_isValid(dateString)) {
        m_year = m_month = m_day = m_hour = m_minute = 0;
    } else {
        m_year = string_to_int(dateString, 0, 4);
        m_month = string_to_int(dateString, 5, 2);
        m_day = string_to_int(dateString, 8, 2);
        m_hour = string_to_int(dateString, 11, 2);
        m_minute = string_to_int(dateString, 14, 2);
    }    
}
/**
* @brief return the year of a Date
* @return   a integer indicate the year of a date
*/
int Date::getYear(void) const {
    return m_year;
}

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) {
    m_year = t_year;
}

/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const {
    return m_month;
}

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) {
    m_month = t_month;
}

/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const {
    return m_day;
}

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) {
    m_day = t_day;
}
/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const {
    return m_hour;
}

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const {
    return m_minute;
}

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

/**
*   @brief check whether the date is valid or not
*   @return the bool indicate valid or not
*/
bool Date::isValid(const Date t_date) {
    if (!(t_date.getYear() >= 1000 && t_date.getYear() <= 9999
        && t_date.getMonth() <= 12 && t_date.getMonth() >= 1 &&
        t_date.getDay() >= 1 &&
        t_date.getHour() >= 0 && t_date.getHour() <= 23 &&
        t_date.getMinute() >= 0 && t_date.getMinute() <= 59)) return false;
    if (t_date.getMonth() == 1 || t_date.getMonth() == 3 || t_date.getMonth() == 5 ||
        t_date.getMonth() == 7 || t_date.getMonth() == 8 || t_date.getMonth() == 10 ||
        t_date.getMonth() == 12) return t_date.getDay() <= 31;
    if (t_date.getMonth()== 4 || t_date.getMonth()== 6 || t_date.getMonth() == 9 ||
        t_date.getMonth() == 11) return t_date.getDay() <= 30;
    if (t_date.getMonth() == 2) {
        if (t_date.getYear() % 100 == 0) {
            if (t_date.getYear() % 400 == 0) return t_date.getDay() <= 29;
            else return t_date.getDay() <= 28;
        } else if (t_date.getYear() % 4 == 0) {
            return t_date.getDay() <= 29;
        } else {
            return t_date.getDay() <= 28;
        }
    }
}

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string t_dateString) {
    return Date(t_dateString);
}

/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
std::string Date::dateToString(Date t_date) {
    if (!Date::isValid(t_date)) {
        return "0000-00-00/00:00";
    }
    std::stringstream ss;
    ss << t_date.getYear() << '-';
    if (t_date.getMonth() < 10) {
        ss << '0';
    }
    ss << t_date.getMonth() << '-';
    if (t_date.getDay() < 10) {
        ss << '0';
    }
    ss << t_date.getDay() << '/';
    if (t_date.getHour() < 10) {
        ss << '0';
    }
    ss << t_date.getHour() << ':';
    if (t_date.getMinute() < 10) {
        ss << '0';
    }
    ss << t_date.getMinute();
    return ss.str();
}

/**
*  @brief overload the assign operator
*/
Date& Date::operator=(const Date &t_date) {
    m_year = t_date.getYear();
    m_month = t_date.getMonth();
    m_day = t_date.getDay();
    m_hour = t_date.getHour();
    m_minute = t_date.getMinute();
    return *this;
}

/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const {
    return (m_year == t_date.getYear() &&
    m_month == t_date.getMonth() &&
    m_day == t_date.getDay() &&
    m_hour == t_date.getHour() &&
    m_minute == t_date.getMinute());
}

/**
* @brief check whether the CurrentDate is  greater than the t_date
*/
bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.getYear()) {
        return true;
    } else if (m_year < t_date.getYear()) {
        return false;
    } else if (m_month > t_date.getMonth()) {
        return true;
    } else if (m_month < t_date.getMonth()) {
        return false;
    } else if (m_day > t_date.getDay()) {
        return true;
    } else if (m_day < t_date.getDay()) {
        return false;
    } else if (m_hour > t_date.getHour()) {
        return true;
    } else if (m_hour < t_date.getHour()) {
        return false;
    } else {
        return (m_minute > t_date.getMinute());
    }
}

/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const {
    if (m_year < t_date.getYear()) {
        return true;
    } else if (m_year > t_date.getYear()) {
        return false;
    } else if (m_month < t_date.getMonth()) {
        return true;
    } else if (m_month > t_date.getMonth()) {
        return false;
    } else if (m_day < t_date.getDay()) {
        return true;
    } else if (m_day > t_date.getDay()) {
        return false;
    } else if (m_hour < t_date.getHour()) {
        return true;
    } else if (m_hour > t_date.getHour()) {
        return false;
    } else {
        return (m_minute < t_date.getMinute());
    }
}

/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const {
    return !(*this < t_date);
}

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const {
    return !(*this > t_date);
}
