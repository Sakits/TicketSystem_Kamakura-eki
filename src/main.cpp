#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "vector.hpp"

using namespace std;

void initialize();

void function_chooser();

class ErrorOccur {
};

ofstream main_log("log.dat", ios::app);
ofstream finance_log("financelog.dat", ios::app);
ofstream operation_log("operation.dat", ios::app | ios::binary);

namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {

    void add_train(TrainID, StationNum, SeatNum, Stations, Prices, StartTime, TravelTimes, StopoverTimes, SaleDates, Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(Station, Station, MonthDate, TwoChoice);

    void query_transfer(Station, Station, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, Station, Station, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNum);
}

namespace sys {
    void log();

    void clean();

    void exit();
}

#define check(x,y) {cout << ' ' << #x << "=" << y;};
template<class A, class B>
void printall(A x, B y) {
    check(x,y)
}
template<class A, class B, class... Args>
void printall(A x, B y, const Args&... rest) {
    check(x,y)
    printAll(rest...);
}

//void checkpm(const string& _keyword){
//    stringstream ss(_keyword);
//    string oneword;
//    while (getline(ss, oneword, ' ')) {
//        cout <<
//    }
//    cout << endl;
//}

int main() {
#ifdef FileIO
    FileIO
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            cout << "Invalid" << endl;
        }
    }
}

string input;

void initialize() {

}

void function_chooser() {
    smatch parameter;
    auto pluralStrMaker = [](const string &str) -> const string {
        const string strNoSpace = str.substr(1);
        return " (" + strNoSpace + "(?:\\|" + strNoSpace + ")*)";
    };
    static const string
            chinese = "\\w",
//            chinese = "\\w/*[\u4e00-\u9fa5]*/",
            username = " ([a-zA-z]\\w{1,19})", _c = " -c" + username, _u = " -u" + username,
            passwd = " (\\w{6,30})", _pu = " -p" + passwd,
            name = " ("+ chinese +"{2,5})", _nu = " -n" + name,
            mailAddr = " ([0-9a-zA-Z\\@\\.]{1,30})", _mu = " -m" + mailAddr,
            privilege = " (10|0-9)", _g = " -g" + privilege;
    static const string
            trainID = username, _i = " -i" + trainID,
            stationNum = " (100|[1-9][0-9]|[2-9])", _n = " -n" + stationNum, _num = " -n (\\%d+)",
            station = " ("+chinese+"{1,10})", _startPlace = " -s" + station,
            _fromPlace = " -f" + station, _toPlace = " -t" + station, _ss = " -s" + pluralStrMaker(station),
            seatNum = " (100000|[1-9]\\d{0,4}|0)", _m = " -m" + seatNum,
            price = seatNum, _p = " -p" + price, _pt = " -p (time|cost)", _qt = " -q (false|true)",
            _ps = " -p" + pluralStrMaker(price),
            startTime = " ((?:[0-1][0-9]|2[0-3]|[0-9]):[0-5][0-9])", _x = " -x" + startTime, _s = " -s" + startTime,
            travelTime = " (10000|[1-9]\\\\d{0,3}|0)", _t = " -t" + travelTime,
            _ts = " -t" + pluralStrMaker(travelTime),
            stopoverTime = travelTime, _o = " -o" + stopoverTime, _os = " -o" + pluralStrMaker(stopoverTime),
            saleDate = " (06-(?:0[1-9]|[1-2][0-9]|30)|0[7-8]-(?:0[1-9]|[1-2][0-9]|3[0-1]))",
            _d = " -d" + saleDate, _ds = " -d" + pluralStrMaker(saleDate),
            type = " [A-Z]", _y = " -y" + type;
    getline(cin, input);
    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    if (cin.eof()) exit(0);
    if (input == "") return;
    Info("try to " + input);

    static auto match = [&parameter](const string &str) -> bool {
        return regex_search(input, parameter, regex("^" + str));
    };
    static auto hasparm = [&parameter](const string &str) -> bool {
        return regex_search(input, parameter, regex(str));
    };
    static auto pm = [&parameter](const string &str) -> string {
        if (hasparm(str)) {
//            auto debug = hasparm(str);
//            auto debug2 = parameter.str(1);
            return parameter.str(1);
        }
        return string();
    };
    static auto pmint = [&parameter](const string &str) -> int {
        if (hasparm(str)) return stoi(parameter.str(1));
        return -1;
    };
#define trymatch(str, func) if(match(str)){func;return;}
    trymatch("add_user", user::add_user(pm(_c), pm(_u), pm(_pu),
                                        pm(_nu),
                                        pm(_mu),
                                        pmint(_g)))
    trymatch("login", user::login(pm(_u), pm(_pu)))
    trymatch("logout", user::logout(pm(_u)))
    trymatch("query_profile", user::query_profile(pm(_c), pm(_u)))
    trymatch("modify_profile", user::modify_profile(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), pmint(_g)))

    trymatch("add_train",
             train::add_train(pm(_i), pmint(_n), pmint(_m), pm(_ss), pm(_ps), pm(_x), pm(_ts), pm(_os), pm(_ds),
                              pm(_y)))
    trymatch("release_train", train::release_train(pm(_i)))
    trymatch("query_train", train::query_train(pm(_i), pm(_d)))
    trymatch("delete_train", train::delete_train(pm(_i)))
    trymatch("query_ticket", train::query_ticket(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch("query_transfer", train::query_transfer(pm(_startPlace), pm(_toPlace), pm(_d), pm(_pt)))//FIXME
    trymatch("buy_ticket",
             train::buy_ticket(pm(_u), pm(_i), pm(_d), pmint(_num), pm(_fromPlace), pm(_toPlace), pm(_qt)))
    trymatch("query_order", train::query_order(pm(_u)))//FIXME
    trymatch("refund_ticket", train::refund_ticket(pm(_u), pmint(_num)))
    trymatch("log", sys::log())
    trymatch("clean", sys::clean())
    trymatch("exit", sys::exit())
#undef trymatch
    Error("SYNTAX ERROR");
    throw ErrorOccur();
}

void user::add_user(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr, Privilege privilege) {
    cout << "Username=" << cur_username << " Username=" << username << " Password=" << password << " Name=" << name << " MailAddr=" << mailAddr << " Privilege" << privilege<<endl;
}

void user::login(Username username, Password password) {
    printf("Username=%d Password=%d\n",username,password);
}

void user::logout(Username username) {

}


void user::query_profile(Username cur_username, Username username) {
    cout << "Username=" << cur_username << " Username=" << username << endl;
}

void user::modify_profile(Username, Username, Password, Name, MailAddr, Privilege) {

}

template<class T>
sjtu::vector<T> words_spliter(const string &_keyword) {
    sjtu::vector<T> ret;
    stringstream ss(_keyword);
    string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(oneword);
    }
    return ret;
}

sjtu::vector<int> ints_spliter(const string &_keyword) {
    sjtu::vector<int> ret;
    stringstream ss(_keyword);
    string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(stoi(oneword));
    }
    return ret;
}

void train::add_train(TrainID trainId, StationNum stationNum, SeatNum seatNum, Stations stations, Prices prices,
                      StartTime startTime, TravelTimes travelTimes, StopoverTimes stopoverTimes, SaleDates saleDates,
                      Type type) {
    sjtu::vector<Station> station_s = words_spliter<Station>(stations);
    sjtu::vector<TravelTime> travelTime_s = ints_spliter(travelTimes);
    sjtu::vector<StopoverTime> stopoverTime_s = ints_spliter(stopoverTimes);
    sjtu::vector<Price> price_s = ints_spliter(prices);
    sjtu::vector<SaleDate> saleDate_s = words_spliter<SaleDate>(saleDates);

}

void train::release_train(TrainID trainId) {

}

void train::query_train(TrainID, MonthDate) {

}

void train::delete_train(TrainID) {

}

void train::query_ticket(Station, Station, MonthDate, TwoChoice) {

}

void train::query_transfer(Station, Station, MonthDate, TwoChoice) {

}

void train::buy_ticket(Username, TrainID, MonthDate, TicketNum, Station, Station, TwoChoice) {

}

void train::query_order(Username) {

}

void train::refund_ticket(Username, OrderNum) {

}

void sys::log() {
    FLUSHLOG;
    ifstream fin("log.dat");
    string s;
    while (!fin.eof()) {
        getline(fin, s);
        cout << s << endl;
    }
    fin.close();
    Success;
}

void sys::clean() {

}

void sys::exit() {
    cout << "bye" << endl;
    std::exit(0);
}


