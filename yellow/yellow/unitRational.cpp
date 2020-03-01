#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

int GCD(int a, int b) {
    while (a && b) {
        if (a > b) {
            int temp = a;
            a = b;
            b = temp;
        }
        b %= a;
    }
    return a + b;
}

int LCM(int a, int b) {
    return a / GCD(a, b) * b;
}

class Rational {
public:
    Rational() {
        numerator = 0;
        denominator = 1;
    }

    Rational(int numerator, int denominator) {
        if (denominator == 0) {
            throw invalid_argument("");
        }

        this->numerator = numerator;
        this->denominator = denominator;
        Normalize();
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

    friend istream &operator>>(istream &stream, Rational &rational);

    friend ostream &operator<<(ostream &stream, const Rational &rational);

private:
    void Normalize() {
        if (numerator == 0) {
            denominator = 1;
        } else {
            NormalizeSign();
            Reduce();
        }
    }

    void Reduce() {
        int gcd = GCD(abs(numerator), denominator);
        numerator /= gcd;
        denominator /= gcd;
    }

    void NormalizeSign() {
        int sign = 1;
        if (numerator < 0) {
            sign *= -1;
        }
        if (denominator < 0) {
            sign *= -1;
        }
        numerator = sign * abs(numerator);
        denominator = abs(denominator);
    }


private:
    int numerator;
    int denominator;
};

Rational operator+(const Rational &op1,
                   const Rational &op2) {
    int common_dominator = LCM(op1.Denominator(), op2.Denominator());
    return {op1.Numerator() * (common_dominator / op1.Denominator())
            + op2.Numerator() * (common_dominator / op2.Denominator()),
            common_dominator};
}

Rational operator-(const Rational &op1,
                   const Rational &op2) {
    int common_dominator = LCM(op1.Denominator(), op2.Denominator());
    return {op1.Numerator() * (common_dominator / op1.Denominator())
            - op2.Numerator() * (common_dominator / op2.Denominator()),
            common_dominator};
}

bool operator==(const Rational &op1,
                const Rational &op2) {
    return op1.Numerator() == op2.Numerator()
           && op1.Denominator() == op2.Denominator();
}

bool operator!=(const Rational &op1,
                const Rational &op2) {
    return !(op1 == op2);
}

Rational operator*(const Rational &op1,
                   const Rational &op2) {
    return {op1.Numerator() * op2.Numerator(),
            op1.Denominator() * op2.Denominator()};
}

Rational operator/(const Rational &op1,
                   const Rational &op2) {
    if (op2 == Rational(0, 1)) {
        throw domain_error("");
    }

    return {op1.Numerator() * op2.Denominator(),
            op2.Numerator() * op1.Denominator()};
}

bool operator<(const Rational &op1,
               const Rational &op2) {
    return (op1 - op2).Numerator() < 0;
}

istream &operator>>(istream &stream, Rational &rational) {
    stream >> rational.numerator;
    stream.ignore(1);
    stream >> rational.denominator;
    rational.Normalize();
    return stream;
}

ostream &operator<<(ostream &stream, const Rational &rational) {
    stream << rational.numerator << '/' << rational.denominator;
    return stream;
}

template<typename First, typename Second>
ostream &operator<<(ostream &os, std::pair<First, Second> pr) {
    os << '<' << pr.first << ", " << pr.second << '>';
    return os;
}

template<typename T, typename U>
void AssertEqual(const T &t, const U &u, const std::string &hint) {
    if (t != u) {
        ostringstream os;
        os << "Assertation failed: " << t << " != "
           << u << "Hint: " << hint;
        throw runtime_error(os.str());
    }
}

class TestRunner {

public:
    ~TestRunner() {
        if (fail_count != 0) {
            std::cerr << fail_count << " tests failed. Terminate";
            exit(1);
        }
    }

    template<typename TestFunc>
    void RunTest(TestFunc func, const string &test_name) {
        try {
            func();
            std::cerr << test_name << " OK" << endl;
        }
        catch (runtime_error &ex) {
            ++fail_count;
            std::cerr << test_name << "fail:" << ex.what() << endl;
        }
    }

private:
    int fail_count = 0;
};

void TestCreation() {
    {
        Rational r(3, 10);
        AssertEqual(make_pair(r.Numerator(), r.Denominator()),
                    make_pair(3, 10),
                    "TestCreation: Rational(3, 10) != 3/10");
    }

    {
        Rational r(8, 12);
        AssertEqual(make_pair(r.Numerator(), r.Denominator()),
                    make_pair(2, 3),
                    "TestCreation: Rational(8, 12) != 2/3");
    }

    {
        Rational r(-4, 6);
        AssertEqual(make_pair(r.Numerator(), r.Denominator()),
                    make_pair(-2, 3),
                    "TestCreation: Rational(-4, 6) != -2/3");
    }

    {
        Rational r(4, -6);
        AssertEqual(make_pair(r.Numerator(), r.Denominator()),
                    make_pair(-2, 3),
                    "TestCreation: Rational(4, -6) != -2/3");
    }

    {
        Rational r(0, 15);
        AssertEqual(make_pair(r.Numerator(), r.Denominator()),
                    make_pair(0, 1),
                    "TestCreation: Rational(0, 15) != 0/1");
    }

    {
        Rational defaultConstructed;
        AssertEqual(make_pair(defaultConstructed.Numerator(),
                              defaultConstructed.Denominator()),
                    make_pair(0, 1),
                    "TestCreation: default constructed");
    }
}

void TestOperations() {
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        AssertEqual(r1, r2,
                    "TestOperations: 4/6 != 2/3");
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        AssertEqual(c, Rational(2, 1),
                    "TestOperations: 2/3 + 4/3 != 2");
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        AssertEqual(c, Rational(31, 63),
                    "TestOperations: 5/7 - 2/9 != 31/63");
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        AssertEqual(c, Rational(8, 9),
                    "TestOperations: 2/3 * 4/3 != 8/9");
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        AssertEqual(c, Rational(2, 3),
                    "TestOperations: 5/4 / 15/8 != 2/3");
    }
}


void TestIO() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        AssertEqual(output.str(), "-3/4",
                    "TestIO : Rational(-6, 8) should be written as \"-3/4\"");
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        ostringstream os;
        os << "TestIO: 5/7 is incorrectly read as " << r;
        AssertEqual(r,
                    Rational(5, 7),
                    os.str());
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        ostringstream os;
        os << "TestIO: Multiple values are read incorrectly: "
           << r1 << " " << r2;

        AssertEqual(r1,
                    Rational(5, 7),
                    os.str());
        AssertEqual(r2,
                    Rational(5, 4),
                    os.str());

        input >> r1;
        input >> r2;
        os.clear();
        os << "TestIO: Read from empty stream shouldn't change arguments: "
           << r1 << " " << r2;
        AssertEqual(r1,
                    Rational(5, 7),
                    os.str());
        AssertEqual(r2,
                    Rational(5, 4),
                    os.str());
    }

}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestCreation, "TestCreation");
    tr.RunTest(TestOperations, "TestOperations");
    tr.RunTest(TestIO, "TestIO");
}

int main() {
    TestAll();
//
//    {
//        const set<Rational> rs = {{1, 2},
//                                  {1, 25},
//                                  {3, 4},
//                                  {3, 4},
//                                  {1, 2}};
//        if (rs.size() != 3) {
//            cout << "Wrong amount of items in the set" << endl;
//            return 1;
//        }
//
//        vector<Rational> v;
//        for (auto x : rs) {
//            v.push_back(x);
//        }
//        if (v != vector<Rational>{{1, 25},
//                                  {1, 2},
//                                  {3, 4}}) {
//            cout << "Rationals comparison works incorrectly" << endl;
//            return 2;
//        }
//    }
//
//    {
//        map<Rational, int> count;
//        ++count[{1, 2}];
//        ++count[{1, 2}];
//
//        ++count[{2, 3}];
//
//        if (count.size() != 2) {
//            cout << "Wrong amount of items in the map" << endl;
//            return 3;
//        }
//    }
//
//    try {
//        Rational r(1, 0);
//        cout << "Doesn't throw in case of zero denominator" << endl;
//        return 1;
//    } catch (invalid_argument &) {
//    }
//
//    try {
//        auto x = Rational(1, 2) / Rational(0, 1);
//        cout << "Doesn't throw in case of division by zero" << endl;
//        return 2;
//    } catch (domain_error &) {
//    }
//
//
//    cout << "OK" << endl;
    return 0;
}