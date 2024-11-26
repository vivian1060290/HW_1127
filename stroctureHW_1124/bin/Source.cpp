#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

class Polynomial {
private:
    map<int, int> terms; // map �� key ��ܦ���Avalue ��ܸӦ��誺�Y��

    // �N�r��ѪR���h����
    void parse(const string& poly) {
        int i = 0;
        while (i < poly.length()) {
            int coef = 0, exp = 0;
            bool hasX = false, isNegative = false;

            // �B�z���t��
            if (poly[i] == '-') {
                isNegative = true;
                i++;
            }
            else if (poly[i] == '+') {
                i++;
            }

            // ���o�Y��
            while (i < poly.length() && isdigit(poly[i])) {
                coef = coef * 10 + (poly[i] - '0');
                i++;
            }
            if (coef == 0) coef = 1; // �Y�Y�Ƭٲ��A�q�{�� 1
            if (isNegative) coef = -coef;

            // �ˬd�O�_�� x
            if (i < poly.length() && poly[i] == 'x') {
                hasX = true;
                i++;
            }

            // �B�z����
            if (hasX && i < poly.length() && poly[i] == '^') {
                i++;
                while (i < poly.length() && isdigit(poly[i])) {
                    exp = exp * 10 + (poly[i] - '0');
                    i++;
                }
            }
            else if (hasX) {
                exp = 1; // �Y�L����Ÿ��A�q�{���謰 1
            }

            // �Y�L x�A���謰 0
            if (!hasX) exp = 0;

            // ��s map
            terms[exp] += coef;
        }
    }

public:
    // �q�{�c�y���
    Polynomial() {}

    // �q�r��c�y�h����
    Polynomial(const string& poly) {
        parse(poly);
    }

    // �[�k
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        for (auto it : other.terms) {
            result.terms[it.first] += it.second;
        }
        return result;
    }

    // ���k
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

    // �N�h�����ର�r��榡
    string toString() const {
        string result = "";
        for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
            int coef = it->second;
            int exp = it->first;

            if (coef == 0) continue; // �����Y�Ƭ� 0 ����

            // �B�z���t��
            if (!result.empty()) {
                if (coef > 0) result += " + ";
                else result += " - ";
            }
            else {
                if (coef < 0) result += "-";
            }

            // �B�z�Y��
            coef = abs(coef);
            if (coef != 1 || exp == 0) result += to_string(coef);

            // �B�z����
            if (exp > 0) result += "x";
            if (exp > 1) result += "^" + to_string(exp);
        }

        return result.empty() ? "0" : result;
    }
};

int main() {
    string poly1, poly2;

    cout << "��J�Ĥ@�Ӧh���� (�Ҧp 3x^2 + 2x - 5): ";
    getline(cin, poly1);
    cout << "��J�ĤG�Ӧh���� (�Ҧp x^2 - 4x + 6): ";
    getline(cin, poly2);

    Polynomial p1(poly1);
    Polynomial p2(poly2);

    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;

    cout << "�[�k���G: " << sum.toString() << endl;
    cout << "���k���G: " << product.toString() << endl;

    return 0;
}
