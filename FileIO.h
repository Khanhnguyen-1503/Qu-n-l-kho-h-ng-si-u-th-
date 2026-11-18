// FileIO.h - Doc tep khi khoi dong, ghi tep khi thoat
// Phu trach: Huynh Tin
//
// Dinh dang (moi dong 1 ban ghi, phan cach bang '|', ngay dd/mm/yyyy):
//   mathang.txt : maVach|ten|nhom|donGia|nguongCanhBao
//   lo.txt      : maVach|maLo|ngayNhap|hsd|soLuong|giaNhap      (ghi theo thu tu FIFO)
//   phieu.txt   : maPhieu|loai(0=nhap,1=xuat)|ngay|maVach|soLuong|donGia
// tonKho KHONG ghi ra tep, tinh lai tu cac lo khi doc.
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

// Tra ve so ban ghi doc duoc, -1 neu khong mo duoc tep.
// Dong sai dinh dang thi bo qua va in canh bao kem so dong.
int docMatHang(Kho& kho, const std::string& duongDan);
int docLo(Kho& kho, const std::string& duongDan);      // goi sau docMatHang
int docPhieu(Kho& kho, const std::string& duongDan);

bool ghiMatHang(const Kho& kho, const std::string& duongDan);
bool ghiLo(const Kho& kho, const std::string& duongDan);
bool ghiPhieu(const Kho& kho, const std::string& duongDan);

// Goi 3 ham tren theo dung thu tu
bool docToanBo(Kho& kho, const std::string& thuMuc);
bool ghiToanBo(const Kho& kho, const std::string& thuMuc);
