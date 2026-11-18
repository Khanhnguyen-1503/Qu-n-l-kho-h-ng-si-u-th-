// Sort.cpp - Mang dong DsMatHang + Merge Sort tu cai dat
// Phu trach: Long Vu
//
// Merge Sort (de quy tu tren xuong):
//   - Chia doi mang, sap xep tung nua, roi tron 2 nua da sap xep.
//   - Dung 1 mang phu cap phat 1 lan cho ca qua trinh (khong cap phat lai moi lan tron).
//   - On dinh: khi 2 phan tu "bang nhau" thi lay phan tu o nua trai truoc,
//     nen thu tu ban dau duoc giu (vd danh sach tu btTraverse da tang theo ma vach
//     -> cac mat hang cung ten/cung ton se dung theo ma vach tang dan).
//   - Thoi gian O(n log n) trong moi truong hop, bo nho phu O(n).
#include "Sort.h"
#include "Date.h"

// ============================ MANG DONG ============================

void dsInit(DsMatHang& ds) {
    ds.a = nullptr;
    ds.n = 0;
    ds.cap = 0;
}

// Them vao cuoi; day thi cap phat mang gap doi va chep sang. O(1) khau hao
void dsThem(DsMatHang& ds, MatHang* mh) {
    if (ds.n == ds.cap) {
        int capMoi = (ds.cap == 0) ? 8 : ds.cap * 2;
        MatHang** moi = new MatHang*[capMoi];
        for (int i = 0; i < ds.n; ++i) moi[i] = ds.a[i];
        delete[] ds.a;
        ds.a = moi;
        ds.cap = capMoi;
    }
    ds.a[ds.n++] = mh;
}

// Chi giai phong mang con tro; MatHang thuoc ve B-Tree/Inventory nen khong delete
void dsClear(DsMatHang& ds) {
    delete[] ds.a;
    dsInit(ds);
}

// ============================ MERGE SORT ============================

namespace {

// truoc(x, y) = true neu x phai dung TRUOC y (nghiem ngat). Bang nhau -> false.
// Tron a[trai..giua] va a[giua+1..phai] (deu da sap) vao tam, roi chep lai vao a.
template <typename T, typename SoSanh>
void tron(T* a, T* tam, int trai, int giua, int phai, SoSanh truoc) {
    int i = trai, j = giua + 1, k = trai;
    while (i <= giua && j <= phai) {
        if (truoc(a[j], a[i])) tam[k++] = a[j++];   // chi lay ben phai khi no NHO HON han
        else tam[k++] = a[i++];                     // bang nhau -> lay ben trai: on dinh
    }
    while (i <= giua) tam[k++] = a[i++];
    while (j <= phai) tam[k++] = a[j++];
    for (k = trai; k <= phai; ++k) a[k] = tam[k];
}

template <typename T, typename SoSanh>
void mergeSortDeQuy(T* a, T* tam, int trai, int phai, SoSanh truoc) {
    if (trai >= phai) return;                  // 0 hoac 1 phan tu: da sap
    int giua = trai + (phai - trai) / 2;
    mergeSortDeQuy(a, tam, trai, giua, truoc);
    mergeSortDeQuy(a, tam, giua + 1, phai, truoc);
    tron(a, tam, trai, giua, phai, truoc);
}

template <typename T, typename SoSanh>
void mergeSort(T* a, int n, SoSanh truoc) {
    if (a == nullptr || n < 2) return;
    T* tam = new T[n];
    mergeSortDeQuy(a, tam, 0, n - 1, truoc);
    delete[] tam;
}

// Cac ham so sanh "x dung truoc y"
bool tenNhoHon(MatHang* x, MatHang* y)               { return x->ten < y->ten; }
bool tonNhoHon(MatHang* x, MatHang* y)               { return x->tonKho < y->tonKho; }
bool hsdSomHon(const CanhBaoHSD& x, const CanhBaoHSD& y) {
    return soSanhNgay(x.lo->hsd, y.lo->hsd) < 0;
}
bool banNhieuHon(const BanChay& x, const BanChay& y) { return x.soLuongBan > y.soLuongBan; }
bool giaTriLonHon(const GiaTriNhom& x, const GiaTriNhom& y) { return x.giaTriTon > y.giaTriTon; }

} // namespace

void sapXepTheoTen(MatHang** a, int n)     { mergeSort(a, n, tenNhoHon); }
void sapXepTheoTonKho(MatHang** a, int n)  { mergeSort(a, n, tonNhoHon); }
void sapXepTheoHSD(CanhBaoHSD* a, int n)   { mergeSort(a, n, hsdSomHon); }
void sapXepBanChay(BanChay* a, int n)      { mergeSort(a, n, banNhieuHon); }
void sapXepGiaTriNhom(GiaTriNhom* a, int n){ mergeSort(a, n, giaTriLonHon); }
