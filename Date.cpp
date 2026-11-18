// Date.cpp - Cai dat cac ham xu ly ngay thang khai bao trong Date.h
// Phu trach: Huynh Tin
//
// Y tuong chinh: doi ngay sang "so ngay thu tu" (01/01/0001 = ngay 1, lich Gregory
// ap dung nguoc ve truoc). Khi do cong/tru/so sanh/khoang cach chi la phep tinh
// tren so nguyen, moi ham deu O(1).
// Mien nam hop le: 1..9999.
#include "Date.h"
#include <ctime>

namespace {

const int NAM_MIN = 1;
const int NAM_MAX = 9999;

// So ngay trong 1 chu ky 400, 100, 4 nam cua lich Gregory
const long NGAY_400_NAM = 146097;   // 400*365 + 97 ngay nhuan
const long NGAY_100_NAM = 36524;    // 100*365 + 24
const long NGAY_4_NAM   = 1461;     // 4*365 + 1

// So ngay tu dau nam den truoc ngay 1 cua thang m (nam khong nhuan), m = 1..12
const int NGAY_TRUOC_THANG[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

// So ngay cua tat ca cac nam truoc nam y (tu nam 1 den nam y-1)
long soNgayTruocNam(int y) {
    long n = y - 1;
    return n * 365 + n / 4 - n / 100 + n / 400;
}

// So ngay tu dau nam y den truoc ngay 1 cua thang m
int soNgayTruocThang(int m, int y) {
    return NGAY_TRUOC_THANG[m] + ((m > 2 && laNamNhuan(y)) ? 1 : 0);
}

bool laChuSo(char c) { return c >= '0' && c <= '9'; }

bool laKhoangTrang(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Doc mot so nguyen khong dau tu s[i..], toi thieu minCS va toi da maxCS chu so.
// Thanh cong -> tra true, i tro toi ky tu ngay sau so.
bool docSo(const std::string& s, size_t& i, int minCS, int maxCS, int& out) {
    int soCS = 0;
    int giaTri = 0;
    while (i < s.size() && laChuSo(s[i]) && soCS < maxCS) {
        giaTri = giaTri * 10 + (s[i] - '0');
        ++i;
        ++soCS;
    }
    if (soCS < minCS) return false;
    out = giaTri;
    return true;
}

// Ghi so nguyen khong am x vao chuoi, dem 0 phia truoc cho du doRong chu so
void ghiSo(std::string& s, int x, int doRong) {
    char buf[12];
    int len = 0;
    do {
        buf[len++] = char('0' + x % 10);
        x /= 10;
    } while (x > 0);
    for (int k = len; k < doRong; ++k) s += '0';
    while (len > 0) s += buf[--len];
}

} // namespace

Date makeDate(int d, int m, int y) {
    Date dt;
    dt.d = d;
    dt.m = m;
    dt.y = y;
    return dt;
}

bool laNamNhuan(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int soNgayTrongThang(int m, int y) {
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return laNamNhuan(y) ? 29 : 28;
        default:
            return 0;   // thang sai
    }
}

bool ngayHopLe(const Date& dt) {
    if (dt.y < NAM_MIN || dt.y > NAM_MAX) return false;
    if (dt.m < 1 || dt.m > 12) return false;
    return dt.d >= 1 && dt.d <= soNgayTrongThang(dt.m, dt.y);
}

long toSoNgay(const Date& dt) {
    return soNgayTruocNam(dt.y) + soNgayTruocThang(dt.m, dt.y) + dt.d;
}

Date tuSoNgay(long soNgay) {
    // Tach theo chu ky 400 / 100 / 4 / 1 nam -> O(1)
    long n = soNgay - 1;                       // so ngay ke tu 01/01/0001 (bat dau tu 0)

    long n400 = n / NGAY_400_NAM;  n %= NGAY_400_NAM;
    long n100 = n / NGAY_100_NAM;  n %= NGAY_100_NAM;
    long n4   = n / NGAY_4_NAM;    n %= NGAY_4_NAM;
    long n1   = n / 365;           n %= 365;

    int y = int(n400 * 400 + n100 * 100 + n4 * 4 + n1) + 1;

    // n100 == 4 hoac n1 == 4: roi dung vao ngay 31/12 cua nam nhuan cuoi chu ky
    if (n100 == 4 || n1 == 4) return makeDate(31, 12, y - 1);

    // n = so ngay ke tu 01/01 cua nam y (0..364 hoac 0..365)
    int m = 1;
    while (m < 12 && soNgayTruocThang(m + 1, y) <= n) ++m;   // toi da 11 buoc
    int d = int(n - soNgayTruocThang(m, y)) + 1;
    return makeDate(d, m, y);
}

Date congNgay(const Date& dt, int soNgay) {
    return tuSoNgay(toSoNgay(dt) + soNgay);   // soNgay am = lui ngay
}

int khoangCachNgay(const Date& tu, const Date& den) {
    return int(toSoNgay(den) - toSoNgay(tu));
}

int soSanhNgay(const Date& a, const Date& b) {
    if (a.y != b.y) return a.y < b.y ? -1 : 1;
    if (a.m != b.m) return a.m < b.m ? -1 : 1;
    if (a.d != b.d) return a.d < b.d ? -1 : 1;
    return 0;
}

bool parseDate(const std::string& s, Date& out) {
    // Bo khoang trang dau/cuoi (ke ca '\r' khi doc tep CRLF tren Linux)
    size_t dau = 0, cuoi = s.size();
    while (dau < cuoi && laKhoangTrang(s[dau])) ++dau;
    while (cuoi > dau && laKhoangTrang(s[cuoi - 1])) --cuoi;
    std::string t = s.substr(dau, cuoi - dau);

    // Chap nhan d/m/yyyy hoac dd/mm/yyyy; nam bat buoc du 4 chu so
    size_t i = 0;
    int d, m, y;
    if (!docSo(t, i, 1, 2, d)) return false;
    if (i >= t.size() || t[i] != '/') return false;
    ++i;
    if (!docSo(t, i, 1, 2, m)) return false;
    if (i >= t.size() || t[i] != '/') return false;
    ++i;
    if (!docSo(t, i, 4, 4, y)) return false;
    if (i != t.size()) return false;           // con ky tu thua

    Date dt = makeDate(d, m, y);
    if (!ngayHopLe(dt)) return false;           // vd 31/04, 29/02 nam khong nhuan
    out = dt;
    return true;
}

std::string formatDate(const Date& dt) {
    std::string s;
    s.reserve(10);
    ghiSo(s, dt.d, 2);
    s += '/';
    ghiSo(s, dt.m, 2);
    s += '/';
    ghiSo(s, dt.y, 4);
    return s;
}

Date homNay() {
    std::time_t now = std::time(nullptr);
    std::tm* t = std::localtime(&now);
    if (t == nullptr) return makeDate(1, 1, 1970);   // phong truong hop loi he thong
    return makeDate(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}
