// Sort.h - Giai thuat sap xep tu cai dat (Merge Sort tren mang) + mang dong DsMatHang
// Phu trach: Long Vu
#pragma once
#include "Types.h"

// ---- Mang dong thay cho vector ----
void dsInit(DsMatHang& ds);
void dsThem(DsMatHang& ds, MatHang* mh);   // tu tang gap doi cap khi day
void dsClear(DsMatHang& ds);               // chi giai phong mang, khong delete MatHang

// ---- Sap xep (deu dung Merge Sort, O(n log n), on dinh) ----
void sapXepTheoTen(MatHang** a, int n);              // ten tang dan
void sapXepTheoTonKho(MatHang** a, int n);           // ton kho tang dan
void sapXepTheoHSD(CanhBaoHSD* a, int n);            // HSD gan nhat truoc
void sapXepBanChay(BanChay* a, int n);               // so luong ban giam dan
void sapXepGiaTriNhom(GiaTriNhom* a, int n);         // gia tri ton giam dan
