// Report.h - Tim kiem, liet ke, canh bao, bao cao
// Phu trach: Long Vu
// Cac ham tra mang dong qua tham so out; nguoi goi giai phong (delete[] / dsClear).
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

// Tim theo ten (chuoi con, khong phan biet hoa thuong). Duyet toan cay O(n * L)
void timTheoTen(Kho& kho, const std::string& tuKhoa, DsMatHang& out);

// Liet ke mat hang thuoc mot nhom, sap theo ten
void lietKeTheoNhom(Kho& kho, const std::string& nhom, DsMatHang& out);

// Canh bao ton kho < nguongCanhBao, sap theo ton kho tang dan
void canhBaoSapHetHang(Kho& kho, DsMatHang& out);

// Cac LO co HSD trong [homNay, homNay + soNgay] (va lo da het han), sap theo HSD
void canhBaoSapHetHan(Kho& kho, const Date& homNay, int soNgay,
                      CanhBaoHSD*& out, int& n);

// Gia tri ton kho theo tung nhom
void baoCaoGiaTriTheoNhom(Kho& kho, GiaTriNhom*& out, int& n);

// Top k mat hang ban chay trong thang/nam (dua tren phieu xuat).
// Cach lam: dat demTam = 0 -> duyet phieu xuat trong thang, btSearch roi cong demTam
// -> gom cac mat hang co demTam > 0 -> sapXepBanChay -> lay k phan tu dau
void topBanChay(Kho& kho, int thang, int nam, int k, BanChay*& out, int& n);

// In ket qua dang bang
void inBangMatHang(MatHang** a, int n);
void inBangCanhBaoHSD(const CanhBaoHSD* a, int n);
void inBangGiaTriNhom(const GiaTriNhom* a, int n);
void inBangBanChay(const BanChay* a, int n);
