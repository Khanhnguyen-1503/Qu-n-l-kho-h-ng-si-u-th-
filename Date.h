// Date.h - Xu ly ngay thang
// Phu trach: Huynh Tin
#pragma once
#include <string>
#include "Types.h"

Date makeDate(int d, int m, int y);
bool laNamNhuan(int y);
int  soNgayTrongThang(int m, int y);
bool ngayHopLe(const Date& dt);

// So ngay ke tu 01/01/0001 -> dung de cong/tru/so sanh. O(1)
long toSoNgay(const Date& dt);
Date tuSoNgay(long soNgay);

Date congNgay(const Date& dt, int soNgay);
int  khoangCachNgay(const Date& tu, const Date& den);   // den - tu
int  soSanhNgay(const Date& a, const Date& b);           // <0, 0, >0

// "dd/mm/yyyy" <-> Date. parseDate tra ve false neu sai dinh dang hoac ngay khong hop le
bool        parseDate(const std::string& s, Date& out);
std::string formatDate(const Date& dt);

Date homNay();   // lay ngay he thong (dung <ctime>)
