// Report.cpp - Tim kiem, liet ke, canh bao, bao cao
// Phu trach: Long Vu
//
// Moi chuc nang duyet kho qua btTraverse (duyet B-Tree theo ma vach tang dan),
// gom ket qua vao mang dong rieng (DsMatHang hoac mang new[]), roi sap xep bang
// Merge Sort tu cai trong Sort.cpp. Khong dung container / thuat toan cua STL.
// Nguoi goi giai phong ket qua: dsClear(DsMatHang) hoac delete[] (mang struct).
#include "Report.h"
#include "BTree.h"
#include "Date.h"
#include "Sort.h"
#include <iomanip>
#include <iostream>

namespace {

// ============================ TIEN ICH ============================

char thuong(char c) { return (c >= 'A' && c <= 'Z') ? char(c - 'A' + 'a') : c; }

std::string chuThuongChuoi(const std::string& s) {
    std::string kq = s;
    for (size_t i = 0; i < kq.size(); ++i) kq[i] = thuong(kq[i]);
    return kq;
}

// So sanh 2 chuoi khong phan biet hoa thuong
bool bangNhauKhongHoa(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (thuong(a[i]) != thuong(b[i])) return false;
    return true;
}

// Cat chuoi cho vua cot khi in bang
std::string vuaCot(const std::string& s, size_t rong) {
    return s.size() <= rong ? s : s.substr(0, rong - 2) + "..";
}

// ============================ CONTEXT CHO btTraverse ============================

struct LocTenCtx  { std::string tuKhoa; DsMatHang* out; };
struct LocNhomCtx { const std::string* nhom; DsMatHang* out; };

void locTheoTen(MatHang* mh, void* ctx) {
    LocTenCtx* c = static_cast<LocTenCtx*>(ctx);
    if (chuThuongChuoi(mh->ten).find(c->tuKhoa) != std::string::npos) dsThem(*c->out, mh);
}

void locTheoNhom(MatHang* mh, void* ctx) {
    LocNhomCtx* c = static_cast<LocNhomCtx*>(ctx);
    if (bangNhauKhongHoa(mh->nhom, *c->nhom)) dsThem(*c->out, mh);
}

void locSapHet(MatHang* mh, void* ctx) {
    if (mh->tonKho < mh->nguongCanhBao) dsThem(*static_cast<DsMatHang*>(ctx), mh);
}

// Canh bao HSD: duyet 2 luot - luot 1 dem, luot 2 ghi vao mang da cap dung kich thuoc
struct HsdCtx {
    Date        homNay;
    int         soNgay;
    CanhBaoHSD* out;    // nullptr o luot dem
    int         n;
};

void quetLoHsd(MatHang* mh, void* ctx) {
    HsdCtx* c = static_cast<HsdCtx*>(ctx);
    for (Lo* lo = mh->dsLo.front; lo != nullptr; lo = lo->next) {
        if (lo->soLuong <= 0) continue;
        int conLai = khoangCachNgay(c->homNay, lo->hsd);   // < 0: da het han
        if (conLai > c->soNgay) continue;
        if (c->out != nullptr) {
            c->out[c->n].mh = mh;
            c->out[c->n].lo = lo;
            c->out[c->n].soNgayConLai = conLai;
        }
        c->n++;
    }
}

// Gia tri theo nhom: mang dong GiaTriNhom, tim nhom bang duyet tuan tu (so nhom nho)
struct NhomCtx {
    GiaTriNhom* a;
    int         n;
    int         cap;
};

void congVaoNhom(MatHang* mh, void* ctx) {
    NhomCtx* c = static_cast<NhomCtx*>(ctx);
    int i = 0;
    while (i < c->n && c->a[i].nhom != mh->nhom) ++i;
    if (i == c->n) {                                   // nhom moi
        if (c->n == c->cap) {
            int capMoi = (c->cap == 0) ? 8 : c->cap * 2;
            GiaTriNhom* moi = new GiaTriNhom[capMoi];
            for (int j = 0; j < c->n; ++j) moi[j] = c->a[j];
            delete[] c->a;
            c->a = moi;
            c->cap = capMoi;
        }
        c->a[i].nhom = mh->nhom;
        c->a[i].soMatHang = 0;
        c->a[i].tongTon = 0;
        c->a[i].giaTriTon = 0;
        c->n++;
    }
    c->a[i].soMatHang++;
    c->a[i].tongTon += mh->tonKho;
    for (Lo* lo = mh->dsLo.front; lo != nullptr; lo = lo->next)
        c->a[i].giaTriTon += lo->soLuong * lo->giaNhap;   // gia tri theo GIA NHAP tung lo
}

// Top ban chay
void datDemTamBang0(MatHang* mh, void*) { mh->demTam = 0; }

void demCoBan(MatHang* mh, void* ctx) {
    if (mh->demTam > 0) ++*static_cast<int*>(ctx);
}

struct BanChayCtx { BanChay* out; int n; };

void gomCoBan(MatHang* mh, void* ctx) {
    if (mh->demTam <= 0) return;
    BanChayCtx* c = static_cast<BanChayCtx*>(ctx);
    c->out[c->n].mh = mh;
    c->out[c->n].soLuongBan = mh->demTam;
    c->n++;
}

// Luu / khoi phuc dinh dang cout de khong anh huong cac mo-dun khac
struct GiuDinhDang {
    std::ios cu;
    GiuDinhDang() : cu(nullptr) { cu.copyfmt(std::cout); }
    ~GiuDinhDang() { std::cout.copyfmt(cu); }
};

void inDongKe(int rong) { std::cout << std::string(rong, '-') << "\n"; }

} // namespace

// ============================ TIM KIEM / LIET KE ============================

void timTheoTen(Kho& kho, const std::string& tuKhoa, DsMatHang& out) {
    dsInit(out);
    LocTenCtx ctx = {chuThuongChuoi(tuKhoa), &out};
    btTraverse(kho.cay, locTheoTen, &ctx);
    sapXepTheoTen(out.a, out.n);
}

void lietKeTheoNhom(Kho& kho, const std::string& nhom, DsMatHang& out) {
    dsInit(out);
    LocNhomCtx ctx = {&nhom, &out};
    btTraverse(kho.cay, locTheoNhom, &ctx);
    sapXepTheoTen(out.a, out.n);
}

// ============================ CANH BAO ============================

void canhBaoSapHetHang(Kho& kho, DsMatHang& out) {
    dsInit(out);
    btTraverse(kho.cay, locSapHet, &out);
    sapXepTheoTonKho(out.a, out.n);
}

void canhBaoSapHetHan(Kho& kho, const Date& homNay, int soNgay, CanhBaoHSD*& out, int& n) {
    HsdCtx ctx = {homNay, soNgay, nullptr, 0};
    btTraverse(kho.cay, quetLoHsd, &ctx);          // luot 1: dem
    n = ctx.n;
    out = nullptr;
    if (n == 0) return;

    out = new CanhBaoHSD[n];
    ctx.out = out;
    ctx.n = 0;
    btTraverse(kho.cay, quetLoHsd, &ctx);          // luot 2: ghi
    sapXepTheoHSD(out, n);
}

// ============================ BAO CAO ============================

void baoCaoGiaTriTheoNhom(Kho& kho, GiaTriNhom*& out, int& n) {
    NhomCtx ctx = {nullptr, 0, 0};
    btTraverse(kho.cay, congVaoNhom, &ctx);
    sapXepGiaTriNhom(ctx.a, ctx.n);
    out = ctx.a;
    n = ctx.n;
}

void topBanChay(Kho& kho, int thang, int nam, int k, BanChay*& out, int& n) {
    out = nullptr;
    n = 0;

    // 1. demTam = 0 cho moi mat hang
    btTraverse(kho.cay, datDemTamBang0, nullptr);

    // 2. Duyet phieu xuat trong thang/nam, cong so luong vao demTam
    for (Phieu* p = kho.phieu.head; p != nullptr; p = p->next) {
        if (p->loai != PHIEU_XUAT || p->ngay.m != thang || p->ngay.y != nam) continue;
        MatHang* mh = btSearch(kho.cay, p->maVach);
        if (mh != nullptr) mh->demTam += p->soLuong;   // mat hang da bi xoa -> bo qua
    }

    // 3. Gom cac mat hang co demTam > 0
    int soCoBan = 0;
    btTraverse(kho.cay, demCoBan, &soCoBan);
    if (soCoBan == 0 || k <= 0) return;
    BanChayCtx ctx = {new BanChay[soCoBan], 0};
    btTraverse(kho.cay, gomCoBan, &ctx);

    // 4. Sap giam dan, lay k phan tu dau (mang cap soCoBan o, n = so phan tu hop le)
    sapXepBanChay(ctx.out, ctx.n);
    out = ctx.out;
    n = (k < ctx.n) ? k : ctx.n;
}

// ============================ IN BANG ============================

void inBangMatHang(MatHang** a, int n) {
    if (n <= 0) {
        std::cout << "  (Khong co mat hang nao)\n";
        return;
    }
    GiuDinhDang giu;
    std::cout << std::left << std::setw(5) << "STT" << std::setw(15) << "Ma vach"
              << std::setw(24) << "Ten" << std::setw(16) << "Nhom" << std::right
              << std::setw(12) << "Don gia" << std::setw(8) << "Ton" << std::setw(8) << "Nguong"
              << "\n";
    inDongKe(88);
    std::cout << std::fixed << std::setprecision(0);
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(15) << a[i]->maVach
                  << std::setw(24) << vuaCot(a[i]->ten, 23) << std::setw(16)
                  << vuaCot(a[i]->nhom, 15) << std::right << std::setw(12) << a[i]->donGia
                  << std::setw(8) << a[i]->tonKho << std::setw(8) << a[i]->nguongCanhBao << "\n";
    }
    std::cout << "Tong: " << n << " mat hang\n";
}

void inBangCanhBaoHSD(const CanhBaoHSD* a, int n) {
    if (n <= 0) {
        std::cout << "  (Khong co lo nao sap het han)\n";
        return;
    }
    GiuDinhDang giu;
    std::cout << std::left << std::setw(5) << "STT" << std::setw(15) << "Ma vach"
              << std::setw(24) << "Ten" << std::right << std::setw(7) << "Ma lo"
              << std::setw(13) << "HSD" << std::setw(8) << "SL" << "  " << std::left
              << "Tinh trang" << "\n";
    inDongKe(88);
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(15) << a[i].mh->maVach
                  << std::setw(24) << vuaCot(a[i].mh->ten, 23) << std::right << std::setw(7)
                  << a[i].lo->maLo << std::setw(13) << formatDate(a[i].lo->hsd)
                  << std::setw(8) << a[i].lo->soLuong << "  ";
        if (a[i].soNgayConLai < 0) std::cout << "DA HET HAN " << -a[i].soNgayConLai << " ngay";
        else if (a[i].soNgayConLai == 0) std::cout << "Het han hom nay";
        else std::cout << "Con " << a[i].soNgayConLai << " ngay";
        std::cout << "\n";
    }
}

void inBangGiaTriNhom(const GiaTriNhom* a, int n) {
    if (n <= 0) {
        std::cout << "  (Kho rong)\n";
        return;
    }
    GiuDinhDang giu;
    std::cout << std::left << std::setw(5) << "STT" << std::setw(20) << "Nhom" << std::right
              << std::setw(10) << "So MH" << std::setw(12) << "Tong ton" << std::setw(20)
              << "Gia tri ton" << "\n";
    inDongKe(67);
    std::cout << std::fixed << std::setprecision(0);
    double tong = 0;
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(20) << vuaCot(a[i].nhom, 19)
                  << std::right << std::setw(10) << a[i].soMatHang << std::setw(12)
                  << a[i].tongTon << std::setw(20) << a[i].giaTriTon << "\n";
        tong += a[i].giaTriTon;
    }
    inDongKe(67);
    std::cout << std::left << std::setw(47) << "Tong gia tri ton kho" << std::right
              << std::setw(20) << tong << "\n";
}

void inBangBanChay(const BanChay* a, int n) {
    if (n <= 0) {
        std::cout << "  (Khong co phieu xuat trong thang)\n";
        return;
    }
    GiuDinhDang giu;
    std::cout << std::left << std::setw(5) << "Hang" << std::setw(15) << "Ma vach"
              << std::setw(24) << "Ten" << std::setw(16) << "Nhom" << std::right
              << std::setw(10) << "SL ban" << "\n";
    inDongKe(70);
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(15) << a[i].mh->maVach
                  << std::setw(24) << vuaCot(a[i].mh->ten, 23) << std::setw(16)
                  << vuaCot(a[i].mh->nhom, 15) << std::right << std::setw(10)
                  << a[i].soLuongBan << "\n";
    }
}
