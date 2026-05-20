#include <iostream>
using namespace std;

int isexit = false;

struct userProfile{
    int currentAge;
    int targetAge;
    long double monthlyExpense;
    double inflationRate;
    bool isDone;
    
};

struct users{
    string username;
    string password;
    userProfile Profile;

};

int jumlahUser=0,menuAwal,userMax=100;
int userAktif=-1;
string usernameNew,passwordNew,logPass,logUser;

void halUtama(){
    int pilHalUtama;

    do{
        cout << "\n+==================================================+\n";
        cout << "  |   [FIRE]  F.I.R.E. DASHBOARD                     |\n";
        cout << "  |   Financial Independence, Retire Early           |\n";
        cout << "  +==================================================+\n";
        cout << "  |  Halo, " << u.username << "!\t\t\t\t\t     |\n"; // \t tu kaya tab buat ngasih jarak
        //cout << "  |  Saldo: " << u.Profile.currentSavings << "\t\t\t\t     |\n"; 
        
        cout << "  +==================================================+\n";
        cout << "  |  [1] Kelola Profil & Target Pensiun              |\n";
        cout << "  |  [2] Setor Celengan                              |\n";
        cout << "  |  [3] Riwayat & Analisis Tabungan                 |\n";
        cout << "  |  [4] Ramalan Pensiun (Rekursi)                   |\n";
        cout << "  |  [0] Simpan, Logout & Keluar                     |\n";
        cout << "  +==================================================+\n";
        cout << "\n  Pilih Menu (0-4): "; cin >> pilHalUtama;

        cin.ignore();

        /*
        switch(pilHalUtama){
            case 1:
                //menu profile & target pensiun
                break;
            case 2:
                //menu setor celengan
                break;
            case 3:
                //menu riwayat & analisis tabungan
                break;
            case 4:
                //menu ramalan pensiun
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
}

void menuProfile(){
    users u[userAktif];

    //buat pengkondisian kallau pernah ngisi profile belum kalao belum ngisi kaalu udah tampilin yang lama
    if(u[userAktif].Profile.isDone){

    }
    //ngisi profile kalau belum
    cout<<"\n Masukan data Profile Anda: \n";
    cout<<"\n Umur Sekarang : ";cin>>u[userAktif].Profile.currentAge;
    cout<<"\n Target Pensiun Dini : ";cin>>u[userAktif].Profile.targetAge;
    cout<<"\n Asumsi Tingkat Inflasi(%) : ";cin>>u[userAktif].Profile.inflationRate;
    cout<<"\n Pengeluaran anda (/month) : ";cin>>u[userAktif].Profile.monthlyExpense;

    u[userAktif].Profile.isDone = true;             //udah ngisi profile

    // buat proses penghitungan aturan 4%+inflasi
    //1. hitung pengeluaran pertahun function

    //2. proyeksi infalsi masa depan function
    //3. target asetnya minimal menggunakan the 4%rule kaliin 25 function 
    //tampilin hasil hitungan

}

string formatMoney(double money){
    

}

long double yearlyExpenses(){
    
}

int main(){
    users user[userMax];

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

