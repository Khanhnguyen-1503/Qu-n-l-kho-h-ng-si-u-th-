// Validate.h - Nhap lieu co kiem tra (sai kieu, sai mien) cho menu console
// Phu trach: Huynh Tin
// Moi ham lap lai cho den khi nguoi dung nhap dung.
#pragma once
#include <string>
#include "Types.h"

int         nhapSoNguyen(const std::string& loiNhac, int min, int max);
double      nhapSoThuc(const std::string& loiNhac, double min, double max);
std::string nhapChuoi(const std::string& loiNhac, bool choPhepRong);
Date        nhapNgay(const std::string& loiNhac);              // dd/mm/yyyy
std::string nhapMaVach(const std::string& loiNhac);            // 8 hoac 13 chu so

bool laMaVachHopLe(const std::string& s);
bool laSoNguyen(const std::string& s);
std::string catKhoangTrang(const std::string& s);              // trim
std::string chuThuong(const std::string& s);                   // dung cho tim theo ten
