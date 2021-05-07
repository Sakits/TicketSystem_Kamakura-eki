

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "vector.hpp"
#include <functional>

void initialize();

void function_chooser();


namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {

    void
    add_train(TrainID, StationNum, SeatNum, StationNames, Prices, StartTime, TravelTimes, StopoverTimes, SaleDates,
              Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(StationName, StationName, MonthDate, TwoChoice);

    void query_transfer(StationName, StationName, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, StationName, StationName, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNumth);
}

namespace sys {

    void noReturnClean();

    void clean();

    void exit();
}


int main() {
#ifdef FileI
    FileI
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            Return(-1);
        }
    }
}

std::string input;

void initialize() {
    sys::noReturnClean();//FIXME used to debug
}

void function_chooser() {//FIXME 时间性能异常，首先要把所有regex东西都提出来改成static使她快20倍，而即使这样也特别的慢。
    ResetClock;

    std::smatch parameter;
    static auto pluralStrMaker = [](const std::string &str) -> const std::string {
        const std::string strNoSpace = str.substr(1);
        return " (" + strNoSpace + "(?:\\|" + strNoSpace + ")*)";
    };
    static const std::string
            chinese = "[^\\s\\|]{3}",
//            chinese = "\\w/*[\u4e00-\u9fa5]*/",
    username = " ([a-zA-z]\\w{0,19})",
            passwd = " (\\S{1,30})",
            name = " (" + chinese + "{2,5})",
            mailAddr = " ([0-9a-zA-Z\\@\\.]{1,30})",
            privilege = " (10|[0-9])";
    static const std::regex
            _c(" -c" + username),
            _u(" -u" + username),
            _pu(" -p" + passwd),
            _nu(" -n" + name),
            _mu(" -m" + mailAddr),
            _g(" -g" + privilege),
            rule_add_user("^add_user"),
            rule_login("^login"),
            rule_logout("^logout"),
            rule_query_profile("^query_profile"),
            rule_modify_profile("^modify_profile");
    static const std::string
            trainID = username,
            stationNum = " (100|[1-9][0-9]|[2-9])",
            station = " (" + chinese + "{1,10})",
            seatNum = " (100000|[1-9]\\d{0,4}|0)",
            price = seatNum,
            startTime = " ((?:[0-1][0-9]|2[0-3]|[0-9]):[0-5][0-9])",
            travelTime = " (10000|[1-9]\\d{0,3}|0)",
            stopoverTime = travelTime,
            saleDate = " (06-(?:0[1-9]|[1-2][0-9]|30)|0[7-8]-(?:0[1-9]|[1-2][0-9]|3[0-1]))",
            type = " ([A-Z])";
    static const std::regex
            _i(" -i" + trainID),
            _n(" -n" + stationNum), _num(" -n (\\d+)"),
            _startPlace(" -s" + station),
            _fromPlace(" -f" + station), _toPlace(" -t" + station), _ss(" -s" + pluralStrMaker(station)),
            _m(" -m" + seatNum),
            _p(" -p" + price), _pt(" -p (time|cost)"), _qt(" -q (false|true)"),
            _ps(" -p" + pluralStrMaker(price)),
            _x(" -x" + startTime), _s(" -s" + startTime),
            _t(" -t" + travelTime),
            _ts(" -t" + pluralStrMaker(travelTime)),
            _o(" -o" + stopoverTime), _os(" -o" + pluralStrMaker(stopoverTime)),
            _d(" -d" + saleDate), _ds(" -d" + pluralStrMaker(saleDate)),
            _y(" -y" + type),
            rule_add_train("^add_train"),
            rule_release_train("^release_train"),
            rule_query_train("^query_train"),
            rule_delete_train("^delete_train"),
            rule_query_ticket("^query_ticket"),
            rule_query_transfer("^query_transfer"),
            rule_buy_ticket("^buy_ticket"),
            rule_query_order("^query_order"),
            rule_refund_ticket("^refund_ticket"),
            rule_log("^log"),
            rule_clean("^clean"),
            rule_exit("^exit");

    getline(std::cin, input);

    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
//    if (cin.eof()) exit(0);
    //maybe 不知道有没有eof自动关闭的机制，先认为没有，因为可能没有buy返回值，但又要动态放回。
    if (input == "") return;

    Info(input);
    //memo 日志较为耗时，关掉即可


    //FIXME match这一步依然是耗时的大头
    static auto match = [&parameter](const std::regex &str) -> bool {

        return std::regex_search(input, parameter, str);
    };
    static auto pm = [&parameter](const std::regex &str) -> std::string {
//        ResetClock;
        if (match(str))
            return parameter.str(1);
        return std::string();
    };
    static auto pmint = [&parameter](const std::regex &str) -> int {
//        ResetClock;
        if (match(str)) return stoi(parameter.str(1));
        return -1;
    };

    static std::pair<std::regex, std::function<void()>> arr[17] = {{rule_add_user,       []() {
        user::add_user(pm(_c), pm(_u), pm(_pu), pm(_nu), pm(_mu), pmint(_g));
    }},
                                                                   {rule_login,          []() {
                                                                       user::login(pm(_u), pm(_pu));
                                                                   }},
                                                                   {rule_logout,         []() {
                                                                       user::logout(pm(_u));
                                                                   }},
                                                                   {rule_query_profile,  []() {
                                                                       user::query_profile(pm(_c), pm(_u));
                                                                   }},
                                                                   {rule_modify_profile, []() {
                                                                       user::modify_profile(pm(_c), pm(_u), pm(_pu),
                                                                                            pm(_nu), pm(_mu),
                                                                                            pmint(_g));
                                                                   }},
                                                                   {rule_add_train,      []() {
                                                                       train::add_train(pm(_i), pmint(_n), pmint(_m),
                                                                                        pm(_ss), pm(_ps), pm(_x),
                                                                                        pm(_ts), pm(_os), pm(_ds),
                                                                                        pm(_y));
                                                                   }},
                                                                   {rule_release_train,  []() {
                                                                       train::release_train(pm(_i));
                                                                   }},
                                                                   {rule_query_train,    []() {
                                                                       train::query_train(pm(_i), pm(_d));
                                                                   }},
                                                                   {rule_delete_train,   []() {
                                                                       train::delete_train(pm(_i));
                                                                   }},
                                                                   {rule_query_ticket,   []() {
                                                                       train::query_ticket(pm(_startPlace),
                                                                                           pm(_toPlace), pm(_d),
                                                                                           pm(_pt));
                                                                   }},
                                                                   {rule_query_transfer, []() {
                                                                       train::query_transfer(pm(_startPlace),
                                                                                             pm(_toPlace), pm(_d),
                                                                                             pm(_pt));
                                                                   }},
                                                                   {rule_buy_ticket,     []() {
                                                                       train::buy_ticket(pm(_u), pm(_i), pm(_d),
                                                                                         pmint(_num), pm(_fromPlace),
                                                                                         pm(_toPlace), pm(_qt));
                                                                   }},
                                                                   {rule_query_order,    []() {
                                                                       train::query_order(pm(_u));
                                                                   }},
                                                                   {rule_refund_ticket,  []() {
                                                                       train::refund_ticket(pm(_u), pmint(_num));
                                                                   }},
                                                                   {rule_log,            []() { log(); }},
                                                                   {rule_clean,          []() { sys::clean(); }},
                                                                   {rule_exit,           []() { sys::exit(); }}};

    for (auto i = begin(arr); i != end(arr); ++i) {
        if (match(i->first)) {
            i->second();
            ResetClock;
            return;
        }
    }

    Error("SYNTAX ERROR");
}

void user::add_user(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                    Privilege privilege) {
    ResetClock;
    cks(4, username, password, name, mailAddr);
    if (!existUsers.empty()) {//这里把它的语义改了下，不过应该无伤大雅，本来要判的是isFirstUser()的
        ck(cur_username);
        ckint(privilege);
        Privilege curPrivilege = *(loginUsers.find(cur_username));
        if (curPrivilege == -1)Error("CURRENT USER DOES NOT LOGIN");
        if (privilege >= curPrivilege)Error("NO PRIVILEGE");
//        if (password.size() < 6)Error("PASSWORD IS TOO SHORT");
    } else {
        privilege = 10;
        ckint(privilege);
    }
    existUsers.insert({username, User(privilege, name, mailAddr, password)});
    Return(0);

}

void user::login(Username username, Password password) {
    ResetClock;
    LoginTracing;
    cks(2, username, password);
    auto CurUserPair = existUsers.find(username);
    if (!CurUserPair.second) Error("USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(CurUserPair.first);
    if (foundUser.password != password) Error("WRONG PASSWORD");
    if (!loginUsers.insert({username, foundUser.privilege}))Error("USER HAS ALREADY LOGIN");
    Return(0);
}
//buyticket只改变loginUser的orderNumth，在logout或exit的时候才写回所有orderNumth。这可以在每次buyticket都节约一次写existUser的时间。

void user::logout(Username username) {
    ResetClock;
    LogoutTracing;
    ck(username);
    auto erasePair = loginUsers.erase(username);
    if (!erasePair.second)Error("CURRENT USER DOES NOT LOGIN");//erase in loginUsers

    Return(0);

}


void user::query_profile(Username cur_username, Username username) {
    ResetClock;
    cks(2, cur_username, username);
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    auto UserPair = existUsers.find(username);
    if (!UserPair.second) Error("FINDING USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(UserPair.first);
    Privilege &curPrivilege = *curUserPtr;
    if (!(curPrivilege > foundUser.privilege || cur_username == username)) Error("NO PRIVILEGE");
    Return(username + ' ' + foundUser.name + ' ' + foundUser.mailAddr + ' ' +
           std::to_string(foundUser.privilege));

}

void user::modify_profile(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                          Privilege privilege) {
    ResetClock;
    cks(2, cur_username, username);
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    Privilege curPrivilege = (cur_username == username) ? -2 : *curUserPtr;//-2表示解禁，小于任何权限

    auto ptr = existUsers.find(username);
    if (!ptr.second) Error("FINDING USER DOES NOT EXIST");
    User previous_user = existUsers.getItem(ptr.first);
    if (previous_user.privilege >= curPrivilege) Error("NO PRIVILEGE");
    const User &changedUser = User(privilege == -1 ? previous_user.privilege : privilege,
                                   name == "" ? previous_user.name : name,
                                   mailAddr == "" ? previous_user.mailAddr : mailAddr,
                                   password == "" ? previous_user.password : password);
    existUsers.setItem(ptr.first, changedUser);
    if (privilege != -1) {
        auto ptr = loginUsers.find(username);
        if (ptr)
            *ptr = privilege;//better 可改完上面后更短
    }
    Return(username + ' ' + changedUser.name + ' ' + changedUser.mailAddr + ' ' +
           std::to_string(changedUser.privilege));//显然应该以她现在的名字返回啊

}

template<class T>
sjtu::vector<T> words_spliter(const std::string &_keyword) {
    sjtu::vector<T> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(oneword);
    }
    return ret;
}

sjtu::vector<int> ints_spliter(const std::string &_keyword) {
    sjtu::vector<int> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(stoi(oneword));
    }
    return ret;
}

void train::add_train(TrainID trainID, StationNum stationNum, SeatNum seatNum, StationNames stations, Prices prices,
                      StartTime startTime, TravelTimes travelTimes, StopoverTimes stopoverTimes, SaleDates saleDates,
                      Type type) {
    ResetClock;
//    Tracer;
    cks(8, trainID, stations, prices,
        startTime, travelTimes, stopoverTimes, saleDates,
        type);
    ckints(2, stationNum, seatNum);
    sjtu::vector<StationName> station_s = words_spliter<StationName>(stations);
    sjtu::vector<PassedMinutes> travelTime_s = ints_spliter(travelTimes);
    sjtu::vector<PassedMinutes> stopoverTime_s = ints_spliter(stopoverTimes);
    sjtu::vector<Price> price_s = ints_spliter(prices);
    sjtu::vector<MonthDate> saleDate_s = words_spliter<MonthDate>(saleDates);
    if (!(station_s.size() == stationNum && price_s.size() == stationNum - 1 && travelTime_s.size() == stationNum - 1
          && stopoverTime_s.size() == std::max(1, stationNum - 2))) {
        Error("NUM OF '|' DOES NOT MATCH");
    }

    if (!existTrains.insert(
            {trainID, Train(stationNum, station_s, seatNum, price_s, startTime, travelTime_s, stopoverTime_s,
                            saleDate_s, type)}))
        Error("TRAIN ALREADY EXIST!");
    Return(0);
}

auto getTrainPtr(TrainID trainID) {
    auto curTrainPair = existTrains.find(trainID);
    if (!curTrainPair.second) Error("FINDING TRAIN DOES NOT EXIST");
    return curTrainPair.first;
}

Train getTrain(TrainID trainID) {
    return existTrains.getItem(getTrainPtr(trainID));
}

void AssureLogin(Username username) {
    if (!loginUsers.find(username))Error("USER DOES NOT LOGIN");//fixme 注意，存在它的地方都可以被另一个map优化掉，之后来做这个替换。
}

void train::release_train(TrainID trainID) {
    ResetClock;
//    Tracer;
    ck(trainID);
    auto trainPtr = getTrainPtr(trainID);
    Train train = existTrains.getItem(trainPtr);
    if (train.is_released == 1)Error("TRAIN HAS ALREADY BE RELEASED");
    train.is_released = 1;
    existTrains.setItem(trainPtr, train);

//stub
    for (int i = 0; i < train.stationNum; ++i)
        addPassedTrain(train.stations[i], trainID);

    Return(0);
}

void train::query_train(TrainID trainID, MonthDate startingMonthDate) {
    ResetClock;
//    Tracer;
    cks(2, trainID, startingMonthDate);
    Train train = getTrain(trainID);
    if (startingMonthDate < train.startSaleDate ||   train.endSaleDate < startingMonthDate)
        Error("QUERY DATE NOT IN SALE DATE");
//既然周聪说了，按他说的改 maybe
    std::string ans;
    Return(trainID + " " + train.type);
    //TODO 检验日期是否在内
    for (int i = 0; i < train.stationNum; ++i) {
        Return(train.stations[i] + " " + ((i!=0)?std::string(FullDate(startingMonthDate, train.startTime) += train
                .arrivingTimes[i]):"xx-xx xx:xx") + " -> " + ((i+1 != train.stationNum) ?std::string(FullDate(startingMonthDate, train.startTime) += train
                .leavingTimes[i]):"xx-xx xx:xx") + " " + std::to_string(train.prices[i]) + " " +
               ((i+1 != train.stationNum) ? std::to_string(train.ticketNums[startingMonthDate][i]) : "x"));
    }
}

void train::delete_train(TrainID trainID) {
    ResetClock;
//    Tracer;
    ck(trainID);
    if (!existTrains.erase(trainID))Error("DELETE TRAIN DOES NOT EXIST");
    Return(0);
}


void train::query_order(Username username) {
    ResetClock;
    ck(username);
    Tracer;
    AssureLogin(username);
    auto orderList = userOrders.find(username);
    Return(orderList->size());
    for (auto it = orderList->begin(); it != orderList->end(); ++it) {
        Return(std::string("[") + it->status + "] " + std::string(*it));
    }
}


//被informQueue调用。。。，或者被其返回的东西调用
//better 只要钻入,把两个函数的地方变成一个函数，就可以取消用sjtu::vector传信息的消耗了
void orderPendingChangeToSuccess(Order order) {

}

void informQueue(TrainID trainID, FullDate arrivingTime, FullDate leavingTime);

struct OrderCalculator {
    int timeret;
    Username username;
    TicketNum ticketNum;
    TwoChoice wannaWaitToBuyIfNoEnoughTicket;
    Order order;

    decltype(waitQueue.begin()) *ititer;

    void run(std::string functionName, TrainID trainID, MonthDate monthDate, StationName fromStation,
             StationName toStation) {
        Tracer;
        auto trainPtr = getTrainPtr(trainID);
        Train train = existTrains.getItem(trainPtr);
        if (!train.is_released) Error("TRAIN HAS NOT BEEN RELEASED YET");
        if (train.endSaleDate < monthDate || monthDate < train.startSaleDate) Error("OUT OF SALEDATE");
        const int fromint = train.findStation(fromStation), toint = train.findStation(toStation);
        const PassedMinutes leavingTime = train.leavingTimes[fromint], arrivingTime = train.arrivingTimes[toint];
        HourMinute startTime = train.startTime;
        int index = int(monthDate) - int(train.startSaleDate) - (startTime += leavingTime);
        if(index < 0) Error("INDEX IS -X");
        MonthDate md = train.startSaleDate;
        MonthDate trainStartDay(md += index);
        if (fromint == -1 || toint == -1)Error("CANNOT FIND STATION");
        TicketNum minTicket = 0x3f3f3f3f;
        for (int i = fromint; i < toint; ++i)
            minTicket = std::min(minTicket, train.ticketNums[index][i]);
        order = Order("", "pending",
                      trainID, fromStation, toStation,
                      FullDate(trainStartDay, train.startTime) += leavingTime,
                      FullDate(trainStartDay, train.startTime) += arrivingTime,
                      train.prices[toint] - train.prices[fromint],
                      minTicket
        );

        //构造order，及共同模式

        if (functionName == "query_ticket") {
            timeret = train.arrivingTimes[toint] - train.leavingTimes[fromint];
            return;
        }
        if (functionName == "buy_ticket") {
            AssureLogin(username);
            if (order.num < ticketNum) {//no enough ticket
                if (wannaWaitToBuyIfNoEnoughTicket == "false") Error("NO ENOUGH TICKET");
                waitQueue.push_front(order);//pending
                userOrders.insert({username, order});
                Return("queue");
                return;
            }

            order.status = "success";//success
            order.num = ticketNum;
            order.username = username;
            userOrders.insert({username, order});
            for (int i = fromint; i < toint; ++i) {
                train.ticketNums[index][i] -= ticketNum;
            }
            existTrains.setItem(trainPtr, train);
            Return(order.price * ticketNum);
            return;
        }

        if (functionName == "refund_ticket") {
            for (int i = fromint; i < toint; ++i)//strange 为什么不能lambda化？
                train.ticketNums[index][i] += order.num;
            existTrains.setItem(trainPtr, train);
            //todo 给那人退票操作。
            for (auto it = waitQueue.begin(); it != waitQueue.end(); ++it) {
                if (it->trainID != trainID || it->arrivingTime < order.leavingTime ||
                    order.arrivingTime < it->leavingTime)
                    continue;
                ititer = &it;
                run("informQueue", trainID, it->leavingTime, it->fromStation, it->toStation);
            }
            if (functionName == "informQueue") {//通知补票队列
                auto &it = *ititer;
                if (order.num < it->num)return;
                //todo check valid
                it->status = "success";//success
                for (int i = fromint; i < toint; ++i) {
                    train.ticketNums[index][i] -= it->num;
                }
                existTrains.setItem(trainPtr, train);
                orderPendingChangeToSuccess(*it);
                return;
            }

            // queue 可以持有快速访问order的方法，反之亦然。但反之的重要性不那么重要，因为内存queue既短又快。
            Return(0);
            return;
        }
    }
} orderCalculator;

//better 用order_calculator 都存在微小冗余，不用判错误等等

void train::query_ticket(StationName fromStation, StationName toStation, MonthDate monthDate,
                         TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    cks(3, fromStation, toStation, monthDate);
    if (sortFromLowerToHigherBy == "")sortFromLowerToHigherBy = "time";
    auto trains = findCommonTrain(fromStation, toStation);
    //stub in order to use sort in <algorithm>, I use std::vector instead of sjtu::vector
    std::vector<std::pair<int, std::string>> vans;
    for (TrainID trainID : trains) {
        orderCalculator.run("query_ticket", trainID, monthDate, fromStation, toStation);
        vans.push_back({(sortFromLowerToHigherBy == "time") ? orderCalculator.timeret : orderCalculator.order.price,
                        std::string(orderCalculator.order)});
    }
    //stub 对vector由低到高排序
#include <algorithm>

    sort(vans.begin(), vans.end());
    Return(vans.size());
    for (auto i : vans) Return(i.second);
//TODO
}

void train::query_transfer(StationName fromStation, StationName toStation, MonthDate monthDateWhenStartFromfromStation,
                           TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    cks(3, fromStation, toStation, monthDateWhenStartFromfromStation);
    //TODO

}


void train::buy_ticket(Username username, TrainID trainID, MonthDate monthDate, TicketNum ticketNum,
                       StationName fromStation,
                       StationName toStation, TwoChoice wannaWaitToBuyIfNoEnoughTicket) {
    ResetClock;
    Tracer;
    cks(5, username, trainID, monthDate, fromStation, toStation);
    ckint(ticketNum);
    if (wannaWaitToBuyIfNoEnoughTicket == "") wannaWaitToBuyIfNoEnoughTicket = "false";

    orderCalculator.ticketNum = ticketNum;
    orderCalculator.username = username;
    orderCalculator.wannaWaitToBuyIfNoEnoughTicket = wannaWaitToBuyIfNoEnoughTicket;
    orderCalculator.run("buy_ticket", trainID, monthDate, fromStation, toStation);
}


//queue开局加载，关闭放回
//existUsers开局加载，关闭放回

//对类的成员广播，让它们同意做什么，是可以的，只要有个地方持有所有类成员的指针就可以了。不过没必要

//todo User 的 orderTotalNum 放在exist和login里面好了

//guai,到底怎么优化掉这个重复代码啊


void train::refund_ticket(Username username, OrderNumth orderNumth) {
    ResetClock;
    Tracer;
    ck(username);
    ckint(orderNumth);
    if (orderNumth == -1) orderNumth = 1;
    AssureLogin(username);

    const auto orderIter = userOrders.find(username, orderNumth);
    if (!orderIter)Error("WRONG ORDERNUMTH");
    Order &order = *orderIter;
    if (order.status == "refunded")Error("ALREADY REFUNDED");
    if (order.status == "pending") {
        // todo 删除队列里的对应的等待Order
    }
    order.status = "refunded";
    orderCalculator.order = order;
    orderCalculator.run("refund_ticket", order.trainID, order.leavingTime, order.fromStation, order.toStation);
}


void sys::noReturnClean() {
    cleanlog();
    existUsers.clear();
    loginUsers.clear();
    waitQueue.clear();
}

void sys::clean() {
    sys::noReturnClean();
    Return(0);
}

void cache_putback() {
    //maybe use loginUser.ordernumth to write existUsers.ordernumth
}

void sys::exit() {
    Return("bye");
    log();//FIXME to debug
    cache_putback();
    std::exit(0);
}

