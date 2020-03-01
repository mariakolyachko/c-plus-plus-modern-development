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

    friend istream& operator>>(istream& stream, Rational &rational);
    friend ostream& operator<<(ostream& stream, const Rational &rational);
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

istream& operator>>(istream& stream, Rational &rational) {
    stream >> rational.numerator;
    stream.ignore(1);
    stream >> rational.denominator;
    rational.Normalize();
    return stream;
}

ostream& operator<<(ostream& stream, const Rational &rational) {
    stream << rational.numerator << '/' << rational.denominator;
    return stream;
}

int main() {
    {
        const Rational r(3, 10);
        if (r.Numerator() != 3 || r.Denominator() != 10) {
            cout << "Rational(3, 10) != 3/10" << endl;
            return 1;
        }
    }

    {
        const Rational r(8, 12);
        if (r.Numerator() != 2 || r.Denominator() != 3) {
            cout << "Rational(8, 12) != 2/3" << endl;
            return 2;
        }
    }

    {
        const Rational r(-4, 6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(-4, 6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(4, -6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(4, -6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(0, 15);
        if (r.Numerator() != 0 || r.Denominator() != 1) {
            cout << "Rational(0, 15) != 0/1" << endl;
            return 4;
        }
    }

    {
        const Rational defaultConstructed;
        if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
            cout << "Rational() != 0/1" << endl;
            return 5;
        }
    }

    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }


    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    {
        const set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }


    cout << "OK" << endl;
    return 0;
}