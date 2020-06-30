#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
using namespace std;

//------------------------------

//Okan Ögütlü 1306170076
//Programým temel olarak 3 katmandan oluþuyor,kullanýcýnýn programa verdiði komutlar ve deðerlendirme, komutlarýn oyun kurallarýna uygun olup olmadýðýný denetleme ve dosya iþlemleri
//Oyun Kurallarý: yoz taþlar arka ve çapraz hareket hariç 1 kare ilerler, tablo sonuna gelen taþ dama olur, damalar istediði kadar ilerler(L þeklinde)
//Eðer ki Tabloda Yiyilebilecek Taþ Var Ýse Önce Onu Yemek Mecburidir.

//------------------------------

//Fonksiyon Tanýmlamalarý:
bool izin();
void ilkAtama();
void GirisEkrani();
char siraKimde();
void Game();
int cevirici(char harf);
bool damaMý(int bsatir, int bsutun);
bool exist(int satir, int sutun);
bool legalMi(int bsatir, int bsutun, int vsatir, int vsutun);
bool yoz(int bsatir, int bsutun, int vsatir, int vsutun);
bool oyunMerkezi(string hamle);
void hamleYap(int bsatir, int bsutun, int vsatir, int vsutun, int hamleTuru);
bool Dama(int bsatir, int bsutun, int vsatir, int vsutun);
bool damaLegalMi(int bsatir, int bsutun, int vsatir, int vsutun);
bool DHamleYap(int bsatir, int  bsutun, int  vsatir, int  vsutun, int yiyilenR, int yiyilenC);
int kimKazandý();
void DosyaSifirla();
vector<vector<char>> dama;//tabloyu buraya aktarýyorum.

void ilkAtama() {

	dama.clear();//ilk olarak global olan tablo temizlenir.
	vector<vector<char>> vec(8);//local deðiþken tanýmladým,deðerleri buna atýp, buradan globale aktarýyorum.
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
	dama = vec;//Global deðiþkene aktarma
	for (const std::vector<char> &v : dama){
		for (char x : v) std::cout << x;
		std::cout << std::endl;

	}
}


void DosyaSifirla() {

	ofstream temizle;//Dosyayý açar ve içini temizler.
	temizle.open("dama.dat", std::ofstream::out | std::ofstream::trunc);
	temizle.close();
	ofstream temizle2;//Dosyayý açar ve içini temizler.
	temizle2.open("hamle.dat", std::ofstream::out | std::ofstream::trunc);
	temizle2.close();


	fstream myFile("dama.dat", ios::out | ios::trunc);//Dosyayý tekrar yazmak için açar.
	string satýr1 = "-- -- -- -- -- -- -- --";
	string satýr2 = "w* w* w* w* w* w* w* w*";
	string satýr6 = "b* b* b* b* b* b* b* b*";
	string satýr9 = "W";
	myFile << satýr1;
	myFile << '\n';
	myFile << satýr2;
	myFile << '\n';
	myFile << satýr2;
	myFile << '\n';
	myFile << satýr1;
	myFile << '\n';
	myFile << satýr1;
	myFile << '\n';
	myFile << satýr6;
	myFile << '\n';
	myFile << satýr6;
	myFile << '\n';
	myFile << satýr1;
	myFile << '\n';
	myFile << satýr9;


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

int kimKazandý() {
	int sayac1 = 0, sayac2 = 0;
	for (int satir = 0; satir < 7; satir++) {//Tüm tabloyu tarar;
		for (int sutun = 0; sutun < 16; sutun++) {//Tüm tabloyu tarar;
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
				<< endl << "Dama Oyununa Hosgeldiniz. \n Yeni Oyun için 1\nVarolan Oyuna Devam Etmek Ýçin 2 Tuslayiniz."
				<< endl;
		}
	}
}

char  siraKimde() {//Sýranýn kimde olduðunu belirler.
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
	bool devamMý = true;
	bool oynandýMý = true;
	int kazanan;
	string hamle;
	HamleAta(hamle);
	while (devamMý) {
		if (!oynandýMý) {
			cout << endl << "YANLIS HAMLE YAPTINIZ.TEKRAR DENEYIN" << endl;
		}
		cout << endl << "SIRA..:" << siraKimde() << "  Lutfen hamlenizi yapiniz :";
		cin >> hamle;
		HamleAta(hamle);
		oynandýMý = oyunMerkezi(hamle);
		kazanan = kimKazandý();
		if (kimKazandý() == 1) {
			cout << endl << endl << "BEYAZ TAKIM KAZANDI!";
			break;
		}
		if (kimKazandý() == 2) {
			cout << endl << endl << "SIYAH TAKIM KAZANDI!";
			break;
		}

	}
}

int cevirici(char harf) { //hamleleri integer a dönüþtürür.
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

bool damaMý(int bsatir, int bsutun) {//Taþýn dama mý yoksa yoz mu olduðunu belirler.
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

bool exist(int satir, int sutun) {//satýt-sutun tabloda var mý,kontrol eder.
	if (satir < 8 && satir >= 0 && sutun >= 0 && sutun < 16) {
		return true;
	}
	else return false;
}

bool legalMi(int bsatir, int bsutun, int vsatir, int vsutun) {
	
	if ((dama[vsatir][vsutun] == 'w') || (dama[vsatir][vsutun] == 'b')) {//gideceði yerde baþka taþ var mý?
		return false;
	}
	

	if (siraKimde() == 'b') {

		if ((bsatir == vsatir - 1) && ((bsutun == vsutun - 2) || (bsutun == vsutun + 2))) {//siyah için çapraz hareket mi?
			return false;
		}
		
		if (vsatir == bsatir + 1) {//siyah için arka hareket mi?
			return false;
		}

		if (exist(bsatir, bsutun + 2)) {//Eðer ki saðýnda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun + 2] == 'w')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir, bsutun - 2)) {//Eðer ki solunda rakip varsa iki hamleye izin verir.
				if ((dama[bsatir][bsutun - 2] == 'w')) {
					if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
						return true;
					}
				}
				
		}
		if (exist(bsatir - 1, bsutun)) {//Eðer ki önünde rakip varsa iki hamleye izin verir.
			if ((dama[bsatir - 1][bsutun] == 'w')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
		}
		
		if (abs(bsatir - vsatir) >= 2 || abs(bsutun - vsutun) >= 4) {//eðer ki yiyeceði taþ yoksa bir hamleye izin verir.
			return false;
		}	
		
	}
	if (siraKimde() == 'w') {

		if ((bsatir == vsatir + 1) && ((bsutun == vsutun - 2) || (bsutun == vsutun + 2))) {//beyaz için çapraz hareket mi?
			return false;
		}

		if (bsatir == vsatir + 1) {//beyaz için arka hareket mi?
			return false;
		}
	
		if (exist(bsatir, bsutun + 2)) {//Eðer ki saðýnda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun + 2] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir, bsutun - 2)) {//Eðer ki solunda rakip varsa iki hamleye izin verir.
			if ((dama[bsatir][bsutun - 2] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
			
		}
		if (exist(bsatir + 1, bsutun)) {//Eðer ki önünde rakip varsa iki hamleye izin verir.
			if ((dama[bsatir + 1][bsutun] == 'b')) {
				if (abs(bsatir - vsatir) == 2 || abs(bsutun - vsutun) == 4) {
					return true;
				}
			}
		}
		if (abs(bsatir - vsatir) >= 2 || abs(bsutun - vsutun) >= 4) {//Eðer ki yiyeceði taþ yoksa bir hamleye izin verir.
				return false;
		}
	}
	return true;
}

bool yoz(int bsatir, int bsutun, int vsatir, int vsutun) {//Yoz taþlarýn hareket merkezi

	if (!legalMi(bsatir, bsutun, vsatir, vsutun))return false; //arkaya ya da çapraz a hareket var mý?kontrol eder.
	
	int sayac = 0;

	if (siraKimde() == 'w') { //eðer ki oynayan kiþi w ise
		
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			if (exist(bsatir, (bsutun - 2))) {
				if (dama[bsatir][bsutun - 2] == 'b') {//eðer ki solunda karþý takým var ise
					if (exist(bsatir, bsutun - 4)) {
						if(dama[bsatir][bsutun-4]=='-'){//eðer ki gidebileceði yer boþ ise
							if (vsatir == bsatir && bsutun - 4 == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			
			if (exist(bsatir, (bsutun + 2))) {
				if (dama[bsatir][bsutun + 2] == 'b') { //eðer ki saðýnda karþý takým var ise
					if (exist(bsatir, bsutun + 4)) {
						if (dama[bsatir][bsutun + 4] == '-') {// eðer ki gidebilecði yer boþ ise
							if (vsatir == bsatir && bsutun + 4 == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
		
		if (exist(vsatir, vsutun) && (dama[vsatir][vsutun] == '-') ) {//eðer ki gideceði yer boþ ise
			if (exist((bsatir + 1), bsutun)) {
				if (dama[bsatir + 1][bsutun] == 'b') { //eðer ki önünde karþý takým var ise
					if (exist(bsatir + 2, bsutun)) {
						if (dama[bsatir+2][bsutun] == '-') {
							if (vsatir == bsatir + 2 && bsutun == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
			return false;//eðer ki yeme ihtimali olduðu halde yemezse false döndür.
		
		else {//Yeme ihtimali yoksa
			if (!izin()) return false;
			hamleYap(bsatir, bsutun, vsatir, vsutun, 7);
			return true;
		}
	}

	else if (siraKimde() == 'b') { //eðer ki oyuncu b ise
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			if (exist(bsatir, (bsutun - 2))) {
				if (dama[bsatir][bsutun - 2] == 'w') {//eðer ki solunda karþý takým var ise
					if (exist(bsatir, bsutun - 4)) {
						if (dama[bsatir][bsutun - 4] == '-') {
							if (vsatir == bsatir && bsutun - 4 == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
		 if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			if (exist(bsatir, (bsutun + 2))) {
				if (dama[bsatir][bsutun + 2] == 'w') { //eðer ki saðýnda karþý takým var ise
					if (exist(bsatir, bsutun + 4)) {
						if (dama[bsatir][bsutun + 4] == '-') {
							if (vsatir == bsatir && bsutun + 4 == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
		 if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			if (exist((bsatir - 1), bsutun)) {
				if (dama[bsatir - 1][bsutun] == 'w') { //eðer ki önünde karþý takým var ise
					if (exist(bsatir - 2, bsutun)) {
						if (dama[bsatir-2][bsutun] == '-') {
							if (vsatir == bsatir - 2 && bsutun == vsutun) {//eðer ki hamlesini oraya yapmýþ ise

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
			return false;//eðer ki yeme ihtimali olduðu halde yemezse false döndür.
		else {
			if (!izin()) return false;
			hamleYap(bsatir, bsutun, vsatir, vsutun, 7);
			return true;
		}
	}	
}

bool oyunMerkezi(string hamle) {//hamleleri çözümler
	int bsatir = 0, vsatir = 0, bsutun = 0, vsutun = 0;
	bsatir = hamle[1] - '0';
	bsatir = bsatir - 1;
	vsatir = hamle[3] - '0';
	vsatir = vsatir - 1;
	bsutun = cevirici(hamle[0]);
	bsutun = bsutun * 2;
	vsutun = cevirici(hamle[2]);
	vsutun = vsutun * 2;
	bool damamý = false;
	damamý=damaMý(bsatir,bsutun);
	if (!damamý) {//Taþ dama deðilse yoz bölümüne gider.
		if (!yoz(bsatir, bsutun, vsatir, vsutun))
			return false;
	}
	else {//Taþ dama ise Dama bölümüne gider.
		if(!Dama(bsatir, bsutun, vsatir, vsutun)) return false;
	}
	return true;
}

void hamleYap(int bsatir, int bsutun, int vsatir, int vsutun, int hamleTuru) {//Yoz taþlarý Dosyaya yazma iþlemi
	char sifirla = '-';
	char tas1;
	char tas2;
	fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
	char sifirla2 = '-';
	char wdama = 'ww';
	char bdama = 'bb';

	myFile.seekg((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý alýr
	myFile.read((char*)&tas1, sizeof(char));
	myFile.read((char*)&tas2, sizeof(char));

	if (tas1 == 'w' && vsatir == 7) { //Eðer ki taþ damaya ulaþmýþsa
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý yerine yazar
		myFile.write((char*)&wdama, sizeof(char));
		myFile.write((char*)&wdama, sizeof(char));
	}
	else if (tas1 == 'b' && vsatir == 0) { //Eðer ki taþ damaya ulaþmýþsa
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý yerine yazar
		myFile.write((char*)&bdama, sizeof(char));
		myFile.write((char*)&bdama, sizeof(char));
	}
	else{
		myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý yerine yazar
		myFile.write((char*)&tas1, sizeof(char));
		myFile.write((char*)&tas2, sizeof(char));
	}
	myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//Baþlangýç yerini sýfýrlar.
	myFile.write((char*)&sifirla, sizeof(char));
	myFile.write((char*)&sifirla, sizeof(char));


	if (hamleTuru == 1) { //beyaz sol yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun ) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 2) { //beyaz sað yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun+4) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 3) { //beyaz ön yediyse

		myFile.seekp((((bsatir+1) * 25) + ((((bsutun+2) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 4) { //siyah sol yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 5) { //siyah sað yediyse

		myFile.seekp((((bsatir) * 25) + ((((bsutun+4) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}
	if (hamleTuru == 6) { //siyah ön yediyse

		myFile.seekp((((bsatir-1) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);
		myFile.write((char*)&sifirla2, sizeof(char));
		myFile.write((char*)&sifirla2, sizeof(char));
	}

	myFile.close();

	if (hamleTuru == 7) { //Eðer ki ortada yeme durumu yoksa
		if (siraKimde() == 'w') { // sýra þuan beyazdaysa
			hamleTuru = 4; //siyaha geçsin
		}
		else if (siraKimde() == 'b') { //sýra þuan siyahtaysa
			hamleTuru = 1; //sýra beyaza geçsin
		}
	}
	if ((hamleTuru == 1) | (hamleTuru==2) | (hamleTuru==3) ) { //Eðer ki beyaz yediyse sýra tekrar ona geçsin
		fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
		myFile.seekp(-1, ios::end);
		char sira = 'W';
		myFile.write((char*)&sira, sizeof(char));
		myFile.close();
	}
	if ((hamleTuru == 4) | (hamleTuru == 5) | (hamleTuru == 6)) { //Eðer ki siyah yediyse sýra tekrar ona geçsin
		fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
		myFile.seekp(-1, ios::end);
		char sira = 'B';
		myFile.write((char*)&sira, sizeof(char));
		myFile.close();
	}
	ilkAtama();
}

bool DHamleYap(int bsatir, int  bsutun, int  vsatir, int  vsutun, int yiyilenR,int yiyilenC) {//Dama taþý Dosyaya yazma bölümü
	char sifirla = '-';
	char tas1;
	char tas2;
	fstream myFile("dama.dat", ios::binary | ios::out | ios::in);
	char sifirla2 = '-';
	char wdama = 'ww';
	char bdama = 'bb';

	myFile.seekg((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý alýr
	myFile.read((char*)&tas1, sizeof(char));
	myFile.read((char*)&tas2, sizeof(char));

	myFile.seekp((((vsatir) * 25) + ((((vsutun + 2) / 2) - 1) * 3)), ios::beg);//taþý yerine yazar
	myFile.write((char*)&tas1, sizeof(char));
	myFile.write((char*)&tas2, sizeof(char));
	if(yiyilenR!=-1){
		myFile.seekp((((yiyilenR) * 25) + ((((yiyilenC + 2) / 2) - 1) * 3)), ios::beg);//taþ yediyse yiyilen taþý sýfýrlar 
		myFile.write((char*)&sifirla, sizeof(char));
		myFile.write((char*)&sifirla, sizeof(char));

		myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//baþlangýc yerini sýfýrlar 
		myFile.write((char*)&sifirla, sizeof(char));
		myFile.write((char*)&sifirla, sizeof(char));
		
	}
	else if (yiyilenR == -1) {
		myFile.seekp((((bsatir) * 25) + ((((bsutun + 2) / 2) - 1) * 3)), ios::beg);//Eðer ki taþ yemeden hareket ettiyse 
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

bool izin() { //Oyunda yenilebilecek baþka taþ var mý kontrol eder.
	
	if (siraKimde() == 'w') { //sýra beyazdaysa sadece beyaz taþlarý kontrol eder.

		for (int satir = 0; satir < 7; satir++) {//Tüm tabloyu tarar;
			for (int sutun = 0; sutun < 16; sutun++) {//Tüm tabloyu tarar;
				
				if(dama[satir][sutun]=='w'){
					if (exist(satir, sutun - 4)) {
						if (dama[satir][sutun - 4] == '-') {//eðer ki gidebileceði yer boþ ise
							if (exist(satir, (sutun - 2))) {
								if (dama[satir][sutun - 2] == 'b') {//eðer ki solunda karþý takým var ise
									return false;
								}
							}
						}
					}
					if (exist(satir, sutun + 4)) {
						if (dama[satir][sutun + 4] == '-') {//eðer ki gidebileceði yer boþ ise
							if (exist(satir, (sutun + 2))) {
								if (dama[satir][sutun + 2] == 'b') { //eðer ki saðýnda karþý takým var ise
									return false;
								}
							}
						}
					}

					if (exist((satir + 2), sutun)) {
						if (dama[satir+2][sutun] == '-')  {//eðer ki gideceði yer boþ ise
							if(exist((satir+1),sutun)){
								if (dama[satir + 1][sutun] == 'b') { //eðer ki önünde karþý takým var ise
									return false;
								}
							}
						}
					}
				}
				
				if (damaMý(satir, sutun) && (dama[satir][sutun]=='w')) {//eðer ki taþ Beyaz Dama ise
					
					int sol = sutun; //Damanýn soluna bakar.
					if (exist(satir, sol - 2)) {
						sol = sol - 2;
						while (dama[satir][sol] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(satir, sol - 2)) {
								sol = sol - 2;
							}
							else
								break;
						}
						if (exist(satir, sol - 2)){
							if ((dama[satir][sol] == 'b') && (dama[satir][sol - 2] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
								return false;
							}
						}
					}

					int sag = sutun;//Damanýn saðýna bakar.
					if (exist(satir, sag + 2)) {
						sag = sag + 2;
						while (dama[satir][sag] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(satir, sag + 2)) {
								sag = sag + 2;
							}
							else
								break;
						}
						if (exist(satir, sag + 2)) {
							if ((dama[satir][sag] == 'b') && (dama[satir][sag + 2] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
								return false;
							}
						}
					}


					int on = satir;//Damanýn önüne bakar.
					if (exist(on+1, sutun)) {
						on = on + 1;
						while (dama[on][sutun] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(on+1, sutun)) {
								on = on + 1;
							}
							else
								break;
						}
						if (exist(on+1, sutun)) {
							if ((dama[on][sutun] == 'b') && (dama[on+1][sutun] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
								return false;
							}
						}
					}

					int arka = satir;//Damanýn arkasýna bakar.
					if (exist(arka - 1, sutun)) {
						arka = arka - 1;
						while (dama[arka][sutun] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(arka - 1, sutun)) {
								arka = arka - 1;
							}
							else
								break;
						}
						if (exist(arka - 1, sutun)) {
							if ((dama[arka][sutun] == 'b') && (dama[arka - 1][sutun] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
								return false;
							}
						}
					}
				}
					

				
			}
		
			
		}

	}

	if (siraKimde() == 'b') { //sýra siyahtaysa sadece siyah taþlarý kontrol eder.

		for (int satir = 0; satir <= 7; satir++) {
			for (int sutun = 0; sutun < 16; sutun++) {
				if(dama[satir][sutun]=='b'){
					if (exist(satir, sutun - 4)) {
						if (dama[satir][sutun - 4] == '-') {//eðer ki gidebileceði yer boþ ise

							if (exist(satir, (sutun - 2))) {
								if (dama[satir][sutun - 2] == 'w') {//eðer ki solunda karþý takým var ise
									return false;
								}
							}
						}
					}
					if (exist(satir, sutun + 4)) {
						if (dama[satir][sutun + 4] == '-') {//eðer ki gidebileceði yer boþ ise

							if (exist(satir, (sutun + 2))) {
								if (dama[satir][sutun + 2] == 'w') { //eðer ki saðýnda karþý takým var ise
									return false;
								}
							}
						}
					}

					if (exist((satir - 2), sutun)) {
						if (dama[satir - 2][sutun] == '-') {//eðer ki gideceði yer boþ ise			
							if (exist(satir - 1, sutun)) {
								if (dama[satir - 1][sutun] == 'w') { //eðer ki önünde karþý takým var ise

									return false;
								}
							}
						}
					}
				}
			
				if (damaMý(satir, sutun) && (dama[satir][sutun]=='b') ) {//eðer ki taþ Siyah Dama ise
					
					int sol = sutun;//Damanýn soluna bakar.
					if (exist(satir, sol - 2)) {
						sol = sol - 2;
						while (dama[satir][sol] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(satir, sol - 2)) {
								sol = sol - 2;
							}
							else
								break;
						}
						if (exist(satir, sol - 2)) {
							if ((dama[satir][sol] == 'w') && (dama[satir][sol - 2] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.	
								return false;
							}
						}
					}

					int sag = sutun;//Damanýn saðýna bakar.
					if (exist(satir, sag + 2)) {
						sag = sag + 2;
						while (dama[satir][sag] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(satir, sag + 2)) {
								sag = sag + 2;
							}
							else
								break;
						}
						if (exist(satir, sag + 2)) {
							if ((dama[satir][sag] == 'w') && (dama[satir][sag + 2] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.
								return false;
							}
						}
					}


					int on = satir;//Damanýn önüne bakar.
					if (exist(on - 1, sutun)) {
						on = on - 1;
						while (dama[on][sutun] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(on - 1, sutun)) {
								on = on - 1;
							}
							else
								break;
						}
						if (exist(on - 1, sutun)) {
							if ((dama[on][sutun] == 'w') && (dama[on - 1][sutun] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
								return false;
							}
						}
					}


					int arka = satir;//Damanýn arkasýna bakar.
					if (exist(arka + 1, sutun)) {
						arka = arka + 1;
						while (dama[arka][sutun] == '-') { //Boþluk olduðu müddetçe sola ilerler
							if (exist(arka + 1, sutun)) {
								arka = arka + 1;
							}
							else
								break;
						}
						if (exist(arka + 1, sutun)) { 
							if ((dama[arka][sutun] == 'w') && (dama[arka + 1][sutun] == '-')) { //Eðer ki boþluðun bir yaný rakip, onun da bir yaný boþ ise false döndürür.				
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

bool damaLegalMi(int bsatir, int bsutun, int vsatir, int vsutun) { //Dama taþýnýn gideceði yeri denetler
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
	if ((abs(bsatir - vsatir) != 0) && (abs(bsutun - vsutun) == 0)) {//hareket satýrlar arasýnda mý?
		for (; minR < maxR; minR++) {
			if (dama[minR][bsutun] == siraKimde()) { //baþlangýç-varýþ arasýnda kendi taþýndan var mý?
				return false;
			}
		}

	}
	if ((abs(bsutun - vsutun) != 0) && (abs(bsatir - vsatir) == 0)) {//hareket sutunlar arasýnda mý?
		for (; minC < maxC; minC++) {
			if (dama[bsatir][minC] == siraKimde()) { //baþlangýç-varýþ arasýnda kendi taþýndan var mý?
				return false;
			}
		}

	}
	if ( ((bsatir - vsatir != 0) && (bsutun - vsutun == 0) ) || ( (bsatir - vsatir == 0) && (bsutun - vsutun != 0))) { //hareket doðrusal mý?
		return true;
	}
	else return false;
}

bool Dama(int bsatir, int bsutun, int vsatir, int vsutun) {
	int sayac1=0, sayac2=0, sayac3=0, sayac4 = 0;
	if(!damaLegalMi(bsatir, bsutun, vsatir, vsutun))return false;
	
	if(siraKimde()=='w'){
		//Beyaz Dama sol taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for(int i=bsutun-2;i>=vsutun;i-=2){
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'b') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i+2; j < bsutun; j+=2) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[bsatir][j] != '-')sayac1++;
						}
						for (int k = i-2; k > vsutun; k -= 2) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if(dama[bsatir][k]!='-'){ //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
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

		//Beyaz Dama sag taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsutun + 2; i <= vsutun; i += 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'b') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i - 2; j > bsutun; j -= 2) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[bsatir][j] != '-')sayac2++;
						}
						for (int k = i + 2; k < vsutun; k += 2) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[bsatir][k] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
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
		
		
		//Beyaz Dama ön taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsatir +1; i < vsatir; i += 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'b') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i - 1; j > bsatir; j -= 1) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[j][bsutun] != '-')sayac3++;;
						}
						for (int k = i + 1; k < vsatir; k += 1) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[k][bsutun] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
							}
						}
						if(sayac3 == 0){
						return DHamleYap(bsatir, bsutun, vsatir, vsutun, i,bsutun);
						}
					}
				}
			}
		}
		

		//Beyaz Dama arka taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsatir - 1; i > vsatir; i -= 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'b') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i + 1; j < bsatir; j += 1) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[j][bsutun] != '-')sayac4++;;
						}
						for (int k = i - 1; k > vsatir; k -= 1) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[k][bsutun] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
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
		//Siyah Dama sol taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsutun - 2; i > vsutun; i -= 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'w') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i + 2; j > bsutun; j += 2) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[bsatir][j] != '-')sayac1++;
						}
						for (int k = i - 2; k > vsutun; k -= 2) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[bsatir][k] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
							}
						}
						if (sayac1 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}

		//Siyah Dama sag taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsutun + 2; i <vsutun; i += 2) {
				if (exist(bsatir, (i))) {
					if (dama[bsatir][i] == 'w') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i - 2; j > bsutun; j -= 2) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[bsatir][j] != '-')sayac2++;
						}
						for (int k = i + 2; k < vsutun; k += 2) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[bsatir][k] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
							}
						}
						if (sayac2 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, bsatir, i);
						}
					}
				}
			}
		}


		//Siyah Dama ön taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsatir - 1; i >vsatir; i -= 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'w') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i + 1; j < bsatir; j += 1) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[j][bsutun] != '-')sayac3++;
						}
						for (int k = i - 1; k > vsatir; k -= 1) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[k][bsutun] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
							}
						}
						if (sayac3 == 0){
							return DHamleYap(bsatir, bsutun, vsatir, vsutun, i, bsutun);
						}
					}
				}
			}
		}


		//Siyah Dama arka taraftan taþ yeme olayý:
		if (exist(vsatir, vsutun) && dama[vsatir][vsutun] == '-') {//eðer ki gideceði yer boþ ise
			for (int i = bsatir + 1; i <vsatir; i += 1) {
				if (exist(i, bsutun)) {
					if (dama[i][bsutun] == 'w') {//eðer ki sol yolunda karþý takým var ise
						for (int j = i - 1; j > bsatir; j -= 1) { //burada dama taþ atlýyor mu ona bakýlýr.Damanýn yolu rakip taþa kadar boþ olmalýdýr.
							if (dama[j][bsutun] != '-')sayac4++;
						}
						for (int k = i + 1; k < vsatir; k += 1) {//rakip taþ ile gideceði yer arasýnda baþka taþ var mý?
							if (dama[k][bsutun] != '-') { //gideceði yer-rakip taþ arasý boþ deðilse false döndürür.
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
	//Taþ rakip taþý yemeden hareket edecekse oyunda yiyilebilecek baþka taþ var mý bakýlýr.Eðer varsa izin verilmez.Ayrýca oynanan taþýn da yiyebileceði taþ olup da yemezse sayaclarla denetlenir.
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
