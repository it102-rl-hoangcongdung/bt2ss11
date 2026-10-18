#include <stdio.h>

typedef struct {
    char ma_kien_hang[20];
    float trong_luong;
    float nhiet_do_yeu_cau;
} PalletItem;

typedef struct {
    char ma_ke[20];
    float trong_luong_hien_tai;
    float tai_trong_toi_da;
    int la_kho_lanh;
} WarehouseShelf;

int kiem_tra_nhap_kho(WarehouseShelf ke, PalletItem kien) {
    if (ke.trong_luong_hien_tai + kien.trong_luong > ke.tai_trong_toi_da) {
        return 0;
    }

    if (ke.la_kho_lanh == 1) {
        if (kien.nhiet_do_yeu_cau >= -18.0 && kien.nhiet_do_yeu_cau <= 5.0) {
            return 1;
        } else {
            return 0;
        }
    }

    return 1;
}

int main() {
    WarehouseShelf ke_lanh_A1 = {"KE-COLD-01", 350.0, 500.0, 1};
    
    PalletItem kien_hop_le = {"PL-COLD-01", 100.0, -10.0};
    PalletItem kien_nong_loi = {"PL-DRY-88", 80.0, 25.0};
    PalletItem kien_qua_lanh = {"PL-ICE-99", 50.0, -25.0};
    PalletItem kien_qua_tai = {"PL-HEAVY-02", 160.0, -5.0};

    printf("--- HE THONG KIEM DINH NHAP KHO WAREHOUSE ---\n");

    int kq1 = kiem_tra_nhap_kho(ke_lanh_A1, kien_hop_le);
    printf("Test 1 (Hop le -10.0C): %s\n", kq1 == 1 ? "CHO PHEP" : "TU CHOI");

    int kq2 = kiem_tra_nhap_kho(ke_lanh_A1, kien_nong_loi);
    printf("Test 2 (Nhiet do nong 25.0C): %s\n", kq2 == 1 ? "CHO PHEP" : "TU CHOI");

    int kq3 = kiem_tra_nhap_kho(ke_lanh_A1, kien_qua_lanh);
    printf("Test 3 (Nhiet do thap -25.0C): %s\n", kq3 == 1 ? "CHO PHEP" : "TU CHOI");

    int kq4 = kiem_tra_nhap_kho(ke_lanh_A1, kien_qua_tai);
    printf("Test 4 (Vuot tai trong 350+160>500): %s\n", kq4 == 1 ? "CHO PHEP" : "TU CHOI");

    return 0;
}
