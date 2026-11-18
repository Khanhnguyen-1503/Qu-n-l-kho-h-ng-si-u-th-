// Benchmark.h - Do thoi gian va thong ke so nut truy xuat (Chuong 5 bao cao)
// Phu trach: Khanh Nguyen + Huynh Tin
#pragma once
#include <string>

struct KetQuaDo {
    int       n;              // so mat hang
    int       m;              // bac B-Tree
    int       chieuCao;
    double    msXayDung;      // thoi gian chen n mat hang
    double    msTimKiem;      // thoi gian tim soLanTim ma ngau nhien
    double    nutTBMoiLanTim; // so nut truy xuat trung binh
};

// Chen n ma vach ngau nhien vao B-Tree bac m, tim soLanTim lan (dung std::chrono)
KetQuaDo doBTree(int n, int m, int soLanTim, unsigned int seed);

// Chay bang thuc nghiem: n = 10^3, 10^4, 10^5 x m = 3, 5, 10; in bang va ghi CSV
void chayThucNghiem(const std::string& tepCSV);
