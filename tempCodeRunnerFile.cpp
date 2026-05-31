/*
 * ============================================================
 *   F.I.R.E. DASHBOARD - Financial Independence, Retire Early
 *   Sistem Perencanaan Pensiun dengan Aturan 4% & Inflasi
 *   
 *   MODIFIKASI v2.1:
 *   - Validasi syarat minimal ROI (inflasi + 4%)
 *   - Array multi-dimensi skenario simulasi investasi
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

const string DATABASE_FILE = "fire_database_v2.txt";
const int MAX_USER = 100;
const int MAX_SAVINGS = 500;


struct SavingRecord {
    int month;
    int year;
    double amount;
    string note;
};

struct UserProfile {
    int currentAge;
    int targetAge;
    double monthlyExpense;
    double inflationRate;
    double targetAmount;
    double currentSavings;
    bool isLoaded;
    
    UserProfile() : currentAge(0), targetAge(0), monthlyExpense(0), inflationRate(0.06),
                    targetAmount(0), currentSavings(0), isLoaded(false) {}
};

struct User {
    string username;
    string password;
    UserProfile profile;
    SavingRecord savings[MAX_SAVINGS];
    int savingsCount = 0;
};

// Global Variables
User databaseUser[MAX_USER];
int jumlahUser = 0;
int userIndexAktif = -1;

// --- FUNGSI UTILITAS & FORMATTING ---

string formatRupiah(double amount) {
    ostringstream oss;
    oss << fixed << setprecision(0) << amount;
    string s = oss.str();
    bool negative = false;
    if (!s.empty() && s[0] == '-') { negative = true; s = s.substr(1); }
    int n = (int)s.size();
    string result;
    for (int i = 0; i < n; i++) {
        if (i > 0 && (n - i) % 3 == 0) result += '.';
        result += s[i];
    }
    return string(negative ? "-" : "") + "Rp " + result;
}

void printHeader(const string& title) {
    cout << "\n";
    cout << "  +==================================================+\n";
    cout << "  |  [FIRE] F.I.R.E. DASHBOARD                      |\n";
    cout << "  |  " << left << setw(49) << title << "|\n";
    cout << "  +==================================================+\n\n";
}

void printDivider() { cout << "  --------------------------------------------------\n"; }
void printSuccess(const string& msg) { cout << "\n  [OK]  " << msg << "\n"; }
void printError  (const string& msg) { cout << "\n  [!!]  " << msg << "\n"; }
void printInfo   (const string& msg) { cout << "  [i]   " << msg << "\n"; }

void pauseScreen() {
    cout << "\n  Tekan ENTER untuk melanjutkan...";
    cin.get();
}

// --- FILE I/O ---

void saveData() {
    ofstream file(DATABASE_FILE);
    if (!file.is_open()) { printError("Gagal menyimpan database sistem!"); return; }
    
    file << jumlahUser << "\n";
    for (int i = 0; i < jumlahUser; i++) {
        file << databaseUser[i].username << "\n" << databaseUser[i].password << "\n";
        file << databaseUser[i].profile.currentAge << "\n"
             << databaseUser[i].profile.targetAge << "\n"
             << fixed << setprecision(2) << databaseUser[i].profile.monthlyExpense << "\n"
             << databaseUser[i].profile.inflationRate << "\n"
             << databaseUser[i].profile.targetAmount << "\n"
             << databaseUser[i].profile.currentSavings << "\n"
             << databaseUser[i].profile.isLoaded << "\n";
             
        file << databaseUser[i].savingsCount << "\n";
        for (int j = 0; j < databaseUser[i].savingsCount; j++) {
            file << databaseUser[i].savings[j].month << "\n"
                 << databaseUser[i].savings[j].year << "\n"
                 << fixed << setprecision(2) << databaseUser[i].savings[j].amount << "\n"
                 << databaseUser[i].savings[j].note << "\n";
        }
    }
    file.close();
}

void loadData() {
    ifstream file(DATABASE_FILE);
    if (!file.is_open()) return;
    
    file >> jumlahUser;
    file.ignore();
    for (int i = 0; i < jumlahUser && i < MAX_USER; i++) {
        getline(file, databaseUser[i].username);
        getline(file, databaseUser[i].password);
        file >> databaseUser[i].profile.currentAge 
             >> databaseUser[i].profile.targetAge 
             >> databaseUser[i].profile.monthlyExpense
             >> databaseUser[i].profile.inflationRate
             >> databaseUser[i].profile.targetAmount 
             >> databaseUser[i].profile.currentSavings
             >> databaseUser[i].profile.isLoaded;
             
        file >> databaseUser[i].savingsCount;
        file.ignore();
        for (int j = 0; j < databaseUser[i].savingsCount && j < MAX_SAVINGS; j++) {
            file >> databaseUser[i].savings[j].month 
                 >> databaseUser[i].savings[j].year 
                 >> databaseUser[i].savings[j].amount;
            file.ignore();
            getline(file, databaseUser[i].savings[j].note);
        }
    }
    file.close();
}

// --- ALGORITMA CORE ---

void menuProfil() {
    printHeader("Kelola Profil & Perhitungan Target 4%");
    User& u = databaseUser[userIndexAktif];
    
    if (u.profile.isLoaded) {
        cout << "  Data profil saat ini:\n";
        printDivider();
        cout << "  Username             : " << u.username << "\n";
        cout << "  Umur Sekarang        : " << u.profile.currentAge  << " tahun\n";
        cout << "  Target Usia Pensiun  : " << u.profile.targetAge << " tahun\n";
        cout << "  Pengeluaran Bulanan  : " << formatRupiah(u.profile.monthlyExpense) << "/bulan\n";
        cout << "  Asumsi Inflasi       : " << (u.profile.inflationRate * 100) << "%\n";
        printDivider();
        cout << "  TARGET ASSET (4% Rule): " << formatRupiah(u.profile.targetAmount) << "\n";
        cout << "  Saldo Celengan Kini  : " << formatRupiah(u.profile.currentSavings) << "\n";
        printDivider();
        cout << "\n  Update profil? (y/n): ";
        char c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c != 'y' && c != 'Y') return;
    }
    
    cout << "\n  Masukkan data profil baru:\n\n";
    cout << "  Umur sekarang               : "; cin >> u.profile.currentAge;
    cout << "  Target usia pensiun dini    : "; cin >> u.profile.targetAge;
    cout << "  Pengeluaran saat ini (/bulan): "; cin >> u.profile.monthlyExpense;
    cout << "  Asumsi tingkat inflasi (%)  : "; double inflasiInput; cin >> inflasiInput;
    u.profile.inflationRate = inflasiInput / 100.0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    int jarakTahun = u.profile.targetAge - u.profile.currentAge;
    if (jarakTahun < 0) jarakTahun = 0;
    
    double pengeluaranTahunanKini   = u.profile.monthlyExpense * 12;
    double pengeluaranTahunanDepan  = pengeluaranTahunanKini * pow((1.0 + u.profile.inflationRate), jarakTahun);
    u.profile.targetAmount          = pengeluaranTahunanDepan * 25;
    u.profile.isLoaded = true;
    saveData();
    
    printSuccess("Profil berhasil disimpan & Target Aset dihitung!");
    printDivider();
    cout << "  Pengeluaran Masa Depan : " << formatRupiah(pengeluaranTahunanDepan) << "/tahun\n";
    cout << "  TARGET KEKAYAAN (FIRE) : " << formatRupiah(u.profile.targetAmount) << "\n";
    cout << "  Sisa waktu pencapaian  : " << jarakTahun << " tahun\n";
    printDivider();
    pauseScreen();
}

void menuSetorCelengan() {
    printHeader("Setor Celengan");
    User& u = databaseUser[userIndexAktif];
    
    if (!u.profile.isLoaded) { printError("Lengkapi profil dulu di Menu 1!"); pauseScreen(); return; }
    if (u.savingsCount >= MAX_SAVINGS) { printError("Kapasitas riwayat penuh!"); pauseScreen(); return; }
    
    SavingRecord* newRecord = &u.savings[u.savingsCount];
    cout << "  Masukkan detail setoran:\n\n";
    cout << "  Bulan (1-12) : "; cin >> newRecord->month;
    cout << "  Tahun        : "; cin >> newRecord->year;
    cout << "  Nominal (Rp) : "; cin >> newRecord->amount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Catatan      : "; getline(cin, newRecord->note);
    
    u.profile.currentSavings += newRecord->amount;
    u.savingsCount++;
    
    saveData();
    printSuccess("Setoran berhasil dicatat!");
    printDivider();
    cout << "  Jumlah setor  : " << formatRupiah(newRecord->amount) << "\n";
    cout << "  Total saldo   : " << formatRupiah(u.profile.currentSavings) << "\n";
    double sisa = u.profile.targetAmount - u.profile.currentSavings;
    if (sisa > 0) cout << "  Sisa target   : " << formatRupiah(sisa) << "\n";
    else          cout << "  CONGRATS! Target F.I.R.E Anda tercapai!\n";
    printDivider();
    pauseScreen();
}

void bubbleSortDesc(SavingRecord arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].amount < arr[j+1].amount) {
                SavingRecord t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;
            }
}

void searchTransaksi(SavingRecord arr[], int n) {
    printHeader("Cari Transaksi");
    cout << "  [1] Bulan & Tahun\n  [2] Nominal\n\n  Pilihan: ";
    int p; cin >> p;
    bool found = false; int count = 0;
    if (p == 1) {
        int b, t; cout << "  Bulan: "; cin >> b; cout << "  Tahun: "; cin >> t; cout << "\n";
        printDivider();
        for (int i = 0; i < n; i++)
            if (arr[i].month == b && arr[i].year == t) {
                cout << "  [OK] " << b << "/" << t << "  " << left << setw(20) << arr[i].note
                     << right << formatRupiah(arr[i].amount) << "\n";
                found = true; count++;
            }
    } else if (p == 2) {
        double nom; cout << "  Nominal (Rp): "; cin >> nom; double tol = nom * 0.01; cout << "\n";
        printDivider();
        for (int i = 0; i < n; i++)
            if (fabs(arr[i].amount - nom) <= tol) {
                cout << "  [OK] " << arr[i].month << "/" << arr[i].year
                     << "  " << left << setw(20) << arr[i].note
                     << right << formatRupiah(arr[i].amount) << "\n";
                found = true; count++;
            }
    }
    printDivider();
    if (!found) printInfo("Tidak ditemukan."); else cout << "  Ditemukan " << count << " transaksi.\n";
    pauseScreen();
}

void menuRiwayat() {
    User& u = databaseUser[userIndexAktif];
    if (u.savingsCount == 0) {
        printHeader("Riwayat & Analisis Tabungan");
        printInfo("Belum ada data. Setor dulu!"); pauseScreen(); return;
    }
    
    SavingRecord tempArr[MAX_SAVINGS];
    for (int i = 0; i < u.savingsCount; i++) tempArr[i] = u.savings[i];
    int sub = 0;
    do {
        printHeader("Riwayat & Analisis Tabungan");
        cout << "  [1] Lihat Semua Catatan\n";
        cout << "  [2] Ranking Kehematan (Sorting)\n";
        cout << "  [3] Cari Transaksi (Searching)\n";
        cout << "  [0] Kembali\n\n  Pilihan: ";
        cin >> sub; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (sub == 1) {
            printHeader("Semua Catatan Tabungan");
            cout << "  " << left << setw(6) << "No" << setw(10) << "Bln/Thn"
                 << setw(22) << "Catatan" << right << setw(18) << "Nominal" << "\n";
            printDivider();
            double total = 0;
            for (int i = 0; i < u.savingsCount; i++) {
                cout << "  " << left << setw(6) << (i+1)
                     << setw(2) << u.savings[i].month << "/" << setw(6) << u.savings[i].year
                     << setw(22) << u.savings[i].note.substr(0, 20)
                     << right << setw(18) << formatRupiah(u.savings[i].amount) << "\n";
                total += u.savings[i].amount;
            }
            printDivider();
            cout << "  " << left << setw(38) << "TOTAL" << right << setw(18) << formatRupiah(total) << "\n";
            pauseScreen();
        } else if (sub == 2) {
            printHeader("Ranking Kehematan");
            bubbleSortDesc(tempArr, u.savingsCount);
            cout << "  " << left << setw(8) << "Rank" << setw(10) << "Bln/Thn"
                 << setw(22) << "Catatan" << right << setw(16) << "Nominal" << "\n";
            printDivider();
            for (int i = 0; i < u.savingsCount; i++) {
                string medal = (i==0)?"[1]":(i==1)?"[2]":(i==2)?"[3]":"   ";
                cout << "  " << medal << " " << left << setw(4) << (i+1)
                     << setw(2) << tempArr[i].month << "/" << setw(6) << tempArr[i].year
                     << setw(22) << tempArr[i].note.substr(0, 20)
                     << right << setw(16) << formatRupiah(tempArr[i].amount) << "\n";
            }
            printDivider(); pauseScreen();
        } else if (sub == 3) {
            searchTransaksi(u.savings, u.savingsCount);
        }
    } while (sub != 0);
}

// --- FUNGSI REKURSI (tidak berubah) ---

double hitungBungaBerbunga(double principal, double rate, int years) {
    if (years == 0) return principal;
    return hitungBungaBerbunga(principal * (1.0 + rate), rate, years - 1);
}

int tahunHinggaTarget(double saldo, double tabunganPerTahun, double rate, double target, int tahun) {
    if (saldo >= target) return tahun;
    if (tahun > 100)    return -1;
    return tahunHinggaTarget(hitungBungaBerbunga(saldo, rate, 1) + tabunganPerTahun,
                             tabunganPerTahun, rate, target, tahun + 1);
}


// ============================================================
//  MENU 4: LAPORAN LENGKAP F.I.R.E.
//
//  Isi laporan:
//    [1] Ringkasan Profil
//    [2] Ringkasan Hasil FIRE (validasi ROI + rekursi)
//    [3] Tabel Perkembangan Dana  <-- ARRAY 2D tabel[tahun][5]
//        tabel[t][0] = usia
//        tabel[t][1] = saldo awal tahun
//        tabel[t][2] = tabungan ditambahkan
//        tabel[t][3] = imbal hasil/bunga
//        tabel[t][4] = saldo akhir tahun
//    [4] Statistik Akhir
//
//  Semua bagian dicetak ke layar dan disimpan ke file .txt
// ============================================================
void menuLaporan() {
    printHeader("Cetak Laporan Lengkap F.I.R.E.");
    User& u = databaseUser[userIndexAktif];
    if (!u.profile.isLoaded) { printError("Lengkapi profil dulu di Menu 1!"); pauseScreen(); return; }

    // --- INPUT: rata-rata tabungan dari riwayat ---
    double rataTabBulanan = 0;
    if (u.savingsCount > 0) {
        double tot = 0;
        for (int i = 0; i < u.savingsCount; i++) tot += u.savings[i].amount;
        rataTabBulanan = tot / u.savingsCount;
    }

    cout << "  Data berikut digunakan untuk proyeksi laporan:\n\n";
    double tabBulanan;
    cout << "  Tabungan rutin/bulan (Rp)";
    if (rataTabBulanan > 0) cout << " [rata2: " << formatRupiah(rataTabBulanan) << "]";
    cout << "\n  (0 = pakai rata-rata riwayat): ";
    cin >> tabBulanan;
    if (tabBulanan <= 0 && rataTabBulanan > 0) tabBulanan = rataTabBulanan;
    else if (tabBulanan <= 0) { printError("Masukkan nominal tabungan bulanan!"); pauseScreen(); return; }

    // --- INPUT ROI + VALIDASI MINIMAL RETURN ---
    double rate;
    cout << "  Asumsi ROI Investasi/tahun (%): "; cin >> rate; rate /= 100.0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    double minimalReturn = u.profile.inflationRate + 0.04;
    if (rate < minimalReturn) {
        printError("Peringatan: ROI terlalu rendah!");
        printDivider();
        cout << "  Inflasi diasumsikan    : " << (u.profile.inflationRate * 100) << "%\n";
        cout << "  Minimal ROI aman       : " << (minimalReturn * 100) << "% (inflasi + 4%)\n";
        cout << "  ROI Anda               : " << (rate * 100) << "%\n";
        printDivider();
        cout << "  [1] Tetap lanjutkan\n";
        cout << "  [2] Sesuaikan ke batas aman (" << (minimalReturn * 100) << "%)\n";
        cout << "\n  Pilihan Anda: ";
        int opsi; cin >> opsi; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (opsi == 2) { rate = minimalReturn; printSuccess("ROI disesuaikan ke batas aman."); }
        else             printInfo("Melanjutkan dengan ROI yang dimasukkan.");
    }

    double tabPerTahun   = tabBulanan * 12;
    int    jarakTahun    = u.profile.targetAge - u.profile.currentAge;
    if (jarakTahun < 1) jarakTahun = 1;
    int    batasSimulasi = (jarakTahun > 50) ? 50 : jarakTahun;

    // ============================================================
    //  ARRAY 2D: tabel[MAKS_BARIS][MAKS_KOLOM]
    //  Baris = satu baris per tahun simulasi (maks 50 tahun)
    //  Kolom:
    //    [t][0] = usia pada akhir tahun ke-t
    //    [t][1] = saldo awal tahun ke-t
    //    [t][2] = tabungan yang ditambahkan tahun ke-t
    //    [t][3] = imbal hasil (ROI x saldo awal) tahun ke-t
    //    [t][4] = saldo akhir tahun ke-t
    // ============================================================
    const int MAKS_BARIS = 50;
    const int MAKS_KOLOM = 5;
    double tabel[MAKS_BARIS][MAKS_KOLOM];

    double saldoBerjalan = u.profile.currentSavings;
    for (int t = 0; t < batasSimulasi; t++) {
        double saldoAwal  = saldoBerjalan;
        double bunga      = saldoAwal * rate;
        double saldoAkhir = saldoAwal + bunga + tabPerTahun;

        tabel[t][0] = u.profile.currentAge + t + 1; // usia akhir tahun ini
        tabel[t][1] = saldoAwal;
        tabel[t][2] = tabPerTahun;
        tabel[t][3] = bunga;
        tabel[t][4] = saldoAkhir;

        saldoBerjalan = saldoAkhir;
    }

    // Rekursi: estimasi tahun nyata mencapai target
    int tahunNyata = tahunHinggaTarget(u.profile.currentSavings, tabPerTahun, rate,
                                       u.profile.targetAmount, 0);

    // Hitung statistik dari array 2D
    double totalTabunganManual = 0, totalBunga = 0;
    for (int t = 0; t < batasSimulasi; t++) {
        totalTabunganManual += tabel[t][2];
        totalBunga          += tabel[t][3];
    }
    double saldoAkhirSimulasi = tabel[batasSimulasi - 1][4];
    double persenTercapai = (u.profile.targetAmount > 0)
                            ? (saldoAkhirSimulasi / u.profile.targetAmount * 100) : 0;
    if (persenTercapai > 100) persenTercapai = 100;

    // ============================================================
    //  CETAK KE LAYAR & FILE SEKALIGUS
    // ============================================================
    string namaFile = "laporan_" + u.username + ".txt";
    ofstream file(namaFile);
    bool adaFile = file.is_open();

    auto tulis = [&](const string& baris) {
        cout << baris << "\n";
        if (adaFile) file << baris << "\n";
    };
    auto tulisDiv = [&]() {
        tulis("  --------------------------------------------------");
    };

    tulis("");
    tulis("  +==================================================+");
    tulis("  |        LAPORAN LENGKAP F.I.R.E. DASHBOARD       |");
    tulis("  |        Financial Independence, Retire Early      |");
    tulis("  +==================================================+");

    // --------------------------------------------------------
    //  BAGIAN 1: RINGKASAN PROFIL
    // --------------------------------------------------------
    tulis("");
    tulis("  [1] RINGKASAN PROFIL");
    tulisDiv();
    tulis("  Username           : " + u.username);
    tulis("  Usia Sekarang      : " + to_string(u.profile.currentAge) + " tahun");
    tulis("  Target Pensiun     : " + to_string(u.profile.targetAge)  + " tahun");
    tulis("  Sisa Waktu         : " + to_string(jarakTahun)           + " tahun");
    {
        ostringstream oi, or2;
        oi  << fixed << setprecision(1) << (u.profile.inflationRate * 100);
        or2 << fixed << setprecision(1) << (rate * 100);
        tulis("  Asumsi Inflasi     : " + oi.str()  + "% per tahun");
        tulis("  ROI Investasi      : " + or2.str() + "% per tahun");
    }
    tulis("  Pengeluaran Bulanan: " + formatRupiah(u.profile.monthlyExpense));
    tulisDiv();

    // --------------------------------------------------------
    //  BAGIAN 2: RINGKASAN HASIL FIRE
    // --------------------------------------------------------
    tulis("");
    tulis("  [2] RINGKASAN HASIL F.I.R.E.");
    tulisDiv();
    tulis("  Target Dana (4% Rule) : " + formatRupiah(u.profile.targetAmount));
    tulis("  Saldo Awal Saat Ini   : " + formatRupiah(u.profile.currentSavings));
    tulis("  Tabungan Rutin/bulan  : " + formatRupiah(tabBulanan));
    tulis("  Tabungan Rutin/tahun  : " + formatRupiah(tabPerTahun));
    tulisDiv();
    if (tahunNyata == -1) {
        tulis("  [!!] Target TIDAK tercapai dalam 100 tahun.");
        tulis("       Naikkan nominal tabungan atau ROI investasi!");
    } else {
        int usiaPensiun = u.profile.currentAge + tahunNyata;
        tulis("  Estimasi Tercapai     : " + to_string(tahunNyata) + " tahun lagi");
        tulis("  Prediksi Usia Pensiun : " + to_string(usiaPensiun) + " tahun");
        if (usiaPensiun <= u.profile.targetAge)
            tulis("  Status : [AHEAD] Pensiun " + to_string(u.profile.targetAge - usiaPensiun) + " tahun lebih cepat!");
        else
            tulis("  Status : [BEHIND] Terlambat " + to_string(usiaPensiun - u.profile.targetAge) + " tahun dari rencana.");
    }
    tulisDiv();

    // --------------------------------------------------------
    //  BAGIAN 3: TABEL PERKEMBANGAN DANA (ARRAY 2D)
    // --------------------------------------------------------
    tulis("");
    tulis("  [3] TABEL PERKEMBANGAN DANA (Proyeksi " + to_string(batasSimulasi) + " Tahun)");
    tulisDiv();
    tulis("  Thn  Usia  Saldo Awal          +Tabungan         +Imbal Hasil      Saldo Akhir");
    tulisDiv();

    // Loop array 2D — iterasi setiap baris tabel[t][kolom]
    for (int t = 0; t < batasSimulasi; t++) {
        ostringstream baris;
        baris << "  "
              << left  << setw(5)  << (t + 1)
              << setw(6)           << (int)tabel[t][0]
              << right << setw(18) << formatRupiah(tabel[t][1])
              << setw(18)          << formatRupiah(tabel[t][2])
              << setw(18)          << formatRupiah(tabel[t][3])
              << setw(18)          << formatRupiah(tabel[t][4]);

        // Tandai baris ketika saldo pertama kali melampaui target
        if (tabel[t][4] >= u.profile.targetAmount &&
            (t == 0 || tabel[t-1][4] < u.profile.targetAmount))
            baris << "  << TARGET TERCAPAI";

        tulis(baris.str());
    }
    tulisDiv();

    // --------------------------------------------------------
    //  BAGIAN 4: STATISTIK AKHIR
    // --------------------------------------------------------
    tulis("");
    tulis("  [4] STATISTIK AKHIR");
    tulisDiv();
    tulis("  Total Tabungan Aktif  : " + formatRupiah(totalTabunganManual));
    tulis("  Total Imbal Hasil     : " + formatRupiah(totalBunga));
    tulis("  Saldo Akhir Proyeksi  : " + formatRupiah(saldoAkhirSimulasi));
    {
        ostringstream op, ork;
        op << fixed << setprecision(1) << persenTercapai;
        double rasio = (totalTabunganManual + u.profile.currentSavings > 0)
                       ? (totalBunga / (totalTabunganManual + u.profile.currentSavings) * 100) : 0;
        ork << fixed << setprecision(1) << rasio;
        tulis("  Persen Target Tercapai: " + op.str() + "%");
        tulis("  Kontribusi Imbal Hasil: " + ork.str() + "% dari total pertumbuhan");
    }
    tulisDiv();
    tulis("  * Proyeksi dihitung dengan asumsi ROI & tabungan konstan.");
    tulis("  +==================================================+");
    tulis("");

    if (adaFile) {
        file.close();
        printSuccess("Laporan disimpan ke file: " + namaFile);
    } else {
        printError("Gagal menyimpan file laporan.");
    }

    pauseScreen();
}

// --- MAIN DASHBOARD INTERFACE ---

void dashboardUtama() {
    int pilihanDashboard;
    User& u = databaseUser[userIndexAktif];
    
    do {
        cout << "\n  +==================================================+\n";
        cout << "  |   [FIRE]  F.I.R.E. DASHBOARD                    |\n";
        cout << "  |   Financial Independence, Retire Early           |\n";
        cout << "  +==================================================+\n";
        cout << "  |  Halo, " << left << setw(42) << (u.username + "!") << "|\n";
        cout << "  |  Saldo: " << left << setw(41) << formatRupiah(u.profile.currentSavings) << "|\n";
        
        int persen = 0;
        if (u.profile.targetAmount > 0) {
            persen = (int)((u.profile.currentSavings / u.profile.targetAmount) * 100);
            if (persen > 100) persen = 100;
        }
        int barWidth = 20;
        int progress = (persen * barWidth) / 100;
        string visualBar = string(progress, 'X') + string(barWidth - progress, '.');
        
        cout << "  |  Progress: [" << visualBar << "] " << setw(3) << persen << "% menuju target   |\n";
        cout << "  +==================================================+\n";
        cout << "  |  [1] Kelola Profil & Hitung Target Pensiun       |\n";
        cout << "  |  [2] Setor Celengan                              |\n";
        cout << "  |  [3] Riwayat & Analisis Tabungan                 |\n";
        cout << "  |  [4] Cetak Laporan Lengkap F.I.R.E.              |\n";
        cout << "  |  [0] Simpan, Logout & Keluar                     |\n";
        cout << "  +==================================================+\n";
        cout << "\n  Pilih menu (0-4): ";
        cin >> pilihanDashboard;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (pilihanDashboard) {
            case 1: menuProfil();         break;
            case 2: menuSetorCelengan();  break;
            case 3: menuRiwayat();        break;
            case 4: menuLaporan(); break;
            case 0:
                printInfo("Menyimpan data akun secara aman...");
                saveData();
                userIndexAktif = -1;
                break;
            default:
                printError("Pilihan tidak valid!");
                pauseScreen();
        }
    } while (userIndexAktif != -1);
}

int main() {
    loadData();
    
    if (jumlahUser == 0) {
        databaseUser[0].username = "admin";
        databaseUser[0].password = "123";
        jumlahUser++;
        saveData();
    }

    int pilihanMenuAwal;
    do {
        cout << "\n=== HALAMAN AWAL SYSTEM ===" << endl;
        cout << "1. Register (Daftar Akun Baru)" << endl;
        cout << "2. Login Aplikasi" << endl;
        cout << "3. Keluar Sistem" << endl;
        cout << "Pilih (1-3): ";
        cin >> pilihanMenuAwal;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "--------------------" << endl;

        switch (pilihanMenuAwal) {
            case 1: {
                cout << "[ PROSES REGISTER ]" << endl;
                if (jumlahUser >= MAX_USER) { cout << "Memori penuh!\n"; break; }
                string usernameBaru, passwordBaru;
                cout << "Buat Username: "; cin >> usernameBaru;
                bool ditemukan = false;
                for (int i = 0; i < jumlahUser; i++)
                    if (databaseUser[i].username == usernameBaru) { ditemukan = true; break; }
                if (ditemukan) {
                    cout << ">>> Gagal! Username sudah ada. <<<\n";
                } else {
                    cout << "Buat Password: "; cin >> passwordBaru;
                    databaseUser[jumlahUser].username = usernameBaru;
                    databaseUser[jumlahUser].password = passwordBaru;
                    jumlahUser++;
                    saveData();
                    cout << ">>> Akun Berhasil Terdaftar! <<<\n";
                }
                break;
            }
            case 2: {
                cout << "[ PROSES LOGIN ]" << endl;
                string inputUser, inputPass;
                cout << "Masukkan Username: "; cin >> inputUser;
                cout << "Masukkan Password: "; cin >> inputPass;
                bool apakahLogin = false;
                for (int i = 0; i < jumlahUser; i++) {
                    if (databaseUser[i].username == inputUser && databaseUser[i].password == inputPass) {
                        apakahLogin = true; userIndexAktif = i; break;
                    }
                }
                if (apakahLogin) {
                    cout << ">>> Login Berhasil! <<<\n";
                    dashboardUtama();
                } else {
                    cout << ">>> Login Gagal! Username/Password salah. <<<\n";
                }
                break;
            }
            case 3:
                cout << "Keluar sistem. Sampai jumpa!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihanMenuAwal != 3);

    return 0;
}