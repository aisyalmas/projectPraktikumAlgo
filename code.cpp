#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

bool isexit = false;
const int maxHistory =100;

struct Transaction{
    int id;
    int bulan;
    int tahun;
    long long nominal;
    string catatan;
};

struct userProfile{
    int currentAge;
    int targetAge;
    long long monthlyExpense;
    double inflationRate;
    long long targetAmount;
    bool isDone;
    
};

struct users{
    string username;
    string password;
    userProfile Profile;
    
    long long totalSavings; //nyimpen total saldo
    Transaction history[maxHistory]; //nyimpen riwayat catatan
    int jumlahTransaksi = 0;
    
};


long double targetAset = 0;
int jumlahUser=0,menuAwal;
const int userMax=100;
users user[userMax];
int userAktif=-1;
string usernameNew,passwordNew,logPass,logUser;

void pauseScreen(){
    cout<<"\n Tekan Enter jika ingin kembali ..... ";
    cin.ignore();
    cin.get();
}

string formatMoney(long long money){
    bool isNegative =false;
    if(money<0){// if uang negative
        isNegative = true;
        money =-money;
    }

    if(money == 0){//if uang=0
        return "Rp. 0";
    }
    //menggunakan logika length

    string angkaStr = to_string(money);
    string hasil="";

    int panjang = angkaStr.length();
    int posisiBlkng= 0;

    for(int i=panjang -1;i>=0;i--){
        hasil=angkaStr[i]+hasil;
        posisiBlkng++;
        
        if(posisiBlkng %3 == 0 && i !=0){
            hasil= "."+hasil;
        }
    }
    
    if(isNegative)return "-Rp "+ hasil;
    return "Rp "+ hasil;
}


void saveAll(){
    ofstream file("database_users.txt",ios::out);

    if(!file.is_open()){
        cout<<"\n Maaf file tidak dapat dibuka, gagal menyimpan file";
        return;
    }

    file<<jumlahUser<<"\n";
    for(int i=0;i<jumlahUser;i++){
        file<<user[i].username<<"\n"<<user[i].password<<"\n";
        
        file<<user[i].Profile.currentAge<<"\n"
        <<user[i].Profile.targetAge<<"\n"
        <<user[i].Profile.monthlyExpense<<"\n"
        <<user[i].Profile.inflationRate<<"\n"
        <<user[i].Profile.targetAmount<<"\n"
        <<user[i].totalSavings<<"\n"
        <<user[i].Profile.isDone<<"\n";

        file<<user[i].jumlahTransaksi<<"\n";
        for(int j=0;j<user[i].jumlahTransaksi;j++){
            file<<user[i].history[j].id<<"\n"
                <<user[i].history[j].bulan<<"\n"
                <<user[i].history[j].tahun<<"\n"
                <<user[i].history[j].nominal<<"\n"
                <<user[i].history[j].catatan<<"\n";
            }
    }
    file.close();
}
void menuProfile(){
    users& u=user[userAktif];
    char pilih;
    //buat pengkondisian kalau pernah ngisi profile belum kalao belum ngisi kaalu udah tampilin yang lama
    if(u.Profile.isDone){
        cout<<"\n Data Proflie Anda: ";
        cout<<"\n -----------------------------------------------------";
        cout<<"\n Username            : "<<u.username;
        cout<<"\n Umur Sekarang       : "<<u.Profile.currentAge;
        cout<<"\n Target Usia Pensiun : "<<u.Profile.targetAge;
        cout<<"\n Pengeluaran Bulanan : "<<formatMoney(u.Profile.monthlyExpense)<<"/month";
        cout<<"\n Asumsi Inflasi      : "<<u.Profile.inflationRate<<"%";
        cout<<"\n------------------------------------------------------";
        cout<<"\n Target Aset         : "<<formatMoney(u.Profile.targetAmount);
        cout<<"\n Saldo Tabungan      : "<<formatMoney(u.totalSavings);
        cout<<"\n------------------------------------------------------";
        
        pauseScreen();
        cout<<"\n Update Profile anda?(y/n):";cin>>pilih;
        if(pilih != 'y' && pilih !='Y')return;
    }
    //ngisi profile kalau belum
    cout<<"\n--------------------------------------------------------------------------";
    cout<<"\n Masukan data Profile Anda \n";
    cout<<"\n Umur Sekarang             : "; cin>>u.Profile.currentAge;
    cout<<" Target Umur Pensiun Dini  : "; cin>>u.Profile.targetAge;
    cout<<" Asumsi Tingkat Inflasi(%) : "; cin>>u.Profile.inflationRate;
    cout<<" Pengeluaran anda (/month) : "; cin>>u.Profile.monthlyExpense;
    
    int jarakTahun = u.Profile.targetAge -u.Profile.currentAge;
    //udah ngisi profile
    
    // buat proses penghitungan aturan 4%+inflasi
    //1. hitung pengeluaran pertahun 
    long long yearlyExpenses = u.Profile.monthlyExpense * 12;
    //2. proyeksi infalsi masa depan function FV= PV* (1+inflation rate)^n n=jumlah tahun targetnya
    long long futureValue = yearlyExpenses*pow((1+(u.Profile.inflationRate/100)),jarakTahun);
    //3. target asetnya minimal menggunakan the 4%rule kaliin 25 function 
    u.Profile.targetAmount = futureValue * 25;
    
    
    //tampilin hasil hitungan

    // save data ya
    u.Profile.isDone = true;     
    saveAll();
    
    cout<<"\n --------------------------------------------------------------------------";
    cout<<"\n Pengeluaran Masa Depan : "<<formatMoney(futureValue); cout<<"/ tahun";
    cout<<"\n Target Kekayaan        : "<<formatMoney(u.Profile.targetAmount); 
    cout<<"\n Sisa Waktu Pencapaian  : "<< jarakTahun<<" tahun"; 
    cout<<"\n --------------------------------------------------------------------------";
    
    pauseScreen();
}


void menuSetor(){
    int bln, thn;
    long long nominal;
    string catatan;
    users& u = user[userAktif];
    
    if(!user[userAktif].Profile.isDone){

        cout << "\n [!] Harap lengkapi profil terlebih dahulu (Menu 1).\n";
        pauseScreen();
        return;
    }
    
    cout << "\n +=================================================+\n";
    cout << " |   [FIRE] F.I.R.E. DASHBOARD                      |\n";
    cout << " |   Setor Celengan                                 |\n";
    cout << " +=================================================+\n";
    cout << "\n Masukkan detail setoran:\n\n";
    
    while(true){
        cout << " Bulan (1-12) : "; cin >> bln;
        if(!cin.fail() && bln >= 1 && bln <=12){ //cek validasi input bulan yang harus angka dan 1-12
            break; //input bener, keluar dari loop bulan
        }
        cin.clear(); //hapus error kalo input selain angka
        cin.ignore(1000, '\n'); //buat bersihin sisa input yang salah
        cout << "[Error] Bulan tidak valid! Masukkan bulan 1-12.\n";
    } 
    
    cout << " Tahun        : "; cin >> thn;
    
    while(true){
        cout << " Nominal (Rp) : "; cin >> nominal;
        if(!cin.fail() && nominal >0 ){ //cek validasi input nominal yang harus angka dan lebih besar dari 0
            break; //input bener, keluar dari loop nominal
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "[Error] Nominal harus berupa angka dan lebih besar dari 0.\n";
    }
    
    cin.ignore();
    cout << " Catatan      : "; getline(cin, catatan);
    
    u.totalSavings += nominal; //menambah dan update total saldo
    
    //nyimpan ke array history user yang aktif
    int idx = u.jumlahTransaksi; //buat nampung index transaksi yang sekarang
    u.history[idx].id = idx + 1;
    u.history[idx].bulan = bln; //masukin data transaksi ke database riwayat
    u.history[idx].tahun = thn;   
    u.history[idx].nominal = nominal;
    u.history[idx].catatan = catatan; 
    u.jumlahTransaksi++; //update jumlah transaksi    
    
    long double sisaTarget = u.Profile.targetAmount - u.totalSavings; //hitung sisa target aset   
    if(sisaTarget < 0){
        sisaTarget = 0;
        cout<<"\n CONGRATSS!!!! TARGET F.I.R.E ANDA TERCAPAIIIII";
    }

    //tampilkan output berhasil
    cout << "\n[OK] Setoran berhasil dicatat!\n\n";
    cout << "----------------------------------------------------\n";
    cout << "Jumlah setor : "<<formatMoney(nominal)<< "\n";
    cout << "Total saldo  : "<<formatMoney(u.totalSavings)<< "\n";
    cout << "Sisa target  : "<<formatMoney(sisaTarget)<< "\n";
    cout << "----------------------------------------------------\n";   
    
    saveAll(); 
    pauseScreen();
}


double hitungBungaBerbunga(double saldo, double  roi, int tahun){
    if(tahun==0)return saldo; 
    return hitungBungaBerbunga(saldo*(1.0 +roi),roi,tahun-1);
} //hitung pertumbuhan uang di instrument investasi pertahun

int tahunHinggaTarget(double saldo, double yearlySav, double roi, double target, int tahun){
    if(saldo>= target){
        return tahun;
    } //sudah tercapai target pensiunnnya return tahun yang dibutuhkan
    if(tahun >100){ //pengkondisian where lebih dari 100 tahun berheniti 
        return -1;
    }
    
    return tahunHinggaTarget(hitungBungaBerbunga(saldo,roi,1)+yearlySav,yearlySav,roi,target,tahun+1);
}


void menulaporan(){
    long long monthlySav;
    users& u= user[userAktif];
    long long avgMonthly=0;
    string statusPensiun="";
    long long totalNominal = 0;
    
    if(u.jumlahTransaksi>0){
        for(int i = 0; i < u.jumlahTransaksi; i++){
            totalNominal += u.history[i].nominal;
        }
        avgMonthly = totalNominal / u.jumlahTransaksi;
    }

    cout<<"\n =================== CETAK LAPORAN LENGKAP ========================\n";
    if(!u.Profile.isDone){
        cout<<"\n Lengkapi profile terlebih dahulu, bisa di isi di menu [1].";
        pauseScreen();
        return;
    }

    cout<<"\n -----------------------------------------------------------";
    if(avgMonthly>0){
        cout<<"\n Tabungan Rutin /month (Rp) [ rata-rata: "<<formatMoney(avgMonthly)<<"]";     
        cout<<"\n -----------------------------------------------------------";
        cout<<"\n [!] Apabila ingin menggunakan rata-rata diatas (0)";
    }
    cout<<"\n [!] Atau ingin memasukan target tabungan sendiri /month\n";
    cout<<"\n Berapa target tabungan rutin perbulan?";
    cout<<"\n Pilihan Anda (Rp) : ";cin>>monthlySav;
    
    if(monthlySav<=0 && avgMonthly>0){
        monthlySav =avgMonthly;
    }else if(monthlySav<=0){
        cout<<"\n Masukkan nominal investasi bulanan Anda!";
    }
    
    double roi;
    cout<<"\n Asumsi Return of investment (ROI) yang diletakan pada instrument investasi /tahun(%): ";cin>>roi;
    
    roi=roi/100;
    double minimalReturn = (u.Profile.inflationRate/100) + 0.04;
    
    int menuROI;
    if(roi< minimalReturn){
        cout<<"\n [!] Asumsi ROI terlalu rendah [!]";
        cout<<"\n Inflasi diasumsikan     : "<<u.Profile.inflationRate<<"%";
        cout<<"\n Aturan Penarikan aman   : "<<"4%";
        cout<<"\n Minimal ROI aman /tahun : "<<minimalReturn*100<<"% per tahun";
        cout<<"\n ROI yang nada masukan "<<roi<<"% per tahun";
        cout<<"\n ------------------------------------------------------------------------------------------------";
        cout<<"\n [Apabila anda melanjutkan untuk menaruh uang di instrumen yang roi nya di bawah minimal value, \n dana akan berresiko habis di masa tua karena tidak bisa menutup inflasi pertahunnya].";
        cout<<"\n [1.] Tetap melanjutkan dengan return of investment sekarang";
        cout<<"\n [2.] Mengubah angka return of investment ke batas minimal aman secara otomatis";
        cout<<"\n ------------------------------------------------------------------------------------------------";
        cout<<"\n pilihan anda(1-2): ";cin>>menuROI;
        
        if(menuROI == 2){
            roi=minimalReturn;
            cout<<"\n ROI di sesuaikan dengan batas minimal aman instrumen pertumbuhan";
        }else{
            cout<<"\n Menggunakan inputan ROI, harap meninjau terkait risiko";
        }
        
    }
    
    //konversi saving ke tahunan
    long long yearlySav= monthlySav*12;
    
    //untuk array 2d
    const int maxTahun = 50;
    double tabel[maxTahun][5];
    //mencari sisa tahun buat pensiun
    int tahunPensiun=tahunHinggaTarget(u.totalSavings,yearlySav,roi,u.Profile.targetAmount,0);
    
    cout<<"\n ------------------------------------------------------------------------------------------------";
    cout<<"\n Target Dana F.I.R.E           : "<<formatMoney(u.Profile.targetAmount);
    cout<<"\n Investasi Rutin anda          : "<<monthlySav<<" / bulan";
    cout<<"\n Asumsi Return of Investment   : "<<roi*100<<" % per tahun";
    cout<<"\n ------------------------------------------------------------------------------------------------";
    
    //pengkondisian what if
    int usiaPensiun=0;
    if(tahunPensiun != -1){
        usiaPensiun= u.Profile.currentAge + tahunPensiun;
    }
    if( tahunPensiun == -1){
        cout<<"\n Target tidak tercapai dalam 100 tahun";
        cout<<"\n naikan nilai pertumbuhan instrumen investasi";
        cout<<"\n ------------------------------------------------------------------------------------------------";
        pauseScreen();
    }else{
        cout << "\n Waktu Realistis Dicapai: " << tahunPensiun << " tahun lagi\n";
        cout << " Prediksi Usia Pensiun  : " << usiaPensiun<< " tahun\n";
        if(usiaPensiun<= u.Profile.targetAge){

            int pensiunAwal = u.Profile.targetAge - usiaPensiun;
            cout<<"\n----@@  [SELAMAT ANDA BERHASIL PENSIUN TEPAT WAKTU/ LEBIH CEPAT] @@-----";
            statusPensiun = "----@@  [SELAMAT ANDA BERHASIL PENSIUN TEPAT WAKTU ATAU LEBIH CEPAT] @@-----";
            cout<<"\n Anda pensiun "<<pensiunAwal<<" tahun lebih cepat";
            cout<<"\n ------------------------------------------------------------------------------------------------";
            pauseScreen();
        }else{
            int pensiunTerlambat=   usiaPensiun-u.Profile.targetAge;
            cout<<"\n ## [PERINGATAN] ##";
            cout<<"\n Pensiun anda Terlambat "<<pensiunTerlambat<<" tahun";
            
            cout<<"\n Tabungan / Instrument investasi anda kurang agresif sehingga telambat dari target pensiun";
            cout<<"\n ------------------------------------------------------------------------------------------------";
            statusPensiun ="[PENSIUN ANDA TERLAMBAT]";
            pauseScreen();
        }
    }
   
    //for decision making
    long long saldoBerjalan = u.totalSavings;
    int lamaSimulasi = (tahunPensiun == -1 || tahunPensiun > maxTahun) ? maxTahun : tahunPensiun;
    
    for (int i = 0; i < lamaSimulasi; i++) {
        tabel[i][0] = i + 1;                             // Tahun ke-
        tabel[i][1] = u.Profile.currentAge + (i + 1);    // Usia pada tahun itu
        tabel[i][2] = saldoBerjalan;                     // Saldo awal tahun
        tabel[i][3] = yearlySav;                         // Setoran setahun
        
        saldoBerjalan = (saldoBerjalan + yearlySav) * (1.0 + roi);
        tabel[i][4] = saldoBerjalan;                     // Saldo akhir tahun
    }
    
    string namaFile = u.username + "_laporan_fire.txt"; 
    ofstream fileLaporan(namaFile);
    
    if (fileLaporan.is_open()) {
        fileLaporan << "=================================================================================\n";
        fileLaporan << "                      LAPORAN PERENCANAAN DANA F.I.R.E                           \n";
        fileLaporan << "=================================================================================\n";
        fileLaporan << " Username Pengguna      : " << u.username << "\n";
        fileLaporan << " Usia Saat Ini          : " << u.Profile.currentAge << " tahun\n";
        fileLaporan << " Target Usia Pensiun    : " << u.Profile.targetAge << " tahun\n";
        fileLaporan << " Target Dana F.I.R.E    : Rp " << formatMoney(u.Profile.targetAmount) << "\n";
        fileLaporan << " Investasi Bulanan      : Rp " << formatMoney(monthlySav) << " / bulan\n";
        fileLaporan << " Asumsi ROI Pertahun    : " << roi * 100 << " %\n";
        fileLaporan << " Hasil Analisis Simulasi: " << statusPensiun << "\n";
        if(tahunPensiun != -1) {
            fileLaporan << " Prediksi Usia Pensiun  : " << usiaPensiun << " tahun (" << tahunPensiun << " tahun dari sekarang)\n";
        }
        fileLaporan << "=================================================================================\n\n";
        
        fileLaporan << "---------------------------------------------------------------------------------\n";
        fileLaporan << " Thn | Usia |     Saldo Awal (Rp)    |   Investasi/Thn (Rp)  |    Saldo Akhir (Rp)   \n";
        fileLaporan << "---------------------------------------------------------------------------------\n";
        
        for (int i = 0; i < lamaSimulasi; i++) {
            fileLaporan << " " << tabel[i][0] << "\t| " 
                        << tabel[i][1] << "\t| "
                        << formatMoney((long long)tabel[i][2]) << "\t\t| "
                        << formatMoney((long long)tabel[i][3]) << "\t\t| "
                        << formatMoney((long long)tabel[i][4]) << "\n";
        }
        fileLaporan << "---------------------------------------------------------------------------------\n";
        fileLaporan << "* Catatan: Angka di atas merupakan hasil simulasi matematika berdasar asumsi ROI tetap.\n";
        
        fileLaporan.close();
        cout << "\n [✓] Laporan berhasil disimpan ke file: " << namaFile << "\n";
        pauseScreen();
    } else {
        cout << "\n [X] Gagal menyimpan laporan ke file.";
    }
    
}

void menuTransaksi(){
    users& u = user[userAktif];
    int pilSub;
    
    do{
        system("cls");

        cout << "\n  +================================================+\n";
        cout << " |   [FIRE] KELOLA, CARI & HAPUS                    |\n";
        cout << "  +================================================+\n";
        cout << " |  [1] Tampilkan Semua Transaksi                   |\n";
        cout << " |  [2] Cari Transaksi                              |\n";
        cout << " |  [3] Hapus Transaksi                             |\n";
        cout << " |  [0] Kembali ke Menu Utama                       |\n";
        cout << "  +================================================+\n";
        cout << "Pilih Menu (0-3): "; cin >> pilSub;
        
        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n [Error] Masukkan pilihan harus berupa angka (0-3)!\n";
            pauseScreen();
            continue;
        }
        cin.ignore();
        
        switch(pilSub){
            case 1:{
                cout << "\n================ DAFTAR TRANSAKSI ================\n\n";
                if(u.jumlahTransaksi == 0){
                    cout << "Belum ada transaksi yang tercatat.\n";
                }else{
                    cout << "-----------------------------------------------\n";
                    cout << " ID  | Bulan/Tahun | Nominal (Rp) | Catatan\n";
                    cout << "-----------------------------------------------\n";
                    for(int i = 0; i < u.jumlahTransaksi; i++){
                        cout << " " << u.history[i].id << "   | "
                             << u.history[i].bulan << "/" << u.history[i].tahun << "     | "
                             << formatMoney(u.history[i].nominal) << "  | "
                             << u.history[i].catatan << "\n";
                    } 
                    cout << "--------------------------------------------------\n";
                    cout << " Total Saldo Saat Ini: " << formatMoney(u.totalSavings) << "\n";
                }
                cout << "==================================================\n";
                pauseScreen();
                break;
            }
               
            case 2:{
                if(u.jumlahTransaksi == 0){
                    cout << "\n Belum terdapat transaksi yang tercatat.\n";
                    pauseScreen();
                    break;
                }

                int idCari;
                cout << "\n Masukkan ID transaksi yang ingin dicari: ";
                cin >> idCari;

                if(cin.fail()){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n [Error] ID harus berupa angka!\n";
                    pauseScreen();
                    break;
                }
                cin.ignore();
                
                bool ditemukan = false;
                for(int i = 0; i < u.jumlahTransaksi; i++){
                    if(u.history[i].id == idCari){
                        cout << "\n---------------- HASIL PENCARIAN ----------------\n";
                        cout << " ID  | Bulan/Tahun | Nominal (Rp) | Catatan\n";
                        cout << "-----------------------------------------------\n";
                        cout << " " << u.history[i].id << "   | "
                             << u.history[i].bulan << "/" << u.history[i].tahun << "  | "
                             << formatMoney(u.history[i].nominal) << "  | "
                             << u.history[i].catatan << "\n";
                        ditemukan = true;
                        break;
                    }
                }
                if(!ditemukan){
                    cout << "\n [!] Transaksi dengan ID " << idCari << " tidak ditemukan.\n";
                }
                cout << "------------------------------------------------\n";
                pauseScreen();
                break;
            }
            
            case 3:{
                if(u.jumlahTransaksi == 0){
                    cout << "\n [!] Belum ada transaksi yang tercatat.\n";
                    pauseScreen();
                    break;
                }

                cout << "\n  ID  | Bulan/Tahun | Nominal (Rp) | Catatan\n";
                cout << "-----------------------------------------------\n";
                for(int i = 0; i < u.jumlahTransaksi; i++){
                    cout << " " << u.history[i].id << "   | "
                    << u.history[i].bulan << "/" << u.history[i].tahun << "  | "
                         << formatMoney(u.history[i].nominal) << "  | "
                         << u.history[i].catatan << "\n";
                }
                cout << "------------------------------------------------\n";
                
                int idHapus;
                cout << " Masukkan ID transaksi yang ingin dihapus: ";
                cin >> idHapus;
                
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n [Error] ID harus berupa angka!\n";
                    pauseScreen();
                    break;
                }
                cin.ignore();
                
                int idxHapus = -1;
                for(int i = 0; i < u.jumlahTransaksi; i++){
                    if(u.history[i].id == idHapus){
                        idxHapus = i;
                        break;
                    }
                }

                if(idxHapus == -1){
                    cout << "\n [!] Transaksi dengan ID " << idHapus << " tidak ditemukan.\n";
                }else{
                    u.totalSavings -= u.history[idxHapus].nominal; //update total saldo setelah hapus transaksi

                    for(int i = idxHapus; i < u.jumlahTransaksi - 1; i++){
                        u.history[i] = u.history[i + 1]; //geser transaksi setelah yang dihapus ke kiri
                    }
                    u.jumlahTransaksi--; //update jumlah transaksi
                    
                    for(int i = 0; i < u.jumlahTransaksi; i++){
                        u.history[i].id = i + 1; // reset ID agar tetap urut
                    }//update id
                    saveAll(); //simpan perubahan ke file
                    
                    cout << "\n [OK] Transaksi dengan ID " << idHapus << " berhasil dihapus!\n";
                    cout << " Total Saldo Saat Ini: " << formatMoney(u.totalSavings) << "\n";
                    
                }
                pauseScreen();
                break;
            }
              
            case 0:
            break;
            default:
            cout << "\n [Error] Pilihan tidak valid! Masukkan angka antara 0-3.\n";
            pauseScreen();
        }
    }
    while(pilSub != 0);
}


void loadData(){
    ifstream file("database_users.txt",ios::in);

    if(!file.is_open()){
        return;
    }

    file>>jumlahUser;
    file.ignore();
    for(int i=0;i<jumlahUser && i<userMax;i++){
        getline(file,user[i].username); //tring data type
        getline(file,user[i].password);
        file>>user[i].Profile.currentAge
        >>user[i].Profile.targetAge
            >>user[i].Profile.monthlyExpense
            >>user[i].Profile.inflationRate
            >>user[i].Profile.targetAmount
            >>user[i].totalSavings
            >>user[i].Profile.isDone;
            file>> user[i].jumlahTransaksi;
    file.ignore();
    for(int j=0; j<user[i].jumlahTransaksi && j<maxHistory;j++){
        file>>user[i].history[j].id
                >>user[i].history[j].bulan
                >>user[i].history[j].tahun
                >>user[i].history[j].nominal;
                file.ignore();
                getline(file, user[i].history[j].catatan);
        }
    }
    file.close();
}

void halUtama(){
    int pilHalUtama;
    users& u = user[userAktif];

    do{
        system("cls");
        cout << "\n  +==================================================+\n";
        cout << "  |   [FIRE]  F.I.R.E. MAIN MENU                     |\n";
        cout << "  |   Financial Independence, Retire Early           |\n";
        cout << "  +==================================================+\n";
        cout << "     Halo, " << user[userAktif].username << "!\n"; 
        cout << "     Saldo: " << formatMoney(u.totalSavings) << "\n"; 
        
        cout << "  +==================================================+\n";
        cout << "  |  [1] Kelola Profil & Target Pensiun              |\n";
        cout << "  |  [2] Setor Celengan                              |\n";
        cout << "  |  [3] Proyeksi dan Laporan F.I.R.E                |\n";
        cout << "  |  [4] Hapus, Cari & Tampilkan Transaksi           |\n";
        cout << "  |  [0] Simpan, Logout & Keluar                     |\n";
        cout << "  +==================================================+\n";
        cout << "\n  Pilih Menu (0-4): "; cin >> pilHalUtama;

        cin.ignore();

        
        switch(pilHalUtama){
            case 1:
                menuProfile();
                break;
            case 2:
                menuSetor();
                break;
            case 3:
                menulaporan();
                break;
            case 4:
                menuTransaksi();
                break;
            case 0:
                saveAll();
                cout << "\nTerima kasih telah menggunakan aplikasi kami. Sampai jumpa!\n";
                pauseScreen();
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan pilih antara 0-4.\n";
        } 
    }while(pilHalUtama != 0);
}

int main(){
    //inisialisasi data
    loadData();
    do{
        //halaman login
        do{
            system("cls");
                cout<<"\n  #============================#";
                cout<<"\n |        F.I.R.E APP           |";
                cout<<"\n  #============================#";
                cout<<"\n | 1. Register                  |";
                cout<<"\n | 2. Login                     |";
                cout<<"\n | 3. Exit                      |";
                cout<<"\n  #============================#";
                cout<<"\n Pilih Menu (1-3): ";cin>>menuAwal;
                
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(1000,'\n');
                    cout<<" kesalahan dalam input!";
                    menuAwal=0;
                }
        }while (menuAwal<1 || menuAwal>3);
    
        switch (menuAwal){
        case 1:{

            //register
            //1. pengkondisian apakah memori masi ada
            if(jumlahUser>= userMax){
                cout<<" Memori penuh! tidak bisa daftar";
                break;
            }
            //2. masukin uname
            bool found=false;
            do{
                found=false;
                cout<<"\n=========== REGISTER ================\n";
                cout<<"\n Username: ";cin>>usernameNew;
                for(int i=0;i<jumlahUser;i++){

            //3.cek uname dah ada atau belum, kalau udah bisalanjut kalau belum ulang dari masukin uname
                    if(usernameNew == user[i].username){
                        found=true;
                        cout<<"\n username sudah dipakai, silahkan buat yang baru";
                        break;
                    }
                }
            }while(found);
            // jangan lupa kalau berhasil pindahin dari username new ke database username
            
                //4. masukin pass
                cout<<" Password :";cin>>passwordNew;

                user[jumlahUser].username = usernameNew;
                user[jumlahUser].password = passwordNew;
                user[jumlahUser].Profile.isDone=false;

                jumlahUser++;
                saveAll();

                cout<<"\n Selamat Registrasi Berhasil!";
                cout<<"\n ==================================================";
                pauseScreen();
            //5. simpan di database pass and uname
            break;
        }
        case 2:{
            //Login
            //1.masukin uname and password, kasih eror handling uname atau password yang salah
            int attempt=0;
            bool login=false;
            do{
                cout<<"\n=========== LOGIN PAGE ===============";
                cout<<"\n Kembali ke Menu [0]\n";
                cout<<"\n Username : ";cin>>logUser;
                if (logUser == "0")break;
                cout<<" Password : ";cin>>logPass;
                
                attempt++;
                //2. if benar masuk applikasi kalau salah balik ke halaman login
                for(int j=0;j<jumlahUser;j++){
                    if(user[j].password == logPass && user[j].username == logUser){
                        login=true;
                        userAktif=j;
                        break;
                    }
                }
                if(!login)cout<<"\n [!] Username atau Password salah\n";
                if(attempt>=3){
                    cout<<"\n Terlalu Banyak Percobaan";
                    pauseScreen();
                    break;
                }
            }while(!login);
            
            if(login){

                cout<<"\n Selamat Anda berhasil LOGIN";
                //function buat menu apps
                halUtama();
            }

            break;
        }
        case 3:
            //keluar program
            cout<<"\nTerimakasih telah menggunakan apps kami ;))";
            //exit
            isexit = true;
            break;
        
        default:
            cout<<"\n Pilihan anda tidak valid";
            break;
        
        }

    }while (isexit == false);

}

