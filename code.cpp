#include <iostream>
using namespace std;

int isexit = false;

struct users{
    string username;
    string password;
};



int main(){
    int jumlahUser=0,menuAwal,userMax=100;
    string usernameNew,passwordNew,logPass,logUser;
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
            if(found == false){
                //4. masukin pass
                cout<<"\n password :";cin>>passwordNew;

                user[jumlahUser].username = usernameNew;
                user[jumlahUser].password = passwordNew;

                cout<<"\n selamat registrasi berhasil";
            }
            
            //5. simpan di database pass and uname
            break;
        }
        case 2:
            //Login
            //1.masukin uname and password, kasih eror handling uname atau password yang salah
            
            do{
                cout<<"\n=========== LOGIN PAGE ===============";
                cout<<"\n Username : ";cin>>logUser;
                cout<<"\n Password : ";cin>>logPass;
                
                for(int j;j<jumlahUser;j++){
                    if(user[j].password == logPass && user[j].username == logUser){

                    }
                }

            }while();
            //2. if benar masuk applikasi kalau salah balik ke halaman login
            //a.buat app disini masuk ke fungsi appnya tampilan menu

            break;
        case 3:
            //keluar program
            //exit
            isexit = true;
            break;
        
        default:
            cout<<"\n Pilihan anda tidak valid";
            break;
        
        }

    }while (isexit == false);

}