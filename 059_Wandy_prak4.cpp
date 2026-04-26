#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Service
{
    string modelMobil, merekMobil, namaPelanggan, tanggalAmbil, levelKepentingan, namaMontir, deskripsiKendala;
    Service *next;
};
Service *headServ = NULL;
Service *tailServ = NULL;
Service *frontAntri = NULL;
Service *rearAntri = NULL;

// ================= TAMBAHAN =================
string tambahHari(string tanggal)
{
    int d, m, y;
    sscanf(tanggal.c_str(), "%d-%d-%d", &d, &m, &y);

    d += 1;

    int hariPerBulan[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (d > hariPerBulan[m])
    {
        d = 1;
        m++;
        if (m > 12)
        {
            m = 1;
            y++;
        }
    }

    char ubahString[15];
    sprintf(ubahString, "%02d-%02d-%02d", d, m, y);
    return string(ubahString);
}

bool cekTanggalDipakai(string tanggal, Service *skip)
{
    Service *temp = frontAntri;
    while (temp != NULL)
    {
        if (temp != skip && temp->tanggalAmbil == tanggal)
            return true; // kalo true berarti di pake
        temp = temp->next;
    }
    return false;
}

string cariTanggalKosong(string tanggal, int count, Service *skip, bool &berubah)
{
    if (!cekTanggalDipakai(tanggal, skip))
        return tanggal;

    berubah = true;

    if (count == 0)
    {
        cout << "\nTanggal sudah dijadwalkan oleh servis lain\n";
    }

    if (count < 5)
    {
        cout << "Tanggal diundur " << count + 1 << " hari setelah tanggal baru...\n";
        return cariTanggalKosong(tambahHari(tanggal), count + 1, skip, berubah);
    }

    cout << "Tanggal sudah diundur sebanyak 5 kali, masukkan tanggal baru\n: >";
    string baru;
    getline(cin, baru);

    return cariTanggalKosong(baru, 0, skip, berubah);
}

int tanggalKeINT(string tanggal)
{
    int d, m, y;
    sscanf(tanggal.c_str(), "%d-%d-%d", &d, &m, &y);
    return y * 10000 + m * 100 + d;
}

void tampilkanService(Service *antrian[], int n)
{
    cout << "====== Antrian Servis ======\n";
    for (int i = 0; i < n; i++)
    {
        cout << "-----------------------\n";
        cout << "Model Mobil: " << antrian[i]->modelMobil << endl;
        cout << "Merek Mobil: " << antrian[i]->merekMobil << endl;
        cout << "Kendala: " << antrian[i]->deskripsiKendala << endl;
        cout << "Montir: " << antrian[i]->namaMontir << endl;
        cout << "Nama Pelanggan: " << antrian[i]->namaPelanggan << endl;
        cout << "Kepentingan: " << antrian[i]->levelKepentingan << endl;
        cout << "Tanggal Ambil: " << antrian[i]->tanggalAmbil << endl;
        cout << "-----------------------\n\n";
    }
}

void antrianService()
{
    Service *temp = frontAntri;
    int banyakData = 0;
    while (temp != NULL)
    {
        banyakData++;
        temp = temp->next;
    }

    if (banyakData == 0)
    {
        cout << "Tidak ada antrian\n";
        return;
    }

    Service **dataSort = new Service *[banyakData];
    temp = frontAntri;
    for (int i = 0; i < banyakData; i++)
    {
        dataSort[i] = temp;
        temp = temp->next;
    }

    Service **dataAsli = new Service *[banyakData];
    for (int i = 0; i < banyakData; i++)
    {
        dataAsli[i] = dataSort[i];
    }

    tampilkanService(dataAsli, banyakData);

    while (true)
    {
        cout << "Sort by:\n[D]ate [U]rgency [Q]ueue [E]xit\n";
        char pilihan;
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 'E' || pilihan == 'e')
            break;

        if (pilihan == 'Q' || pilihan == 'q')
        {
            for (int i = 0; i < banyakData; i++)
            {
                dataSort[i] = dataAsli[i];
            }
            tampilkanService(dataSort, banyakData);
        }
        else if (pilihan == 'D' || pilihan == 'd')
        {
            for (int i = 0; i < banyakData - 1; i++)
            {
                for (int j = 0; j < banyakData - i - 1; j++)
                {
                    if (tanggalKeINT(dataSort[j]->tanggalAmbil) > tanggalKeINT(dataSort[j + 1]->tanggalAmbil))
                        swap(dataSort[j], dataSort[j + 1]);
                }
            }
            tampilkanService(dataSort, banyakData);
        }
        else if (pilihan == 'U' || pilihan == 'u')
        {
            for (int i = 0; i < banyakData - 1; i++)
            {
                for (int j = 0; j < banyakData - i - 1; j++)
                {
                    if (stoi(dataSort[j]->levelKepentingan) < stoi(dataSort[j + 1]->levelKepentingan))
                        swap(dataSort[j], dataSort[j + 1]);
                    else if (stoi(dataSort[j]->levelKepentingan) == stoi(dataSort[j + 1]->levelKepentingan) &&
                             tanggalKeINT(dataSort[j]->tanggalAmbil) > tanggalKeINT(dataSort[j + 1]->tanggalAmbil))
                        swap(dataSort[j], dataSort[j + 1]);
                }
            }
            tampilkanService(dataSort, banyakData);
        }
    }

    delete[] dataSort;
    delete[] dataAsli;
}
// ================= END TAMBAHAN =================

void tambahService(string modelMobil, string merekMobil, string namaPelanggan, string tanggalAmbil, string levelKepentingan, string namaMontir, string deskripsiKendala)
{
    bool berubah = false;
    tanggalAmbil = cariTanggalKosong(tanggalAmbil, 0, NULL, berubah);

    Service *baru = new Service;
    baru->modelMobil = modelMobil;
    baru->merekMobil = merekMobil;
    baru->deskripsiKendala = deskripsiKendala;
    baru->namaMontir = namaMontir;
    baru->namaPelanggan = namaPelanggan;
    baru->levelKepentingan = levelKepentingan;
    baru->tanggalAmbil = tanggalAmbil;
    baru->next = NULL;

    if (frontAntri == NULL)
    {
        frontAntri = rearAntri = baru;
    }
    else
    {
        rearAntri->next = baru;
        rearAntri = baru;
    }

}

void tambahSelesai(Service *srv)
{
    srv->next = NULL;

    if (headServ == NULL)
    {
        headServ = tailServ = srv;
    }
    else
    {
        tailServ->next = srv;
        tailServ = srv;
    }
}

void bacaData()
{
    ifstream fileAntrian("Data_AntrianP4.csv");
    string line;

    if (fileAntrian.is_open())
    {
        while (getline(fileAntrian, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string modelMobil, merekMobil, namaPelanggan, tanggalAmbil, levelKepentingan, namaMontir, deskripsiKendala;

            getline(ss, modelMobil, ';');
            getline(ss, merekMobil, ';');
            getline(ss, namaPelanggan, ';');
            getline(ss, tanggalAmbil, ';');
            getline(ss, levelKepentingan, ';');
            getline(ss, namaMontir, ';');
            getline(ss, deskripsiKendala, ';');

            Service *baru = new Service;
            baru->modelMobil = modelMobil;
            baru->merekMobil = merekMobil;
            baru->namaPelanggan = namaPelanggan;
            baru->tanggalAmbil = tanggalAmbil;
            baru->levelKepentingan = levelKepentingan;
            baru->namaMontir = namaMontir;
            baru->deskripsiKendala = deskripsiKendala;
            baru->next = NULL;

            if (frontAntri == NULL)
            {
                frontAntri = rearAntri = baru;
            }
            else
            {
                rearAntri->next = baru;
                rearAntri = baru;
            }
        }
        fileAntrian.close();
    }

    ifstream fileSelesai("Data_SelesaiP4.csv");
    if (fileSelesai.is_open())
    {
        while (getline(fileSelesai, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string modelMobil, merekMobil, namaPelanggan, tanggalAmbil, levelKepentingan, namaMontir, deskripsiKendala;

            getline(ss, modelMobil, ';');
            getline(ss, merekMobil, ';');
            getline(ss, namaPelanggan, ';');
            getline(ss, tanggalAmbil, ';');
            getline(ss, levelKepentingan, ';');
            getline(ss, namaMontir, ';');
            getline(ss, deskripsiKendala, ';');

            Service *srv = new Service;
            srv->modelMobil = modelMobil;
            srv->merekMobil = merekMobil;
            srv->namaPelanggan = namaPelanggan;
            srv->tanggalAmbil = tanggalAmbil;
            srv->levelKepentingan = levelKepentingan;
            srv->namaMontir = namaMontir;
            srv->deskripsiKendala = deskripsiKendala;
            srv->next = NULL;

            tambahSelesai(srv);
        }
        fileSelesai.close();
    }
}

void simpanData()
{
    ofstream fileAntrian("Data_AntrianP4.csv");
    Service *ambilAntrian = frontAntri;

    while (ambilAntrian != NULL)
    {
        fileAntrian << ambilAntrian->modelMobil << ";"
                    << ambilAntrian->merekMobil << ";"
                    << ambilAntrian->namaPelanggan << ";"
                    << ambilAntrian->tanggalAmbil << ";"
                    << ambilAntrian->levelKepentingan << ";"
                    << ambilAntrian->namaMontir << ";"
                    << ambilAntrian->deskripsiKendala << endl;

        ambilAntrian = ambilAntrian->next;
    }
    fileAntrian.close();

    ofstream fileService("Data_SelesaiP4.csv");
    Service *ambilService = headServ;

    while (ambilService != NULL)
    {
        fileService << ambilService->modelMobil << ";"
                    << ambilService->merekMobil << ";"
                    << ambilService->namaPelanggan << ";"
                    << ambilService->tanggalAmbil << ";"
                    << ambilService->levelKepentingan << ";"
                    << ambilService->namaMontir << ";"
                    << ambilService->deskripsiKendala << endl;

        ambilService = ambilService->next;
    }
    fileService.close();
}

void bookingServis()
{
    string modelMobil, merekMobil, namaPelanggan, tanggalAmbil, levelKepentingan, namaMontir, deskripsiKendala;

    cout << "\n====== New Service ======" << endl;
    cout << "Model Mobil: >";
    getline(cin, modelMobil);
    cout << "Merek Mobil: >";
    getline(cin, merekMobil);
    cout << "Kendala: >";
    getline(cin, deskripsiKendala);
    cout << "Montir: >";
    getline(cin, namaMontir);
    cout << "Nama Pelanggan: >";
    getline(cin, namaPelanggan);

    do
    {
        cout << "Kepentingan (1-4): >";
        getline(cin, levelKepentingan);
    } while (levelKepentingan != "1" && levelKepentingan != "2" && levelKepentingan != "3" && levelKepentingan != "4");

    cout << "Tanggal Ambil: >";
    getline(cin, tanggalAmbil);

    tambahService(modelMobil, merekMobil, namaPelanggan, tanggalAmbil, levelKepentingan, namaMontir, deskripsiKendala);
    simpanData();

    cout << "\nPress any key to go back ...";
    cin.get();
}

void gantiTanggal()
{
    if (frontAntri == NULL)
    {
        cout << "Tidak ada data\n";
        return;
    }

    cout << "\n====== Ganti Tanggal Ambil ======\n";

    Service *temp = frontAntri;
    int i = 1;

    while (temp != NULL)
    {
        cout << i++ << ". Servis:\n";
        cout << "\tModel Mobil: " << temp->modelMobil << endl;
        cout << "\tMerek Mobil: " << temp->merekMobil << endl;
        cout << "\tKendala: " << temp->deskripsiKendala << endl;
        cout << "\tMontir: " << temp->namaMontir << endl;
        cout << "\tTanggal Ambil: " << temp->tanggalAmbil << endl;
        temp = temp->next;
    }

    cout << "Pilih servis yang ingin di reschedule\n: >";
    int pilih;
    cin >> pilih;
    cin.ignore();

    temp = frontAntri;
    for (int j = 1; j < pilih; j++)
        temp = temp->next;

    cout << "\n====== Ganti Tanggal Ambil ======\n";
    cout << "1. Servis:\n";
    cout << "\tModel Mobil: " << temp->modelMobil << endl;
    cout << "\tMerek Mobil: " << temp->merekMobil << endl;
    cout << "\tKendala: " << temp->deskripsiKendala << endl;
    cout << "\tMontir: " << temp->namaMontir << endl;
    cout << "\tTanggal Ambil: " << temp->tanggalAmbil << endl;

    cout << "\nMasukkan Tanggal baru: >";
    string baru;
    getline(cin, baru);

    bool berubah = false;
    string hasil = cariTanggalKosong(baru, 0, temp, berubah);

    if (berubah)
    {
        cout << "Tanggal ambil yang baru adalah " << hasil << endl;
    }

    temp->tanggalAmbil = hasil;

    cout << "\nServis " << temp->modelMobil << " berhasil di undur!!\n";

    simpanData();

    cout << "\nPress any key to go back ...";
    cin.get();
}

Service *ambilServiceMontir(string montir)
{
    Service *temp = frontAntri;
    Service *target = NULL;

    while (temp != NULL)
    {
        if (temp->namaMontir == montir)
        {
            if (target == NULL)
            {
                target = temp;
            }
            else
            {
                int penting1 = stoi(temp->levelKepentingan);
                int penting2 = stoi(target->levelKepentingan);

                if (penting1 > penting2)
                {
                    target = temp;
                }
                else if (penting1 == penting2 && tanggalKeINT(temp->tanggalAmbil) < tanggalKeINT(target->tanggalAmbil))
                {
                    target = temp;
                }
            }
        }
        temp = temp->next;
    }

    if (target == NULL)
        return NULL;

    temp = frontAntri;
    Service *prev = NULL;

    while (temp != NULL)
    {
        if (temp == target)
        {
            if (prev == NULL)
                frontAntri = temp->next;
            else
                prev->next = temp->next;

            if (temp == rearAntri)
                rearAntri = prev;

            temp->next = NULL;
            return temp;
        }

        prev = temp;
        temp = temp->next;
    }

    return NULL;
}

void selesaikanPekerjaan()
{
    string montirList[] = {"Suby", "Farhan", "Dimas", "Aldo", "Rafi"};

    cout << "\n====== Jobs Done ======" << endl;
    for (int i = 0; i < 5; i++)
        cout << i + 1 << ". " << montirList[i] << endl;

    int p;
    cout << "Pilihan : >";
    cin >> p;
    cin.ignore();

    Service *srv = ambilServiceMontir(montirList[p - 1]);

    if (srv == NULL)
    {
        cout << "Tidak ada pekerjaan untuk montir ini" << endl;
    }
    else
    {
        cout << "\n====== Service ======" << endl;
        cout << "Model Mobil: " << srv->modelMobil << endl;
        cout << "Merek Mobil: " << srv->merekMobil << endl;
        cout << "Kendala: " << srv->deskripsiKendala << endl;
        cout << "Montir: " << srv->namaMontir << endl;
        cout << "Tanggal Ambil: " << srv->tanggalAmbil << endl;
        cout << "Kepentingan: " << srv->levelKepentingan << endl;
        cout << "Nama Pelanggan: " << srv->namaPelanggan << endl;

        string jawab;
        cout << "\nApakah servis ini sudah selesai?(yes/no): ";
        getline(cin, jawab);

        if (jawab == "yes")
        {
            tambahSelesai(srv);
            simpanData();
        }
        else
        {
            tambahService(
                srv->modelMobil,
                srv->merekMobil,
                srv->namaPelanggan,
                srv->tanggalAmbil,
                srv->levelKepentingan,
                srv->namaMontir,
                srv->deskripsiKendala);
            delete srv;
        }
    }

    cout << "\nPress any key to go back ...";
    cin.get();
}

int main()
{
    bacaData();

    string a;

    while (true)
    {
        cout << "\n====== Welcome To Lognuts ======" << endl;
        cout << "Pilih Menu!" << endl;
        cout << "1. Booking Servis" << endl;
        cout << "2. Lihat Antrian" << endl;
        cout << "3. Selesaikan Service" << endl;
        cout << "4. Ganti Tanggal Ambil" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilihan: ";
        getline(cin, a);

        if (a == "1")
            bookingServis();
        else if (a == "2")
            antrianService();
        else if (a == "3")
            selesaikanPekerjaan();
        else if (a == "4")
            gantiTanggal();
        else if (a == "5")
            break;
        else
            cout << "Pilihan Anda Tidak Valid!";
    }

    return 0;
}