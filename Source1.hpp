#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

class BigInt {
public:
	bool neg, strPriority;
	long long int n;
	string s;

	BigInt() {
		n = 0;
		s = '0';
	}


	BigInt(long long int x) {
		n = x;

		if (x < 0) neg = true;
		else neg = false;

		s = to_string(x);
		strPriority = false;
	}


	BigInt(std::string str) {
		if (str[0] == '-') {
			std::reverse(str.begin(), str.end());
			str.pop_back();
			std::reverse(str.begin(), str.end());
			neg = true;
			s = str;
		}
		else {
			s = str;
			neg = false;
		}

		try {
			for (int i = 0; i < str.length(); i++) {
				if (isdigit(str[i]) == 0) throw exception();
			}
			strPriority = true;
		}
		catch (exception&) {
			n = 0;
			s = '0';
		}
	} // бросать исключение std::invalid_argument при ошибке
	BigInt(const BigInt& a) {
		n = a.n;
		s = a.s;
		neg = a.neg;
	}


	~BigInt() {
	}

	string deleter(string s) {
		int i = s.length();

		std::reverse(s.begin(), s.end());

		while (s[i - 1] == '0') {
			s.pop_back();
			i--;
		}

		std::reverse(s.begin(), s.end());

		return s;
	}

	BigInt& operator=(const BigInt& rh) {
		this->n = rh.n;
		this->s = rh.s;
		this->neg = rh.neg;

		return *this;
	}  //возможно присваивание самому себе!


	BigInt operator~() const {
		BigInt a = *this, b("2"), c("0");
		string bitS = "";

		BigInt temp = a;

		while (temp > c) {
			a = temp;
			a %= b;
			bitS += a.s;
			temp /= b;
		}

		std::reverse(bitS.begin(), bitS.end());

		for (int i = 0; i < bitS.length(); i++) {
			if (bitS[i] == '0') bitS[i] = '1';
			else bitS[i] = '0';
		}

		BigInt ans;

		for (int i = 0; i < bitS.length(); i++) {
			if (bitS[i] == '1') {
				BigInt d("2");

				for (int j = (bitS.length() - 1) - i; j > 0; j--) d *= 2;

				ans += d;
			}

		}

		if (this->neg) ans.neg = false;
		else ans.neg = true;

		return ans;
	}


	BigInt& operator++() {
		return (*this += 1);
	}
	const BigInt operator++(int) const;
	BigInt& operator--() {
		return (*this -= 1);
	}
	const BigInt operator--(int) const;

	BigInt& operator+=(const BigInt& rh) {
		string s1 = this->s, s2 = rh.s, s3 = "";
		bool inc = false;

		if (rh.neg && this->neg) {
			BigInt a = *this, b = rh;

			a = -a, b = -b;

			a += b;

			*this = a;
			this->neg = true;

			return *this;
		}
		if (rh.neg) {
			BigInt a = *this, b = rh;
			b = -b;

			a -= b;

			*this = a;
			return a;
		}
		if (this->neg) {
			BigInt a = rh, b = *this;
			b = -b;

			a -= b;

			*this = a;

			return *this;
		}

		this->n += rh.n;

		std::reverse(s1.begin(), s1.end());
		std::reverse(s2.begin(), s2.end());

		if (s1.length() > s2.length()) {
			while (s2.length() != s1.length()) s2 += "0";
		}
		else {
			while (s1.length() != s2.length()) s1 += "0";
		}

		for (int i = 0; i < s1.length(); i++) {
			int n1 = s1[i] - '0', n2 = s2[i] - '0', n3;
			
			if (inc) n3 = n1 + n2 + 1;
			else n3 = n1 + n2;

			if (n3 < 10) {
				s3 += to_string(n3);
				inc = false;
			}
			else {
				n3 -= 10;
				s3 += to_string(n3);
				inc = true;
			}
		}

		if (inc) s3 += '1';

		std::reverse(s3.begin(), s3.end());

		this->s = s3;

		return *this;
	}


	BigInt& operator*=(const BigInt& rh) {
		string s1 = this->s, s2 = rh.s;
		string* s3 = new string[s1.length()];
		int inc = 0, max = 0;

		this->n *= rh.n;

		if (this->s == "0" || rh.s == "0") {
			this->s = "0";
			return *this;
		}

		for (int i = 1; i < s1.length(); i++) {
			for (int j = 0; j < i; j++) s3[i] += '0';
		}

		std::reverse(s1.begin(), s1.end());
		std::reverse(s2.begin(), s2.end());

		for (int i = 0; i < s1.length(); i++) {
			int ni = s1[i] - '0';
			for (int j = 0; j < s2.length(); j++) {
				int nj = s2[j] - '0';

				int nk = ni * nj + inc;
				inc = 0;

				if (j != s2.length() - 1) {
					while (nk >= 10) {
						inc++;
						nk -= 10;
					}

					s3[i] += to_string(nk);
				}
				else {
					string temp = to_string(nk);
					std::reverse(temp.begin(), temp.end());

					s3[i] += temp;
				}
			}
		}

		for (int i = 0; i < s1.length(); i++) {
			if (s3[i].length() > s3[max].length()) max = i;
		}
		for (int i = 0; i < s1.length(); i++) {
			for (int j = s3[i].length(); j < s3[max].length(); j++) s3[i] += '0';
		}

		string ans = s3[0];

		for (int i = 0; i < s1.end() - s1.begin() - 1; i++) {
			string temp = ans;
			ans = "";
			for (int j = 0; j < s3[i].length(); j++) {
				int n1 = temp[j] - '0', n2 = s3[i + 1][j] - '0', n3;

				if (inc) n3 = n1 + n2 + 1;
				else n3 = n1 + n2;

				if (n3 < 10) {
					ans += to_string(n3);
					inc = false;
				}
				else {
					n3 -= 10;
					ans += to_string(n3);
					inc = true;
				}
			}
		}

		std::reverse(ans.begin(), ans.end());
		this->s = ans;

		delete[] s3;

		if (rh.neg) this->neg = !this->neg;

		return *this;
	}


	BigInt& operator-=(const BigInt& rh) {
		string s1 = this->s, s2 = rh.s, s3 = "";
		bool dec = false;

		if (rh.neg) {
			BigInt a = rh;
			a = -a;

			*this += a;

			return *this;
		}
		if (this->neg) {
			BigInt a = *this;
			a = -a;

			a += rh;

			*this = a;
			return *this;
		}

		this->n -= rh.n;

		if (*this < rh) {
			std::reverse(s1.begin(), s1.end());
			std::reverse(s2.begin(), s2.end());

			if (s2.length() > s1.length()) {
				while (s1.length() != s2.length()) s1 += "0";
			}

			for (int i = 0; i < s2.length(); i++) {
				int n1 = s1[i] - '0', n2 = s2[i] - '0', n3;

				if (dec) n3 = (n2 - 1) - n1;
				else n3 = n2 - n1;

				if (n3 >= 0) {
					s3 += to_string(n3);
					dec = false;
				}
				else {
					n3 += 10;
					s3 += to_string(n3);
					dec = true;
				}
			}

			while (s3[s3.length() - 1] == '0') s3.pop_back();

			std::reverse(s3.begin(), s3.end());

			this->s = s3;
			this->neg = true;

			return *this;
		}
		else {
			std::reverse(s1.begin(), s1.end());
			std::reverse(s2.begin(), s2.end());

			if (s1.length() > s2.length()) {
				while (s2.length() != s1.length()) s2 += "0";
			}

			for (int i = 0; i < s1.length(); i++) {
				int n1 = s1[i] - '0', n2 = s2[i] - '0', n3;

				if (dec) n3 = (n1 - 1) - n2;
				else n3 = n1 - n2;

				if (n3 >= 0) {
					s3 += to_string(n3);
					dec = false;
				}
				else {
					n3 += 10;
					s3 += to_string(n3);
					dec = true;
				}
			}

			while (s3[s3.length() - 1] == '0' && s3.length() > 1) s3.pop_back();

			std::reverse(s3.begin(), s3.end());

			this->s = s3;

			return *this;
		}
	}


	/*BigInt& operator/=(const BigInt& rh) {
		this->n /= rh.n;

		BigInt a(rh.s), c(0);
		string s = this->s, temp = "", ans = "";
		bool rest = true;
		int i = 0, count = 0, ni, nj;

		if (*this < rh || rh.s == "0") {
			this->s = "0";
			return *this;
		}
		if (rh.s == "1") return *this;

		if (s.length() != a.s.length()) {
			while (i < s.length() && temp != "0") {
				if (i == 0) {
					while (temp.length() < a.s.length()) {
						temp += s[i];
						i++;
					}
				}
				else temp += s[i], i++;

				BigInt b(temp);

				while (b >= a) {
					b -= a;
					count++;
				}

				if (count == 0) {
					if (b.s.length() == s.length()) {
						if (i < s.length() && rest) {
							temp += s[i];
							i++;
						}
						else {
							ans += '0';
							BigInt check(ans);

							if (check > *this) ans.pop_back(), i++;

							if (i < s.length()) {
								rest = true, temp += s[i], i++;
							}
						}
					}
					else {
						ans += '0';
						if (temp == "0") {
							temp = "";
							temp += s[i];
							i++;
						}
					}
				}
				else {
					ans += to_string(count);
					if (i < s.length() && b.s == "0") count = 0, temp = "", rest = false;
					else count = 0, temp = b.s, rest = true;
				}
			}
		}
		else {
			while (temp.length() < a.s.length()) {
				temp += s[i];
				i++;
			}

			BigInt b(temp);

			while (b >= a) {
				b -= a;
				count++;
			}

			ans += to_string(count);
			count = 0, temp = b.s;
		}

		ans = deleter(ans);

		this->s = ans;
		if (rh.neg) this->neg = !this->neg;

		return *this;
	}*/

	/*BigInt& operator/=(const BigInt& rh) {
		if (*this < BigInt(0) && rh < BigInt(0)) {
			BigInt a = *this, b = rh;

			a.neg = false, b.neg = false;

			if (a < b) {
				this->s = "0";
				this->neg = false;
				return *this;
			}
			if (a > b) {
				int count = 0;

				while (a > b) {
					a -= b;
					count++;
				}

				this->s = to_string(count);
				this->neg = false;

				return *this;
			}

			this->s = "1";
			this->neg = false;

			return *this;
		}

		if (*this < BigInt(0)) {
			BigInt a = *this, b = rh;

			a = -a;

			if (a <= b) {
				this->s = "1";
				return *this;
			}
			else {
				int count = 0;

				while (a > b) {
					a -= b;
					count++;
				}

				this->s = to_string(count);
				return *this;
			}
		}

		if (rh < BigInt(0)) {
			BigInt a = *this, b = rh;

			b = -b;

			if (a <= b) {
				this->s = "1";
				this->neg = true;
				return *this;
			}
			else {
				int count = 0;

				while (a > b) {
					a -= b;
					count++;
				}

				this->s = to_string(count);
				this->neg = true;
				return *this;
			}
		}

		BigInt a = *this, b = rh;
		int count = 0;

		if (a < b) {
			this->s = "0";
			return *this;
		}

		while (a > b) {
			a -= b;
			count++;
		}

		this->s = to_string(count);
	}*/

	BigInt& operator/=(const BigInt& rh) {
		string s, temp = "", ans = "";
		this->n /= rh.n;
		int i = 0;

		if (rh == BigInt(1)) return *this;
		if (rh == BigInt(-1)) {
			*this = -*this;

			return *this;
		}
		if (this->s.length() < rh.s.length()) {
			this->s = "0";

			if (this->neg && rh.neg) {
				this->neg = false;
				return *this;
			}
			if (this->neg || rh.neg) {
				this->s = "1";
				this->neg = true;
				return *this;
			}

			return *this;
		}

		while (i < this->s.length()) {
			BigInt b(rh);
			b.neg = false;
			int count = 0;

			if (i == 0) {
				s += this->s[i++];
				while (BigInt(s) < b) s += this->s[i++];
			}
			else {
				bool takes = false; //сколько раз был взят разряд из деимого

				while (BigInt(s) < b && i < this->s.length()) {
					if (!takes) {
						s += this->s[i++];
						takes = true;
					}
					else {
						s += this->s[i++];
						ans += '0';
					}
				}

				if (BigInt(s) < rh) break;
			}

			BigInt a(s);

			while (a >= b) {
				a -= b;
				count++;
			}

			if (a.s == "0") {
				ans += to_string(count);
				s = "";
			}
			else {
				ans += to_string(count);
				s = a.s;
			}
		}

		this->s = ans;

		if (this->neg && rh.neg) {
			this->neg = false;
			return *this;
		}
		if (this->neg || rh.neg) {
			this->neg = true;
			return *this;
		}

		return *this;
	}

	BigInt& operator^=(const BigInt& rh) {
		BigInt a = *this, b = rh, c("2"), d("0");
		string bitS1 = "", bitS2 = "";
		bool nega = false, negb = false;

		if (this->neg) {
			nega = true;
			a.neg = false;
		}
		if (rh.neg) {
			negb = true;
			b.neg = false;
		}

		BigInt temp = a;

		while (temp > d) {
			a = temp;
			a %= c;
			bitS1 += a.s;
			temp /= c;
		}

		temp = b;

		while (temp > d) {
			b = temp;
			b %= c;
			bitS2 += b.s;
			temp /= c;
		}

		if (bitS1.length() > bitS2.length()) {
			while (bitS2.length() < bitS1.length()) bitS2 += '0';
		}
		if (bitS2.length() > bitS1.length()) {
			while (bitS1.length() < bitS2.length()) bitS1 += '0';
		}

		std::reverse(bitS1.begin(), bitS1.end());
		std::reverse(bitS2.begin(), bitS2.end());

		string bitRes = "";

		for (int i = 0; i < bitS1.length(); i++) {
			if (bitS1[i] == '1' && bitS2[i] == '1') {
				bitRes += '0';
				continue;
			}
			if (bitS1[i] == '0' && bitS2[i] == '0') bitRes += '1';
			else bitRes += '1';
		}

		BigInt ans;

		for (int i = 0; i < bitRes.length(); i++) {
			if (bitRes[i] == '1') {
				BigInt e("2");

				for (int j = (bitRes.length() - 1) - i; j > 0; j--) e *= 2;

				ans += e;
			}

		}

		*this = ans;

		if (nega || negb) this->neg = true;

		return *this;
	}


	BigInt& operator%=(const BigInt& rh) {
		BigInt a = *this, b = rh;
		a.neg = false, b.neg = false;

		a /= b;
		a *= b;

		this->neg = false;
		*this -= a;

		return *this;
	}


	BigInt& operator&=(const BigInt& rh) {
		BigInt a = *this, b = rh, c("2"), d("0");
		string bitS1 = "", bitS2 = "";
		bool nega = false, negb = false;

		if (this->neg) {
			nega = true;
			a.neg = false;
		}
		if (rh.neg) {
			negb = true;
			b.neg = false;
		}

		BigInt temp = a;

		while (temp > d) {
			a = temp;
			a %= c;
			bitS1 += a.s;
			temp /= c;
		}

		temp = b;

		while (temp > d) {
			b = temp;
			b %= c;
			bitS2 += b.s;
			temp /= c;
		}

		if (bitS1.length() > bitS2.length()) {
			while (bitS2.length() < bitS1.length()) bitS2 += '0';
		}
		if(bitS2.length() > bitS1.length()) {
			while (bitS1.length() < bitS2.length()) bitS1 += '0';
		}

		std::reverse(bitS1.begin(), bitS1.end());
		std::reverse(bitS2.begin(), bitS2.end());

		string bitRes = "";

		for (int i = 0; i < bitS1.length(); i++) {
			if (bitS1[i] == '0' || bitS2[i] == '0') bitRes += '0';
			else bitRes += '1';
		}

		BigInt ans;

		for (int i = 0; i < bitRes.length(); i++) {
			if (bitRes[i] == '1') {
				BigInt e("2");

				for (int j = (bitRes.length() - 1) - i; j > 0; j--) e *= 2;

				ans += e;
			}

		}

		*this = ans;

		if (nega || negb) this->neg = true;

		return *this;
	}


	BigInt& operator|=(const BigInt& rh) {
		BigInt a = *this, b = rh, c("2"), d("0");
		string bitS1 = "", bitS2 = "";
		bool nega = false, negb = false;

		if (this->neg) {
			nega = true;
			a.neg = false;
		}
		if (rh.neg) {
			negb = true;
			b.neg = false;
		}

		BigInt temp = a;
		
		while (temp > d) {
			a = temp;
			a %= c;
			bitS1 += a.s;
			temp /= c;
		}

		temp = b;

		while (temp > d) {
			b = temp;
			b %= c;
			bitS2 += b.s;
			temp /= c;
		}

		if (bitS1.length() > bitS2.length()) {
			while (bitS2.length() < bitS1.length()) bitS2 += '0';
		}
		if (bitS2.length() > bitS1.length()) {
			while (bitS1.length() < bitS2.length()) bitS1 += '0';
		}

		std::reverse(bitS1.begin(), bitS1.end());
		std::reverse(bitS2.begin(), bitS2.end());

		string bitRes = "";

		for (int i = 0; i < bitS1.length(); i++) {
			if (bitS1[i] == '1' || bitS2[i] == '1') bitRes += '1';
			else bitRes += '0';
		}

		BigInt ans;

		for (int i = 0; i < bitRes.length(); i++) {
			if (bitRes[i] == '1') {
				BigInt e("2");

				for (int j = (bitRes.length() - 1) - i; j > 0; j--) e *= 2;

				ans += e;
			}

		}

		*this = ans;

		if (nega || negb) this->neg = true;

		return *this;
	}

	BigInt operator+() const {
		return *this;
	} // unary +


	BigInt operator-() const {
		BigInt a = *this;

		a.n = -a.n;
		a.neg = !a.neg;

		return a;
	}  // unary -

	bool operator==(const BigInt& rh) const {
		if ((!this->neg && rh.neg) || (this->neg && !rh.neg)) return false;
		return (this->s.compare(rh.s) == 0);
	}
	bool operator!=(const BigInt& rh) const {
		return (this->s.compare(rh.s) != 0);
	}
	bool operator<(const BigInt& rh) const {
		if (this->s.length() == rh.s.length()) {
			if (this->neg && !rh.neg) return true;
			if (!this->neg && rh.neg) return false;
			else {
				if (!this->neg) {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return false;
						if (n1 < n2) return true;
					}
					return false;
				}
				else {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return true;
						if (n1 < n2) return false;
					}
					return false;
				}
			}
		}
		else {
			if (this->s.length() > rh.s.length()) {
				if (this->neg) return true;
				else return false;
			}
			else {
				if (this->neg) return false;
				else return true;
			}
		}
	}
	bool operator>(const BigInt& rh) const {
		if (this->s.length() == rh.s.length()) {
			if (this->neg && !rh.neg) return false;
			if (!this->neg && rh.neg) return true;
			else {
				if (!this->neg) {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return true;
						if (n1 < n2) return false;
					}
					return false;
				}
				else {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 < n2) return true;
						if (n1 > n2) return false;
					}
					return false;
				}
			}
		}
		else {
			if (this->s.length() > rh.s.length()) {
				if (this->neg) return false;
				else return true;
			}
			else {
				if (rh.neg) return true;
				else return false;
			}
		}
	}
	bool operator<=(const BigInt& rh) const {
		if (this->s.length() == rh.s.length()) {
			if (this->neg && !rh.neg) return true;
			if (!this->neg && rh.neg) return false;
			else {
				if (!this->neg) {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return false;
						if (n1 < n2) return true;
					}
					return true;
				}
				else {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return true;
						if (n1 < n2) return false;
					}
					return true;
				}
			}
		}
		else {
			if (this->s.length() > rh.s.length()) {
				if (this->neg) return true;
				else return false;
			}
			else {
				if (this->neg) return false;
				else return true;
			}
		}
	}
	bool operator>=(const BigInt& rh) const {
		if (this->s.length() == rh.s.length()) {
			if (this->neg && !rh.neg) return false;
			if (!this->neg && rh.neg) return true;
			else {
				if (!this->neg) {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 > n2) return true;
						if (n1 < n2) return false;
					}
					return true;
				}
				else {
					for (int i = 0; i < this->s.length(); i++) {
						string a = "", b = "";
						int n1, n2;

						a += this->s[i], b += rh.s[i];

						n1 = stoi(a), n2 = stoi(b);

						if (n1 < n2) return true;
						if (n1 > n2) return false;
					}
					return true;
				}
			}
		}
		else {
			if (this->s.length() > rh.s.length()) {
				if (this->neg) return false;
				else return true;
			}
			else {
				if (rh.neg) return true;
				else return false;
			}
		}
	}

	operator int() const;
	operator std::string() const;

	size_t size() const;  // size in bytes
};

BigInt operator+(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c += d;

	return c;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c -= d;

	return c;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c *= d;

	return c;
}

BigInt operator/(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	try {
		if (b.n == 0) throw exception();
		else c /= d;
	}
	catch (exception&) {
		cout << "Знаменатель равен нулю ";
		c.n = 0;
		c.s = "0";
	}

	return c;
}

BigInt operator^(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c ^= d;

	return c;
}

BigInt operator%(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c %= d;

	return c;
}

BigInt operator&(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c &= d;

	return c;
}

BigInt operator|(const BigInt& a, const BigInt& b) {
	BigInt c = a, d = b;

	c |= d;

	return c;
}

std::ostream& operator<<(std::ostream& o, const BigInt& i) {
	if (!i.neg) return o << i.s;
	else return o << '-' << i.s;
}