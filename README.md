# BÁO CÁO PHÂN TÍCH LỖI LOGIC HỆ THỐNG KIỂM ĐỊNH NHẬP KHO WAREHOUSE_LOGISTICS

> 👤 **Học viên:** Hoàng Công Dũng | **Mã SV:** PTIT-HCM-009
> 🏫 **Môn học:** it102-rl-hoangcongdung

---

## 📊 Sơ đồ thiết kế hệ thống (Activity Diagram)

> 💡 *Sơ đồ dưới đây được render tự động trực tiếp trên GitHub bằng Mermaid. Bạn cũng có thể tải file **`bt2.drawio`** trong repository này để mở và chỉnh sửa trực tiếp trên [Draw.io (diagrams.net)](https://app.diagrams.net).* 

```mermaid
graph TD
  Start([Bắt đầu kiểm định]) --> CheckWeight{Tổng trọng lượng hiện tại + kiện mới > 500.0 kg?}
  CheckWeight -- Đúng --> RejectWeight[Từ chối: Vi phạm tải trọng kệ] --> EndFail([Kết thúc: Từ chối nhập kho])
  CheckWeight -- Sai --> CheckCold{Kệ thuộc kho lạnh: la_kho_lanh == 1?}
  CheckCold -- Không --> AcceptNormal([Kết thúc: Cho phép nhập kho thường])
  CheckCold -- Có --> CheckTemp{Nhiệt độ kiện hàng nằm trong [-18.0, 5.0]?}
  CheckTemp -- Sai --> RejectTemp[Từ chối: Không phù hợp nhiệt độ kho lạnh] --> EndFail
  CheckTemp -- Đúng --> AcceptCold([Kết thúc: Cho phép nhập kho lạnh])
```

---

## 1. Mục tiêu và Bối cảnh bài toán

Bài tập tập trung củng cố kỹ năng đọc hiểu mã nguồn C kế thừa (legacy code), dò luồng thực thi và phát hiện lỗ hổng logic điều kiện trong phân hệ kiểm định nhập kho của phần mềm WAREHOUSE_LOGISTICS.

Quy định kho bãi yêu cầu kiểm soát chặt chẽ tải trọng tối đa 500.0 kg cho mỗi ô kệ, và quy tắc nghiêm ngặt về nhiệt độ từ -18.0°C đến 5.0°C đối với các kiện hàng xếp vào khu vực kho lạnh.

- Kiểm tra tải trọng: Tổng trọng lượng hiện tại cộng kiện hàng mới không được vượt quá 500.0 kg.
- Kiểm tra kho lạnh: Nếu la_kho_lanh == 1, nhiệt độ yêu cầu bắt buộc phải nằm trong đoạn [-18.0, 5.0].

## 2. Phân tích lỗi sai AS-IS và Test Case chứng minh

Trong đoạn mã nguồn hiện tại, hàm kiem_tra_nhap_kho đang sử dụng biểu thức điều kiện sai logic tại phần kiểm tra nhiệt độ kho lạnh.

Cụ thể, dòng code 'if (kien.nhiet_do_yeu_cau >= -18.0 || kien.nhiet_do_yeu_cau <= 5.0)' dùng toán tử logic OR (||). Do mọi số thực đều hoặc lớn hơn hoặc bằng -18.0 hoặc nhỏ hơn hoặc bằng 5.0, biểu thức này luôn trả về giá trị đúng (true) với mọi giá trị nhiệt độ, khiến các kiện hàng nóng (ví dụ 25.0°C) hoặc quá lạnh (-25.0°C) vẫn lọt qua hàng rào kiểm định.

- Vùng code lỗi: Dòng kiểm tra nhiệt độ trong hàm kiem_tra_nhap_kho.
- Nguyên nhân: Sử dụng toán tử OR (||) thay vì AND (&&), kết hợp với điều kiện bao hàm sai phạm.
- Actual Output (Kết hiện tại): Chương trình in ra 'KET QUA: CHO PHEPT NHAP KHO!' đối với kiện hàng linh kiện điện tử 25.0°C.
- Expected Output (Kết quả mong đợi): Chương trình phải từ chối ngay lập tức và in ra 'KET QUA: TU CHOI NHAP KHO (Vi pham an toan)!'.

## 3. Bảng đặc tả chi tiết quy trình kiểm định nhập kho

Dưới đây là bảng mô tả chi tiết các bước xử lý logic sau khi đã vá lỗi phần điều kiện:

| Bước | Tác nhân | Hành động nghiệp vụ | Điều kiện / Kết quả kiểm tra |
| --- | --- | --- | --- |
| 1 | Hệ thống | Tiếp nhận thông tin kệ và kiện hàng mới | Đọc trọng lượng và nhiệt độ yêu cầu |
| 2 | Hệ thống | Kiểm tra tải trọng ô kệ | Nếu (trong_luong_hien_tai + trong_luong) > 500.0 -> Từ chối |
| 3 | Hệ thống | Kiểm tra thuộc tính kho lạnh | Nếu la_kho_lanh == 0 -> Cho phép nhập kho thường |
| 4 | Hệ thống | Kiểm tra dải nhiệt độ kho lạnh | Nếu nhiet_do < -18.0 hoặc nhiet_do > 5.0 -> Từ chối |
| 5 | Hệ thống | Phê duyệt cuối cùng | Cho phép nhập kho lạnh nếu thỏa mãn toàn bộ điều kiện |

## 4. Mã nguồn C hoàn chỉnh và Kiểm thử

File mã nguồn 'debug_inbound.c' đã được chỉnh sửa lại toán tử logic thành AND (&&) và bổ sung các test case trong hàm main để kiểm tra đầy đủ 3 trường hợp nghiệp vụ theo yêu cầu.

- Trường hợp 1: Kiện hàng hợp lệ vào kho lạnh (-10.0°C, 100.0 kg).
- Trường hợp 2: Kiện hàng không đủ điều kiện nhiệt độ kho lạnh (25.0°C và -25.0°C).
- Trường hợp 3: Kiện hàng vượt quá tải trọng tối đa 500.0 kg.

---

## 📁 Danh sách tệp tin nộp bài trong Repository
- 📝 `bt2.docx`: Báo cáo tài liệu phân tích nghiệp vụ hoàn chỉnh.
- 🎨 `bt2.drawio`: File thiết kế sơ đồ chuẩn theo quy định đề bài (mở trực tiếp bằng [Draw.io](https://app.diagrams.net) hoặc Lucidchart).
- 💻 `debug_inbound.c`: Mã nguồn chương trình.
