#include <iostream>
#include <string>
#include <vector>

using namespace std;



class Person {
public:
  Person(const string name, const string type): _name(name), _type(type) {}
  string GetName() const {
    return _name;
  }
  string GetType() const {
    return _type;
  }

  virtual void Walk(const string destination) const {
    cout <<  GetType() << " " << GetName() << " walks to: " << destination << endl;
  }
private:
  const string _name;
  const string _type;
};

class Student: public Person {
public:

  Student(const string name, const string favouriteSong): Person(name, "Student"),_favouriteSong(favouriteSong) {};

  string getFavouriteSong() const {
    return _favouriteSong;
  }

  void Learn() const {
    cout << GetType() << ": " << GetName() << " learns" << endl;
  }

  void SingSong() const {
    cout << GetType() << ": " << GetName() << " sings a song: " << getFavouriteSong() << endl;
  }

  void Walk(const string destination) const override {
    Person::Walk(destination);
    SingSong();
  }

private:
  const string _favouriteSong;
};


class Teacher: public Person {
public:

  Teacher(const string name, const string subject): Person(name, "Teacher"), _subject(subject) {}

  void Teach() const {
    cout << "Teacher: " << GetName() << " teaches: " << _subject << endl;
  }

private:
  const string _subject;
};


class Policeman: public Person {
public:
  Policeman(const string name): Person(name, "Policeman") {};

  void Check(const Person& p) const {
    cout << GetType() << ": " << GetName() << " checks " << p.GetType() << ". " << p.GetType() << "'s name is: " << p.GetName() << endl;
  }
};

void VisitPlaces(Person& person, vector<string> places) {
  for (auto p : places) {
    person.Walk(p);
  }
}


int main() {
  Teacher t("Jim", "Math");
  Student s("Ann", "We will rock you");
  Policeman p("Bob");

  VisitPlaces(t, {"Moscow", "London"});
  p.Check(s);
  VisitPlaces(s, {"Moscow", "London"});
  return 0;
}
