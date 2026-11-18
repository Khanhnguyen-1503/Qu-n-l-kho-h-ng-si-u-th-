// BTree.cpp - Cai dat B-Tree bac m, khoa = MatHang::maVach
// Phu trach: Khanh Nguyen
//
// Quy uoc (theo Knuth, dung duoc cho moi m >= 3, ca m chan lan m le):
//   - Moi nut co toi da m-1 khoa va m con.
//   - Nut khac goc co it nhat ceil(m/2)-1 khoa; goc co it nhat 1 khoa (neu cay khong rong).
//   - Moi la nam cung mot muc.
//
// Chen: chen vao la roi di nguoc len (de quy). Nut nao bi TRAN (du m khoa) thi tach
//       thanh 2 nut, day khoa giua len cha. Goc bi tach -> cay cao them 1 muc.
//       Vi the mang keys cap m o (m-1 + 1 o tam khi tran), child cap m+1 o.
// Xoa : xoa o la; neu khoa nam o nut trong thi thay bang khoa lien truoc (lon nhat cua
//       cay con trai) roi xoa khoa do o la. Di nguoc len, nut con nao THIEU khoa thi
//       muon tu anh em trai/phai (neu anh em du), neu khong thi gop voi anh em.
//       Goc rong sau khi gop -> cay thap di 1 muc.
//
// Do phuc tap (n mat hang, chieu cao h = O(log_m n)):
//   btSearch/btInsert/btRemove: O(h) nut, moi nut tim nhi phan O(log m) + dich mang O(m)
//   btTraverse, btSoNut, btDestroy: O(n);  btHeight: O(h);  btPrint: O(h * so nut)
#include "BTree.h"
#include "LotQueue.h"
#include <iostream>

namespace {

// Thong ke cua lan chen/xoa dang chay, dung de in khi t.inKhiTachGop = true
struct ThongKe {
    int soTach;
    int soMuon;
    int soGop;
};
ThongKe tk = {0, 0, 0};

void datLaiThongKe() { tk.soTach = tk.soMuon = tk.soGop = 0; }

int soKhoaToiThieu(const BTree& t) { return (t.m + 1) / 2 - 1; }   // ceil(m/2) - 1

BTreeNode* taoNut(int m, bool laLa) {
    BTreeNode* x = new BTreeNode;
    x->n = 0;
    x->leaf = laLa;
    x->keys = new MatHang*[m];          // m-1 khoa + 1 o tam khi tran
    x->child = new BTreeNode*[m + 1];   // m con + 1 o tam khi tran
    for (int i = 0; i < m; ++i) x->keys[i] = nullptr;
    for (int i = 0; i <= m; ++i) x->child[i] = nullptr;
    return x;
}

void huyNut(BTreeNode* x) {
    delete[] x->keys;
    delete[] x->child;
    delete x;
}

// Tim nhi phan: vi tri i nho nhat (0..n) sao cho keys[i]->maVach >= k
int timViTri(const BTreeNode* x, const std::string& k) {
    int lo = 0, hi = x->n;
    while (lo < hi) {
        int giua = (lo + hi) / 2;
        if (x->keys[giua]->maVach < k) lo = giua + 1;
        else hi = giua;
    }
    return lo;
}

void inNut(const BTreeNode* x) {
    std::cout << "[";
    for (int i = 0; i < x->n; ++i) {
        if (i > 0) std::cout << " | ";
        std::cout << x->keys[i]->maVach;
    }
    std::cout << "]";
}

// ============================ CHEN ============================

// Chen khoa k vao vi tri i cua nut x, con phai cua k la 'phai' (nullptr neu x la la)
void chenVaoNut(BTreeNode* x, int i, MatHang* k, BTreeNode* phai) {
    for (int j = x->n; j > i; --j) {
        x->keys[j] = x->keys[j - 1];
        x->child[j + 1] = x->child[j];
    }
    x->keys[i] = k;
    x->child[i + 1] = phai;
    x->n++;
}

// Tach nut x dang tran (x->n == m):
//   x giu keys[0..giua-1], khoa keys[giua] day len cha, nut moi 'phai' nhan keys[giua+1..m-1]
//   voi giua = m/2 -> nut trai co m/2 khoa, nut phai co ceil(m/2)-1 khoa (deu >= toi thieu)
void tachNut(const BTree& t, BTreeNode* x, MatHang*& khoaLen, BTreeNode*& phai) {
    int giua = t.m / 2;
    phai = taoNut(t.m, x->leaf);
    phai->n = x->n - giua - 1;
    for (int j = 0; j < phai->n; ++j) {
        phai->keys[j] = x->keys[giua + 1 + j];
        x->keys[giua + 1 + j] = nullptr;
    }
    if (!x->leaf) {
        for (int j = 0; j <= phai->n; ++j) {
            phai->child[j] = x->child[giua + 1 + j];
            x->child[giua + 1 + j] = nullptr;
        }
    }
    khoaLen = x->keys[giua];
    x->keys[giua] = nullptr;
    x->n = giua;

    tk.soTach++;
    if (t.inKhiTachGop) {
        std::cout << "  [Tach nut] ";
        inNut(x);
        std::cout << " <- " << khoaLen->maVach << " (day len) -> ";
        inNut(phai);
        std::cout << "\n";
    }
}

// Chen de quy vao cay con goc x.
// Tra ve true neu x bi tach: khi do khoaLen va phai la phan nguoi goi phai chen vao cha.
bool chenDeQuy(const BTree& t, BTreeNode* x, MatHang* mh, bool& trung,
               MatHang*& khoaLen, BTreeNode*& phai) {
    int i = timViTri(x, mh->maVach);
    if (i < x->n && x->keys[i]->maVach == mh->maVach) {
        trung = true;
        return false;
    }

    if (x->leaf) {
        chenVaoNut(x, i, mh, nullptr);
    } else {
        MatHang* khoaCon = nullptr;
        BTreeNode* phaiCon = nullptr;
        if (!chenDeQuy(t, x->child[i], mh, trung, khoaCon, phaiCon)) return false;
        chenVaoNut(x, i, khoaCon, phaiCon);   // con bi tach -> nhan khoa day len
    }

    if (x->n < t.m) return false;              // chua tran
    tachNut(t, x, khoaLen, phai);
    return true;
}

// ============================ XOA ============================

// Con thu i cua p thieu khoa: lay khoa phan cach p->keys[i-1] xuong dau con,
// day khoa cuoi cua anh trai len thay; con cuoi cua anh trai chuyen sang.
void muonTrai(BTreeNode* p, int i) {
    BTreeNode* c = p->child[i];
    BTreeNode* trai = p->child[i - 1];
    for (int j = c->n; j > 0; --j) c->keys[j] = c->keys[j - 1];
    for (int j = c->n + 1; j > 0; --j) c->child[j] = c->child[j - 1];
    c->keys[0] = p->keys[i - 1];
    c->child[0] = trai->child[trai->n];
    trai->child[trai->n] = nullptr;
    c->n++;

    p->keys[i - 1] = trai->keys[trai->n - 1];
    trai->keys[trai->n - 1] = nullptr;
    trai->n--;
}

// Doi xung voi muonTrai: lay p->keys[i] xuong cuoi con, day khoa dau cua anh phai len.
void muonPhai(BTreeNode* p, int i) {
    BTreeNode* c = p->child[i];
    BTreeNode* phai = p->child[i + 1];
    c->keys[c->n] = p->keys[i];
    c->child[c->n + 1] = phai->child[0];
    c->n++;

    p->keys[i] = phai->keys[0];
    for (int j = 0; j < phai->n - 1; ++j) phai->keys[j] = phai->keys[j + 1];
    for (int j = 0; j < phai->n; ++j) phai->child[j] = phai->child[j + 1];
    phai->keys[phai->n - 1] = nullptr;
    phai->child[phai->n] = nullptr;
    phai->n--;
}

// Gop con j, khoa phan cach p->keys[j] va con j+1 thanh mot nut (con j); xoa con j+1.
// So khoa sau gop <= (min-1) + 1 + min = 2*min <= m-1 nen khong tran.
void gopNut(BTreeNode* p, int j) {
    BTreeNode* trai = p->child[j];
    BTreeNode* phai = p->child[j + 1];

    trai->keys[trai->n] = p->keys[j];
    for (int k = 0; k < phai->n; ++k) trai->keys[trai->n + 1 + k] = phai->keys[k];
    if (!trai->leaf) {
        for (int k = 0; k <= phai->n; ++k) trai->child[trai->n + 1 + k] = phai->child[k];
    }
    trai->n += 1 + phai->n;

    for (int k = j; k < p->n - 1; ++k) p->keys[k] = p->keys[k + 1];
    for (int k = j + 1; k < p->n; ++k) p->child[k] = p->child[k + 1];
    p->keys[p->n - 1] = nullptr;
    p->child[p->n] = nullptr;
    p->n--;

    huyNut(phai);
}

// Neu con thu i cua p thieu khoa thi sua: uu tien muon trai, muon phai, cuoi cung moi gop.
void suaThieu(const BTree& t, BTreeNode* p, int i) {
    int toiThieu = soKhoaToiThieu(t);
    if (p->child[i]->n >= toiThieu) return;

    const char* thaoTac;
    if (i > 0 && p->child[i - 1]->n > toiThieu) {
        muonTrai(p, i);
        tk.soMuon++;
        thaoTac = "  [Muon trai] ";
    } else if (i < p->n && p->child[i + 1]->n > toiThieu) {
        muonPhai(p, i);
        tk.soMuon++;
        thaoTac = "  [Muon phai] ";
    } else {
        if (i > 0) --i;          // gop voi anh trai; con dau tien thi gop voi anh phai
        gopNut(p, i);
        tk.soGop++;
        thaoTac = "  [Gop nut]   ";
    }

    if (t.inKhiTachGop) {
        std::cout << thaoTac << "nut cha ";
        inNut(p);
        std::cout << ", nut con sau khi sua ";
        inNut(p->child[i]);
        std::cout << "\n";
    }
}

// Go khoa lon nhat cua cay con goc x (di het ve ben phai roi lay o la)
MatHang* goKhoaLonNhat(const BTree& t, BTreeNode* x) {
    if (x->leaf) {
        x->n--;
        MatHang* kq = x->keys[x->n];
        x->keys[x->n] = nullptr;
        return kq;
    }
    int i = x->n;
    MatHang* kq = goKhoaLonNhat(t, x->child[i]);
    suaThieu(t, x, i);
    return kq;
}

MatHang* xoaDeQuy(const BTree& t, BTreeNode* x, const std::string& k) {
    int i = timViTri(x, k);

    if (i < x->n && x->keys[i]->maVach == k) {
        MatHang* kq = x->keys[i];
        if (x->leaf) {
            for (int j = i; j < x->n - 1; ++j) x->keys[j] = x->keys[j + 1];
            x->n--;
            x->keys[x->n] = nullptr;
        } else {
            // Thay bang khoa lien truoc (lon nhat cua cay con trai), roi sua cay con do
            x->keys[i] = goKhoaLonNhat(t, x->child[i]);
            suaThieu(t, x, i);
        }
        return kq;
    }

    if (x->leaf) return nullptr;               // khong co khoa
    MatHang* kq = xoaDeQuy(t, x->child[i], k);
    if (kq != nullptr) suaThieu(t, x, i);
    return kq;
}

// ============================ DUYET / DEM / IN ============================

void duyetDeQuy(const BTreeNode* x, BtVisitFn visit, void* ctx) {
    for (int i = 0; i < x->n; ++i) {
        if (!x->leaf) duyetDeQuy(x->child[i], visit, ctx);
        visit(x->keys[i], ctx);
    }
    if (!x->leaf) duyetDeQuy(x->child[x->n], visit, ctx);
}

int demNut(const BTreeNode* x) {
    if (x == nullptr) return 0;
    int dem = 1;
    if (!x->leaf) {
        for (int i = 0; i <= x->n; ++i) dem += demNut(x->child[i]);
    }
    return dem;
}

// In cac nut o muc 'muc' (goc = muc 0), tu trai sang phai
void inMuc(const BTreeNode* x, int muc) {
    if (muc == 0) {
        inNut(x);
        std::cout << "  ";
        return;
    }
    if (x->leaf) return;
    for (int i = 0; i <= x->n; ++i) inMuc(x->child[i], muc - 1);
}

void huyDeQuy(BTreeNode* x, bool xoaMatHang) {
    if (!x->leaf) {
        for (int i = 0; i <= x->n; ++i) huyDeQuy(x->child[i], xoaMatHang);
    }
    if (xoaMatHang) {
        for (int i = 0; i < x->n; ++i) {
            lqClear(x->keys[i]->dsLo);   // giai phong cac lo truoc
            delete x->keys[i];
        }
    }
    huyNut(x);
}

} // namespace

// ============================ HAM GIAO DIEN ============================

void btInit(BTree& t, int m) {
    t.root = nullptr;
    t.m = (m < 3) ? 3 : m;   // bac toi thieu la 3
    t.size = 0;
    t.soNutTruyXuat = 0;
    t.inKhiTachGop = false;
}

MatHang* btSearch(BTree& t, const std::string& maVach) {
    BTreeNode* x = t.root;
    while (x != nullptr) {
        t.soNutTruyXuat++;
        int i = timViTri(x, maVach);
        if (i < x->n && x->keys[i]->maVach == maVach) return x->keys[i];
        if (x->leaf) return nullptr;
        x = x->child[i];
    }
    return nullptr;
}

bool btInsert(BTree& t, MatHang* mh) {
    if (mh == nullptr) return false;
    datLaiThongKe();

    if (t.root == nullptr) {
        t.root = taoNut(t.m, true);
        t.root->keys[0] = mh;
        t.root->n = 1;
        t.size = 1;
        return true;
    }

    bool trung = false;
    MatHang* khoaLen = nullptr;
    BTreeNode* phai = nullptr;
    if (chenDeQuy(t, t.root, mh, trung, khoaLen, phai)) {
        // Goc bi tach -> tao goc moi, cay cao them 1 muc
        BTreeNode* goc = taoNut(t.m, false);
        goc->keys[0] = khoaLen;
        goc->child[0] = t.root;
        goc->child[1] = phai;
        goc->n = 1;
        t.root = goc;
        if (t.inKhiTachGop) std::cout << "  [Tach goc] cay cao them 1 muc\n";
    }
    if (trung) return false;

    t.size++;
    if (t.inKhiTachGop && tk.soTach > 0) {
        std::cout << "Sau khi chen " << mh->maVach << " (tach " << tk.soTach << " nut):\n";
        btPrint(t);
    }
    return true;
}

MatHang* btRemove(BTree& t, const std::string& maVach) {
    if (t.root == nullptr) return nullptr;
    datLaiThongKe();

    MatHang* kq = xoaDeQuy(t, t.root, maVach);
    if (kq == nullptr) return nullptr;
    t.size--;

    if (t.root->n == 0) {
        BTreeNode* gocCu = t.root;
        t.root = gocCu->leaf ? nullptr : gocCu->child[0];   // goc rong -> cay thap di 1 muc
        huyNut(gocCu);
        if (t.inKhiTachGop && t.root != nullptr) std::cout << "  [Ha goc] cay thap di 1 muc\n";
    }

    if (t.inKhiTachGop && (tk.soMuon > 0 || tk.soGop > 0)) {
        std::cout << "Sau khi xoa " << maVach << " (muon " << tk.soMuon
                  << ", gop " << tk.soGop << "):\n";
        btPrint(t);
    }
    return kq;
}

void btTraverse(const BTree& t, BtVisitFn visit, void* ctx) {
    if (t.root != nullptr && visit != nullptr) duyetDeQuy(t.root, visit, ctx);
}

int btHeight(const BTree& t) {
    int h = 0;
    for (const BTreeNode* x = t.root; x != nullptr; x = x->leaf ? nullptr : x->child[0]) ++h;
    return h;
}

int btSoNut(const BTree& t) {
    return demNut(t.root);
}

void btPrint(const BTree& t) {
    if (t.root == nullptr) {
        std::cout << "  (cay rong)\n";
        return;
    }
    int h = btHeight(t);
    for (int muc = 0; muc < h; ++muc) {
        std::cout << "  Muc " << muc << ": ";
        inMuc(t.root, muc);
        std::cout << "\n";
    }
}

void btResetDem(BTree& t) {
    t.soNutTruyXuat = 0;
}

void btDestroy(BTree& t, bool xoaMatHang) {
    if (t.root != nullptr) huyDeQuy(t.root, xoaMatHang);
    t.root = nullptr;
    t.size = 0;
}
