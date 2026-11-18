Hình thức: làm theo nhóm, tối đa 4 sinh viên một đề tài; mỗi nhóm chọn một đề tài và mỗi
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
tự tạo tệp dữ liệu mẫu tối thiểu 30 bản ghi và một bộ dữ liệu lớn (sinh ngẫu nhiên, từ 104 bản
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
