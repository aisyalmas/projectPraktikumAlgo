#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int isexit = false;
const int maxHistory =100;

struct Transaction{
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

void halUtama(){
    users u;
    int pilHalUtama;

    do{
        cout << "\n+==================================================+\n";
        cout << "  |   [FIRE]  F.I.R.E. MAIN MENU                     |\n";
        cout << "  |   Financial Independence, Retire Early           |\n";
        cout << "  +==================================================+\n";
        cout << "  |  Halo, " << user[userAktif].username << "!\t\t\t\t\t     |\n"; // \t tu kaya tab buat ngasih jarak
        //cout << "  |  Saldo: " << u.Profile.currentSavings << "\t\t\t\t     |\n"; 
        
        cout << "  +==================================================+\n";
        cout << "  |  [1] Kelola Profil & Target Pensiun              |\n";
        cout << "  |  [2] Setor Celengan                              |\n";
        cout << "  |  [3] Riwayat & Analisis Tabungan                 |\n";
        cout << "  |  [4] Proyeksi F.I.R.E                            |\n";
        cout << "  |  [0] Simpan, Logout & Keluar                     |\n";
        cout << "  +==================================================+\n";
        cout << "\n  Pilih Menu (0-4): "; cin >> pilHalUtama;

        cin.ignore();

        /*
        switch(pilHalUtama){
            case 1:
                menuProfile();
                break;
            case 2:
                //menu setor celengan
                break;
            case 3:
                //menu riwayat & analisis tabungan
                break;
            case 4:
                proyeksiKeuangan();
                break;
            case 0:
                simpanKeFile();
                cout << "\nTerima kasih telah menggunakan aplikasi kami. Sampai jumpa!\n";
                isexit = true;
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan pilih antara 0-4.\n";
        } */
    }
}while();

void menuProfile(){
    users& u=user[userAktif];
    char pilih;
    //buat pengkondisian kallau pernah ngisi profile belum kalao belum ngisi kaalu udah tampilin yang lama
    if(u.Profile.isDone){
        cout<<"\n Data Proflie Anda: ";
        cout<<"------------------------------------------------------";
        cout<<"\n Username            : "<<u.username;
        cout<<"\n Umur Sekarang       : "<<u.Profile.currentAge;
        cout<<"\n Target Usia Pensiun : "<<u.Profile.targetAge;
        cout<<"\n Pengeluaran Bulanan : "<<formatMoney(u.Profile.monthlyExpense)<<"/month";
        cout<<"\n Asumsi Inflasi      : "<<u.Profile.inflationRate<<"%";
        cout<<"------------------------------------------------------";
        cout<<"\n Target Aset         : "<<formatMoney(u.Profile.targetAmount);
        cout<<"\n Saldo Tabungan      : "<<formatMoney(u.totalSavings);
        cout<<"------------------------------------------------------";

        cout<<"\n Update Profile anda?(y/n):";cin>>pilih;
        if(pilih != 'y'|| pilih !='Y')return;
    }
    //ngisi profile kalau belum
    cout<<"\n Masukan data Profile Anda: \n";
    cout<<"------------------------------------------------------";
    cout<<"\n Umur Sekarang : ";cin>>u.Profile.currentAge;
    cout<<"\n Target Umur Pensiun Dini : ";cin>>u.Profile.targetAge;
    cout<<"\n Asumsi Tingkat Inflasi(%) : ";cin>>u.Profile.inflationRate;
    cout<<"\n Pengeluaran anda (/month) : ";cin>>u.Profile.monthlyExpense;

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
    saveAll();

    u.Profile.isDone = true;     
    cout<<"\n --------------------------------------------------------------------------";
    cout<<"\n Pengeluaran Masa Depan : ";formatMoney(futureValue); cout<<"/ tahun";
    cout<<"\n Target Kekayaan : ";formatMoney(u.Profile.targetAmount); 
    cout<<"\n Sisa Waktu Pencapaian : "<< jarakTahun<<" tahun"; 
    cout<<"\n --------------------------------------------------------------------------";
    
    pauseScreen();
}


void menuSetor(){
    int bln, thn;
    long double nominal;
    string catatan;
    users& u = user[userAktif];

    cout << "\n+==================================================+\n";
    cout << " |   [FIRE] F.I.R.E. DASHBOARD                      |\n";
    cout << " |   Setor Celengan                                 |\n";
    cout << "+==================================================+\n";
    cout << "\n Masukkan detail setoran:\n\n";

    while(true){
        cout << "Bulan (1-12): "; cin >> bln;
        if(!cin.fail() && bln < 1 && bln > 12){ //cek validasi input bulan yang harus angka dan 1-12
            break; //input bener, keluar dari loop bulan
        }
        cin.clear(); //hapus error kalo input selain angka
        cin.ignore(1000, '\n'); //buat bersihin sisa input yang salah
        cout << "[Error] Bulan tidak valid! Masukkan bulan 1-12.\n";
    } 

    cout << "Tahun: "; cin >> thn;

    while(true){
        cout << "Nominal (Rp): "; cin >> nominal;
        if(!cin.fail() && nominal >0 ){ //cek validasi input nominal yang harus angka dan lebih besar dari 0
            break; //input bener, keluar dari loop nominal
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "[Error] Nominal harus berupa angka dan lebih besar dari 0.\n";
    }
    
    cin.ignore();
    cout << "Catatan: "; getline(cin, catatan);

    u.totalSavings += nominal; //menambah dan update total saldo

    //nyimpan ke array history user yang aktif
    int idx = u.jumlahTransaksi; //buat nampung index transaksi yang sekarang
    u.history[idx].bulan = bln; //masukin data transaksi ke database riwayat
    u.history[idx].tahun = thn;   
    u.history[idx].nominal = nominal;
    u.history[idx].catatan = catatan; 
    u.jumlahTransaksi++; //update jumlah transaksi    

    long double sisaTarget = targetAset - u.totalSavings; //hitung sisa target aset   
    if(sisaTarget < 0){
        sisaTarget = 0;
        cout<<"\n CONGRATSS!!!! TARGET F.I.R.E ANDA TERCAPAIIIII";
    }

    //tampilkan output berhasil
    cout << "[OK] Setoran berhasil dicatat!\n";
    cout << "----------------------------------------------------\n";
    cout << "Jumlah setor : "<<formatMoney(nominal)<< "\n";
    cout << "Total saldo  : "<<formatMoney(u.totalSavings)<< "\n";
    cout << "Sisa target  : "<<formatMoney(sisaTarget)<< "\n";
    cout << "----------------------------------------------------\n";   
    pauseScreen();
}

void riwayat_analisis(){}


void proyeksiKeuangan(){
    long long pilih=0;
    users& u= user[userAktif];
    long long avg=u.totalSavings/u.jumlahTransaksi;

    if(!u.Profile.isDone){
        cout<<"\n Lengkapi profile terlebih dahulu, bisa di sis di menu [1].";
        return;
    }

    cout<<"\n --------------------------------------------------";
    if(avg>0){
        cout<<"\n Tabungan Rutin /month (Rp) [ rata-rata: "<<formatMoney(avg)<<"]";     
        cout<<"\n --------------------------------------------------";
        cout<<"\n [!] apabila ingin menggunakan rata-rata diatas (0)";
    }
    cout<<"\n Berapa target tabungan rutin perbulan?";
    cout<<"\n [!] atau ingin memasukan target tabungan sendiri /month";
    cout<<"\n pilihan anda (Rp ) :";cin>>pilih;



}

void loadData(){
    ifstream file("database_users.txt",ios::in);

    if(!file.is_open()){
        cout<<"\n maaf file tidak dapat dibuka, gagal memuat file";
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
            file>>user[i].history[j].bulan
                >>user[i].history[j].tahun
                >>user[i].history[j].nominal;
                file.ignore();
            getline(file, user[i].history[j].catatan);
        }
    }
    file.close();
}
void saveAll(){
    ofstream file("database_users.txt",ios::out);

    if(!file.is_open()){
        cout<<"\n Maaf file tidak dapat dibuka, gagal menyimpan file";
        return;
    }

    file<<jumlahUser;
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
            file<<user[i].history[j].bulan<<"\n"
                <<user[i].history[j].tahun<<"\n"
                <<user[i].history[j].nominal<<"\n"
                <<user[i].history[j].catatan<<"\n";
        }
    }
    file.close();
}

int main(){
    //inisialisasi data
    loadData();
    do{
        
        //halaman login
        do{
                cout<<"\n #=============================#";
                cout<<"\n |        Theme                 |";
                cout<<"\n #=============================#";
                cout<<"\n | 1. Register                  |";
                cout<<"\n | 2. Login                     |";
                cout<<"\n | 3. Exit                      |";
                cout<<"\n #=============================#";
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
                cout<<"\n=========== REGISTER ============";
                cout<<"\n Username: ";cin>>usernameNew;
                for(int i;i<jumlahUser;i++){

            //3.cek uname dah ada atau belum, kalau udah bisalanjut kalau belum ulang dari masukin uname
                    if(usernameNew == user[i].username){
                        found=true;
                        break;
                    }
                }
            }while(found);
            // jangan lupa kalau berhasil pindahin dari username new ke database username
            
                //4. masukin pass
                cout<<"\n password :";cin>>passwordNew;

                user[jumlahUser].username = usernameNew;
                user[jumlahUser].password = passwordNew;
                user[jumlahUser].Profile.isDone=false;

                jumlahUser++;
                cout<<"\n selamat registrasi berhasil";

            
            //5. simpan di database pass and uname
            break;
        }
        case 2:
            //Login
            //1.masukin uname and password, kasih eror handling uname atau password yang salah
            bool login=false;
            do{
                cout<<"\n=========== LOGIN PAGE ===============";
                cout<<"\n Username : ";cin>>logUser;
                cout<<"\n Password : ";cin>>logPass;
                
                //2. if benar masuk applikasi kalau salah balik ke halaman login
                for(int j;j<jumlahUser;j++){
                    if(user[j].password == logPass && user[j].username == logUser){
                        login=true;
                        userAktif=j;
                        break;
                    }
                }

            }while(login);
            
            cout<<"\n Selamat Anda berhasil LOGIN";
            //function buat menu apps
            halUtama();

            break;
        case 3:
            //keluar program
            cout<<"terimakasih telah menggunakan apps kammi ;))";
            //exit
            isexit = true;
            break;
        
        default:
            cout<<"\n Pilihan anda tidak valid";
            break;
        
        }

    }while (isexit == false);

}

