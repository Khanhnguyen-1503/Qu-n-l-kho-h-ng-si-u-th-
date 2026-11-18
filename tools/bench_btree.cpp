// bench_btree.cpp - Chuong trinh rieng chay thuc nghiem B-Tree (Chuong 5 bao cao)
// Phu trach: Khanh Nguyen
// Build (tu thu muc goc repo, nho bat -O2 khi do thoi gian):
//   g++ -std=c++17 -O2 -I. tools/bench_btree.cpp Benchmark.cpp BTree.cpp LotQueue.cpp -o bench_btree
// Chay:  ./bench_btree [tep.csv]     (mac dinh ketqua_btree.csv)
#include "Benchmark.h"
#include <string>

int main(int argc, char** argv) {
    std::string tep = (argc > 1) ? argv[1] : "ketqua_btree.csv";
    chayThucNghiem(tep);
    return 0;
}
