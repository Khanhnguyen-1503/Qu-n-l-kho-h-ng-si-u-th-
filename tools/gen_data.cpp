// gen_data.cpp - Chuong trinh rieng sinh bo du lieu mau vao thu muc hien hanh
// Phu trach: Huynh Tin
// Build: g++ -std=c++17 -I. tools/gen_data.cpp DataGen.cpp Date.cpp -o gen_data
#include <iostream>
#include "DataGen.h"

int main() {
    std::cout << "Dang sinh du lieu mau (35 mat hang)...\n";
    
    sinhDuLieuMau(35, "mathang.txt", "lo.txt", "phieu.txt");
    
    std::cout << "Hoan tat! Da tao xong mathang.txt, lo.txt, phieu.txt.\n";
    return 0;
}