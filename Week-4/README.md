# Week 4

*Directory chứa bài tập về nhà trong tuần 4, có tổng cộng 4 bài tập, các bài đôi một đi kèm với nhau vì mỗi bài yêu cầu lập trình riêng biệt server hoặc client.*

## Đề Bài

*Đề bài nằm ở các **slide 136 và 137** trong [file PDF bài giảng](../Week-1/Lap_trinh_mang_IT4060.pdf) trong directory Week-1.*

### Bài 1

Viết chương trình tcp_client, kết nối đến một máy chủ xác định bởi địa chỉ IP và cổng. Sau đó nhận dữ liệu từ bàn phím và gửi đến server. Tham số được
truyền vào từ dòng lệnh có dạng:

> ``tcp_client  địa_chỉ_IP  cổng``

### Bài 2

Viết chương trình tcp_server, đợi kết nối ở cổng xác định bởi tham số dòng lệnh. Mỗi khi có client kết nối đến, thì gửi xâu chào được chỉ ra trong một tệp tin xác định, sau đó ghi toàn bộ nội dung client gửi đến vào một tệp tin khác được chỉ ra trong tham số dòng lệnh:

> ``tcp_server cổng  file_chứa_câu_chào  file_chứa_nội_dung_client_gửi``

### Bài 3

Viết chương trình sv_client, cho phép người dùng nhập dữ liệu là thông tin của sinh viên bao gồm **MSSV, Họ tên, ngày sinh, và điểm trung bình các môn học**. Các thông tin trên được đóng gói và gửi sang sv_server. Yêu cầu số byte cần gửi là nhỏ nhất có thể. Địa chỉ server được nhập từ tham số dòng lệnh.

### Bài 4

Viết chương trình sv_server, nhận dữ liệu từ sv_client, in ra màn hình và đồng thời ghi vào file sv_log. Ngoài dữ liệu được gửi từ client cần phải lưu
lại địa chỉ và thời gian client đã gửi. Tham số cổng và tên file log được nhập từ tham số dòng lệnh.

## Cấu Trúc Của Directory

Directory bao gồm 2 thư mục *tcp_programs_src* và *sv_programs_src* lần lượt lưu mã nguồn của bài tập 1 - 2 và bài tập 3 - 4.

Riêng trong thư mục *sv_client* có hai phiên bản client khác nhau: sv_client.c và sv_client2.c