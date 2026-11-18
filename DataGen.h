// DataGen.h - Sinh du lieu mau va du lieu lon ngau nhien
// Phu trach: Huynh Tin
#pragma once
#include <string>

// Sinh mathang.txt, lo.txt, phieu.txt vao thuMuc.
// soMatHang: 30 (mau) hoac 10^4, 10^5 (do thoi gian). Ma vach khong trung.
// Moi mat hang co 1..soLoToiDa lo, HSD ngau nhien quanh ngay hien tai.
// TODO (Huynh Tin): chua cai dat - hien dung 2 ham ben duoi.
bool sinhDuLieu(const std::string& thuMuc, int soMatHang, int soLoToiDa,
                int soPhieuXuat, unsigned int seed);

// Da cai dat trong DataGen.cpp
// Bo mau: mathang + lo + phieu, moi mat hang 1..3 lo, ~60% mat hang co 1 phieu xuat. Seed 42.
void sinhDuLieuMau(int soMatHang, const std::string& fileMatHang,
                   const std::string& fileLo, const std::string& filePhieu);
// Bo lon (do thoi gian B-Tree): chi sinh mathang.txt, ma vach hoan vi (i*7919 mod 10^9). Seed 100.
void sinhDuLieuLon(int soMatHang, const std::string& fileMatHang);
