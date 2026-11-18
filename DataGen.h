// DataGen.h - Sinh du lieu mau va du lieu lon ngau nhien
// Phu trach: Huynh Tin
#pragma once
#include <string>

// Sinh mathang.txt, lo.txt, phieu.txt vao thuMuc.
// soMatHang: 30 (mau) hoac 10^4, 10^5 (do thoi gian). Ma vach khong trung.
// Moi mat hang co 1..soLoToiDa lo, HSD ngau nhien quanh ngay hien tai.
bool sinhDuLieu(const std::string& thuMuc, int soMatHang, int soLoToiDa,
                int soPhieuXuat, unsigned int seed);
