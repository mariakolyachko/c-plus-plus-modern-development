#include <iostream>
#include <set>
#include <iomanip>
#include <map>

using namespace std;

class WrongDateFormatException {
public:
    string wrongDate;

    WrongDateFormatException(string invalidDate) {
        this->wrongDate = invalidDate;
    }
};


class WrongMonthException {
public:
    int wrongMonth;

    WrongMonthException(int wrongMonth) {
        this->wrongMonth = wrongMonth;
    }
};


class WrongDayException {
public:
    int wrongDay;

    WrongDayException(int wrongDay) {
        this->wrongDay = wrongDay;
    }
};


class Date {
public:
    Date(int day = 1, int month = 1, int year = 0) {
        this->day = day;
        this->month = month;
        this->year = year;
        checkDate();
    }

    Date(const string &date) {
        if (isDateFormat(date)) {
            stringstream stream(date);
            stream >> year;
            stream.ignore(1);
            stream >> month;
            stream.ignore(1);
            stream >> day;
            checkDate();
        } else {
            throw WrongDateFormatException(date);
        }
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

    void checkDate() {
        if (!isCorrectMonth()) {
            throw WrongMonthException(month);
        } else if (!isCorrectDay()) {
            throw WrongDayException(day);
        }
    }

    bool isCorrectMonth() {
        return !(month <= 0 || month > 12);
    }

    bool isCorrectDay() {
        return !(day <= 0 || day > 31);
    }

    bool isDateFormat(const string &date) {
        int pos = 0;

        for (int i = 0; i < 3; i++) {
            if (i) {
                if (date[pos] != '-') {
                    return false;
                }
                pos++;
            }
            if (date[pos] == '-' || date[pos] == '+') {
                pos++;
            }
            int count = 0;
            while (pos < date.size() && date[pos] != '-') {
                if (date[pos] < '0' || date[pos] > '9') {
                    return false;
                }
                count++;
                pos++;
            }
            if (count == 0) {
                return false;
            }
        }
        return pos >= date.size();
    }

    friend ostream &operator<<(ostream &stream, const Date &item) {
        stream << setw(4) << setfill('0') << item.year;
        stream << '-' << setw(2) << setfill('0') << item.month;
        stream << '-' << setw(2) << setfill('0') << item.day;
        return stream;
    }

    friend istream &operator>>(istream &stream, Date &item) {
        string date;
        getline(stream, date);
        try {
            item = Date(date);
        } catch (WrongDateFormatException &ex) {
            cout << "Wrong date format: " << ex.wrongDate << endl;
        }
        return stream;
    }

private:
    int day{};
    int month{};
    int year{};
};

bool operator<(const Date &lhs, const Date &rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetYear() < rhs.GetYear();
}

bool operator==(const Date &lhs, const Date &rhs) {
    return lhs.GetYear() == rhs.GetYear()
           && lhs.GetMonth() == rhs.GetMonth()
           && lhs.GetDay() == rhs.GetDay();
}


class Database {
public:
    void AddEvent(const Date &date, const string &event) {
       events[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (events.find(date) != events.end()) {
            if (events[date].find(event) != events[date].end()) {
                events[date].erase(event);
                return true;
            }
        }
        return false;
    }

    int DeleteDate(const Date &date) {
        int count = 0;
        if (events.find(date) != events.end()) {
            count = events[date].size();
            events.erase(date);
        }
        return count;
    }

    set<string> Find(const Date &date) const {
        if (events.find(date) != events.end()) {
            return events.find(date)->second;
        }
        return set<string>();
    }

    void Print() const {
        for (const auto &it : events) {
            for (const auto &event : it.second){
                std::cout << it.first << ' ' << event << std::endl;
            }
        }
    }

private:
    map<Date, set<string>> events;
};

int main() {
    Database db;

    string command;
    try {
        while (getline(cin, command)) {
            if (!command.empty()) {
                command += ' ';

                auto command_name = command.substr(0, command.find(' '));
                command.erase(0, command.find(' ') + 1);

                if (command_name == "Print") {
                    db.Print();
                } else {
                    string date = command.substr(0, command.find(' '));
                    command.erase(0, command.find(' ') + 1);
                    if (!command.empty())
                        command.erase(command.size() - 1, 1);

                    if (command_name == "Add") {
                        db.AddEvent(Date(date), command);
                    } else if (command_name == "Find") {
                        for (const auto &event : db.Find(Date(date))) {
                            cout << event << endl;
                        }
                    } else if (command_name == "Del") {
                        if (command.empty()) {
                            int cnt = db.DeleteDate(Date(date));
                            cout << "Deleted " << cnt << " events" << endl;
                        } else {
                            if (db.DeleteEvent(Date(date), command)) {
                                cout << "Deleted successfully" << endl;
                            } else {
                                cout << "Event not found" << endl;
                            }
                        }
                    } else {
                        cout << "Unknown command: " << command_name << endl;
                        return 0;
                    }
                }
            }
        }
    }
    catch (WrongDateFormatException& ex) {
        cout << "Wrong date format: " << ex.wrongDate << endl;
        return 0;
    }
    catch (WrongMonthException& ex) {
        cout << "Month value is invalid: " << ex.wrongMonth << endl;
        return 0;
    }
    catch (WrongDayException& ex) {
        cout << "Day value is invalid: " << ex.wrongDay << endl;
        return 0;
    }

    return 0;
}