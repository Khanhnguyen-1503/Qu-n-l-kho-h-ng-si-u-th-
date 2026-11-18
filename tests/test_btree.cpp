// test_btree.cpp - Kiem thu B-Tree (Khanh Nguyen)
// Build (tu thu muc goc repo):
//   g++ -std=c++17 -Wall -Wextra -I. tests/test_btree.cpp BTree.cpp LotQueue.cpp -o test_btree
// Chay:  ./test_btree        -> kiem thu tu dong
//        ./test_btree demo   -> in cay sau moi lan tach/gop voi m = 3 (vi du chay tay cho bao cao)
#include "BTree.h"
#include "LotQueue.h"
#include <iostream>
#include <string>

namespace {

int soLoi = 0;

void kiemTra(bool dk, const char* moTa) {
    if (!dk) {
        ++soLoi;
        std::cout << "  LOI: " << moTa << "\n";
    }
}

std::string maTuSo(int x) {   // 893 + 10 chu so
    std::string s = "893";
    char buf[10];
    for (int i = 9; i >= 0; --i) {
        buf[i] = char('0' + x % 10);
        x /= 10;
    }
    s.append(buf, 10);
    return s;
}

MatHang* taoMH(const std::string& ma) {
    MatHang* mh = new MatHang;
    mh->maVach = ma;
    mh->ten = "MH " + ma;
    mh->nhom = "Test";
    mh->donGia = 1000;
    mh->tonKho = 0;
    mh->nguongCanhBao = 0;
    mh->demTam = 0;
    lqInit(mh->dsLo);
    return mh;
}

// Kiem tra moi tinh chat B-Tree cua cay con goc x.
// lo/hi: can duoi/tren cua khoa (nullptr = khong gioi han). Tra ve so khoa trong cay con.
int kiemTraNut(const BTree& t, const BTreeNode* x, bool laGoc, int muc, int& mucLa,
               const std::string* lo, const std::string* hi) {
    int toiThieu = (t.m + 1) / 2 - 1;
    kiemTra(x->n <= t.m - 1, "nut co qua m-1 khoa");
    kiemTra(laGoc ? x->n >= 1 : x->n >= toiThieu, "nut co it hon so khoa toi thieu");
    for (int i = 0; i < x->n; ++i) {
        if (i > 0) kiemTra(x->keys[i - 1]->maVach < x->keys[i]->maVach, "khoa trong nut khong tang dan");
        if (lo) kiemTra(*lo < x->keys[i]->maVach, "khoa nho hon can duoi");
        if (hi) kiemTra(x->keys[i]->maVach < *hi, "khoa lon hon can tren");
    }
    if (x->leaf) {
        if (mucLa < 0) mucLa = muc;
        kiemTra(mucLa == muc, "cac la khong cung muc");
        return x->n;
    }
    int tong = x->n;
    for (int i = 0; i <= x->n; ++i) {
        kiemTra(x->child[i] != nullptr, "nut trong thieu con");
        if (x->child[i] == nullptr) continue;
        const std::string* l = (i == 0) ? lo : &x->keys[i - 1]->maVach;
        const std::string* h = (i == x->n) ? hi : &x->keys[i]->maVach;
        tong += kiemTraNut(t, x->child[i], false, muc + 1, mucLa, l, h);
    }
    return tong;
}

void kiemTraCay(const BTree& t) {
    if (t.root == nullptr) {
        kiemTra(t.size == 0, "cay rong nhung size != 0");
        return;
    }
    int mucLa = -1;
    int tong = kiemTraNut(t, t.root, true, 0, mucLa, nullptr, nullptr);
    kiemTra(tong == t.size, "so khoa trong cay khac t.size");
    kiemTra(btHeight(t) == mucLa + 1, "btHeight sai");
}

struct DuyetCtx {
    std::string truoc;
    int dem;
    bool tang;
};

void ghiNhan(MatHang* mh, void* ctx) {
    DuyetCtx* c = static_cast<DuyetCtx*>(ctx);
    if (c->dem > 0 && !(c->truoc < mh->maVach)) c->tang = false;
    c->truoc = mh->maVach;
    c->dem++;
}

// Sinh so gia ngau nhien LCG (giong nhau tren moi may, khong phu thuoc rand())
unsigned int hatGiong = 12345;
int ngauNhien(int n) {
    hatGiong = hatGiong * 1103515245u + 12345u;
    return int((hatGiong >> 8) % unsigned(n));
}

void testNgauNhien(int m) {
    const int N = 3000;          // khong gian ma
    const int SO_THAO_TAC = 20000;
    bool coTrongCay[N];
    for (int i = 0; i < N; ++i) coTrongCay[i] = false;
    int soLoiTruoc = soLoi;

    BTree t;
    btInit(t, m);
    int dangCo = 0;
    for (int b = 0; b < SO_THAO_TAC; ++b) {
        int x = ngauNhien(N);
        std::string ma = maTuSo(x);
        // 60% chen, 40% xoa -> cay lon dan roi dao dong, di qua du tach / muon / gop
        if (ngauNhien(10) < 6) {
            MatHang* mh = taoMH(ma);
            bool ok = btInsert(t, mh);
            kiemTra(ok == !coTrongCay[x], "btInsert tra ket qua sai (trung / khong trung)");
            if (ok) { coTrongCay[x] = true; ++dangCo; }
            else delete mh;
        } else {
            MatHang* mh = btRemove(t, ma);
            kiemTra((mh != nullptr) == coTrongCay[x], "btRemove tra ket qua sai");
            if (mh) {
                kiemTra(mh->maVach == ma, "btRemove tra nham mat hang");
                coTrongCay[x] = false;
                --dangCo;
                delete mh;
            }
        }
        kiemTra(t.size == dangCo, "t.size sai");
        if (b % 97 == 0) kiemTraCay(t);
    }
    kiemTraCay(t);

    // Tim kiem: co -> dung mat hang, khong co -> nullptr
    for (int x = 0; x < N; ++x) {
        MatHang* mh = btSearch(t, maTuSo(x));
        kiemTra((mh != nullptr) == coTrongCay[x], "btSearch sai");
        if (mh) kiemTra(mh->maVach == maTuSo(x), "btSearch tra nham mat hang");
    }

    // Duyet inorder tang dan, du so luong
    DuyetCtx ctx = {"", 0, true};
    btTraverse(t, ghiNhan, &ctx);
    kiemTra(ctx.tang && ctx.dem == t.size, "btTraverse khong tang dan / thieu");

    // Dem nut truy xuat
    btResetDem(t);
    kiemTra(t.soNutTruyXuat == 0, "btResetDem");
    btSearch(t, maTuSo(0));
    kiemTra(t.soNutTruyXuat >= 1 && t.soNutTruyXuat <= btHeight(t), "soNutTruyXuat ngoai [1, h]");

    int soNut = btSoNut(t), h = btHeight(t), size = t.size;

    // Xoa het -> cay rong
    for (int x = 0; x < N; ++x) {
        if (coTrongCay[x]) {
            delete btRemove(t, maTuSo(x));
            coTrongCay[x] = false;
        }
    }
    kiemTra(t.root == nullptr && t.size == 0, "xoa het nhung cay chua rong");
    kiemTra(btRemove(t, maTuSo(1)) == nullptr, "xoa tren cay rong");

    // Xay lai roi btDestroy(true): giai phong ca mat hang (kiem bang -fsanitize=address)
    for (int x = 0; x < 500; ++x) btInsert(t, taoMH(maTuSo(x)));
    btDestroy(t, true);
    kiemTra(t.root == nullptr && t.size == 0, "btDestroy");

    std::cout << "m = " << m << ": " << SO_THAO_TAC << " thao tac, size cuoi " << size
              << ", " << soNut << " nut, cao " << h
              << (soLoi == soLoiTruoc ? "  -> OK\n" : "  -> CO LOI\n");
}

void demo() {
    std::cout << "===== DEMO B-Tree bac 3: chen 1..10 roi xoa 4, 2, 6, 8 =====\n";
    BTree t;
    btInit(t, 3);
    t.inKhiTachGop = true;
    const int CHEN[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int x : CHEN) {
        std::cout << "\n> Chen " << maTuSo(x) << "\n";
        btInsert(t, taoMH(maTuSo(x)));
    }
    std::cout << "\nCay sau khi chen:\n";
    btPrint(t);
    const int XOA[] = {4, 2, 6, 8};
    for (int x : XOA) {
        std::cout << "\n> Xoa " << maTuSo(x) << "\n";
        delete btRemove(t, maTuSo(x));
    }
    std::cout << "\nCay cuoi cung:\n";
    btPrint(t);
    btDestroy(t, true);
}

} // namespace

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "demo") {
        demo();
        return 0;
    }
    std::cout << "Kiem thu B-Tree ngau nhien (doi chieu voi mang danh dau):\n";
    for (int m = 3; m <= 10; ++m) testNgauNhien(m);

    // Ca bien
    BTree t;
    btInit(t, 2);
    kiemTra(t.m == 3, "btInit(m < 3) phai dua ve m = 3");
    kiemTra(btSearch(t, "8930000000001") == nullptr && btHeight(t) == 0 && btSoNut(t) == 0,
            "cay rong");
    kiemTra(!btInsert(t, nullptr), "chen nullptr");
    btDestroy(t, true);

    std::cout << (soLoi == 0 ? "\nTAT CA DEU DUNG\n" : "\nCO LOI!\n");
    return soLoi == 0 ? 0 : 1;
}
