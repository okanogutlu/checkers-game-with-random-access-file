#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
using namespace std;

//------------------------------

//Okan �g�tl� 1306170076
//Program�m temel olarak 3 katmandan olu�uyor,kullan�c�n�n programa verdi�i komutlar ve de�erlendirme, komutlar�n oyun kurallar�na uygun olup olmad���n� denetleme ve dosya i�lemleri
//Oyun Kurallar�: yoz ta�lar arka ve �apraz hareket hari� 1 kare ilerler, tablo sonuna gelen ta� dama olur, damalar istedi�i kadar ilerler(L �eklinde)
//E�er ki Tabloda Yiyilebilecek Ta� Var �se �nce Onu Yemek Mecburidir.

//------------------------------

//Fonksiyon Tan�mlamalar�:
bool izin();
void ilkAtama();
void GirisEkrani();
char siraKimde();
void Game();
int cevirici(char harf);
bool damaM�(int bsatir, int bsutun);
bool exist(int satir, int sutun);
bool legalMi(int bsatir, int bsutun, int vsatir, int vsutun);
bool yoz(int bsatir, int bsutun, int vsatir, int vsutun);
bool oyunMerkezi(string hamle);
void hamleYap(int bsatir, int bsutun, int vsatir, int vsutun, int hamleTuru);
bool Dama(int bsatir, int bsutun, int vsatir, int vsutun);
bool damaLegalMi(int bsatir, int bsutun, int vsatir, int vsutun);
bool DHamleYap(int bsatir, int  bsutun, int  vsatir, int  vsutun, int yiyilenR, int yiyilenC);
int kimKazand�();
void DosyaSifirla();
vector<vector<char>> dama;//tabloyu buraya aktar�yorum.

void ilkAtama() {

	dama.clear();//ilk olarak global olan tablo temizlenir.
	vector<vector<char>> vec(8);//local de�i�ken tan�mlad�m,de�erleri buna at�p, buradan globale aktar�yorum.
	char sira;
	int col;
	char temp;
	ifstream myFile("dama.dat", ios::binary |ios::out | ios::in);

	for (int i = 0; i < 9; i++) {
		myFile.seekg(i * 25, ios::beg);
		col = 0;
		for (int j = 0; j < 24; j++) {
			if (i < 8) {
				myFile.read((char*)&temp, sizeof(char));
				if (temp != ' ') {
					if (temp == '*') {
						vec[i].push_back('*');
					}
					else
						vec[i].push_back(temp);
				}
			}
			else {
				myFile.read((char*)&sira, sizeof(char));
				cout << "SIRA : " << sira << endl;
				break;
			}
		}

	}
	dama = vec;//Global de�i�kene aktarma
	for (const std::vector<char> &v : dama){
		for (char x : v) std::cout << x;
		std::cout << std::endl;

	}
}


void DosyaSifirla() {

	ofstream temizle;//Dosyay� a�ar ve i�ini temizler.
	temizle.open("dama.dat", std::ofstream::out | std::ofstream::trunc);
	temizle.close();
	ofstream temizle2;//Dosyay� a�ar ve i�ini temizler.
	temizle2.open("hamle.dat", std::ofstream::out | std::ofstream::trunc);
	temizle2.close();


	fstream myFile("dama.dat", ios::out | ios::trunc);//Dosyay� tekrar yazmak i�in a�ar.
	string sat�r1 = "-- -- -- -- -- -- -- --";
	string sat�r2 = "w* w* w* w* w* w* w* w*";
	string sat�r6 = "b* b* b* b* b* b* b* b*";
	string sat�r9 = "W";
	myFile << sat�r1;
	myFile << '\n';
	myFile << sat�r2;
	myFile << '\n';
	myFile << sat�r2;
	myFile << '\n';
	myFile << sat�r1;
	myFile << '\n';
	myFile << sat�r1;
	myFile << '\n';
	myFile << sat�r6;
	myFile << '\n';
	myFile << sat�r6;
	myFile << '\n';
	myFile << sat�r1;
	myFile << '\n';
	myFile << sat�r9;


	myFile.close();
}

void HamleAta(string hamle) {
	fstream myFile("hamle.dat", ios::binary | ios::out | ios::in | ios::app);
	char sira = siraKimde();
	myFile << sira;
	myFile << " :";
	myFile << hamle;
	myFile << "\n";
	myFile.close();
}

int kimKazand�() {
	int sayac1 = 0, sayac2 = 0;
	for (int satir = 0; satir < 7; satir++) {//T�m tabloyu tarar;
		for (int sutun = 0; sutun < 16; sutun++) {//T�m tabloyu tarar;
			if (dama[satir][sutun] == 'w') {
				sayac1++;
			}
			if (dama[satir][sutun] == 'b') {
				sayac2++;
			}
		}
	}
	if (sayac1 > 0 && sayac2 == 0) {
		return 1;
	}
	else if (sayac2 > 0 && sayac1 == 0) {
		return 2;
	}
	else
		return 0;
}

void GirisEkrani() {
	cout << endl << "Dama Oyununa Hosgeldiniz. \n Yeni Oyun icin 1\nVarolan Oyuna Devam Etmek Icin 2\Cikis icin 3 Tuslayiniz." << endl;
	int secim = 0;
	cin >> secim;
	switch (secim) {
	case 1:
		DosyaSifirla();
		Game();
		break;
	case 2:
		
		Game();
		break;
	case 3:
		exit(0);
		break;
	default:
		while ((secim == 0) || (secim == 1) ||(secim==3)) {
			cout << endl << "Yanlis Secim Yaptiniz. Lutfen Tekrar Deneyiniz."
				<< endl << "Dama Oyununa Hosgeldiniz. \n Yeni Oyun i�in 1\nVarolan Oyuna Devam Etmek ��in 2 Tuslayiniz."
				<< endl;
		}
	}
}

char  siraKimde() {//S�ran�n kimde oldu�unu belirler.
	ifstream myFile("dama.dat", ios::binary | ios::out | ios::in);
	myFile.seekg(-1, ios::end);
	char sira;
	myFile.read((char*)&sira, sizeof(char));
	myFile.close();
	if (sira == 'W') {
		return 'w';
	}
	else if (sira == 'B') {
		return 'b';
	}
}

void Game() {
	cout << endl << "OYUN BASLATILIYOR..." << endl;
	ilkAtama();
	bool devamM� = true;
	bool oynand�M� = true;
	int kazanan;
	string hamle;
	HamleAta(hamle);
	while (devamM�) {
		if (!oynand�M�) {
			cout << endl << "YANLIS HAMLE YAPTINIZ.TEKRAR DENEYIN" << endl;
		}
		cout << endl << "SIRA..:" << siraKimde() << "  Lutfen hamlenizi yapiniz :";
		cin >> hamle;
		HamleAta(hamle);
		oynand�M� = oyunMerkezi(hamle);
		kazanan = kimKazand�();
		if (kimKazand�() == 1) {
			cout << endl << endl << "BEYAZ TAKIM KAZANDI!";
			break;
		}
		if (kimKazand�() == 2) {
			cout << endl << endl << "SIYAH TAKIM KAZANDI!";
			break;
		}

	}
}

int cevirici(char harf) { //hamleleri integer a d�n��t�r�r.
	if (harf == 'a')return 0;
	if (harf == 'b')return 1;
	if (harf == 'c')return 2;
	if (harf == 'd')return 3;
	if (harf == 'e')return 4;
	if (harf == 'f')return 5;
	if (harf == 'g')return 6;
	if (harf == 'h')return 7;
	else return 0;
}

bool damaM�(int bsatir, int bsutun) {//Ta��n dama m� yoksa yoz mu oldu�unu belirler.
	char harf1 = dama[bsatir][bsutun];
	char harf2 = dama[bsatir][bsutun + 1];
	if ((harf1 == 'w') && (harf2 == 'w')) {
		return true;
	}
	if ((harf1 == 'b') && (harf2 == 'b')) {
		return true;
	}

	return false;
}

bool exist(int satir, int sutun) {//sat�t-sutun tabloda var m�,kontrol eder.
	if (satir < 8 && satir >= 0 && sutun >= 0 && sutun < 16) {
		return true;
	}
	else return false;
}

bool legalMi(int bsatir, int bsutun, int vsatir, int vsutun) {
	
	if ((dama[vsatir][vsutun] == 'w') || (dama[vsatir][vsutun] == 'b')) {//gidece�i yerde ba�ka ta� var m�?
		return false;
	}
	

	if (siraKimde() == 'b') {

		if ((bsatir == vsatir - 1) && ((bsutun == vsutun - 2) || (bsutun == vsutun + 2))) {//siyah i�in �apraz hareket mi?
			return false;
		}
		
		if (vsatir == bsatir + 1) {//siyah i�in arka hareket mi?
			return false;
		}

		if (exist(bsatir, bsutun + 2)) {//E�er ki sa��nda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun + 2] == 'w')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir, bsutun - 2)) {//E�er ki solunda rakip varsa iki hamleye izin verir.
				if ((dama[bsatir][bsutun - 2] == 'w')) {
					if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
						return true;
					}
				}
				
		}
		if (exist(bsatir - 1, bsutun)) {//E�er ki �n�nde rakip varsa iki hamleye izin verir.
			if ((dama[bsatir - 1][bsutun] == 'w')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
		}
		
		if (abs(bsatir - vsatir) >= 2 || abs(bsutun - vsutun) >= 4) {//e�er ki yiyece�i ta� yoksa bir hamleye izin verir.
			return false;
		}	
		
	}
	if (siraKimde() == 'w') {

		if ((bsatir == vsatir + 1) && ((bsutun == vsutun - 2) || (bsutun == vsutun + 2))) {//beyaz i�in �apraz hareket mi?
			return false;
		}

		if (bsatir == vsatir + 1) {//beyaz i�in arka hareket mi?
			return false;
		}
	
		if (exist(bsatir, bsutun + 2)) {//E�er ki sa��nda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun + 2] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir, bsutun - 2)) {//E�er ki solunda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun - 2] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir + 1, bsutun)) {//E�er ki �n�nde rakip varsa iki hamleye izin verir.
			if ((dama[bsatir + 1][bsutun] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
		}
		if (abs(bsatir - vsatir) >= 2 || abs(bsutun - vsutun) >= 4) {//E�er ki yiyece�i ta� yoksa bir hamleye izin verir.
				return false;
		}
	}
	return true;
}

bool yoz(int bsatir, int bsutun, int vsatir, int vsutun) {//Yoz ta�lar�n hareket merkezi

	if (!legalMi(bsatir, bsutun, vsatir, vsutun))return false; //arkaya ya da �apraz a hareket var m�?kontrol eder.
	
	int sayac = 0;

	if (siraKimde() == 'w') { //e�er ki oynayan ki�i w ise
		
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			if (exist(bsatir, (bsutun - 2))) {
				if (dama[bsatir][bsutun - 2] == 'b') {//e�er ki solunda kar�� tak�m var ise
					if (exist(bsatir, bsutun - 4)) {
						if(dama[bsatir][bsutun-4]=='-'){//e�er ki gidebilece�i yer bo� ise
							if (vsatir == bsatir && bsutun - 4 == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 1);
								return true;
							}
							else {
							sayac++;
							}
						}
					}
				}
			}
		}
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			
			if (exist(bsatir, (bsutun + 2))) {
				if (dama[bsatir][bsutun + 2] == 'b') { //e�er ki sa��nda kar�� tak�m var ise
					if (exist(bsatir, bsutun + 4)) {
						if (dama[bsatir][bsutun + 4] == '-') {// e�er ki gidebilec�i yer bo� ise
							if (vsatir == bsatir && bsutun + 4 == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 2);
								return true;
							}
							else {
								sayac++; 
							}
						}
					}
				}
			}
		}
		
		if (exist(vsatir, vsutun) && (dama[vsatir][vsutun] == '-') ) {//e�er ki gidece�i yer bo� ise
			if (exist((bsatir + 1), bsutun)) {
				if (dama[bsatir + 1][bsutun] == 'b') { //e�er ki �n�nde kar�� tak�m var ise
					if (exist(bsatir + 2, bsutun)) {
						if (dama[bsatir+2][bsutun] == '-') {
							if (vsatir == bsatir + 2 && bsutun == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 3);
								return true;
							}
							else {
								sayac++;
							}
						}
					}
				}
			}
		}
		
		if (sayac > 0)
			return false;//e�er ki yeme ihtimali oldu�u halde yemezse false d�nd�r.
		
		else {//Yeme ihtimali yoksa
			if (!izin()) return false;
			hamleYap(bsatir, bsutun, vsatir, vsutun, 7);
			return true;
		}
	}

	else if (siraKimde() == 'b') { //e�er ki oyuncu b ise
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			if (exist(bsatir, (bsutun - 2))) {
				if (dama[bsatir][bsutun - 2] == 'w') {//e�er ki solunda kar�� tak�m var ise
					if (exist(bsatir, bsutun - 4)) {
						if (dama[bsatir][bsutun - 4] == '-') {
							if (vsatir == bsatir && bsutun - 4 == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 4);
								return true;
							}
							else {
								sayac++;
							}
						}
					}
				}
			}
		}
		 if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			if (exist(bsatir, (bsutun + 2))) {
				if (dama[bsatir][bsutun + 2] == 'w') { //e�er ki sa��nda kar�� tak�m var ise
					if (exist(bsatir, bsutun + 4)) {
						if (dama[bsatir][bsutun + 4] == '-') {
							if (vsatir == bsatir && bsutun + 4 == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 5);
								return true;
							}
							else {
								sayac++;
							}
						}
					}
				}
			}
		}
		 if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			if (exist((bsatir - 1), bsutun)) {
				if (dama[bsatir - 1][bsutun] == 'w') { //e�er ki �n�nde kar�� tak�m var ise
					if (exist(bsatir - 2, bsutun)) {
						if (dama[bsatir-2][bsutun] == '-') {
							if (vsatir == bsatir - 2 && bsutun == vsutun) {//e�er ki hamlesini oraya yapm�� ise

								hamleYap(bsatir, bsutun, vsatir, vsutun, 6);
								return true;
							}
							else {
								sayac++;
							}
						}
					}
				}
			}
		}
		if (sayac > 0) 
			return false;//e�er ki yeme ihtimali oldu�u halde yemezse false d�nd�r.
		else {
			if (!izin()) return false;
			hamleYap(bsatir, bsutun, vsatir, vsutun, 7);
			return true;
		}
	}	
}

bool oyunMerkezi(string hamle) {//hamleleri ��z�mler
	int bsatir = 0, vsatir = 0, bsutun = 0, vsutun = 0;
	bsatir = hamle[1] - '0';
	bsatir = bsatir - 1;
	vsatir = hamle[3] - '0';
	vsatir = vsatir - 1;
	bsutun = cevirici(hamle[0]);
	bsutun = bsutun * 2;
	vsutun = cevirici(hamle[2]);
	vsutun = vsutun * 2;
	bool damam� = false;
	damam�=damaM�(bsatir,bsutun);
	if (!damam�) {//Ta� dama de�ilse yoz b�l�m�ne gider.
		if (!yoz(bsatir, bsutun, vsatir, vsutun))
			return false;
	}
	else {//Ta� dama ise Dama b�l�m�ne gider.
		if(!Dama(bsatir, bsutun, vsatir, vsutun)) return false;
	}
	return true;
}

void hamleYap(int bsatir, int bsutun, int vsatir, int vsutun, int hamleTuru) {//Yoz ta�lar� Dosyaya yazma i�lemi
	char sifirla = '-';
	char tas1;
	char tas2;
	fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
	char sifirla2 = '-';
	char wdama = 'ww';
	char bdama = 'bb';

	myFile.seekg((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� al�r
	myFile.read((char*)&tas1, sizeof(char));
	myFile.read((char*)&tas2, sizeof(char));

	if (tas1 == 'w' && vsatir == 7) { //E�er ki ta� damaya ula�m��sa
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� yerine yazar
		myFile.write((char*)&wdama, sizeof(char));
		myFile.write((char*)&wdama, sizeof(char));
	}
	else if (tas1 == 'b' && vsatir == 0) { //E�er ki ta� damaya ula�m��sa
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� yerine yazar
		myFile.write((char*)&bdama, sizeof(char));
		myFile.write((char*)&bdama, sizeof(char));
	}
	else{
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� yerine yazar
		myFile.write((char*)&tas1, sizeof(char));
		myFile.write((char*)&tas2, sizeof(char));
	}
	myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//Ba�lang�� yerini s�f�rlar.
	myFile.write((char*)&sifirla, sizeof(char));
	myFile.write((char*)&sifirla, sizeof(char));


	if (hamleTuru == 1) { //beyaz sol yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun ) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 2) { //beyaz sa� yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun+4) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 3) { //beyaz �n yediyse

		myFile.seekp((((bsatir+1) * 25) + ((((bsutun+2) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 4) { //siyah sol yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 5) { //siyah sa� yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun+4) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 6) { //siyah �n yediyse

		myFile.seekp((((bsatir-1) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}

	myFile.close();

	if (hamleTuru == 7) { //E�er ki ortada yeme durumu yoksa
		if (siraKimde() == 'w') { // s�ra �uan beyazdaysa
			hamleTuru = 4; //siyaha ge�sin
		}
		else if (siraKimde() == 'b') { //s�ra �uan siyahtaysa
			hamleTuru = 1; //s�ra beyaza ge�sin
		}
	}
	if ((hamleTuru == 1) | (hamleTuru==2) | (hamleTuru==3) ) { //E�er ki beyaz yediyse s�ra tekrar ona ge�sin
		fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
		myFile.seekp(-1, ios::end);
		char sira = 'W';
		myFile.write((char*)&sira, sizeof(char));
		myFile.close();
	}
	if ((hamleTuru == 4) | (hamleTuru == 5) | (hamleTuru == 6)) { //E�er ki siyah yediyse s�ra tekrar ona ge�sin
		fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
		myFile.seekp(-1, ios::end);
		char sira = 'B';
		myFile.write((char*)&sira, sizeof(char));
		myFile.close();
	}
	ilkAtama();
}

bool DHamleYap(int bsatir, int  bsutun, int  vsatir, int  vsutun, int yiyilenR,int yiyilenC) {//Dama ta�� Dosyaya yazma b�l�m�
	char sifirla = '-';
	char tas1;
	char tas2;
	fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
	char sifirla2 = '-';
	char wdama = 'ww';
	char bdama = 'bb';

	myFile.seekg((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� al�r
	myFile.read((char*)&tas1, sizeof(char));
	myFile.read((char*)&tas2, sizeof(char));

	myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//ta�� yerine yazar
	myFile.write((char*)&tas1, sizeof(char));
	myFile.write((char*)&tas2, sizeof(char));
	if(yiyilenR!=-1){
		myFile.seekp((((yiyilenR) * 25) + ((((yiyilenC + 2) / 2) - 1) * 3)), ios::beg);//ta� yediyse yiyilen ta�� s�f�rlar 
		myFile.write((char*)&sifirla, sizeof(char));
		myFile.write((char*)&sifirla, sizeof(char));

		myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//ba�lang�c yerini s�f�rlar 
		myFile.write((char*)&sifirla, sizeof(char));
		myFile.write((char*)&sifirla, sizeof(char));
		
	}
	else if (yiyilenR == -1) {
		myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//E�er ki ta� yemeden hareket ettiyse 
		myFile.write((char*)&sifirla, sizeof(char));
		myFile.write((char*)&sifirla, sizeof(char));
		if (siraKimde() == 'w') {
			myFile.seekp(-1, ios::end);
			char sira = 'B';
			myFile.write((char*)&sira, sizeof(char));
		
		}
		else {
			myFile.seekp(-1, ios::end);
			char sira = 'W';
			myFile.write((char*)&sira, sizeof(char));
			
		}
	}
	myFile.close();
	ilkAtama();
	return true;
}

bool izin() { //Oyunda yenilebilecek ba�ka ta� var m� kontrol eder.
	
	if (siraKimde() == 'w') { //s�ra beyazdaysa sadece beyaz ta�lar� kontrol eder.

		for (int satir = 0; satir < 7; satir++) {//T�m tabloyu tarar;
			for (int sutun = 0; sutun < 16; sutun++) {//T�m tabloyu tarar;
				
				if(dama[satir][sutun]=='w'){
					if (exist(satir, sutun - 4)) {
						if (dama[satir][sutun - 4] == '-') {//e�er ki gidebilece�i yer bo� ise
							if (exist(satir, (sutun - 2))) {
								if (dama[satir][sutun - 2] == 'b') {//e�er ki solunda kar�� tak�m var ise
									return false;
								}
							}
						}
					}
					if (exist(satir, sutun + 4)) {
						if (dama[satir][sutun + 4] == '-') {//e�er ki gidebilece�i yer bo� ise
							if (exist(satir, (sutun + 2))) {
								if (dama[satir][sutun + 2] == 'b') { //e�er ki sa��nda kar�� tak�m var ise
									return false;
								}
							}
						}
					}

					if (exist((satir + 2), sutun)) {
						if (dama[satir+2][sutun] == '-')  {//e�er ki gidece�i yer bo� ise
							if(exist((satir+1),sutun)){
								if (dama[satir + 1][sutun] == 'b') { //e�er ki �n�nde kar�� tak�m var ise
									return false;
								}
							}
						}
					}
				}
				
				if (damaM�(satir, sutun) && (dama[satir][sutun]=='w')) {//e�er ki ta� Beyaz Dama ise
					
					int sol = sutun; //Daman�n soluna bakar.
					if (exist(satir, sol - 2)) {
						sol = sol - 2;
						while (dama[satir][sol] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(satir, sol - 2)) {
								sol = sol - 2;
							}
							else
								break;
						}
						if (exist(satir, sol - 2)){
							if ((dama[satir][sol] == 'b') && (dama[satir][sol - 2] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}

					int sag = sutun;//Daman�n sa��na bakar.
					if (exist(satir, sag + 2)) {
						sag = sag + 2;
						while (dama[satir][sag] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(satir, sag + 2)) {
								sag = sag + 2;
							}
							else
								break;
						}
						if (exist(satir, sag + 2)) {
							if ((dama[satir][sag] == 'b') && (dama[satir][sag + 2] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}


					int on = satir;//Daman�n �n�ne bakar.
					if (exist(on+1, sutun)) {
						on = on + 1;
						while (dama[on][sutun] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(on+1, sutun)) {
								on = on + 1;
							}
							else
								break;
						}
						if (exist(on+1, sutun)) {
							if ((dama[on][sutun] == 'b') && (dama[on+1][sutun] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}

					int arka = satir;//Daman�n arkas�na bakar.
					if (exist(arka - 1, sutun)) {
						arka = arka - 1;
						while (dama[arka][sutun] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(arka - 1, sutun)) {
								arka = arka - 1;
							}
							else
								break;
						}
						if (exist(arka - 1, sutun)) {
							if ((dama[arka][sutun] == 'b') && (dama[arka - 1][sutun] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}
				}
					

				
			}
		
			
		}

	}

	if (siraKimde() == 'b') { //s�ra siyahtaysa sadece siyah ta�lar� kontrol eder.

		for (int satir = 0; satir <= 7; satir++) {
			for (int sutun = 0; sutun < 16; sutun++) {
				if(dama[satir][sutun]=='b'){
					if (exist(satir, sutun - 4)) {
						if (dama[satir][sutun - 4] == '-') {//e�er ki gidebilece�i yer bo� ise

							if (exist(satir, (sutun - 2))) {
								if (dama[satir][sutun - 2] == 'w') {//e�er ki solunda kar�� tak�m var ise
									return false;
								}
							}
						}
					}
					if (exist(satir, sutun + 4)) {
						if (dama[satir][sutun + 4] == '-') {//e�er ki gidebilece�i yer bo� ise

							if (exist(satir, (sutun + 2))) {
								if (dama[satir][sutun + 2] == 'w') { //e�er ki sa��nda kar�� tak�m var ise
									return false;
								}
							}
						}
					}

					if (exist((satir - 2), sutun)) {
						if (dama[satir - 2][sutun] == '-') {//e�er ki gidece�i yer bo� ise			
							if (exist(satir - 1, sutun)) {
								if (dama[satir - 1][sutun] == 'w') { //e�er ki �n�nde kar�� tak�m var ise

									return false;
								}
							}
						}
					}
				}
			
				if (damaM�(satir, sutun) && (dama[satir][sutun]=='b') ) {//e�er ki ta� Siyah Dama ise
					
					int sol = sutun;//Daman�n soluna bakar.
					if (exist(satir, sol - 2)) {
						sol = sol - 2;
						while (dama[satir][sol] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(satir, sol - 2)) {
								sol = sol - 2;
							}
							else
								break;
						}
						if (exist(satir, sol - 2)) {
							if ((dama[satir][sol] == 'w') && (dama[satir][sol - 2] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.	
								return false;
							}
						}
					}

					int sag = sutun;//Daman�n sa��na bakar.
					if (exist(satir, sag + 2)) {
						sag = sag + 2;
						while (dama[satir][sag] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(satir, sag + 2)) {
								sag = sag + 2;
							}
							else
								break;
						}
						if (exist(satir, sag + 2)) {
							if ((dama[satir][sag] == 'w') && (dama[satir][sag + 2] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.
								return false;
							}
						}
					}


					int on = satir;//Daman�n �n�ne bakar.
					if (exist(on - 1, sutun)) {
						on = on - 1;
						while (dama[on][sutun] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(on - 1, sutun)) {
								on = on - 1;
							}
							else
								break;
						}
						if (exist(on - 1, sutun)) {
							if ((dama[on][sutun] == 'w') && (dama[on - 1][sutun] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}


					int arka = satir;//Daman�n arkas�na bakar.
					if (exist(arka + 1, sutun)) {
						arka = arka + 1;
						while (dama[arka][sutun] == '-') { //Bo�luk oldu�u m�ddet�e sola ilerler
							if (exist(arka + 1, sutun)) {
								arka = arka + 1;
							}
							else
								break;
						}
						if (exist(arka + 1, sutun)) { 
							if ((dama[arka][sutun] == 'w') && (dama[arka + 1][sutun] == '-')) { //E�er ki bo�lu�un bir yan� rakip, onun da bir yan� bo� ise false d�nd�r�r.				
								return false;
							}
						}
					}

				}
			
			}
		}

	}
	return true;
}

bool damaLegalMi(int bsatir, int bsutun, int vsatir, int vsutun) { //Dama ta��n�n gidece�i yeri denetler
	int maxR = 0; int minR = 0;
	if (bsatir > vsatir) {
		maxR = bsatir;
		minR = vsatir;
	}
	int maxC = 0; int minC = 0;
	if (bsutun > vsutun) {
		maxC = bsutun;
		minC = vsutun;
	}
	if ((abs(bsatir - vsatir) != 0) && (abs(bsutun - vsutun) == 0)) {//hareket sat�rlar aras�nda m�?
		for (; minR < maxR; minR++) {
			if (dama[minR][bsutun] == siraKimde()) { //ba�lang��-var�� aras�nda kendi ta��ndan var m�?
				return false;
			}
		}

	}
	if ((abs(bsutun - vsutun) != 0) && (abs(bsatir - vsatir) == 0)) {//hareket sutunlar aras�nda m�?
		for (; minC < maxC; minC++) {
			if (dama[bsatir][minC] == siraKimde()) { //ba�lang��-var�� aras�nda kendi ta��ndan var m�?
				return false;
			}
		}

	}
	if ( ((bsatir - vsatir != 0) && (bsutun - vsutun == 0) ) || ( (bsatir - vsatir == 0) && (bsutun - vsutun != 0))) { //hareket do�rusal m�?
		return true;
	}
	else return false;
}

bool Dama(int bsatir, int bsutun, int vsatir, int vsutun) {
	int sayac1=0, sayac2=0, sayac3=0, sayac4 = 0;
	if(!damaLegalMi(bsatir, bsutun, vsatir, vsutun))return false;
	
	if(siraKimde()=='w'){
		//Beyaz Dama sol taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for(int i=bsutun-2;i>=vsutun;i-=2){
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'b') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i+2; j < bsutun; j+=2) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[bsatir][j] != '-')sayac1++;
						}
						for (int k = i-2; k > vsutun; k -= 2) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if(dama[bsatir][k]!='-'){ //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
								sayac1++;
							}
						}
						if (sayac1 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}

		//Beyaz Dama sag taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsutun + 2; i <= vsutun; i += 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'b') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i - 2; j > bsutun; j -= 2) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[bsatir][j] != '-')sayac2++;
						}
						for (int k = i + 2; k < vsutun; k += 2) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[bsatir][k] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
								sayac2++;
							}
						}
						if (sayac2 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}
		
		
		//Beyaz Dama �n taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsatir +1; i < vsatir; i += 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'b') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i - 1; j > bsatir; j -= 1) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[j][bsutun] != '-')sayac3++;;
						}
						for (int k = i + 1; k < vsatir; k += 1) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[k][bsutun] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if(sayac3 == 0){
						return DHamleYap(bsatir, bsutun, vsatir, vsutun, i,bsutun);
						}
					}
				}
			}
		}
		

		//Beyaz Dama arka taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsatir - 1; i > vsatir; i -= 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'b') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i + 1; j < bsatir; j += 1) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[j][bsutun] != '-')sayac4++;;
						}
						for (int k = i - 1; k > vsatir; k -= 1) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[k][bsutun] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if (sayac4 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, i, bsutun);
						}
					}
				}
			}
		}
	}
	
	if (siraKimde() == 'b') {
		//Siyah Dama sol taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsutun - 2; i > vsutun; i -= 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'w') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i + 2; j > bsutun; j += 2) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[bsatir][j] != '-')sayac1++;
						}
						for (int k = i - 2; k > vsutun; k -= 2) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[bsatir][k] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if (sayac1 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}

		//Siyah Dama sag taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsutun + 2; i <vsutun; i += 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'w') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i - 2; j > bsutun; j -= 2) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[bsatir][j] != '-')sayac2++;
						}
						for (int k = i + 2; k < vsutun; k += 2) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[bsatir][k] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if (sayac2 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}


		//Siyah Dama �n taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsatir - 1; i >vsatir; i -= 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'w') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i + 1; j < bsatir; j += 1) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[j][bsutun] != '-')sayac3++;
						}
						for (int k = i - 1; k > vsatir; k -= 1) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[k][bsutun] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if (sayac3 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, i, bsutun);
						}
					}
				}
			}
		}


		//Siyah Dama arka taraftan ta� yeme olay�:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//e�er ki gidece�i yer bo� ise
			for (int i = bsatir + 1; i <vsatir; i += 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'w') {//e�er ki sol yolunda kar�� tak�m var ise
						for (int j = i - 1; j > bsatir; j -= 1) { //burada dama ta� atl�yor mu ona bak�l�r.Daman�n yolu rakip ta�a kadar bo� olmal�d�r.
							if (dama[j][bsutun] != '-')sayac4++;
						}
						for (int k = i + 1; k < vsatir; k += 1) {//rakip ta� ile gidece�i yer aras�nda ba�ka ta� var m�?
							if (dama[k][bsutun] != '-') { //gidece�i yer-rakip ta� aras� bo� de�ilse false d�nd�r�r.
							}
						}
						if (sayac4 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, i, bsutun);
						}
					}
				}
			}
		}
	}
	//Ta� rakip ta�� yemeden hareket edecekse oyunda yiyilebilecek ba�ka ta� var m� bak�l�r.E�er varsa izin verilmez.Ayr�ca oynanan ta��n da yiyebilece�i ta� olup da yemezse sayaclarla denetlenir.
	if (izin() &&(sayac1==0) && (sayac2 == 0) && (sayac3 == 0) && (sayac4 == 0)) {
		DHamleYap(bsatir, bsutun, vsatir, vsutun,-1,0);
	}
	else {
		return false;
	}
}

int main()
{
	GirisEkrani();
	system("PAUSE");
}
