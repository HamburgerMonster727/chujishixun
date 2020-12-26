#include <iostream>
#include <sstream>
#include "Date.hpp"
using namespace std;

Date::Date(){
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
    m_month = 0;
    m_year = 0;
};

  /**
  * @brief constructor with arguments
  */
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
    m_month = t_month;
    m_year = t_year;
};

  /**
  * @brief constructor with a string
  */
Date::Date(const std::string &dateString){
    bool flag = true;
    if(dateString[4] == '-' && dateString[7] == '-' && dateString[10] == '/' && dateString[13] == ':' && dateString.size() == 16){
        for(int i = 0;i < dateString.size();i++){
            if(i != 4 && i!= 7 && i!= 10 && i != 13 && (dateString[i] < '0' || dateString[i] > '9')){
                flag = false;
                m_year = 0;
                m_month = 0;
                m_day = 0;
                m_hour = 0;
                m_minute = 0;
                break;
            }
        }
        if(flag == true){
                m_year = (dateString[0] - '0')*1000+(dateString[1] - '0')*100+(dateString[2] - '0')*10+(dateString[3] - '0');
                m_month = (dateString[5] - '0')*10+(dateString[6] - '0');
                m_day = (dateString[8] - '0')*10+(dateString[9] - '0');
                m_hour = (dateString[11] - '0')*10+(dateString[12] - '0');
                m_minute = (dateString[14] - '0')*10+(dateString[15] - '0');
        }
    }
    else{
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;
    }
};
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
int Date::getYear(void) const{
    return m_year;
};

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
void Date::setYear(const int t_year){
    m_year = t_year;
};

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
int Date::getMonth(void) const{
    return m_month;
};

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
void Date::setMonth(const int t_month){
    m_month = t_month;
};

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
int Date::getDay(void) const{
    return m_day;
};

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
void Date::setDay(const int t_day){
    m_day = t_day;
};

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
int Date::getHour(void) const{
    return m_hour;
};

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
void Date::setHour(const int t_hour){
    m_hour = t_hour;
};

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
int Date::getMinute(void) const{
    return m_minute;
};

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
void Date::setMinute(const int t_minute){
    m_minute = t_minute;
};

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
bool Date::isValid(const Date &t_date){
    if(t_date.getYear() < 1000 || t_date.getYear() > 9999){
        return false;
    }
    if(t_date.getMonth() < 1 || t_date.getMonth() > 12){
        return false;
    }
    if(t_date.getHour() < 0 || t_date.getHour() > 23){
        return false;
    }
    if(t_date.getMinute() < 0 || t_date.getMinute() > 59){
        return false;
    }
    if(t_date.getMonth() == 1 || t_date.getMonth() == 3 || t_date.getMonth() == 5 ||
        t_date.getMonth() == 7 || t_date.getMonth() == 8 || t_date.getMonth() == 10 ||
        t_date.getMonth() == 12){
        if(t_date.getDay() < 1 || t_date.getDay() > 31){
            return false;
        }
    }
    if(t_date.getMonth() == 4 || t_date.getMonth() == 6 || t_date.getMonth() == 9 ||
        t_date.getMonth() == 11){
        if(t_date.getDay() < 1 || t_date.getDay() > 30){
            return false;
        }
    }
    if(t_date.getMonth() == 2){
        if(t_date.getYear()%400 == 0 || (t_date.getYear()%4 == 0 && t_date.getYear()%100 != 0)){
            if(t_date.getDay() < 1 || t_date.getDay() > 29){
                return false;
            }
        }
        else{
            if(t_date.getDay() < 1 || t_date.getDay() > 28){
                return false;
            }
        }
    }
    return true;
};

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
Date Date::stringToDate(const std::string &t_dateString){
    Date ans(t_dateString);
    return ans;
};

  /**
  * @brief convert a date to string, if the date is invalid return
  * 0000-00-00/00:00
  */
std::string Date::dateToString(const Date &t_date){
    if(t_date.isValid(t_date) == false){
        string str = "0000-00-00/00:00";
        return str;
    }
    int year = t_date.m_year;
    int month = t_date.m_month;
    int day = t_date.m_day;
    int hour = t_date.m_hour;
    int minute = t_date.m_minute;
    string str;
    str.push_back(year/1000 + '0');
    int three = year - (year/1000)*1000;
    str.push_back(three/100 + '0');
    int two = three - (three/100)*100;
    str.push_back(two/10 + '0');
    str.push_back(two%10 + '0');
    str.push_back('-');
    str.push_back(month/10 + '0');
    str.push_back(month%10 + '0');
    str.push_back('-');
    str.push_back(day/10 + '0');
    str.push_back(day%10 + '0');
    str.push_back('/');
    str.push_back(hour/10 + '0');
    str.push_back(hour%10 + '0');
    str.push_back(':');
    str.push_back(minute/10 + '0');
    str.push_back(minute%10 + '0');
    return str;
};

  /**
  *  @brief overload the assign operator
  */
Date & Date::operator=(const Date &t_date){
    setYear(t_date.getYear());
    setMonth(t_date.getMonth());
    setDay(t_date.getDay());
    setHour(t_date.getHour());
    setMinute(t_date.getMinute());
    return *this;
};

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  * @brief check whether the CurrentDate is equal to the t_date
  * @brief check whether the CurrentDate is equal to the t_date
  */
bool Date::operator==(const Date &t_date) const{
    if(getMinute() == t_date.getMinute() && getMonth() == t_date.getMonth() &&
        getDay() == t_date.getDay() && getYear() == t_date.getYear() && getHour() == t_date.getHour()) 
        return true;
    else
    {
        return false;
    }
    
};

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
bool Date::operator>(const Date &t_date) const{
    if(m_year > t_date.m_year){
        return true;
    }
    else if(m_year < t_date.m_year){
        return false;
    }
    else{
        if(m_month > t_date.m_month){
            return true;
        }
        else if(m_month < t_date.m_month){
            return false;
        }
        else{
            if(m_day > t_date.m_day){
                return true;
            }
            else if(m_day < t_date.m_day){
                return false;
            }
            else{
                if(m_hour > t_date.m_hour){
                    return true;
                }
                else if(m_hour < t_date.m_hour){
                    return false;
                }
                else{
                    if(m_minute > t_date.m_minute){
                        return true;
                    }
                    else if(m_minute < t_date.m_minute){
                        return false;
                    }
                }
            }
        }
    }
};

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
bool Date::operator<(const Date &t_date) const{
    if(m_year < t_date.m_year){
        return true;
    }
    else if(m_year > t_date.m_year){
        return false;
    }
    else{
        if(m_month < t_date.m_month){
            return true;
        }
        else if(m_month > t_date.m_month){
            return false;
        }
        else{
            if(m_day < t_date.m_day){
                return true;
            }
            else if(m_day > t_date.m_day){
                return false;
            }
            else{
                if(m_hour < t_date.m_hour){
                    return true;
                }
                else if(m_hour > t_date.m_hour){
                    return false;
                }
                else{
                    if(m_minute < t_date.m_minute){
                        return true;
                    }
                    else if(m_minute > t_date.m_minute){
                        return false;
                    }
                }
            }
        }
    }
};

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
bool Date::operator>=(const Date &t_date) const{
    if((*this) > t_date || (*this) == t_date){
        return true;
    }
    else{
        return false;
    }
};

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
bool Date::operator<=(const Date &t_date) const{
    if((*this) < t_date || (*this) == t_date){
        return true;
    }
    else{
        return false;
    }
};