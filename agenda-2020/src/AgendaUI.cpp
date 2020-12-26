#include "AgendaUI.hpp"
#include <iostream>
#include <iomanip>
#include <csignal>
using namespace std;
static volatile int keepRunning = 1;

void sig_handler(int sig){
    if(sig == SIGINT){
        keepRunning = 0;
    }
}


AgendaUI::AgendaUI(){
    startAgenda();
};



void AgendaUI::OperationLoop(void){
    string op;
    do{
        signal(SIGINT,sig_handler);
        cout << "----------------------------------------Agenda----------------------------------------" << endl;
        cout << "Action :" << endl;
        cout << "l    - log in Agenda by user name and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "--------------------------------------------------------------------------------------" << endl << endl;
        cout << "Agenda :~$ ";
        cin >> op;
        switch(op[0]){
            case 'l':
                userLogIn();
                break;
            case 'r':
                userRegister();
                break;
            case 'q':
                quitAgenda();
                break;
            default:
                break;   
        } 
        if(op != "l" && op != "r" && op != "q"){
            cout << endl;
            cout << op << " is not a right command,please enter agian!" << endl;
        }
        if(keepRunning == 0){
            quitAgenda();
        }
    }while(op[0] != 'q' && keepRunning != 0);
};

    /**
     * constructor
     */
void AgendaUI::startAgenda(void){
    m_agendaService.startAgenda();
};

    /**
     * catch user's operation
     * @return the operation
     */
std::string AgendaUI::getOperation(){
    cout << "----------------------------------------Agenda----------------------------------------" << endl;
    cout << "Action :" << endl;
    cout << "o    - log out Agenda" << endl;
    cout << "dc   - delete Agenda" << endl;
    cout << "lu   - list all Agenda user" << endl;
    cout << "cm   - create a meeting" << endl;
    cout << "amp  - add meeting participator" << endl;
    cout << "rmp  - remove meeting participator" << endl;
    cout << "rqm  - request to quit meeting" << endl;
    cout << "la   - list all meetings" << endl;
    cout << "las  - list all sponsor meetings" << endl;
    cout << "lap  - list all participator meetings" << endl;
    cout << "qm   - query meeting by title" << endl;
    cout << "qt   - query meeting bt time interval" << endl;
    cout << "dm   - delete meeting bt title" << endl;
    cout << "da   - delete all meetings" << endl;
    cout << "--------------------------------------------------------------------------------------" << endl << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    string op;
    cin >> op;
    return op;
};

    /**
     * execute the operation
     * @return if the operationloop continue
     */
bool AgendaUI::executeOperation(std::string t_operation){
    if(t_operation == "o"){
        userLogOut();
        return false;
    }
    else if(t_operation == "dc"){
        deleteUser();
        return false;
    }
    else if(t_operation == "lu"){
        listAllUsers();
        return true;
    }
    else if(t_operation == "cm"){
        createMeeting();
        return true;
    }
    else if(t_operation == "amp"){
        addMeetingParticipator();
        return true;
    }
    else if(t_operation == "rmp"){
        removeMeetingParticipator();
        return true;
    }
    else if(t_operation == "rqm"){
        quitMeeting();
        return true;
    }
    else if(t_operation == "la"){
        listAllMeetings();
        return true;
    }
    else if(t_operation == "las"){
        listAllSponsorMeetings();
        return true;
    }
    else if(t_operation == "lap"){
        listAllParticipateMeetings();
        return true;
    }
    else if(t_operation == "qm"){
        queryMeetingByTitle();
        return true;
    }
    else if(t_operation == "qt"){
        queryMeetingByTimeInterval();
        return true;
    }
    else if(t_operation == "dm"){
        deleteMeetingByTitle();
        return true;
    }
    else if(t_operation == "da"){
        deleteAllMeetings();
        return true;
    }
    else{
        cout << endl;
        cout << t_operation << " is not a right command,please enter agian!" << endl;
    }
};

    /**
     * user Login
     */
void AgendaUI::userLogIn(void){
    cout << "\n[log in] [username] [password]" << endl;
    cout << "[log in] ";
    string username,password;
    cin >> username >> password;
    if(m_agendaService.userLogIn(username,password)){
        cout << "[log in] succeed!" << endl;
        m_userName = username;
        m_userPassword = password;
        while(keepRunning && executeOperation(getOperation()))
            quitAgenda();
    }
    else{
        cout << "[log in] Password error or user doesn't exist" << endl;
    }
};

    /**
     * user regist
     */
void AgendaUI::userRegister(void){
    cout << "\n[register] [username] [password] [email] [phone]" << endl;
    cout <<"[register] ";
    string name,password,email,phone;
    cin >> name >> password >> email >> phone;
    if(m_agendaService.userRegister(name,password,email,phone)){
        cout << "[register] succeed!" << endl;
    }
    else{
        cout << "[register] This username has been registered!" << endl;
    }
};

    /**
     * user logout
     */
void AgendaUI::userLogOut(void){
    m_userName = "";
    m_userPassword = "";
};

    /**
     * quit the Agenda
     */
void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
};

    /**
     * delete a user from storage
     */
void AgendaUI::deleteUser(void){
    if(m_agendaService.deleteUser(m_userName,m_userPassword)){
        cout << "\n[delete agenda account] succeed!" << endl;
    }
    userLogOut();
};

    /**
     * list all users from storage
     */
void AgendaUI::listAllUsers(void){
    list<User> user = m_agendaService.listAllUsers();
    cout << "\n[list all users]" << endl << endl;
    cout << left << setw(15) << "name" << left << setw(25) << "email" << "phone" << endl;
    for(auto i = user.begin();i != user.end();i++){
        cout << left << setw(15) << (*i).getName() << left << setw(25) << (*i).getEmail() << (*i).getPhone() << endl; 
    }
};

    /**
     * user create a meeting with someone else
     */
void AgendaUI::createMeeting(void){
    cout << "\n[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int participator_num = 0;
    cin >> participator_num;
    vector<string> par;
    for(int i = 0;i < participator_num;i++){
        cout << "[create meeting] [please enter the participator " << i+1 << " ]" << endl;
        string name;
        cin >> name;
        par.push_back(name);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title,start,end;
    cin >> title >> start >> end;
    if(m_agendaService.createMeeting(m_userName,title,start,end,par)){
        cout << "[create meeting] succeed!" << endl;
    }
    else{
        cout << "[create meeting] error!" << endl;
    }
};

    /**
     * list all meetings from storage
     */
void AgendaUI::listAllMeetings(void){
    cout << "\n[list all meetings]" << endl << endl;
    list<Meeting> meeting = m_agendaService.listAllMeetings(m_userName);
    if(meeting.size() == 0){
        cout << "[list all meetings] error!" << endl;
    }
    else
        printMeetings(meeting);
};

    /**
     * list all meetings that this user sponsored
     */
void AgendaUI::listAllSponsorMeetings(void){
    cout << "\n[list all sponsor meetings]" << endl << endl;
    list<Meeting> meeting = m_agendaService.listAllSponsorMeetings(m_userName);
    if(meeting.size() == 0){
        cout << "[list all sponsor meetings] error!" << endl;
    }
    else
        printMeetings(meeting);
};

    /**
     * list all meetings that this user take part in
     */
void AgendaUI::listAllParticipateMeetings(void){
    cout << "\n[list all participator meetings]" << endl << endl;
    list<Meeting> meeting = m_agendaService.listAllParticipateMeetings(m_userName);
    if(meeting.size() == 0){
        cout << "[list all participator meetings] error!" << endl;
    }
    else
        printMeetings(meeting);
};

    /**
     * search meetings by title from storage
     */
void AgendaUI::queryMeetingByTitle(void){
    cout << "\n[query meeting] [title]:" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> meeting = m_agendaService.meetingQuery(m_userName,title);
    if(meeting.size() == 0){
        cout << "[query meeting] error!" << endl;
    }
    else
        printMeetings(meeting);
};

    /**
     * search meetings by timeinterval from storage
     */
void AgendaUI::queryMeetingByTimeInterval(void){
    cout << "\b[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meetings] ";
    string start,end;
    cin >> start >> end;
    cout << "[query meetings]" << endl;
    list<Meeting> meeting = m_agendaService.meetingQuery(m_userName,start,end);
    if(meeting.size() == 0){
        cout << "[query meetings] error!" << endl;
    }
    else
        printMeetings(meeting);
};

    /**
     * delete meetings by title from storage
     */
void AgendaUI::deleteMeetingByTitle(void){
    cout << "\n[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.deleteMeeting(m_userName,title)){
        cout << "[delete meeting] succeed!" << endl;
    }
    else{
        cout << "[error] delete meeting fail!" << endl;
    }
};

    /**
     * delete all meetings that this user sponsored
     */
void AgendaUI::deleteAllMeetings(void){
    if(m_agendaService.deleteAllMeetings(m_userName)){
        cout << "\n[delete all meeting] succeed!" << endl;
    }
    else{
        cout << "\n[delete all meeting] error!" << endl;
    }
};

    /**
     * show the meetings in the screen
     */

string parsepar(vector<string> src){
    if(src.size() == 0){
        string s = "";
        return s;
    }
    string output;
    int i = 0;
    for(i = 0;i < src.size()-1;i++){
        output += src[i];
        output += ",";
    }
    output += src[i];
    return output;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings){
    cout << left << setw(15) << "\ntitle" << left << setw(12) << "sponsor" << left << setw(20) 
        << "start time" << left << setw(20) << "end time" << left << setw(15) << "participators" << endl;
    for(auto i = t_meetings.begin();i != t_meetings.end();i++){
        string par = parsepar((*i).getParticipator());
        cout << left << setw(15) << (*i).getTitle() << left << setw(12) << (*i).getSponsor() << left <<
            setw(20) << Date::dateToString((*i).getStartDate()) << left << setw(20) << Date::dateToString((*i).getEndDate()) 
            << left << setw(15) << par << endl;
    }
};

void AgendaUI::addMeetingParticipator(void){
    cout << "\n[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title,participator;
    cin >> title >> participator;
    if(m_agendaService.addMeetingParticipator(m_userName,title,participator)){
        cout << "[add participator] succeed!" << endl;
    }
    else{
        cout << "[add participator] error!" << endl;
    }
}

void AgendaUI::removeMeetingParticipator(void){
    cout << "\n[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title,participator;
    cin >> title >> participator;
    if(m_agendaService.removeMeetingParticipator(m_userName,title,participator)){
        cout << "[remove participator] succeed!" << endl;
    }
    else{
        cout << "[remove participator] error!" << endl;
    }
}

void AgendaUI::quitMeeting(void){
    cout << "\n[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.quitMeeting(m_userName,title)){
        cout << "[quit meeting] succeed!" << endl;
    }
    else{
        cout << "[quit meeting] error!" << endl;
    }
}
    

