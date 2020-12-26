#include <string>
#include <vector>
#include <fstream>
#include "Path.hpp"
#include "Storage.hpp"
using namespace std;

shared_ptr<Storage>Storage::m_instance = nullptr;

Storage::Storage(){
    m_dirty = false;
    readFromFile();
};

  /**
  *   disallow the copy constructor and assign operator
  */

  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
bool Storage::readFromFile(void){
    ifstream users(Path::userPath);
    ifstream meetings(Path::meetingPath);
    if(!(users.is_open() && meetings.is_open())){
        return false;
    }
    string str1;
    while(getline(users,str1)){
        string name,password,email,phone;
        if(str1.size() == 0){
            continue;
        }
        int len = str1.size();
        vector<int> pos;
        for(int i = 0;i < len;i++){
            if(str1[i] == '"')
                pos.push_back(i);
        }
        for(int i = pos[0]+1;i < pos[1];i++){
            name.push_back(str1[i]);
        }
        for(int i = pos[2]+1;i < pos[3];i++){
            password.push_back(str1[i]);
        }
        for(int i = pos[4]+1;i < pos[5];i++){
            email.push_back(str1[i]);
        }
        for(int i = pos[6]+1;i < pos[7];i++){
            phone.push_back(str1[i]);
        }
        User use(name,password,email,phone);
        m_userList.push_back(use);
    }
    users.close();
    string str2;
    while(getline(meetings,str2)){
        string sponsor,participators,startDate,endDate,title;
        if(str2.size() == 0){
            continue;
        }
        int len = str2.size();
        vector<int> pos;
        for(int i = 0;i < len;i++){
            if(str2[i] == '"')
                pos.push_back(i);
        }
        for(int i = pos[0]+1;i < pos[1];i++){
            sponsor.push_back(str2[i]);
        }
        for(int i = pos[2]+1;i < pos[3];i++){
            participators.push_back(str2[i]);
        }
        for(int i = pos[4]+1;i < pos[5];i++){
            startDate.push_back(str2[i]);
        }
        for(int i = pos[6]+1;i < pos[7];i++){
            endDate.push_back(str2[i]);
        }
        for(int i = pos[8]+1;i < pos[9];i++){
            title.push_back(str2[i]);
        }
        vector<string> t_participators;
        int len2 = participators.size();
        string str3;
        for(int i = 0;i < len2;i++){
            if(participators[i] != '&'){
                str3.push_back(participators[i]);
            }
            else{
                string temp = str3;
                t_participators.push_back(temp);
                str3.clear();
            }
        }
        t_participators.push_back(str3);
        Date start(startDate);
        Date end(endDate);
        Meeting meeting(sponsor,t_participators,start,end,title);
        m_meetingList.push_back(meeting);
    }
    meetings.close();
    return true;
};

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
bool Storage::writeToFile(void){
    ofstream users(Path::userPath);
    ofstream meetings(Path::meetingPath);
    if(!(users.is_open() && meetings.is_open())){
        return false;
    }
    for(auto it = m_userList.begin();it != m_userList.end();it++){
        users << '"' << (*it).getName() << '"' << ',' << '"' << (*it).getPassword() 
        << '"' << ',' << '"' << (*it).getEmail() << '"' << ',' << '"' << (*it).getPhone() << '"';
        users << "\n";
    }
    users.close();
    for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
        meetings << '"' << (*it).getSponsor() << '"' << ',' << '"';
        vector<string> str = (*it).getParticipator();
        int len = str.size();
        for(int i = 0;i < len-1;i++){
            meetings << str[i] << '&';
        }
        meetings << str[len-1];
        meetings << '"' << ',' << '"' << (*it).getStartDate().dateToString((*it).getStartDate()) << '"' << ',' << '"' << 
        (*it).getEndDate().dateToString((*it).getEndDate()) << '"' << ',' << '"' << (*it).getTitle() << '"';
        meetings << "\n";
    }
    meetings.close();
    return true;
};



std::shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance == nullptr){
        m_instance = shared_ptr<Storage>(new Storage());
    }
    return m_instance;
};

  /**
  *   destructor
  */
Storage::~Storage(){
    sync();
};

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
void Storage::createUser(const User &t_user){
    m_userList.push_back(t_user);
    m_dirty = true;
};

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
    list<User> ans;
    for(auto it = m_userList.begin();it != m_userList.end();it++){
        if(filter(*it)){
            ans.push_back(*it);
        }
    }
    return ans;
};

  /**
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher){
    int num = 0;
    for(auto it = m_userList.begin();it != m_userList.end();it++){
        if(filter(*it)){
            switcher(*it);
            num++;
        }
    }  
    if(num > 0){
        m_dirty = true;
    }              
    return num;
};

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
int Storage::deleteUser(std::function<bool(const User &)> filter){
    int num = 0;
    for(auto it = m_userList.begin();it != m_userList.end();){
        if(filter(*it)){
            it = m_userList.erase(it);
            num++;
        }
        else{
            it++;
        }
    }
    if(num > 0){
        m_dirty = true;
    }
    return num;
};

  /**
  * create a meeting
  * @param a meeting object
  */
void Storage::createMeeting(const Meeting &t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
};

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const{
    list<Meeting> meeting;
    for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
        if(filter(*it)){
            meeting.push_back(*it);
        }
    }   
    return meeting;   
};

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher){
    int num = 0;
    for(auto it = m_meetingList.begin();it != m_meetingList.end();it++){
        if(filter(*it)){
            switcher(*it);
            num++;
        }
    }    
    if(num > 0){
        m_dirty = true;
    }            
    return num;
};

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter){
    int num = 0;
    for(auto it = m_meetingList.begin();it != m_meetingList.end();){
        if(filter(*it)){
            it = m_meetingList.erase(it);
            num++;
        }
        else{
            it++;
        }
    } 
    if(num > 0){
        m_dirty = true;
    }
    return num;
};

  /**
  * sync with the file
  */
bool Storage::sync(void){
    m_dirty = false;
    return writeToFile();
};