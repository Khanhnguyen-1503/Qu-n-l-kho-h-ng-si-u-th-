// BTree.h - B-Tree bac m, khoa = MatHang::maVach
// Phu trach: Khanh Nguyen
// Cay chi giu con tro MatHang*; viec cap phat/giai phong MatHang do Inventory quyet dinh.
#pragma once
#include <string>
#include "Types.h"

void btInit(BTree& t, int m);                 // m >= 3

// Tim theo ma vach, cong t.soNutTruyXuat moi nut di qua. O(log_m n) nut, O(m) moi nut
MatHang* btSearch(BTree& t, const std::string& maVach);

// Chen; tra ve false neu trung ma. Tach nut khi day
bool btInsert(BTree& t, MatHang* mh);

// Xoa khoa; tra ve con tro mat hang da go khoi cay (nullptr neu khong co)
// de nguoi goi tu delete. Muon/gop nut khi thieu khoa
MatHang* btRemove(BTree& t, const std::string& maVach);

// Duyet tat ca mat hang theo thu tu ma vach tang dan (inorder)
// visit(mh, ctx) duoc goi voi moi mat hang; ctx la du lieu tuy y cua nguoi goi
typedef void (*BtVisitFn)(MatHang* mh, void* ctx);
void btTraverse(const BTree& t, BtVisitFn visit, void* ctx);

int  btHeight(const BTree& t);
int  btSoNut(const BTree& t);
void btPrint(const BTree& t);                 // in cay theo muc (phan nang cao)
void btResetDem(BTree& t);                    // soNutTruyXuat = 0

// Giai phong toan bo nut; neu xoaMatHang = true thi delete luon cac MatHang (va lo cua chung)
void btDestroy(BTree& t, bool xoaMatHang);
