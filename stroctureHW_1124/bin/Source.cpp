#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

class Polynomial {
private:
    map<int, int> terms;

    void parse(const string& poly) {
        int i = 0;
        while (i < poly.length()) {
            int coef = 0, exp = 0;
            bool hasX = false, isNegative = false;

            if (poly[i] == '-') {
                isNegative = true;
                i++;
            }
            else if (poly[i] == '+') {
                i++;
            }

            while (i < poly.length() && isdigit(poly[i])) {
                coef = coef * 10 + (poly[i] - '0');
                i++;
            }
            if (coef == 0) coef = 1;
            if (isNegative) coef = -coef;

            if (i < poly.length() && poly[i] == 'x') {
                hasX = true;
                i++;
            }

            if (hasX && i < poly.length() && poly[i] == '^') {
                i++;
                while (i < poly.length() && isdigit(poly[i])) {
                    exp = exp * 10 + (poly[i] - '0');
                    i++;
                }
            }
            else if (hasX) {
                exp = 1;
            }

            if (!hasX) exp = 0;

            terms[exp] += coef;
        }
    }

public:
    Polynomial() {}
    Polynomial(const string& poly) { parse(poly); }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        for (auto it : other.terms) {
            result.terms[it.first] += it.second;
        }
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (auto it1 : terms) {
            for (auto it2 : other.terms) {
                int exp = it1.first + it2.first;
                int coef = it1.second * it2.second;
                result.terms[exp] += coef;
            }
        }
        return result;
    }

    string toString() const {
        string result = "";
        for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
            int coef = it->second;
            int exp = it->first;

            if (coef == 0) continue;

            if (!result.empty()) {
                if (coef > 0) result += " + ";
                else result += " - ";
            }
            else {
                if (coef < 0) result += "-";
            }

            coef = abs(coef);
            if (coef != 1 || exp == 0) result += to_string(coef);

            if (exp > 0) result += "x";
            if (exp > 1) result += "^" + to_string(exp);
        }

        return result.empty() ? "0" : result;
    }

    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
};

istream& operator>>(istream& is, Polynomial& p) {
    string poly;
    getline(is, poly);
    p = Polynomial(poly);
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    os << p.toString();
    return os;
}

int main() {
    Polynomial p1, p2;

    cout << "輸入第一個多項式: ";
    cin >> p1;
    cout << "輸入第二個多項式: ";
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;

    cout << "加法結果: " << sum << endl;
    cout << "乘法結果: " << product << endl;

    return 0;
}
