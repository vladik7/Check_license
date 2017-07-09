#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <windows.h>
#include <iphlpapi.h>

using namespace std;
#pragma comment(lib, "iphlpapi.lib")

string mac;
int len;
long long base[10] = { 6162,89120,5310,3683,647,8657,137,11977,7003,10354 };
long long mods[10] = { 593681342,966434045,508107417,955646292,704147723,71383145,658696349,470579678,328427876,917043173 };
long long kf[4] = { 33,96,21,104 };

string get_mac() {
	IP_ADAPTER_INFO ip_ainf[128];
	PIP_ADAPTER_INFO pip_ainf = ip_ainf;
	ULONG bufLen = sizeof(ip_ainf);
	GetAdaptersInfo(ip_ainf, &bufLen);
	vector<string> maci;
	do {
		string s;
		for (int i = 0; i <= 5; i++) {
			int a = pip_ainf->Address[i];
			s += to_string(a);
		}
		maci.push_back(s);
	} while (pip_ainf = pip_ainf->Next);
	sort(maci.begin(), maci.end());
	string ans = "";
	for (int i = 0; i < maci.size(); i++) {
		ans += maci[i];
	}
	return ans;
}

long long get_hash(long long bas, long long mod) {
	vector<long long> pw;
	long long tmp = 1;
	long long hash = 0;
	for (int i = 0; i < len; i++) {
		hash += tmp * (mac[i] - '0');
		hash %= mod;
		tmp *= bas;
		tmp %= mod;
	}
	return hash;
}

long long binpow(long long x, long long y, long long mod) {
	if (y == 1) return x;
	if (y % 2) return (x*binpow((x*x) % mod, y / 2, mod)) % mod;
	return binpow((x*x) % mod, y / 2, mod) % mod;
}

long long encryption(long long x, int type, int mod) {
	long long res;
	if (type == 0) {
		res = ((kf[0] * binpow(x, 3, mod) % mod) ^ (kf[1] * binpow(x, 2, mod) % mod) + 123457689) % mod + kf[3];
	}
	if (type == 1) {
		res = binpow(x, (kf[0] * kf[2]) % kf[1], mod) + kf[1] * binpow(x, kf[3], mod);
	}
	if (type == 2) {
		res = x | 85690 + binpow(kf[3], x + kf[0] * kf[1] * kf[1], mod);
	}
	return res % mod;
}

int main()
{
	freopen("license.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	string license;
	cin >> license;
	mac = get_mac();
	len = mac.length();
	string result = "";
	for (int i = 0; i < 10; i++) {
		long long hash = get_hash(base[i], mods[i]);
		long long encrypt = encryption(hash, i % 3, mods[i]);
		result += to_string(encrypt);
	}
	if (result == license) {
		cout << "YES";
	}
	else {
		cout << "NO";
	}
	return 0;
}