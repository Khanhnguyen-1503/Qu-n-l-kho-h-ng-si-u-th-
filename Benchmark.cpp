// Benchmark.cpp - Do thoi gian va so nut truy xuat cua B-Tree (Chuong 5 bao cao)
// Phu trach: Khanh Nguyen + Huynh Tin
//
// Sinh so ngau nhien bang std::mt19937 (lay truc tiep gia tri tho, khong qua distribution)
// -> cung seed cho cung bo du lieu tren moi may / moi trinh bien dich.
#include "Benchmark.h"
#include "BTree.h"
#include "LotQueue.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

namespace {

typedef std::chrono::steady_clock DongHo;

double msTu(DongHo::time_point batDau) {
    return std::chrono::duration<double, std::milli>(DongHo::now() - batDau).count();
}

// Ma vach 13 chu so: "893" + 10 chu so ngau nhien
std::string maVachNgauNhien(std::mt19937& rng) {
    unsigned long long cao = rng();
    unsigned long long thap = rng();
    unsigned long long x = ((cao << 32) | thap) % 10000000000ULL;
    char buf[10];
    for (int i = 9; i >= 0; --i) {
        buf[i] = char('0' + x % 10);
        x /= 10;
    }
    std::string s = "893";
    s.append(buf, 10);
    return s;
}

MatHang* taoMatHangDo(const std::string& maVach) {
    MatHang* mh = new MatHang;
    mh->maVach = maVach;
    mh->ten = "";
    mh->nhom = "";
    mh->donGia = 0;
    mh->tonKho = 0;
    mh->nguongCanhBao = 0;
    mh->demTam = 0;
    lqInit(mh->dsLo);
    return mh;
}

// Chieu cao toi da theo ly thuyet: h <= 1 + log_{ceil(m/2)}((n+1)/2)
double chieuCaoToiDa(int n, int m) {
    double t = (m + 1) / 2;
    return 1.0 + std::log((n + 1) / 2.0) / std::log(t);
}

} // namespace

KetQuaDo doBTree(int n, int m, int soLanTim, unsigned int seed) {
    KetQuaDo kq;
    kq.n = n;
    kq.m = m;
    kq.chieuCao = 0;
    kq.msXayDung = 0;
    kq.msTimKiem = 0;
    kq.nutTBMoiLanTim = 0;
    if (n <= 0 || soLanTim <= 0) return kq;

    std::mt19937 rng(seed);

    // Chuan bi truoc (khong tinh gio): n mat hang va danh sach chi so can tim
    MatHang** ds = new MatHang*[n];
    for (int i = 0; i < n; ++i) ds[i] = taoMatHangDo(maVachNgauNhien(rng));
    int* canTim = new int[soLanTim];
    for (int i = 0; i < soLanTim; ++i) canTim[i] = int(rng() % unsigned(n));

    BTree t;
    btInit(t, m);

    // 1. Xay dung: chen n mat hang. Trung ma (rat hiem, ~n^2/2e10) thi sinh ma khac
    DongHo::time_point batDau = DongHo::now();
    for (int i = 0; i < n; ++i) {
        while (!btInsert(t, ds[i])) ds[i]->maVach = maVachNgauNhien(rng);
    }
    kq.msXayDung = msTu(batDau);
    kq.chieuCao = btHeight(t);

    // 2. Tim kiem soLanTim ma co trong cay, dem so nut truy xuat
    btResetDem(t);
    int timThay = 0;
    batDau = DongHo::now();
    for (int i = 0; i < soLanTim; ++i) {
        if (btSearch(t, ds[canTim[i]]->maVach) != nullptr) ++timThay;
    }
    kq.msTimKiem = msTu(batDau);
    kq.nutTBMoiLanTim = double(t.soNutTruyXuat) / soLanTim;
    if (timThay != soLanTim) std::cerr << "Canh bao: co ma khong tim thay!\n";

    btDestroy(t, true);   // giai phong ca n mat hang
    delete[] ds;
    delete[] canTim;
    return kq;
}

void chayThucNghiem(const std::string& tepCSV) {
    const int DS_N[] = {1000, 10000, 100000};
    const int DS_M[] = {3, 5, 10};
    const int SO_LAN_TIM = 100000;
    const unsigned int SEED = 2026;

    std::ofstream f(tepCSV);
    if (!f) std::cerr << "Khong mo duoc tep " << tepCSV << ", chi in ra man hinh.\n";
    else f << "n,m,chieuCao,chieuCaoToiDaLyThuyet,msXayDung,msTimKiem,usMoiLanTim,nutTBMoiLanTim\n";

    std::cout << "Thuc nghiem B-Tree: tim " << SO_LAN_TIM << " ma ngau nhien co trong cay, seed "
              << SEED << "\n";
    std::cout << std::left << std::setw(9) << "n" << std::setw(5) << "m" << std::right
              << std::setw(6) << "h" << std::setw(9) << "h_max"
              << std::setw(14) << "Xay dung(ms)" << std::setw(13) << "Tim (ms)"
              << std::setw(13) << "us/lan tim" << std::setw(14) << "Nut TB/lan" << "\n";
    std::cout << std::string(83, '-') << "\n";

    for (int n : DS_N) {
        for (int m : DS_M) {
            KetQuaDo kq = doBTree(n, m, SO_LAN_TIM, SEED);
            double hMax = chieuCaoToiDa(n, m);
            double usMoiLan = kq.msTimKiem * 1000.0 / SO_LAN_TIM;

            std::ios trangThai(nullptr);
            trangThai.copyfmt(std::cout);
            std::cout << std::left << std::setw(9) << kq.n << std::setw(5) << kq.m << std::right
                      << std::setw(6) << kq.chieuCao << std::fixed << std::setprecision(2)
                      << std::setw(9) << hMax << std::setw(14) << kq.msXayDung
                      << std::setw(13) << kq.msTimKiem << std::setprecision(3)
                      << std::setw(13) << usMoiLan << std::setprecision(2)
                      << std::setw(14) << kq.nutTBMoiLanTim << "\n";
            std::cout.copyfmt(trangThai);

            if (f) {
                f << kq.n << "," << kq.m << "," << kq.chieuCao << "," << hMax << ","
                  << kq.msXayDung << "," << kq.msTimKiem << "," << usMoiLan << ","
                  << kq.nutTBMoiLanTim << "\n";
            }
        }
    }
    if (f) std::cout << "Da ghi ket qua vao " << tepCSV << "\n";
}
