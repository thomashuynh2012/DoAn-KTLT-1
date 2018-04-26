# Đồ án 1 KTLT - Khoa CNTT - ĐHKHTN
**MSSV:** 1712909

**Họ tên:** Huỳnh Quang Vinh

**Lớp:** 17CTT7

**Email:** thomashuynh2012@gmail.com

## 1. Hướng dẫn sử dụng
**Bước 1:** Nhập tên file csv chứa thông tin sinh viên (File mặc định: "students.csv").

**Bước 2:** Chọn định dạng file csv của bạn (Đối với file mặc định là "1").

**Bước 3:** File csv của bạn có trường "EMAIL" của sinh viên không? (Đối với file mặc định là "1").

**Bước 4:** Chọn cấu hình của profile page
  - Chọn "0" để chọn chế độ mặc định (in tất cả thông tin)
  - Nếu không chọn chế độ mặc định: Nhập TỔNG SỐ TRƯỜNG sẽ xuất hiện trên profile page. Sau đó nhập số thứ tự của trường mà bạn mong muốn.
  
**Bước 5:** Nếu thành công sẽ có thông báo
```
SUCCESSFUL!
... PROFILE PAGES GENERATED
```
  
## 2. Quy ước
- Phải khai báo đúng định dạng của file csv. Nếu không sẽ gặp lỗi.
- Nếu file csv có trường EMAIL, bắt buộc phải đặt trường EMAIL ngay sau trường KHOA.

## 3. Cách thức hoạt động
- Đọc tất cả thông tin của sinh viên vào struct STUDENT.
- Copy tất cả dữ liệu từ file HTMLSource (chứa mã nguồn HTML) vào một file HTML mới tạo.
- Chỉnh sửa file HTML mới tạo theo thông tin sinh viên, bằng cách:
  - Đếm số dòng trong file HTML mới tạo.
  - Nếu đến dòng cần chỉnh sửa thì gọi hàm wprintf để chỉnh sửa
