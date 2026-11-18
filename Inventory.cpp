// Inventory.cpp - Nghiep vu quan ly kho hang (them/xoa/sua, xuat kho FIFO)
// Nguoi thuc hien: Minh Tan
#include "Inventory.h"
#include "BTree.h"
#include "LotQueue.h"
#include "Phieu.h"
#include <iostream>
#include <iomanip>

void khoInit(Kho& kho, int bacCay) {
    btInit(kho.cay, bacCay);
    dspInit(kho.phieu);
    kho.maLoKeTiep = 1;
}

void khoGiaiPhong(Kho& kho) {
    dspClear(kho.phieu);
    // Xoa toan bo nut tren cay B-Tree va dong thoi xoa luon MatHang + cac Lo cua chung
    btDestroy(kho.cay, true);
}

// ---------------------- QUAN LY MAT HANG ----------------------
KetQua themMatHang(Kho& kho, const std::string& maVach, const std::string& ten,
                   const std::string& nhom, double donGia, int nguongCanhBao) {
    if (maVach.empty() || ten.empty() || donGia < 0 || nguongCanhBao < 0) {
        return KQ_DU_LIEU_SAI;
    }
    if (btSearch(kho.cay, maVach) != nullptr) {
        return KQ_TRUNG_MA;
    }

    MatHang* mh = new MatHang;
    mh->maVach = maVach;
    mh->ten = ten;
    mh->nhom = nhom;
    mh->donGia = donGia;
    mh->tonKho = 0;
    mh->nguongCanhBao = nguongCanhBao;
    mh->demTam = 0;
    lqInit(mh->dsLo);

    if (!btInsert(kho.cay, mh)) {
        delete mh;
        return KQ_TRUNG_MA;
    }
    return KQ_OK;
}

KetQua suaMatHang(Kho& kho, const std::string& maVach, const std::string& tenMoi,
                  const std::string& nhomMoi, double donGiaMoi, int nguongMoi) {
    MatHang* mh = btSearch(kho.cay, maVach);
    if (!mh) return KQ_KHONG_TIM_THAY;
    if (tenMoi.empty() || donGiaMoi < 0 || nguongMoi < 0) {
        return KQ_DU_LIEU_SAI;
    }

    mh->ten = tenMoi;
    mh->nhom = nhomMoi;
    mh->donGia = donGiaMoi;
    mh->nguongCanhBao = nguongMoi;
    return KQ_OK;
}

KetQua xoaMatHang(Kho& kho, const std::string& maVach, bool xoaKeCaTonKho) {
    MatHang* mh = btSearch(kho.cay, maVach);
    if (!mh) return KQ_KHONG_TIM_THAY;

    if (!xoaKeCaTonKho && mh->tonKho > 0) {
        return KQ_CON_TON_KHO;
    }

    MatHang* removedMh = btRemove(kho.cay, maVach);
    if (removedMh) {
    
        delete removedMh;
    }
    return KQ_OK;
}

MatHang* timMatHang(Kho& kho, const std::string& maVach) {
    return btSearch(kho.cay, maVach);
}

// ---------------------- NHAP KHO & XUAT KHO FIFO ----------------------

// Nhap kho: Tao lo moi day vao cuoi hang doi dsLo, tang ton kho va tao phieu nhap
KetQua nhapKho(Kho& kho, const std::string& maVach, int soLuong, double giaNhap,
               const Date& hsd, const Date& ngay, int* maPhieuOut) {
    if (soLuong <= 0 || giaNhap < 0) return KQ_DU_LIEU_SAI;

    MatHang* mh = btSearch(kho.cay, maVach);
    if (!mh) return KQ_KHONG_TIM_THAY;

    // 1. Tao lo moi
    Lo* lo = taoLo(kho.maLoKeTiep++, ngay, hsd, soLuong, giaNhap);
    lqEnqueue(mh->dsLo, lo);

    // 2. Cap nhat ton kho
    mh->tonKho += soLuong;

    // 3. Tao phieu nhap
    Phieu* p = dspThem(kho.phieu, PHIEU_NHAP, ngay, maVach, soLuong, giaNhap);
    if (maPhieuOut) *maPhieuOut = p->maPhieu;

    return KQ_OK;
}

// Xuat kho FIFO: Lay tu lo cu nhat (front). Neu lo het thi dequeue va lay tiep lo sau
KetQua xuatKho(Kho& kho, const std::string& maVach, int soLuong, const Date& ngay,
               int* maPhieuOut) {
    if (soLuong <= 0) return KQ_DU_LIEU_SAI;

    MatHang* mh = btSearch(kho.cay, maVach);
    if (!mh) return KQ_KHONG_TIM_THAY;

    // Kiem tra du ton kho khong
    if (mh->tonKho < soLuong) {
        return KQ_KHONG_DU_HANG;
    }

    int canXuat = soLuong;

    // Thuc hien tru kho theo FIFO
    while (canXuat > 0 && !lqRong(mh->dsLo)) {
        Lo* loHienTai = lqFront(mh->dsLo);

        if (loHienTai->soLuong <= canXuat) {
        
            canXuat -= loHienTai->soLuong;
            lqDequeue(mh->dsLo);
            delete loHienTai;
        } else {
            
            loHienTai->soLuong -= canXuat;
            canXuat = 0;
        }
    }

    // Cap nhat lai tong ton kho
    mh->tonKho -= soLuong;

    // Tao phieu xuat (donGia la don gia ban hien tai cua mat hang)
    Phieu* p = dspThem(kho.phieu, PHIEU_XUAT, ngay, maVach, soLuong, mh->donGia);
    if (maPhieuOut) *maPhieuOut = p->maPhieu;

    return KQ_OK;
}

void inChiTietMatHang(const MatHang& mh) {
    std::cout << "\n---------------- THONG TIN MAT HANG ----------------\n";
    std::cout << "Ma vach         : " << mh.maVach << "\n";
    std::cout << "Ten mat hang    : " << mh.ten << "\n";
    std::cout << "Nhom hang       : " << mh.nhom << "\n";
    std::cout << "Don gia ban     : " << std::fixed << std::setprecision(2) << mh.donGia << "\n";
    std::cout << "Tong ton kho    : " << mh.tonKho << "\n";
    std::cout << "Nguong canh bao : " << mh.nguongCanhBao << "\n";
    std::cout << "So lo hien co   : " << mh.dsLo.count << "\n";
    std::cout << "Chi tiet cac lo hang (FIFO - Cu nhat den moi nhat):\n";

    if (lqRong(mh.dsLo)) {
        std::cout << "  (Hien tai khong con lo hang nao)\n";
    } else {
        std::cout << "  " << std::left << std::setw(8) << "Ma Lo"
                  << std::setw(14) << "Ngay Nhap"
                  << std::setw(14) << "Han Su Dung"
                  << std::setw(10) << "So Luong"
                  << std::setw(12) << "Gia Nhap" << "\n";
        for (Lo* lo = mh.dsLo.front; lo != nullptr; lo = lo->next) {
            std::string sNgay = std::to_string(lo->ngayNhap.d) + "/" + std::to_string(lo->ngayNhap.m) + "/" + std::to_string(lo->ngayNhap.y);
            std::string sHsd  = std::to_string(lo->hsd.d) + "/" + std::to_string(lo->hsd.m) + "/" + std::to_string(lo->hsd.y);
            std::cout << "  " << std::left << std::setw(8) << lo->maLo
                      << std::setw(14) << sNgay
                      << std::setw(14) << sHsd
                      << std::setw(10) << lo->soLuong
                      << std::fixed << std::setprecision(2) << std::setw(12) << lo->giaNhap << "\n";
        }
    }
    std::cout << "----------------------------------------------------\n";
}