#include <iostream>
#include <map>

using namespace std;

class Person {
public:
    Person(const string &first_name,
           const string &last_name,
           int year) {
        firstName[year] = first_name;
        lastName[year] = last_name;
    }

    void ChangeFirstName(int year, const string &first_name) {
        if (year >= firstName.begin()->first)
            this->firstName[year] = first_name;
    }

    void ChangeLastName(int year, const string &last_name) {
        if (year >= lastName.begin()->first)
            this->lastName[year] = last_name;
    }

    string GetFullName(int year) const {
        string name;
        if (year < firstName.begin()->first) {
            name = "No person";
        } else {
            string first_name = getName(year, firstName);
            string last_name = getName(year, lastName);
            if (first_name == "unknown" && last_name == "unknown") {
                name = "Incognito";
            } else if (first_name == "unknown") {
                name = last_name + " with unknown first name";
            } else if (last_name == "unknown") {
                name = first_name + " with unknown last name";
            } else {
                name = first_name + ' ' + last_name;
            }
        }
        return name;
    }

    string GetFullNameWithHistory(int year) const {
        string name;
        if (year < firstName.begin()->first) {
            name = "No person";
        } else {
            string first_name = getName(year, firstName)
                                + getNameHistory(year, firstName);
            string last_name = getName(year, lastName)
                               + getNameHistory(year, lastName);
            if (first_name == "unknown" && last_name == "unknown") {
                name = "Incognito";
            } else if (first_name == "unknown") {
                name = last_name + " with unknown first name";
            } else if (last_name == "unknown") {
                name = first_name + " with unknown last name";
            } else {
                name = first_name + ' ' + last_name;
            }
        }
        return name;
    }

private:
    string getNameHistory(int year, const map<int, string> &nameHistory) const {
        string history;
        auto iter = nameHistory.lower_bound(year);
        if (iter != nameHistory.begin()) {
            if (iter == nameHistory.end()
                || iter->first > year) {
                --iter;
            }
            string last_change = iter->second;
            while (iter != nameHistory.begin()) {
                if (last_change != (--iter)->second) {
                    if (!history.empty()) {
                        history += ", ";
                    }
                    history += iter->second;
                    last_change = iter->second;
                }
            }
        }
        if (!history.empty()) {
            history = " (" + history + ')';
        }
        return history;
    }

    string getName(int year, const map<int, string> &nameHistory) const {
        string name;
        auto iter = nameHistory.lower_bound(year);
        if (iter == nameHistory.end() || iter->first > year) {
            if (iter == nameHistory.begin()) {
                name = "unknown";
            } else {
                name = (--iter)->second;
            }
        } else {
            name = iter->second;
        }
        return name;
    }


private:
    map<int, string> firstName;
    map<int, string> lastName;
};


int main() {
    Person person("Polina", "Sergeeva", 1960);
    for (int year : {1959, 1960}) {
        cout << person.GetFullNameWithHistory(year) << endl;
    }

    person.ChangeFirstName(1965, "Appolinaria");
    person.ChangeLastName(1967, "Ivanova");
    for (int year : {1965, 1967}) {
        cout << person.GetFullNameWithHistory(year) << endl;
    }

    return 0;
}