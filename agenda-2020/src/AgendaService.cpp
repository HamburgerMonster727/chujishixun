#include <list>
#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "Storage.hpp"
#include "AgendaService.hpp"
using namespace std;

  /**
   * constructor
   */
AgendaService::AgendaService(){
    startAgenda();
};

  /**
   * destructor
   */
AgendaService::~AgendaService(){
    quitAgenda();
};

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
    auto filter = [userName,password](const User &user){
        if(userName == user.getName() && password == user.getPassword()){
            return true;
        }
        else{
            return false;
        }
    };
    list<User> tmp = m_storage->queryUser(filter);
    int size = tmp.size();
    if(size > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,const std::string &email, const std::string &phone){
    User use(userName,password,email,phone);
    auto filter = [userName](const User &user){
        if(user.getName() == userName){
            return true;
        }
        else{
            return false;
        }
    };
    list<User> tmp = m_storage->queryUser(filter);
    if(tmp.empty()){
        m_storage->createUser(use);
        return true;
    }
    else{
        return false;
    }
};

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
    auto filter = [userName,password](const User &user){
        if(userName == user.getName() && password == user.getPassword()){
            return true;
        }
        else{
            return false;
        }
    };
    deleteAllMeetings(userName);
    auto parlist = listAllParticipateMeetings(userName);
    for(auto it = parlist.begin();it != parlist.end();it++){
        quitMeeting(userName,it->getTitle());
    }    
    int num = m_storage->deleteUser(filter);
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * list all users from storage
   * @return a user list result
   */
std::list<User> AgendaService::listAllUsers(void) const{
    auto filter = [](const User &user){
        return true;
    };
    list<User> tmp = m_storage->queryUser(filter);
    return tmp;
};

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,const std::string &startDate, const std::string &endDate,const std::vector<std::string> &participator){
    Date start(startDate);
    Date end(endDate);
    Meeting meeting(userName,participator,start,end,title);
    if(participator.empty()){
        return false;
    }
    if(start.isValid(start) == false || end.isValid(end) == false || start >= end){
        return false;
    }
    bool flag = false;
    list<User> tmp = listAllUsers();
    for(auto it = tmp.begin();it != tmp.end();it++){
        if(userName == it->getName()){
            flag = true;
            break;
        }
    } 
    if(flag == false){
        return false;
    }
    if(participator.empty()){
        return false;
    }
    auto filter = [title](const Meeting& meeting){
        if(meeting.getTitle() == title){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> tmp1 = m_storage->queryMeeting(filter);
    if(tmp1.empty() == false){
        return false;
    }
    auto filter1 = [userName](const Meeting& meeting){
        if(userName == meeting.getSponsor()){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> tmp2 = listAllMeetings(userName);
    for(auto it = tmp2.begin();it != tmp2.end();it++){
        if(!(it->getEndDate() <= start || it->getStartDate() >= end)){
            return false;
        }
    }
    vector<string> parti;
    Meeting meet(userName,parti,start,end,title);
    m_storage->createMeeting(meet);
    for(auto it = participator.begin();it != participator.end();it++){
        bool flag1;
        flag1 = addMeetingParticipator(userName,title,*it);
        if(flag1 == true){
            parti.push_back(*it);
        }
        else{
            m_storage->deleteMeeting(filter);
            return false;
        }
    }
    return true;
};

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::addMeetingParticipator(const std::string &userName,const std::string &title,const std::string &participator){
    auto filter = [participator](const User& user){
        if(user.getName() == participator){
            return true;
        }
        else{
            return false;
        }
    };
    list<User> use = m_storage->queryUser(filter);
    if(use.size() != 1){
        return false;
    }
    auto filter1 = [&](const Meeting& meeting){
        if(meeting.getSponsor() == userName && meeting.getTitle() == title){
            list<Meeting> meet = m_storage->queryMeeting([&](const Meeting& meeting1){
                if(meeting1.getSponsor() == participator || meeting1.isParticipator(participator)){
                    if(meeting.getStartDate() >= meeting1.getEndDate() || meeting.getEndDate() <= meeting1.getStartDate()){
                        return false;
                    }
                    else{
                        return true;
                    }
                }
                return false;
            });
            if(meet.empty()){
                return true;
            }
            return false;    
        }
        return false;
    };
    auto switcher = [participator](Meeting& meeting){
        meeting.addParticipator(participator);
    };
    int num = m_storage->updateMeeting(filter1,switcher);
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::removeMeetingParticipator(const std::string &userName,const std::string &title,const std::string &participator){
    if(userName == participator){
        return false;
    }
    auto filter = [userName](const User& user){
        if(user.getName() == userName){
            return true;
        }
        else{
            return false;
        }
    };
    auto filter1 = [participator](const User& user){
        if(user.getName() == participator){
            return true;
        }
        else{
            return false;
        }
    };
    list<User> user1 = m_storage->queryUser(filter);
    list<User> user2 = m_storage->queryUser(filter1);
    if(user1.empty() || user2.empty()){
        return false;
    }
    auto filter2 = [userName,title](const Meeting& meeting){
        if(meeting.getSponsor() == userName && meeting.getTitle() == title){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> meeting = m_storage->queryMeeting(filter2);
    if(meeting.size() != 1){
        return false;
    }
    Meeting meeting1 = *(meeting.begin());
    if(!meeting1.isParticipator(participator)){
        return false;
    }
    auto switcher = [participator](Meeting &meeting){
        meeting.removeParticipator(participator);
    };
    int num = m_storage->updateMeeting(filter2,switcher);
    m_storage->deleteMeeting([](const Meeting &meeting){
        return meeting.getParticipator().empty();
    });
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * quit from a meeting
   * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
   * @param title the meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
    auto filter = [userName,title](const Meeting &meeting){
       if(meeting.isParticipator(userName) && meeting.getTitle() == title){
           return true;
       } 
       else{
           return false;
       }
    };
    auto switcher = [userName](Meeting &meeting){
        meeting.removeParticipator(userName);
    };
    int num = m_storage->updateMeeting(filter,switcher);
    auto filter1 = [](const Meeting &meeting ){
        if(meeting.getParticipator().empty()){
            return true;
        }
        else{
            return false;
        }
    };
    //m_storage->deleteMeeting(filter1);
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * search a meeting by username and title
   * @param userName as a sponsor OR a participator
   * @param title the meeting's title
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,const std::string &title) const{
    auto filter = [userName,title](const Meeting& meeting){
        if((meeting.isParticipator(userName) || meeting.getSponsor() == userName) && meeting.getTitle() == title){
            return true;
        }
        else return false;
    };
    return m_storage->queryMeeting(filter);
};
  /**
   * search a meeting by username, time interval
   * @param userName as a sponsor OR a participator
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,const std::string &startDate,const std::string &endDate) const{
    list<Meeting> empty;
    Date start(startDate);
    Date end(endDate);
    if(!Date::isValid(start) || !Date::isValid(end) || start > end)
        return empty;
    auto filter = [userName,start,end](const Meeting& meeting){
        if(meeting.isParticipator(userName) || meeting.getSponsor() == userName){
            if(meeting.getStartDate() <= end && meeting.getEndDate() >= start){
                return true;
            }
        }
        else return false;
    };
    return m_storage->queryMeeting(filter);
};

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
    auto filter = [userName](const Meeting &meeting){
        if(userName == meeting.getSponsor() || meeting.isParticipator(userName)){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> tmp = m_storage->queryMeeting(filter);
    return tmp;
};

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    auto filter = [userName](const Meeting &meeting){
        if(userName == meeting.getSponsor()){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> tmp = m_storage->queryMeeting(filter);
    return tmp;
};

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const{
    auto filter = [userName](const Meeting &meeting){
        if(meeting.isParticipator(userName)){
            return true;
        }
        else{
            return false;
        }
    };
    list<Meeting> tmp = m_storage->queryMeeting(filter);
    return tmp;
};

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
    auto filter = [userName,title](const Meeting &meeting){
        if(userName == meeting.getSponsor() && title == meeting.getTitle()){
            return true;
        }
        else{
            return false;
        }
    };
    int num = m_storage->deleteMeeting(filter);
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
bool AgendaService::deleteAllMeetings(const std::string &userName){
    auto filter = [userName](const Meeting &meeting){
        if(userName == meeting.getSponsor()){
            return true;
        }
        else{
            return false;
        }
    };
    int num = m_storage->deleteMeeting(filter);
    if(num > 0){
        return true;
    }
    else{
        return false;
    }
};

  /**
   * start Agenda service and connect to storage
   */
void AgendaService::startAgenda(void){
    m_storage = Storage::getInstance();
};

  /**
   * quit Agenda service
   */
void AgendaService::quitAgenda(void){
    m_storage->sync();
};

