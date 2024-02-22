# Đồ án 1 KTLT - Khoa CNTT - ĐHKHTN
**MSSV:** 1712909 | 
**Họ tên:** Huỳnh Quang Vinh | 
**Lớp:** 17CTT7 | 

### CHÚ Ý
**Các file csv để chạy thử**
- students.csv: không có dấu `"`, có email
- students2.csv: có dấu `"`, có email
- students3.csv: không có dấu `"`, không có email
- students4.csv: có dấu `"`, không có email

## I. HƯỚNG DẪN SỬ DỤNG
_Xem thêm phần **"II. Quy ước"** để tránh xảy ra lỗi_

**Bước 1:** Nhập tên file csv chứa thông tin sinh viên (file-mặc-định: "students.csv").

**Bước 2:** Chọn định dạng file csv của bạn (Đối với file-mặc-định là "1").

**Bước 3:** File csv của bạn có trường "EMAIL" của sinh viên không? (Đối với file-mặc-định là "1").

**Bước 4:** Chọn cấu hình của profile page
  - Chọn "0" để chọn chế độ mặc định (in tất cả thông tin)
  - Nếu không chọn chế độ mặc định: Nhập TỔNG SỐ TRƯỜNG sẽ xuất hiện trên profile page. Sau đó nhập số thứ tự của trường mà bạn mong muốn.
  
**Bước 5:** Nếu thành công sẽ có thông báo
```
SUCCESSFUL!
... PROFILE PAGES GENERATED
```
  
## II. QUY ƯỚC
- Phải khai báo đúng định dạng của file csv. Nếu không sẽ gặp lỗi.
- Nếu file csv có trường EMAIL, bắt buộc phải đặt trường EMAIL ngay sau trường KHOA.

## III. CÁCH THỨC CHƯƠNG TRÌNH HOẠT ĐỘNG
**1. Mở file csv do người dùng cung cấp**

**2. Mở file HTML nguồn (HTMLSource)**

**3. Khai báo thông tin từ người dùng**
  - Định dạng của file csv
  - Trường email của file csv
  - Các trường sẽ xuất hiện trên profile page

**4. Đọc thông tin từ file csv vào struct**
  - Đọc số lượng sinh viên có trong file (dựa vào số dòng)
  - Căn cứ vào định dạng file csv, dùng hàm `fwscanf` để đọc từ trường _MSSV_ đến trường _Mô tả_
  - Đếm số trường _Sở thích_ của mỗi sinh viên (dựa vào dấu `"` hoặc `,` tùy định dạng)
  - Tạo mảng các con trỏ `wchar_t **hobby` để lưu các sở thích, đọc mỗi sở thích vào mỗi con trỏ (mảng 1 chiều)
 
**5. Tạo trang HTML**
  - Tạo một file HTML mới (bằng lệnh `w`)
  - Copy tất cả dữ liệu từ file HTMLSource (chứa mã nguồn HTML) vào file HTML mới tạo.
  - Chỉnh sửa file HTML mới tạo theo thông tin sinh viên, bằng cách:
    - Đếm từng dòng trong file HTML mới tạo.
    - Nếu đến dòng cần chỉnh sửa thì gọi hàm `wprintf` để viết thông tin lên file HTML
   
## IV. CÁC CHÚ THÍCH KHÁC
**Tiếng Anh**
- no: MSSV
- faculty: khoa
- bio: mô tả bản thân
- hobby: sở thích

**Tên biến**
- `int CSVformat`: định dạng của file csv (0 hoặc 1)
- `int EmailField`: có giá trị 0 nếu không có trường email, ngược lại có giá trị 1
- Các biến có tiền tố "n" (`nHobby`,`nStudent`,...): số lượng của ...

**Tên hàm**
- `int CountLine(FILE* fp)`: đếm số dòng của file
- `bool IsInArray(int* a, int A, int number)`: kiểm tra xem số nummber có nằm trong mảng a (A phần tử) hay không
