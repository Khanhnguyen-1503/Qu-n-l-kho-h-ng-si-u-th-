// Types.h - Cac struct dung chung cho toan bo chuong trinh Quan ly kho hang sieu thi
// Ca nhom chot file nay o Ngay 1. Muon sua struct phai bao ca nhom truoc.
// Luu y: phan loi KHONG dung vector/list/queue/map/set/sort... cua STL (chi dung string, iostream, fstream).
#pragma once
#include <string>

// ===================== NGAY THANG =====================
struct Date {
    int d;  // ngay 1..31
    int m;  // thang 1..12
    int y;  // nam, vd 2026
};

// ===================== LO HANG (phan tu cua hang doi FIFO) =====================
struct Lo {
    int    maLo;      // tu tang, duy nhat toan kho
    Date   ngayNhap;
    Date   hsd;       // han su dung gan voi TUNG LO, khong gan voi mat hang
    int    soLuong;   // so luong con lai trong lo (giam dan khi xuat)
    double giaNhap;   // don gia nhap cua lo
    Lo*    next;
};

// Hang doi cac lo cua mot mat hang: front = lo cu nhat (xuat truoc), rear = lo moi nhat
struct LotQueue {
    Lo* front;
    Lo* rear;
    int count;        // so lo dang co
};

// ===================== MAT HANG (khoa cua B-Tree = maVach) =====================
struct MatHang {
    std::string maVach;          // 8 hoac 13 chu so, duy nhat
    std::string ten;
    std::string nhom;            // nhom hang: "Sua", "Banh keo", "Do uong", ...
    double      donGia;          // gia ban
    int         tonKho;          // = tong soLuong cua cac lo trong dsLo
    int         nguongCanhBao;   // ton < nguong -> canh bao sap het hang
    LotQueue    dsLo;            // cac lo nhap, xuat theo FIFO
    int         demTam;          // bien tam cho bao cao (vd: so luong ban trong thang), khong ghi ra tep
};

// ===================== PHIEU NHAP / XUAT =====================
enum LoaiPhieu { PHIEU_NHAP = 0, PHIEU_XUAT = 1 };

// Moi phieu ung voi 1 mat hang (de don gian); phieu nhap tao 1 lo moi
struct Phieu {
    int         maPhieu;    // tu tang
    LoaiPhieu   loai;
    Date        ngay;
    std::string maVach;
    int         soLuong;
    double      donGia;     // phieu nhap: gia nhap; phieu xuat: gia ban tai thoi diem xuat
    Phieu*      next;
};

// DSLK don cac phieu, them vao cuoi (tail) nen giu thu tu thoi gian
struct DanhSachPhieu {
    Phieu* head;
    Phieu* tail;
    int    count;
    int    maPhieuKeTiep;
};

// ===================== B-TREE BAC m =====================
// Moi nut co toi da m-1 khoa va m con; nut khac goc co it nhat ceil(m/2)-1 khoa.
struct BTreeNode {
    int         n;       // so khoa hien co
    bool        leaf;
    MatHang**   keys;    // mang dong m-1 phan tu (cap them 1 o de tach nut cho de)
    BTreeNode** child;   // mang dong m phan tu (+1)
};

struct BTree {
    BTreeNode* root;
    int        m;              // bac cua cay (3, 5, 10, ...)
    int        size;           // so mat hang
    long long  soNutTruyXuat;  // dem so nut da tham khi tim kiem (phan nang cao)
    bool       inKhiTachGop;   // true -> in cay sau moi lan tach/gop nut (phan nang cao)
};

// ===================== MANG DONG KET QUA (thay cho vector) =====================
struct DsMatHang {
    MatHang** a;
    int       n;
    int       cap;
};

// ===================== CAC BAN GHI BAO CAO =====================
struct CanhBaoHSD {
    MatHang* mh;
    Lo*      lo;
    int      soNgayConLai;   // <0: da het han
};

struct BanChay {
    MatHang* mh;
    int      soLuongBan;
};

struct GiaTriNhom {
    std::string nhom;
    int         soMatHang;
    int         tongTon;
    double      giaTriTon;   // tong (soLuong lo * giaNhap) cua cac lo
};

// ===================== MA LOI CHUNG =====================
enum KetQua {
    KQ_OK = 0,
    KQ_TRUNG_MA,          // them mat hang da co ma vach
    KQ_KHONG_TIM_THAY,
    KQ_KHONG_DU_HANG,     // xuat nhieu hon ton kho
    KQ_DU_LIEU_SAI,       // so luong <= 0, ngay sai, ...
    KQ_CON_TON_KHO,       // xoa mat hang con hang trong kho
    KQ_LOI_TEP
};
