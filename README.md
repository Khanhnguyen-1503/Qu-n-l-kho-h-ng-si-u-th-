# Đề tài 9 – Quản lý kho hàng siêu thị

Ngôn ngữ C++17, các cấu trúc dữ liệu lõi tự cài đặt (B-Tree, hàng đợi FIFO bằng DSLK, DSLK phiếu, Merge Sort).

**Hạn nộp: 23/11/2026** – mã nguồn, báo cáo (15–30 trang), slide (10–15 trang).

> Trạng thái: 🟡 **In process** = đang làm · ✅ **Done** = xong (đổi trạng thái khi push code đã ghép thử chạy được).

---

## 1. Phân công code

| Thành viên | Phụ trách | Tệp nguồn | Deadline | Trạng thái |
|---|---|---|---|---|
| Khánh Nguyên | B-Tree bậc m, khóa mã vạch: tìm, chèn (tách nút), xóa (mượn, gộp nút), duyệt toàn bộ | `BTree.h/.cpp` | 19/11 | 🟡 In process |
| Minh Tân | Hàng đợi FIFO các lô nhập (DSLK) cho từng mặt hàng; phiếu nhập tạo lô mới, phiếu xuất lấy từ lô cũ nhất (không đủ thì lấy tiếp lô sau); cập nhật tồn kho; lưu danh sách phiếu (DSLK) | `LotQueue.h/.cpp`, `Phieu.h/.cpp`, `Inventory.h/.cpp` | 19/11 | 🟡 In process |
| Long Vũ | Tìm theo tên (chuỗi con), liệt kê theo nhóm; tự cài Merge Sort; cảnh báo tồn dưới ngưỡng; cảnh báo hết hạn trong 7 ngày (sắp theo HSD); giá trị tồn theo nhóm; top 10 bán chạy trong tháng | `Sort.h/.cpp`, `Report.h/.cpp` | 19/11 | 🟡 In process |
| Huỳnh Tín | Kiểu Date; đọc tệp khi khởi động, ghi khi thoát; sinh dữ liệu mẫu ≥30 và bộ lớn ≥10⁴ (10⁵ cho nâng cao); menu `main.cpp`; kiểm tra dữ liệu nhập; đo thời gian; kiểm tra rò rỉ bộ nhớ | `Date.h/.cpp`, `FileIO.h/.cpp`, `DataGen.cpp`, `Validate.h`, `main.cpp`, `Benchmark.cpp` | 20/11 | 🟡 In process |
| Cả nhóm | Chốt các struct dùng chung | `Types.h` | 16/11 | 🟡 In process |

### Chi tiết công việc từng người

#### Khánh Nguyên – B-Tree (`BTree.h/.cpp`, `Benchmark.h`)
| Việc | Deadline | Trạng thái |
|---|---|---|
| `btInit`, `btSearch` (đếm `soNutTruyXuat`), `btInsert` (tách nút) | 18/11 | 🟡 In process |
| `btTraverse`, `btHeight`, `btSoNut`, `btDestroy` | 18/11 | 🟡 In process |
| `btRemove` (mượn, gộp nút) | 19/11 | 🟡 In process |
| Nâng cao: `btPrint` in cây sau mỗi lần tách/gộp | 21/11 | 🟡 In process |
| Nâng cao: đếm số nút truy xuất với m = 3, 5, 10 trên 10⁵ mặt hàng (`doBTree`, `chayThucNghiem`, cùng Huỳnh Tín) | 21/11 | 🟡 In process |

#### Minh Tân – Lô hàng & phiếu (`LotQueue`, `Phieu`, `Inventory`)
| Việc | Deadline | Trạng thái |
|---|---|---|
| `lqInit`, `lqEnqueue`, `lqFront`, `lqDequeue`, `lqTongSoLuong`, `lqClear`, `taoLo` | 19/11 | 🟡 In process |
| `dspInit`, `dspThem`, `dspThemCoSan`, `dspTimTheoMa`, `dspIn`, `dspClear` | 19/11 | 🟡 In process |
| `struct Kho`; `themMatHang`, `suaMatHang`, `xoaMatHang`, `timMatHang` | 19/11 | 🟡 In process |
| `nhapKho`, `xuatKho` (FIFO), `inChiTietMatHang` | 19/11 | 🟡 In process |

#### Long Vũ – Tìm kiếm, sắp xếp, báo cáo (`Sort`, `Report`)
| Việc | Deadline | Trạng thái |
|---|---|---|
| Mảng động `DsMatHang` (`dsInit`, `dsThem`, `dsClear`) | 19/11 | 🟡 In process |
| Merge Sort theo tên, tồn kho, HSD, bán chạy, giá trị nhóm | 19/11 | 🟡 In process |
| `timTheoTen`, `lietKeTheoNhom` | 19/11 | 🟡 In process |
| `canhBaoSapHetHang`, `canhBaoSapHetHan` (7 ngày) | 19/11 | 🟡 In process |
| `baoCaoGiaTriTheoNhom`, `topBanChay` (top 10 trong tháng) | 19/11 | 🟡 In process |
| Các hàm in bảng kết quả | 19/11 | 🟡 In process |

#### Huỳnh Tín – Tích hợp & dữ liệu (`Date`, `FileIO`, `DataGen`, `Validate`, `main`, `Benchmark`)
| Việc | Deadline | Trạng thái |
|---|---|---|
| `Date.cpp`: kiểm tra ngày, đổi sang số ngày, cộng/trừ/so sánh, `parseDate`, `formatDate`, `homNay` | 16/11 | 🟡 In process |
| Sinh dữ liệu mẫu ≥30 bản ghi | 16/11 | 🟡 In process |
| Sinh bộ dữ liệu lớn 10⁴–10⁵ bản ghi (`sinhDuLieu`) | 19/11 | 🟡 In process |
| `FileIO.cpp`: đọc/ghi `mathang.txt`, `lo.txt`, `phieu.txt` | 19/11 | 🟡 In process |
| `Validate`: nhập số, chuỗi, ngày, mã vạch có kiểm tra sai kiểu, sai miền | 19/11 | 🟡 In process |
| `main.cpp`: menu, ghép các mô-đun | 20/11 | 🟡 In process |
| Viết test case, kiểm tra rò rỉ bộ nhớ (valgrind hoặc đếm new/delete) | 20/11 | 🟡 In process |
| `Benchmark.cpp`: đo thời gian chạy (cùng Khánh Nguyên) | 21/11 | 🟡 In process |

---

## 2. Phân công báo cáo và slide

| Chương (theo Mục IV) | Người viết | Deadline | Trạng thái |
|---|---|---|---|
| 1. Giới thiệu, bảng phân công | Huỳnh Tín | 21/11 | 🟡 In process |
| 2. Phân tích thiết kế: lý do chọn cấu trúc (so với ít nhất một phương án khác, ví dụ B-Tree với AVL, hàng đợi DSLK với mảng vòng); sơ đồ mô-đun | Cả nhóm, Khánh Nguyên tổng hợp | 21/11 | 🟡 In process |
| 3. Giải thuật: mã giả, ví dụ chạy tay, độ phức tạp | Mỗi người viết phần mình code | 19/11 | 🟡 In process |
| 4. Cài đặt và kiểm thử: test case (bình thường, biên, sai), ảnh chụp màn hình | Huỳnh Tín (mỗi người gửi test case phần mình) | 21/11 | 🟡 In process |
| 5. Thực nghiệm: bảng và biểu đồ thời gian, so sánh m = 3, 5, 10 | Khánh Nguyên và Huỳnh Tín | 21/11 | 🟡 In process |
| 6. Kết luận, tài liệu tham khảo | Long Vũ | 21/11 | 🟡 In process |
| Slide thuyết trình (10–15 trang) | Long Vũ làm chính, mỗi người gửi 2–3 slide phần mình | 21/11 | 🟡 In process |

---

## 3. Tiến độ 1 tuần

Làm chức năng bắt buộc trước, phần nâng cao chỉ làm nếu còn thời gian. Viết báo cáo song song với code từ ngày 2; mỗi tối cả nhóm push code và ghép thử.

| Ngày | Công việc | Trạng thái |
|---|---|---|
| Ngày 1 (16/11) | Chốt struct trong `Types.h`, tên hàm từng mô-đun và định dạng tệp dữ liệu. Huỳnh Tín làm xong Date và sinh dữ liệu mẫu | 🟡 In process |
| Ngày 2–4 (17–19/11) | Code song song. Khánh Nguyên: tìm và chèn ngày 2–3, xóa ngày 4. Minh Tân, Long Vũ, Huỳnh Tín làm phần mình và viết Chương 3 (mã giả, độ phức tạp) | 🟡 In process |
| Ngày 5 (20/11) | Ghép vào menu, sửa lỗi, kiểm tra rò rỉ bộ nhớ. Huỳnh Tín viết test case | 🟡 In process |
| Ngày 6 (21/11) | Chạy thực nghiệm, làm nâng cao nếu kịp. Hoàn thiện báo cáo; Long Vũ làm slide | 🟡 In process |
| Ngày 7 (22/11) | Đọc chéo code (Khánh Nguyên ↔ Minh Tân, Long Vũ ↔ Huỳnh Tín), tập vấn đáp, chạy thử phần trình bày 10 phút. Ai cũng phải nắm chèn/tách nút B-Tree và xuất kho FIFO | 🟡 In process |
| **23/11** | **Nộp mã nguồn, báo cáo, slide** | 🟡 In process |

---

## 4. Quy ước chung

- Hạn sử dụng gắn với **từng lô**, không gắn với mặt hàng, vì mỗi lần nhập có HSD khác nhau.
- Muốn sửa struct trong `Types.h` phải báo cả nhóm trước.
- B-Tree chỉ giữ con trỏ `MatHang*`. `btRemove` gỡ mặt hàng khỏi cây và trả con trỏ về; **Inventory (Minh Tân) là nơi `delete`**, tránh giải phóng hai lần.
- `tonKho` không ghi ra tệp. Khi đọc tệp, cộng số lượng các lô để tính lại, nên tồn kho không bao giờ lệch với các lô.
- Định dạng tệp (phân cách `|`, ngày `dd/mm/yyyy`):
  - `mathang.txt`: `maVach|ten|nhom|donGia|nguongCanhBao`
  - `lo.txt`: `maVach|maLo|ngayNhap|hsd|soLuong|giaNhap` (ghi theo thứ tự FIFO)
  - `phieu.txt`: `maPhieu|loai|ngay|maVach|soLuong|donGia` (loai: 0 = nhập, 1 = xuất)
- Top bán chạy: đặt `demTam = 0`, duyệt phiếu xuất trong tháng, `btSearch` rồi cộng `demTam`, gom mặt hàng có `demTam > 0`, sắp giảm dần và lấy 10.

---

## 5. Yêu cầu chung của môn học

1. Hình thức: làm theo nhóm, tối đa 4 sinh viên một đề tài; mỗi nhóm chọn một đề tài và mỗi
đề tài chỉ do một nhóm thực hiện trong một lớp. Nhóm đăng ký đề tài với giảng viên trong
tuần thứ 4 của học kỳ.
2. Ngôn ngữ: C/C++ (chuẩn C++17). Các cấu trúc dữ liệu chính của đề tài (danh sách liên kết,
ngăn xếp, hàng đợi, cây, bảng băm) phải tự cài đặt; không dùng vector, list, stack,
queue, map, set, unordered_map, priority_queue và các hàm sort, find của
STL cho phần lõi. Được dùng string, iostream, fstream.
3. Giao diện: chương trình có thể chạy trên console (menu văn bản) hoặc có giao diện đồ
họa (GUI) xây dựng bằng thư viện C/C++ như Qt, wxWidgets, Dear ImGui, SFML, Raylib
hoặc Windows Forms (C++/CLI). Khi làm giao diện đồ họa, mã nguồn phải tách riêng phần
lõi (các cấu trúc dữ liệu, giải thuật tự cài đặt, đặt trong các tệp .h, .cpp riêng, không phụ
thuộc thư viện giao diện) và phần giao diện chỉ gọi các hàm của phần lõi; không dùng các
lớp chứa dữ liệu, mô hình dữ liệu sẵn có của thư viện giao diện (như QList, QMap, QHash,
QStandardItemModel) để thay cho cấu trúc dữ liệu phải tự cài đặt. Giao diện đồ họa
trực quan hóa được cấu trúc dữ liệu (vẽ danh sách liên kết, cây, bảng băm, các bước sắp xếp)
được đánh giá cao ở tiêu chí chức năng chương trình.
4. Dữ liệu: chương trình đọc dữ liệu từ tệp văn bản khi khởi động và ghi lại khi kết thúc; nhóm
tự tạo tệp dữ liệu mẫu tối thiểu 30 bản ghi và một bộ dữ liệu lớn (sinh ngẫu nhiên, từ 10⁴ bản
ghi trở lên) để đo thời gian.
5. Sản phẩm nộp: (a) mã nguồn chia thành nhiều tệp (.h, .cpp) kèm tệp dữ liệu; (b) báo cáo
theo cấu trúc ở Mục IV (15–30 trang); (c) slide thuyết trình 10–15 trang.
6. Bảo vệ: mỗi nhóm trình bày 10 phút, chạy chương trình trực tiếp và trả lời câu hỏi; mọi
thành viên phải giải thích được mã nguồn của nhóm. Nhóm sao chép mã nguồn của nhóm
khác hoặc từ Internet mà không ghi nguồn sẽ nhận điểm 0.

Yêu cầu chung cho mọi đề tài: chương trình có menu hoặc giao diện đồ họa rõ ràng; kiểm tra
dữ liệu nhập (mã trùng, giá trị sai kiểu, sai miền); giải phóng toàn bộ bộ nhớ cấp phát động trước
khi thoát; báo cáo phải nêu lý do chọn cấu trúc dữ liệu, đánh giá độ phức tạp của từng chức năng
chính và có ít nhất một bảng số liệu đo thời gian thực nghiệm.
