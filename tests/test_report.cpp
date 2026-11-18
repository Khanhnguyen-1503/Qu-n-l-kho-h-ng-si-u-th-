// test_report.cpp - Kiem thu Sort + Report tren du lieu mau data/*.txt (Long Vu)
// Build (tu thu muc goc repo):
//   g++ -std=c++17 -Wall -Wextra -I. tests/test_report.cpp Sort.cpp Report.cpp Inventory.cpp
//       Phieu.cpp LotQueue.cpp BTree.cpp Date.cpp -o test_report
// Chay:  ./test_report        -> kiem thu tu dong (dap an tinh doc lap bang mo phong FIFO)
//        ./test_report in     -> in them cac bang bao cao
//
// Kho duoc nap bang chinh cac ham nghiep vu: themMatHang (mathang.txt), nhapKho (lo.txt),
// xuatKho (cac phieu XUAT trong phieu.txt) -> ton kho va lo da tru FIFO dung nhu that.
// (FileIO.cpp chua co nen test tu doc tep.)
#include "Inventory.h"
#include "Report.h"
#include "Sort.h"
#include "BTree.h"
#include "LotQueue.h"
#include "Date.h"
#include <fstream>
#include <iostream>
#include <string>

namespace {

int soLoi = 0;

void kiemTra(bool dk, const std::string& moTa) {
    if (!dk) {
        ++soLoi;
        std::cout << "  LOI: " << moTa << "\n";
    }
}

// Tach dong theo '|' vao mang f (toi da 8 truong), tra ve so truong
int tach(const std::string& dong, std::string f[8]) {
    int n = 0;
    std::string cur;
    for (char c : dong) {
        if (c == '\r') continue;
        if (c == '|') { if (n < 8) f[n++] = cur; cur.clear(); }
        else cur += c;
    }
    if (n < 8) f[n++] = cur;
    return n;
}

const std::string MA = "89300000000";   // + 2 chu so
std::string ma(int i) { return MA + (i < 10 ? "0" : "") + std::to_string(i); }

bool napKho(Kho& kho) {
    std::ifstream fmh("data/mathang.txt"), flo("data/lo.txt"), fph("data/phieu.txt");
    if (!fmh || !flo || !fph) {
        std::cout << "Khong mo duoc data/*.txt - hay chay tu thu muc goc repo\n";
        return false;
    }
    std::string dong, f[8];
    while (std::getline(fmh, dong)) {
        if (tach(dong, f) < 5) continue;
        themMatHang(kho, f[0], f[1], f[2], std::stod(f[3]), std::stoi(f[4]));
    }
    while (std::getline(flo, dong)) {      // maVach|maLo|ngayNhap|hsd|soLuong|giaNhap
        if (tach(dong, f) < 6) continue;
        Date ngay, hsd;
        parseDate(f[2], ngay);
        parseDate(f[3], hsd);
        nhapKho(kho, f[0], std::stoi(f[4]), std::stod(f[5]), hsd, ngay, nullptr);
    }
    while (std::getline(fph, dong)) {      // maPhieu|loai|ngay|maVach|soLuong|donGia
        if (tach(dong, f) < 6 || f[1] != "XUAT") continue;
        Date ngay;
        parseDate(f[2], ngay);
        KetQua kq = xuatKho(kho, f[3], std::stoi(f[4]), ngay, nullptr);
        kiemTra(kq == KQ_OK, "xuatKho phieu " + f[0]);
    }
    return true;
}

void testMergeSort() {
    // On dinh: cung ton kho -> giu thu tu ban dau
    MatHang m[7];
    int ton[7] = {5, 3, 5, 1, 3, 9, 1};
    MatHang* a[7];
    for (int i = 0; i < 7; ++i) {
        m[i].maVach = std::to_string(i);
        m[i].ten = std::string(1, char('g' - i));   // g f e d c b a
        m[i].tonKho = ton[i];
        a[i] = &m[i];
    }
    sapXepTheoTonKho(a, 7);
    const char* mongDoi = "3614025";               // ton 1,1,3,3,5,5,9 - on dinh
    std::string thuTu;
    for (int i = 0; i < 7; ++i) thuTu += a[i]->maVach;
    kiemTra(thuTu == mongDoi, "sapXepTheoTonKho khong dung / khong on dinh: " + thuTu);

    sapXepTheoTen(a, 7);
    std::string ten;
    for (int i = 0; i < 7; ++i) ten += a[i]->ten;
    kiemTra(ten == "abcdefg", "sapXepTheoTen: " + ten);

    sapXepTheoTen(a, 0);                           // n = 0, 1 khong loi
    sapXepTheoTen(a, 1);
    sapXepTheoTen(nullptr, 0);

    // Mang dong tu tang cap
    DsMatHang ds;
    dsInit(ds);
    for (int i = 0; i < 1000; ++i) dsThem(ds, &m[i % 7]);
    kiemTra(ds.n == 1000 && ds.cap >= 1000 && ds.a[999] == &m[999 % 7], "dsThem");
    dsClear(ds);
    kiemTra(ds.a == nullptr && ds.n == 0 && ds.cap == 0, "dsClear");
}

void testKhoRong() {
    Kho kho;
    khoInit(kho, 5);
    DsMatHang ds;
    timTheoTen(kho, "a", ds);
    kiemTra(ds.n == 0, "kho rong: timTheoTen");
    dsClear(ds);
    CanhBaoHSD* hsd;
    int n;
    canhBaoSapHetHan(kho, makeDate(18, 11, 2026), 7, hsd, n);
    kiemTra(n == 0 && hsd == nullptr, "kho rong: canhBaoSapHetHan");
    GiaTriNhom* g;
    baoCaoGiaTriTheoNhom(kho, g, n);
    kiemTra(n == 0, "kho rong: baoCaoGiaTriTheoNhom");
    delete[] g;
    BanChay* bc;
    topBanChay(kho, 11, 2026, 10, bc, n);
    kiemTra(n == 0 && bc == nullptr, "kho rong: topBanChay");
    khoGiaiPhong(kho);
}

void testDuLieuMau(bool inBang) {
    Kho kho;
    khoInit(kho, 5);
    if (!napKho(kho)) { ++soLoi; return; }
    kiemTra(kho.cay.size == 35, "nap 35 mat hang");

    // 1. Tim theo ten (khong phan biet hoa thuong), sap theo ten
    DsMatHang ds;
    timTheoTen(kho, "MI GOI", ds);
    kiemTra(ds.n == 4, "timTheoTen('MI GOI') phai co 4 mat hang");
    if (ds.n == 4) {
        kiemTra(ds.a[0]->maVach == ma(1) && ds.a[1]->maVach == ma(11) &&
                ds.a[2]->maVach == ma(21) && ds.a[3]->maVach == ma(31),
                "timTheoTen sai thu tu ten");
    }
    if (inBang) { std::cout << "\n== Tim 'MI GOI' ==\n"; inBangMatHang(ds.a, ds.n); }
    dsClear(ds);
    timTheoTen(kho, "khong co", ds);
    kiemTra(ds.n == 0, "timTheoTen khong co ket qua");
    dsClear(ds);

    // 2. Liet ke theo nhom (khong phan biet hoa thuong), sap theo ten
    lietKeTheoNhom(kho, "do UONG", ds);
    const int UONG[] = {13, 19, 7, 1, 31, 25};      // Banh quy 13, Ca phe 19, Dau goi 7, ...
    kiemTra(ds.n == 6, "lietKeTheoNhom('Do uong') phai co 6");
    for (int i = 0; i < ds.n && i < 6; ++i)
        kiemTra(ds.a[i]->maVach == ma(UONG[i]), "lietKeTheoNhom sai thu tu o vi tri " + std::to_string(i));
    if (inBang) { std::cout << "\n== Nhom 'Do uong' ==\n"; inBangMatHang(ds.a, ds.n); }
    dsClear(ds);

    // 3. Sap het hang: chi mat hang 18 (ton 14 < nguong 22)
    canhBaoSapHetHang(kho, ds);
    kiemTra(ds.n == 1 && ds.a[0]->maVach == ma(18) && ds.a[0]->tonKho == 14, "canhBaoSapHetHang");
    if (inBang) { std::cout << "\n== Sap het hang ==\n"; inBangMatHang(ds.a, ds.n); }
    dsClear(ds);

    // 4. Het han trong 7 ngay tinh tu 18/11/2026: lo 60, 49, 30, 19 (lo 9, 39, 69 da xuat het)
    CanhBaoHSD* hsd;
    int n;
    canhBaoSapHetHan(kho, makeDate(18, 11, 2026), 7, hsd, n);
    const int LO[] = {60, 49, 30, 19}, CON[] = {1, 2, 4, 5};
    kiemTra(n == 4, "canhBaoSapHetHan phai co 4 lo, co " + std::to_string(n));
    for (int i = 0; i < n && i < 4; ++i)
        kiemTra(hsd[i].lo->maLo == LO[i] && hsd[i].soNgayConLai == CON[i],
                "canhBaoSapHetHan sai o vi tri " + std::to_string(i));
    if (inBang) { std::cout << "\n== Het han trong 7 ngay (tu 18/11/2026) ==\n"; inBangCanhBaoHSD(hsd, n); }
    delete[] hsd;

    // ... va tinh tu 22/11/2026: lo 60, 49 da het han (so ngay con lai am)
    canhBaoSapHetHan(kho, makeDate(22, 11, 2026), 7, hsd, n);
    kiemTra(n >= 2 && hsd[0].lo->maLo == 60 && hsd[0].soNgayConLai == -3 &&
            hsd[1].lo->maLo == 49 && hsd[1].soNgayConLai == -2, "canhBaoSapHetHan: lo da het han");
    if (inBang) { std::cout << "\n== Het han trong 7 ngay (tu 22/11/2026) ==\n"; inBangCanhBaoHSD(hsd, n); }
    delete[] hsd;

    // 5. Gia tri ton theo nhom (theo gia nhap tung lo), giam dan
    GiaTriNhom* g;
    baoCaoGiaTriTheoNhom(kho, g, n);
    const char* NHOM[] = {"Banh keo", "Do gia dung", "Do uong", "Hoa my pham", "Gia vi", "Thuc pham tuoi"};
    const double GIATRI[] = {50973750, 44703750, 29366250, 21881250, 21341250, 7170000};
    const int TON[] = {502, 478, 382, 231, 184, 133};
    kiemTra(n == 6, "baoCaoGiaTriTheoNhom phai co 6 nhom");
    for (int i = 0; i < n && i < 6; ++i)
        kiemTra(g[i].nhom == NHOM[i] && g[i].giaTriTon == GIATRI[i] && g[i].tongTon == TON[i],
                "baoCaoGiaTriTheoNhom sai o nhom " + g[i].nhom);
    if (inBang) { std::cout << "\n== Gia tri ton theo nhom ==\n"; inBangGiaTriNhom(g, n); }
    delete[] g;

    // 6. Top 10 ban chay thang 11/2026 (05 va 14 cung 33 -> on dinh theo ma vach)
    BanChay* bc;
    topBanChay(kho, 11, 2026, 10, bc, n);
    const int TOP[] = {20, 8, 2, 35, 28, 5, 14, 29, 23, 22};
    const int SL[] = {50, 46, 42, 40, 34, 33, 33, 29, 27, 26};
    kiemTra(n == 10, "topBanChay phai tra 10");
    for (int i = 0; i < n && i < 10; ++i)
        kiemTra(bc[i].mh->maVach == ma(TOP[i]) && bc[i].soLuongBan == SL[i],
                "topBanChay sai o hang " + std::to_string(i + 1));
    if (inBang) { std::cout << "\n== Top 10 ban chay 11/2026 ==\n"; inBangBanChay(bc, n); }
    delete[] bc;

    topBanChay(kho, 10, 2026, 10, bc, n);
    kiemTra(n == 0 && bc == nullptr, "topBanChay thang khong co phieu xuat");
    topBanChay(kho, 11, 2026, 100, bc, n);
    kiemTra(n == 28, "topBanChay k > so mat hang co ban: tra du 28");
    delete[] bc;

    // Mat hang da xoa: phieu xuat cu bi bo qua
    kiemTra(xoaMatHang(kho, ma(20), true) == KQ_OK, "xoa mat hang 20");
    topBanChay(kho, 11, 2026, 3, bc, n);
    kiemTra(n == 3 && bc[0].mh->maVach == ma(8), "topBanChay sau khi xoa mat hang 20");
    delete[] bc;

    khoGiaiPhong(kho);
}

} // namespace

int main(int argc, char** argv) {
    bool inBang = argc > 1 && std::string(argv[1]) == "in";
    testMergeSort();
    testKhoRong();
    testDuLieuMau(inBang);
    std::cout << (soLoi == 0 ? "\nTAT CA DEU DUNG\n" : "\nCO " + std::to_string(soLoi) + " LOI!\n");
    return soLoi == 0 ? 0 : 1;
}
