/*************************************************************************
* PROJECT 4 - QUAN LY BAN HANG ONLINE (VERSION 2.15 - FINAL BUFFER FIX)
* Yeu cau: OOP, Template, Giao dien Console, Doc/Ghi File
* Su dung C++26
* Fixes:
* - **Sua loi 'HangGia' -> 'HangHoa' (Loi compile)**
* - **ĐÃ SỬA: Lỗi không nhập được Y/N ở XuLyDonHang do xung đột _getch()**
* - **ĐÃ THÊM: Hỗ trợ ESC để thoát khỏi các chức năng nhập liệu (RB 1.3)**
* - Đã áp dụng các fix logic v2.10, v2.11, v2.13.
*************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>      // For setw
#include <windows.h>    // For console API (colors, cursor position)
#include <conio.h>      // For _getch()
#include <sstream>      // For string streams (data conversion)
#include <algorithm>    // For transform (to_upper, to_lower)
#include <ctime>        // For getting current date
#include <queue>        // For Queue
#include <limits>       // **THEM VAO** de su dung numeric_limits (cho cin.ignore)

using namespace std;

// === CONFIG ===
const int CONSOLE_WIDTH = 90; // Chieu rong console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// ================= Utility console functions =================
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(hConsole, coord);
}

void SetColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void clearScreen() {
    system("cls");
}

void VeKhung(int x1, int y1, int x2, int y2, int color = 11) {
    SetColor(color);
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y1); cout << (char)205; // Canh tren
        gotoxy(i, y2); cout << (char)205; // Canh duoi
    }
    for (int i = y1; i <= y2; i++) {
        gotoxy(x1, i); cout << (char)186; // Canh trai
        gotoxy(x2, i); cout << (char)186; // Canh phai
    }
    gotoxy(x1, y1); cout << (char)201; // Goc tren-trai
    gotoxy(x2, y1); cout << (char)187; // Goc tren-phai
    gotoxy(x1, y2); cout << (char)200; // Goc duoi-trai
    gotoxy(x2, y2); cout << (char)188; // Goc duoi-phai
    SetColor(15);
}

void VeTieuDe(const string& tieuDe, int color = 14) {
    clearScreen();
    SetColor(color);
    int len = (int)tieuDe.length();
    int x = (CONSOLE_WIDTH - len) / 2;
    if (x < 0) x = 0;
    gotoxy(max(0, x - 3), 1);
    cout << " * * * " << tieuDe << " * * * ";
    VeKhung(0, 0, CONSOLE_WIDTH - 1, 24, 11);
    SetColor(15);
    gotoxy(2, 3);
}

string LayNgayHienTai() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << ltm->tm_mday << "/" << (1 + ltm->tm_mon) << "/" << (1900 + ltm->tm_year);
    return ss.str();
}

string StringToUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string NhapMatKhau() {
    string matKhau = "";
    char ch;
    while ((ch = _getch()) != 13) { // Enter
        if (ch == 8) { // backspace
            if (!matKhau.empty()) {
                matKhau.pop_back();
                cout << "\b \b";
            }
        } else if (ch == 27) { // ESC (RB 1.3)
            return "";
        } else {
            matKhau.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return matKhau;
}

void ThongBao(const string& thongBao, int color = 10, int delay = 1500) {
    int x = (CONSOLE_WIDTH - (int)thongBao.length()) / 2;
    if (x < 0) x = 0;
    gotoxy(x, 23);
    SetColor(color);
    cout << thongBao;
    Sleep(delay);
    gotoxy(x, 23);
    for (size_t i = 0; i < thongBao.length(); i++) cout << " ";
    SetColor(15);
    gotoxy(2, 23);
}

// ================ ReadKeySkipCR - đọc phím an toàn (Chỉ dùng cho các câu hỏi Y/N) =================
char ReadKeySkipCR() {
    char c;
    do {
        c = _getch();
    } while (c == 13 || c == 10); // bỏ qua CR/LF
    cout << c;
    return c;
}

// ==================== CLASSES ====================
class Admin {
public:
    string user;
    string pass;
    bool DocFile(ifstream& f) {
        if (!getline(f, user, ',')) return false;
        if (!getline(f, pass)) pass = "";
        return !user.empty();
    }
    void GhiFile(ofstream& f) {
        f << user << "," << pass << endl;
    }
};

class HangHoa {
public:
    string maHang; // 4 ky tu
    string tenHang;
    string noiSanXuat;
    string mauSac;
    double giaBan = 0;
    string ngayNhapKho;
    int soLuong = 0;

    bool DocFile(ifstream& f) {
        string temp;
        if (!getline(f, maHang, ',')) return false;
        getline(f, tenHang, ',');
        getline(f, noiSanXuat, ',');
        getline(f, mauSac, ',');
        getline(f, temp, ',');
        giaBan = (temp.empty() ? 0 : stod(temp));
        getline(f, ngayNhapKho, ',');
        if (!getline(f, temp)) temp = "0";
        soLuong = (temp.empty() ? 0 : stoi(temp));
        return !maHang.empty();
    }

    void GhiFile(ofstream& f) {
        f << maHang << "," << tenHang << "," << noiSanXuat << "," << mauSac << ","
          << giaBan << "," << ngayNhapKho << "," << soLuong << endl;
    }

    void HienThi(int y) {
        SetColor(15);
        gotoxy(2, y);  cout << maHang;
        gotoxy(15, y); cout << tenHang;
        gotoxy(32, y); cout << noiSanXuat;
        gotoxy(50, y); cout << mauSac;
        gotoxy(60, y); cout << fixed << setprecision(0) << giaBan;
        gotoxy(72, y); cout << ngayNhapKho;
        gotoxy(84, y); cout << soLuong;
    }
};

class ChiTietDonHang {
public:
    string maHang;
    int soLuongDat = 0;
    double donGia = 0;

    void GhiFile(ofstream& f) {
        f << maHang << "," << soLuongDat << "," << donGia << ",";
    }

    bool DocFile(ifstream& f) {
        string temp;
        if (!getline(f, maHang, ',')) return false;
        if (!getline(f, temp, ',')) temp = "0";
        soLuongDat = (temp.empty() ? 0 : stoi(temp));
        if (!getline(f, temp, ',')) temp = "0";
        donGia = (temp.empty() ? 0 : stod(temp));
        return !maHang.empty();
    }
};

class DonHang {
public:
    int stt = 0;
    vector<ChiTietDonHang> dsChiTiet;
    double tongTien = 0;
    string tenKhachHang;
    string diaChi;
    string sdt;
    string ngayDatHang;

    void TinhTongTien() {
        tongTien = 0;
        for (size_t i = 0; i < dsChiTiet.size(); ++i) {
            tongTien += dsChiTiet[i].soLuongDat * dsChiTiet[i].donGia;
        }
    }

    void GhiFile(ofstream& f) {
        f << stt << ",";
        f << tenKhachHang << "," << diaChi << "," << sdt << "," << ngayDatHang << ",";
        f << tongTien << ",";
        f << dsChiTiet.size() << ",";
        for (size_t i = 0; i < dsChiTiet.size(); ++i) dsChiTiet[i].GhiFile(f);
        f << endl;
    }

    bool DocFile(ifstream& f) {
        string temp;
        if (!getline(f, temp, ',')) return false;
        if (temp.empty()) return false;
        stt = stoi(temp);
        if (!getline(f, tenKhachHang, ',')) return false;
        getline(f, diaChi, ',');
        getline(f, sdt, ',');
        getline(f, ngayDatHang, ',');

        getline(f, temp, ',');
        tongTien = (temp.empty() ? 0 : stod(temp));
        getline(f, temp, ',');
        int soLuongMatHang = (temp.empty() ? 0 : stoi(temp));

        dsChiTiet.clear();
        for (int i = 0; i < soLuongMatHang; ++i) {
            ChiTietDonHang ct;
            ct.DocFile(f);
            dsChiTiet.push_back(ct);
        }
        // consume newline
        getline(f, temp);
        return !tenKhachHang.empty();
    }
};

// ================= Template Linked List =================
template <class T>
class Node {
public:
    T data;
    Node* pNext;
    Node(const T& value) : data(value), pNext(NULL) {}
};

template <class T>
class DanhSach {
public:
    Node<T>* pHead;
    Node<T>* pTail;
    DanhSach() : pHead(NULL), pTail(NULL) {}
    ~DanhSach() {
        while (pHead != NULL) {
            Node<T>* temp = pHead;
            pHead = pHead->pNext;
            delete temp;
        }
    }

    void ThemVaoCuoi(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (pHead == NULL) {
            pHead = pTail = newNode;
        } else {
            pTail->pNext = newNode;
            pTail = newNode;
        }
    }

    void XoaDau() {
        if (pHead == NULL) return;
        Node<T>* temp = pHead;
        pHead = pHead->pNext;
        delete temp;
        if (pHead == NULL) pTail = NULL;
    }
};

// ================= Specialized Lists =================
class DanhSachAdmin : public DanhSach<Admin> {
public:
    Node<Admin>* TimTheoUser(const string& user) {
        Node<Admin>* p = pHead;
        while (p != NULL) {
            if (p->data.user == user) return p;
            p = p->pNext;
        }
        return NULL;
    }
};

class DanhSachHangHoa : public DanhSach<HangHoa> {
public:
    Node<HangHoa>* TimTheoMa(const string& ma) {
        Node<HangHoa>* p = pHead;
        while (p != NULL) {
            if (p->data.maHang == ma) return p;
            p = p->pNext;
        }
        return NULL;
    }

    bool XoaTheoMa(const string& ma) {
        if (pHead == NULL) return false;
        if (pHead->data.maHang == ma) {
            XoaDau();
            return true;
        }
        Node<HangHoa>* p = pHead;
        while (p->pNext != NULL) {
            if (p->pNext->data.maHang == ma) {
                Node<HangHoa>* temp = p->pNext;
                p->pNext = temp->pNext;
                if (temp == pTail) pTail = p;
                delete temp;
                return true;
            }
            p = p->pNext;
        }
        return false;
    }

    vector<Node<HangHoa>*> TimTheoTen(const string& ten) {
        vector<Node<HangHoa>*> ketQua;
        string tenTim = StringToUpper(ten);
        Node<HangHoa>* p = pHead;
        while (p != NULL) {
            if (StringToUpper(p->data.tenHang).find(tenTim) != string::npos) ketQua.push_back(p);
            p = p->pNext;
        }
        return ketQua;
    }
};

// ================= Global data =================
DanhSachAdmin dsAdmin;
DanhSachHangHoa dsHangHoa;
queue<DonHang> dsDonHangCho;

// ================= File IO helpers =================
template <class T>
void DocFileVaoDanhSach(DanhSach<T>& ds, const string& tenFile) {
    ifstream f(tenFile);
    if (!f.is_open()) {
        ofstream f_out(tenFile);
        if (tenFile == "Admin.txt") {
            f_out << "admin,123" << endl;
        } else if (tenFile == "HangHoa.txt") {
            // sample
            f_out << "A001,Ao So Mi,Viet Nam,Trang,250000,01/01/2023,50" << endl;
            f_out << "Q001,Quan Jean,Campuchia,Xanh,450000,01/01/2023,30" << endl;
            f_out << "V001,Vay Dam,Trung Quoc,Do,650000,01/01/2023,20" << endl;
        }
        f_out.close();
        f.open(tenFile);
    }

    while (!f.eof()) {
        T data;
        if (!data.DocFile(f) || f.fail()) {
            if (f.eof()) break;
            f.clear();
            string line;
            getline(f, line);
            if (line.empty()) break;
            continue;
        }
        ds.ThemVaoCuoi(data);
    }
    f.close();
}

void DocFileDonHang() {
    ifstream f("DonHang.txt");
    if (!f.is_open()) {
        ofstream f_out("DonHang.txt");
        f_out.close();
        return;
    }
    while (!dsDonHangCho.empty()) dsDonHangCho.pop();

    while (f.peek() != EOF) {
        DonHang dh;
        if (!dh.DocFile(f) || f.fail()) {
            if (f.eof()) break;
            f.clear();
            string line;
            getline(f, line);
            if (line.empty()) break;
            continue;
        }
        dsDonHangCho.push(dh);
    }
    f.close();
}

template <class T>
void GhiDanhSachRaFile(DanhSach<T>& ds, const string& tenFile) {
    ofstream f(tenFile);
    if (!f.is_open()) {
        ThongBao("Loi: Khong the mo file de ghi!", 12);
        return;
    }
    Node<T>* p = ds.pHead;
    while (p != NULL) {
        p->data.GhiFile(f);
        p = p->pNext;
    }
    f.close();
}

void GhiFileDonHang() {
    ofstream f("DonHang.txt");
    if (!f.is_open()) {
        ThongBao("Loi: Khong the mo file DonHang.txt!", 12);
        return;
    }
    queue<DonHang> temp = dsDonHangCho;
    while (!temp.empty()) {
        temp.front().GhiFile(f);
        temp.pop();
    }
    f.close();
}

// ================= UI / Features =================
void HienThiBangHeader() {
    SetColor(14);
    gotoxy(2,5);  cout << "Ma Hang";
    gotoxy(15,5); cout << "Ten Hang";
    gotoxy(32,5); cout << "Noi San Xuat";
    gotoxy(50,5); cout << "Mau Sac";
    gotoxy(60,5); cout << "Gia Ban";
    gotoxy(72,5); cout << "Ngay Nhap";
    gotoxy(84,5); cout << "SL";
    SetColor(11);
    gotoxy(1,6);
    for (int i = 0; i < CONSOLE_WIDTH - 1; ++i) cout << "-";
    SetColor(15);
}

void ChucNang_HienThiHangHoa() {
    VeTieuDe("DANH SACH HANG HOA");
    HienThiBangHeader();
    Node<HangHoa>* p = dsHangHoa.pHead;
    int y = 7;
    if (p == NULL) {
        gotoxy(25,10); cout << "Chua co hang hoa nao trong danh sach!";
    }
    while (p != NULL) {
        p->data.HienThi(y);
        y++;
        p = p->pNext;
    }
    gotoxy(2,23);
    system("pause");
}

void ChucNang_TimKiemHangHoa() {
    VeTieuDe("TIM KIEM HANG HOA");
    string tenTim;
    gotoxy(5,4); cout << "Nhap Ten Hang Hoa Can Tim: ";
    getline(cin, tenTim);

    vector<Node<HangHoa>*> ketQua = dsHangHoa.TimTheoTen(tenTim);
    gotoxy(5,4); cout << string(70, ' '); // Xoa dong input

    if (ketQua.empty()) {
        ThongBao("Khong tim thay hang hoa nao co ten " + tenTim, 12);
    } else {
        HienThiBangHeader();
        int y = 7;
        for (auto node : ketQua) {
            node->data.HienThi(y);
            y++;
        }
        gotoxy(2,23);
        system("pause");
    }
}

void ChucNang_DatHang() {
    DonHang dh;
    dh.ngayDatHang = LayNgayHienTai();

    // tinh STT
    DocFileDonHang();
    if (dsDonHangCho.empty()) dh.stt = 1;
    else {
        queue<DonHang> tmp = dsDonHangCho;
        while (tmp.size() > 1) tmp.pop();
        dh.stt = tmp.front().stt + 1;
    }

    VeTieuDe("DAT HANG ONLINE");

    do {
        gotoxy(5,7); cout << "Nhap Ten Khach Hang: ";
        getline(cin, dh.tenKhachHang);
        if(dh.tenKhachHang.empty()) return; // Thoát nếu ESC
    } while (dh.tenKhachHang.empty());

    do {
        gotoxy(5,8); cout << "Nhap Dia Chi Giao Hang: ";
        getline(cin, dh.diaChi);
        if(dh.diaChi.empty()) return; // Thoát nếu ESC
    } while (dh.diaChi.empty());

    do {
        gotoxy(5,9); cout << "Nhap So Dien Thoai: ";
        getline(cin, dh.sdt);
        if(dh.sdt.empty()) return; // Thoát nếu ESC
        // RB 5: Kiêm tra SĐT chỉ chứa số (THÊM VÀO V2.16)
        bool is_valid = true;
        if (dh.sdt.empty()) is_valid = false;
        else {
            for (char c : dh.sdt) {
                if (!isdigit(c)) {
                    is_valid = false;
                    ThongBao("Loi: So dien thoai chi duoc chua ky tu so!", 12);
                    break;
                }
            }
        }
        if (!is_valid) dh.sdt = ""; // Buộc vòng lặp chạy lại
    } while (dh.sdt.empty());

    char tiepTuc;
    do {
        string maHangDat;
        int soLuongDat = 0;
        Node<HangHoa>* hh_node = NULL;

        do {
            gotoxy(5,12); cout << string(70, ' ');
            gotoxy(5,12); cout << "Nhap Ma Hang (4 ky tu): ";
            getline(cin, maHangDat);
            if(maHangDat.empty()) return; // Thoát nếu ESC

            maHangDat = StringToUpper(maHangDat);
            hh_node = dsHangHoa.TimTheoMa(maHangDat);
            if (hh_node == NULL) ThongBao("Ma hang khong ton tai. Vui long nhap lai!", 12);
            else if (hh_node->data.soLuong == 0) {
                ThongBao("Hang hoa nay da het hang. Vui long chon san pham khac!", 12);
                hh_node = NULL;
            }
        } while (hh_node == NULL);

        gotoxy(40,12); SetColor(14); cout << " (San pham: " << hh_node->data.tenHang << ")"; SetColor(15);

        // FIX RB 4: Kiem tra kieu du lieu va so am cho So Luong Dat
        do {
            gotoxy(5,13); cout << string(70, ' ');
            gotoxy(5,13); cout << "Nhap So Luong (<= " << hh_node->data.soLuong << "): ";
            if (!(cin >> soLuongDat)) {
                ThongBao("Loi: So luong phai la so nguyen!", 12);
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                soLuongDat = -1;
            } else if (soLuongDat <= 0 || soLuongDat > hh_node->data.soLuong) {
                ThongBao("So luong khong hop le. Vui long nhap lai!", 12);
            }
        } while (soLuongDat <= 0 || soLuongDat > hh_node->data.soLuong || cin.fail());

        // xoa buffer sau cin >>
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        ChiTietDonHang ct;
        ct.maHang = maHangDat;
        ct.soLuongDat = soLuongDat;
        ct.donGia = hh_node->data.giaBan;
        dh.dsChiTiet.push_back(ct);

        gotoxy(5,15); cout << "Ban co muon them san pham khac? (Y/N): ";
        tiepTuc = ReadKeySkipCR();

        // clean display
        gotoxy(5,12); cout << string(76, ' ');
        gotoxy(5,13); cout << string(76, ' ');
        gotoxy(5,15); cout << string(76, ' ');
    } while (toupper(tiepTuc) == 'Y');

    dh.TinhTongTien();

    gotoxy(5,17); SetColor(10);
    cout << "DAT HANG THANH CONG! Tong so tien: " << fixed << setprecision(0) << dh.tongTien << " VND";
    SetColor(15);

    dsDonHangCho.push(dh);
    GhiFileDonHang();
    ThongBao("Don hang dang cho xu ly. Cam on ban!", 10, 2000);
}

// ================= Admin / Quan ly =================
void QuanLy_XuLyDonHang() {
    VeTieuDe("XU LY DON HANG (FIFO)");
    if (dsDonHangCho.empty()) {
        ThongBao("Khong co don hang nao cho xu ly.", 14);
        return;
    }
    DonHang dh = dsDonHangCho.front();

    SetColor(14);
    gotoxy(5,5); cout << "--- DON HANG CAN XU LY (STT: " << dh.stt << ") ---";
    SetColor(15);
    gotoxy(5,7); cout << "Khach Hang: " << dh.tenKhachHang;
    gotoxy(50,7); cout << "SDT: " << dh.sdt;
    gotoxy(5,8); cout << "Dia Chi: " << dh.diaChi;
    gotoxy(50,8); cout << "Ngay Dat: " << dh.ngayDatHang;
    gotoxy(5,9); cout << "Tong Tien: " << fixed << setprecision(0) << dh.tongTien << " VND";

    SetColor(11);
    gotoxy(5,11);
    for (int i = 0; i < CONSOLE_WIDTH - 10; ++i) cout << "-";
    SetColor(15);

    gotoxy(5,12); SetColor(14); cout << "Chi Tiet Don Hang:"; SetColor(15);
    gotoxy(7,13); cout << "Ma Hang";
    gotoxy(20,13); cout << "So Luong Dat";
    gotoxy(35,13); cout << "Don Gia";
    gotoxy(50,13); cout << "Trang Thai Kho";

    int y = 14;
    bool coTheXuLy = true;
    for (size_t i = 0; i < dh.dsChiTiet.size(); ++i) {
        ChiTietDonHang& ct = dh.dsChiTiet[i];
        Node<HangHoa>* hh_node = dsHangHoa.TimTheoMa(ct.maHang);
        gotoxy(7, y); cout << ct.maHang;
        gotoxy(25, y); cout << ct.soLuongDat;
        gotoxy(35, y); cout << fixed << setprecision(0) << ct.donGia;
        if (hh_node == NULL) {
            SetColor(12);
            gotoxy(50, y); cout << "LOI: Ma hang khong con ton tai!";
            coTheXuLy = false;
        } else if (hh_node->data.soLuong < ct.soLuongDat) {
            SetColor(12);
            gotoxy(50, y); cout << "LOI: Khong du so luong (Con " << hh_node->data.soLuong << ")";
            coTheXuLy = false;
        } else {
            SetColor(10);
            gotoxy(50, y); cout << "OK (Con " << hh_node->data.soLuong << ")";
        }
        y++;
        SetColor(15);
    }

    char luaChon;
    gotoxy(5, y + 2);
    if (coTheXuLy) {
        cout << "Ban co muon xac nhan xu ly don hang nay? (Y/N): ";
        luaChon = ReadKeySkipCR();

        if (toupper(luaChon) == 'Y') {
            for (size_t i = 0; i < dh.dsChiTiet.size(); ++i) {
                ChiTietDonHang& ct = dh.dsChiTiet[i];
                Node<HangHoa>* hh_node = dsHangHoa.TimTheoMa(ct.maHang);
                if (hh_node != NULL) hh_node->data.soLuong -= ct.soLuongDat;
            }
            GhiDanhSachRaFile<HangHoa>(dsHangHoa, "HangHoa.txt");
            dsDonHangCho.pop();
            GhiFileDonHang();
            ThongBao("Xu ly don hang thanh cong! Da cap nhat kho.", 10);
        } else {
            // Tam hoan đơn hàng hợp lệ: đẩy xuống cuối queue
            DonHang tmp = dsDonHangCho.front();
            dsDonHangCho.pop();
            dsDonHangCho.push(tmp);
            GhiFileDonHang();
            ThongBao("Da tam hoan don hang. Don hang duoc chuyen xuong cuoi.", 14);
        }
    } else {
        SetColor(12);
        cout << "Don hang nay bi loi! Ban co muon XOA BO don hang nay? (Y/N): ";
        luaChon = ReadKeySkipCR();
        SetColor(15);
        if (toupper(luaChon) == 'Y') {
            dsDonHangCho.pop();
            GhiFileDonHang();
            ThongBao("Da XOA don hang bi loi khoi hang doi.", 12);
        } else {
            // Giữ đơn hàng lỗi: đẩy xuống cuối queue
            DonHang tmp = dsDonHangCho.front();
            dsDonHangCho.pop();
            dsDonHangCho.push(tmp);
            GhiFileDonHang();
            ThongBao("Don hang loi duoc chuyen xuong cuoi hang doi.", 14);
        }
    }
}

void QuanLy_ThemHangHoa() {
    VeTieuDe("THEM HANG HOA MOI");
    HangHoa hh;
    string maHang;
    Node<HangHoa>* node_check = NULL;

    do {
        gotoxy(5,5); cout << string(70, ' ');
        gotoxy(5,5); cout << "Nhap Ma Hang (4 ky tu): ";
        getline(cin, maHang);
        if(maHang.empty()) return; // Thoát nếu ESC

        maHang = StringToUpper(maHang);
        node_check = dsHangHoa.TimTheoMa(maHang);
        if (maHang.length() != 4) {
            ThongBao("Ma hang phai co dung 4 ky tu!", 12);
        } else if (node_check != NULL) {
            ThongBao("Ma hang nay da ton tai. Vui long nhap ma khac!", 12);
        }
    } while (maHang.length() != 4 || node_check != NULL);

    hh.maHang = maHang;

    gotoxy(5,6); cout << "Nhap Ten Hang:         ";
    getline(cin, hh.tenHang);
    if(hh.tenHang.empty()) return;

    gotoxy(5,7); cout << "Nhap Noi San Xuat:     ";
    getline(cin, hh.noiSanXuat);
    if(hh.noiSanXuat.empty()) return;
    
    gotoxy(5,8); cout << "Nhap Mau Sac:          ";
    getline(cin, hh.mauSac);
    if(hh.mauSac.empty()) return;


    // FIX RB A & B: Kiem tra kieu du lieu va so am cho Gia Ban
    do {
        gotoxy(5,9); cout << string(70, ' ');
        gotoxy(5,9); cout << "Nhap Gia Ban:          ";
        if (!(cin >> hh.giaBan)) {
            ThongBao("Loi: Gia ban phai la so!", 12);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            hh.giaBan = -1.0; 
        } else if (hh.giaBan < 0) {
            ThongBao("Loi: Gia ban khong duoc am!", 12);
        }
    } while (hh.giaBan < 0 || cin.fail());
    // cin.ignore() đã được gọi ở cuối vòng lặp số.

    hh.ngayNhapKho = LayNgayHienTai();
    gotoxy(5,10); cout << "Ngay Nhap Kho:     " << hh.ngayNhapKho;

    // FIX RB A & B: Kiem tra kieu du lieu va so am cho So Luong
    do {
        gotoxy(5,11); cout << string(70, ' ');
        gotoxy(5,11); cout << "Nhap So Luong:         ";
        if (!(cin >> hh.soLuong)) {
            ThongBao("Loi: So luong phai la so nguyen!", 12);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            hh.soLuong = -1;
        } else if (hh.soLuong < 0) {
            ThongBao("Loi: So luong khong duoc am!", 12);
        }
    } while (hh.soLuong < 0 || cin.fail());
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // đúng: sau cin >> (Cuối cùng)

    dsHangHoa.ThemVaoCuoi(hh);
    GhiDanhSachRaFile<HangHoa>(dsHangHoa, "HangHoa.txt");
    ThongBao("Them hang hoa thanh cong!", 10);
}

void QuanLy_XoaHangHoa() {
    VeTieuDe("XOA HANG HOA");
    string maHang;
    Node<HangHoa>* hh_node = NULL;

    do {
        gotoxy(5,5); cout << string(70, ' ');
        gotoxy(5,5); cout << "Nhap Ma Hang Can Xoa: ";
        getline(cin, maHang);
        if(maHang.empty()) return; // Thoát nếu ESC

        maHang = StringToUpper(maHang);
        hh_node = dsHangHoa.TimTheoMa(maHang);
        if (hh_node == NULL) {
            ThongBao("Khong tim thay ma hang nay! Vui long nhap lai.", 12);
        }
    } while (hh_node == NULL);

    HienThiBangHeader();
    hh_node->data.HienThi(7);

    gotoxy(5,10);
    SetColor(12);
    cout << "Ban co chac chan muon xoa hang hoa nay? (Y/N): ";
    SetColor(15);

    char luaChon = ReadKeySkipCR();

    if (toupper(luaChon) == 'Y') {
        bool ok = dsHangHoa.XoaTheoMa(maHang);
        if (ok) {
            GhiDanhSachRaFile<HangHoa>(dsHangHoa, "HangHoa.txt");
            ThongBao("Da xoa hang hoa thanh cong!", 10);
        } else {
            ThongBao("Xoa that bai. Co loi xay ra.", 12);
        }
    } else {
        ThongBao("Da huy thao tac xoa.", 14);
    }
}

void QuanLy_CapNhatSoLuong() {
    VeTieuDe("CAP NHAT SO LUONG HANG HOA");
    string maHang;
    Node<HangHoa>* hh_node = NULL;

    do {
        gotoxy(5,5); cout << string(70, ' ');
        gotoxy(5,5); cout << "Nhap Ma Hang Can Cap Nhat: ";
        getline(cin, maHang);
        if(maHang.empty()) return; // Thoát nếu ESC

        maHang = StringToUpper(maHang);
        hh_node = dsHangHoa.TimTheoMa(maHang);
        if (hh_node == NULL) ThongBao("Khong tim thay ma hang nay! Vui long nhap lai.", 12);
    } while (hh_node == NULL);

    HienThiBangHeader();
    hh_node->data.HienThi(7);

    int soLuongMoi;
    gotoxy(5,10);
    cout << "So luong hien tai: " << hh_node->data.soLuong;
    gotoxy(5,11);
    
    // FIX RB A & B: Kiem tra kieu du lieu va so am cho So Luong
    do {
        gotoxy(5,11); cout << string(70, ' ');
        gotoxy(5,11); cout << "Nhap so luong moi: ";
        if (!(cin >> soLuongMoi)) {
            ThongBao("Loi: So luong phai la so nguyen!", 12);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            soLuongMoi = -1;
        } else if (soLuongMoi < 0) {
            ThongBao("Loi: So luong khong duoc am!", 12);
        }
    } while (soLuongMoi < 0 || cin.fail());
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // đúng: sau cin >> (Cuối cùng)


    hh_node->data.soLuong = soLuongMoi;
    GhiDanhSachRaFile<HangHoa>(dsHangHoa, "HangHoa.txt");
    ThongBao("Cap nhat so luong thanh cong!", 10);
}

void Menu_QuanLyHangHoa() {
    char luaChon;
    do {
        VeTieuDe("QUAN LY HANG HOA");
        SetColor(14);
        gotoxy(30,7); cout << "1. Them hang hoa moi";
        gotoxy(30,9); cout << "2. Xoa hang hoa";
        gotoxy(30,11); cout << "3. Cap nhat so luong hang hoa";
        gotoxy(30,13); cout << "0. Quay lai menu Quan Ly";
        SetColor(15);
        gotoxy(30,16); cout << "Nhap lua chon cua ban: ";
        luaChon = ReadKeySkipCR();

        // ** THÊM cin.ignore() Ở ĐÂY SAU ReadKeySkipCR() (V2.15)
        // Điều này giúp dọn buffer ngay sau khi chọn menu, TRƯỚC KHI gọi hàm con có getline()
        if (luaChon != '0') { 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }

        switch (luaChon) {
            case '1': QuanLy_ThemHangHoa(); break;
            case '2': QuanLy_XoaHangHoa(); break;
            case '3': QuanLy_CapNhatSoLuong(); break;
            case '0': break;
            default: ThongBao("Lua chon khong hop le!", 12);
        }
    } while (luaChon != '0');
}

bool ChucNang_DangNhap() {
    VeTieuDe("DANG NHAP HE THONG");
    string user, pass;
    int soLanSai = 0;

    while (soLanSai < 3) {
        SetColor(14);
        gotoxy(32,8); cout << "*************************";
        gotoxy(32,9); cout << "* DANG NHAP HE THONG    *";
        gotoxy(32,10); cout << "*************************";
        SetColor(15);

        gotoxy(35,12); cout << "User:     " << "                   ";
        gotoxy(35,13); cout << "Password: " << "                   ";

        gotoxy(45,12); getline(cin, user);
        if (user.empty()) return false; // Thoát nếu ESC/Enter

        gotoxy(45,13); pass = NhapMatKhau();
        if (pass == "") { // Thoát nếu ESC
            ThongBao("Da huy dang nhap.", 14);
            return false;
        }

        Node<Admin>* p = dsAdmin.TimTheoUser(user);
        if (p != NULL && p->data.pass == pass) {
            ThongBao("Dang nhap thanh cong!", 10);
            return true;
        } else {
            soLanSai++;
            ThongBao("Sai User hoac Password. Con " + to_string(3 - soLanSai) + " lan thu.", 12);
        }
    }
    ThongBao("Ban da nhap sai 3 lan. Thoat chuong trinh.", 12, 2000);
    return false;
}

void Menu_QuanLy() {
    char luaChon;
    do {
        VeTieuDe("MENU QUAN LY");
        SetColor(14);
        gotoxy(30,8); cout << "So don hang cho: " << dsDonHangCho.size();
        gotoxy(30,10); cout << "1. Xu ly don hang (FIFO)";
        gotoxy(30,12); cout << "2. Quan ly hang hoa";
        gotoxy(30,14); cout << "0. Dang xuat";
        SetColor(15);
        gotoxy(30,17); cout << "Nhap lua chon cua ban: ";
        luaChon = ReadKeySkipCR();

        switch (luaChon) {
            case '1': QuanLy_XuLyDonHang(); break;
            case '2': 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FIX DỊCH CHUYỂN
                Menu_QuanLyHangHoa(); 
                break;
            case '0': ThongBao("Da dang xuat.", 10); break;
            default: ThongBao("Lua chon khong hop le!", 12);
        }
    } while (luaChon != '0');
}

// =================== MAIN ===================
int main() {
    // Set UTF-8 output for Vietnamese on console
    SetConsoleOutputCP(65001);

    // Load data
    DocFileVaoDanhSach<Admin>(dsAdmin, "Admin.txt");
    DocFileVaoDanhSach<HangHoa>(dsHangHoa, "HangHoa.txt");
    DocFileDonHang();

    char luaChon;
    do {
        VeTieuDe("CHUONG TRINH QUAN LY BAN HANG ONLINE");
        SetColor(14);
        gotoxy(30,7); cout << "====== MENU CHINH ======";
        SetColor(10);
        gotoxy(30,10); cout << "1. Hien thi thong tin hang hoa";
        gotoxy(30,12); cout << "2. Tim kiem thong tin hang hoa";
        gotoxy(30,14); cout << "3. Dat hang";
        SetColor(12);
        gotoxy(30,16); cout << "4. Quan ly (Dang nhap)";
        SetColor(15);
        gotoxy(30,18); cout << "0. Thoat chuong trinh";
        SetColor(15);
        gotoxy(30,21); cout << "Nhap lua chon cua ban: ";

        luaChon = ReadKeySkipCR();

        // FIX DỊCH CHUYỂN: Chỉ clear buffer TRƯỚC KHI gọi hàm có getline()
        if (luaChon == '2' || luaChon == '3' || luaChon == '4') {
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (luaChon) {
            case '1': ChucNang_HienThiHangHoa(); break;
            case '2': ChucNang_TimKiemHangHoa(); break;
            case '3': ChucNang_DatHang(); break;
            case '4':
                if (ChucNang_DangNhap()) Menu_QuanLy();
                break;
            case '0': ThongBao("Cam on ban da su dung chuong trinh!", 11, 1000); break;
            default: ThongBao("Lua chon khong hop le. Vui long nhap lai!", 12); break;
        }
    } while (luaChon != '0');

    return 0;
}