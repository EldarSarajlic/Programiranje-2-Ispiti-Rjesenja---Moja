#include <iostream>
#include <regex>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. METODE I KLASE KOJE NE IMPLEMENTIRATE NEMOJTE BRISATI (BEZ OBZIRA STO NEMAJU IMPLEMENTACIJU)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"12. ZA IZRADU ISPITNOG RJESENJA KORISTITI VISUAL STUDIO 2022!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Karakteristike { NARUDZBA, KVALITET, PAKOVANJE, ISPORUKA };
ostream& operator<<(ostream& COUT, const Karakteristike& index) {
	const char* niz[] = { "NARUDZBA", "KVALITET", "PAKOVANJE", "ISPORUKA" };
	cout << niz[index];
	return COUT;
}
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

template<class T1, class T2>
class Rjecnik {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Rjecnik(bool omoguciDupliranje = true) {
		_omoguciDupliranje = omoguciDupliranje;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	Rjecnik(const Rjecnik& obj) {
		_omoguciDupliranje = obj._omoguciDupliranje;
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Rjecnik& operator=(const Rjecnik& obj) {
		if (this != &obj) {
			delete[] _elementi1; _elementi1 = nullptr;
			delete[] _elementi2; _elementi2 = nullptr;

			_omoguciDupliranje = obj._omoguciDupliranje;
			_trenutno = obj._trenutno;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
	~Rjecnik() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }

	friend ostream& operator<< (ostream& COUT, const Rjecnik& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		if (_omoguciDupliranje == false) {
			for (size_t i = 0; i < _trenutno; i++)
			{
				if (getElement1(i) == el1 && getElement2(i) == el2)
					throw exception("Dupli elementi nisu dozvoljeni!\n");
			}
		}
		T1* temp1 = _elementi1;
		T2* temp2 = _elementi2;
		_elementi1 = new T1[_trenutno + 1];
		_elementi2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = temp1[i];
			_elementi2[i] = temp2[i];
		}
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}

	Rjecnik operator()(int pocetak, int kraj) {
		Rjecnik novi;
		if (_trenutno != 0) {
			for (size_t i = pocetak; i <= kraj; i++)
			{
				if (i < 0 || i >= _trenutno)
					continue;
				novi.AddElement(getElement1(i), getElement2(i));
			}
		}
		return novi;
	}

};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}


};

class ZadovoljstvoKupca {
	int _ocjena; //za svaku kupovinu kupac moze ostaviti jednu ocjenu
	Rjecnik<Karakteristike, const char*> _komentariKarakteristika;
public:
	ZadovoljstvoKupca(int ocjena = 0) {
		_ocjena = ocjena;
	}
	ZadovoljstvoKupca(const ZadovoljstvoKupca& obj) {
		_ocjena = obj._ocjena;
		_komentariKarakteristika = obj._komentariKarakteristika;
	}
	ZadovoljstvoKupca& operator=(const ZadovoljstvoKupca& obj) {
		if (this != &obj) {
			_ocjena = obj._ocjena;
			_komentariKarakteristika = obj._komentariKarakteristika;
		}
		return *this;
	}
	~ZadovoljstvoKupca() {

	}
	int GetOcjena() { return _ocjena; }
	Rjecnik<Karakteristike, const char*>& GetKomentareKarakteristika() { return _komentariKarakteristika; }

	//uz ocjenu, kupci mogu komentarisati svaku od karakteristika proizvoda.
	//onemoguciti dupliranje karakteristika tj. svaka karakteristika se moze komentarisati samo jednom...u suprotnom baciti objekat tipa exception
	void DodajKomentarKarakteristike(Karakteristike karakteristika, const char* komentar) {
		if (_komentariKarakteristika.getTrenutno() != 0) {
			for (size_t i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
			{
				if (_komentariKarakteristika.getElement1(i) == karakteristika)
					throw exception("Karakteristika vec komentarisana!\n");
			}
		}
		_komentariKarakteristika.AddElement(karakteristika, komentar);
	}

	friend ostream& operator<<(ostream& COUT, const ZadovoljstvoKupca& obj) {
		COUT << obj._ocjena << endl;
		for (size_t i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
		{
			COUT << obj._komentariKarakteristika.getElement1(i) << " - " << obj._komentariKarakteristika.getElement2(i) << endl;
		}
		return COUT;
	}

	bool operator==(const ZadovoljstvoKupca& obj) {
		if (_ocjena != obj._ocjena)
			return false;
		for (size_t i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
		{
			if (_komentariKarakteristika.getElement1(i) != obj._komentariKarakteristika.getElement1(i))
				return false;
			if (!strcmp(_komentariKarakteristika.getElement2(i), obj._komentariKarakteristika.getElement2(i)))
				return false;
		}
		return true;
	}
};

class Osoba {
protected:
	char* _imePrezime;
	Datum _datumRodjenja;
public:
	Osoba(const char* imePrezime = "", Datum datumRodjenja = Datum()) : _datumRodjenja(datumRodjenja) {
		_imePrezime = GetNizKaraktera(imePrezime);
	}
	Osoba(const Osoba& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_datumRodjenja = obj._datumRodjenja;
	}
	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_datumRodjenja = obj._datumRodjenja;
		}
		return *this;
	}
	virtual ~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	char* GetImePrezime() { return _imePrezime; }
	Datum& GetDatumRodjenja() { return _datumRodjenja; } //kada se vraca po referenci a kada ne? da znamo prepraviti greške

	virtual void Info() = 0;
	friend ostream& operator<< (ostream& COUT, const Osoba& obj) {
		COUT << obj._imePrezime << " " << obj._datumRodjenja << endl;
		return COUT;
	}
};

mutex m;
class Kupac : public Osoba {
	char* _emailAdresa;
	//float se odnosi na iznos racuna za odredjenu kupovinu
	Rjecnik<float, ZadovoljstvoKupca>* _kupovine;
	vector<int> _bodovi;

	/*prilikom svake kupovine, ukoliko je kupac ostvario vise od 5 bodova, potrebno je, u zasebnom thread-u (nakon 3 sekunde), poslati email sa sljedecim sadrzajem:

	  To: denis@fit.ba;
	  Subject: Osvareni bodovi

	  Postovani,

	  Prilikom posljednje kupovine ste ostvarili 8 bodova, tako da trenutno vas ukupan broj bodova iznosi 20.

	  Zahvaljujemo vam na kupovini.
	  Puno pozdrava
  */
	void posaljiMail(int zadnjiBodovi) {
		m.lock();
		cout << "\n\nTo:" << _emailAdresa << ";\n";
		cout << "Subject: Ostvareni bodovi.\n\n";
		cout << "Postovani, \n\n";
		cout << "Prilikom posljednje kupovine ste ostvarili " << zadnjiBodovi << " bodova, tako da trenutno vas ukupan broj bodova iznosi " << GetBodoviUkupno() << endl << endl;
		cout << "Zahvaljujemo vam na kupovini\n";
		cout << "Puno pozdrava\n\n";
		m.unlock();
	}
public:
	Kupac(const char* imePrezime = "", Datum datumRodjenja = Datum(), const char* emailAdresa = "") : Osoba(imePrezime, datumRodjenja) {
		_emailAdresa = GetNizKaraktera(emailAdresa);
		_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(false);
	}
	Kupac(const Kupac& obj) {
		_emailAdresa = GetNizKaraktera(obj._emailAdresa);
		_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(obj._kupovine);
		_bodovi = obj._bodovi;
	}
	Kupac& operator=(const Kupac& obj) {
		if (this != &obj) {
			delete[] _emailAdresa; _emailAdresa = nullptr;
			delete _kupovine; _kupovine = nullptr;
			_emailAdresa = GetNizKaraktera(obj._emailAdresa);
			_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(obj._kupovine);
			_bodovi = obj._bodovi;
		}
		return *this;
	}
	~Kupac()
	{
		delete[] _emailAdresa; _emailAdresa = nullptr;
		delete _kupovine; _kupovine = nullptr;
	}

	char* GetEmail() { return _emailAdresa; }
	Rjecnik<float, ZadovoljstvoKupca>& GetKupovine() { return *_kupovine; } //opet zasto referenca?
	vector<int> GetBodovi() { return _bodovi; }
	int GetBodoviUkupno() {
		int ukupno = 0;
		for (size_t i = 0; i < _bodovi.size(); i++) ukupno += _bodovi[i];
		return ukupno;
	}

	friend ostream& operator<< (ostream& COUT, const Kupac& obj) {
		COUT << crt << obj._imePrezime << " " << obj._datumRodjenja << " " << obj._emailAdresa << " " << endl;
		COUT << "KUPOVINE -> " << crt;
		for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
			COUT << "Iznos racuna: " << obj._kupovine->getElement1(i) << "KM, zadovoljstvo kupca: " << obj._kupovine->getElement2(i) << crt;
		COUT << "BODOVI -> ";
		for (size_t i = 0; i < obj._bodovi.size(); i++)
			COUT << obj._bodovi[i] << ", ";
		COUT << crt;
		return COUT;
	}
	void Info() {
		cout << *this;
	}


	// bodovi sakupljeni tokom kupovine, svakih potrosenih 10KM donosi po 1 bod.
	void DodajKupovinu(float potroseno, ZadovoljstvoKupca zadovoljstvo) {
		_kupovine->AddElement(potroseno, zadovoljstvo);
		if (potroseno >= 10)
			_bodovi.push_back(potroseno / 10);
		if (_bodovi.size() != 0) {
			int zadnjiBodovi = _bodovi[_bodovi.size() - 1];
			if (zadnjiBodovi > 5) {
				this_thread::sleep_for(1s);
				thread t1(&Kupac::posaljiMail, this, zadnjiBodovi);
				t1.join();
			}
		}
	}

	void SacuvajBodove() {
		string naziv = _emailAdresa;
		naziv += ".txt";
		ofstream fajl(naziv);
		if (!fajl.fail())
		{
			for (int i = 0; i < _bodovi.size(); i++)
			{
				fajl << _bodovi[i];
				//dodajemo ovaj uslov jer ne zelimo imati dodatan red koji je ustvari jednak "" jer cemo kasnije citati podatke iz tog fajla
				if (i != _bodovi.size() - 1)
					fajl << endl;
			}
		}
	}

	void UcitajBodove() {
		string naziv = _emailAdresa;
		naziv += ".txt";
		ifstream fajl(naziv);
		const int max = 20;
		char red[max]; //ovo je niz karaktera u koji pohranjujemo vrijednost jednog reda iz fajla
		string brojStr;
		int broj;
		while (!fajl.eof())
		{
			fajl.getline(red, max);
			brojStr = red; //radi lakoce rada sa stringom u string pohranjujemo sadrzaj iz reda
			broj = stoi(brojStr); //stoi funkcija konvertuje string u integer, uzrokuje runtime error ako string nije broj te smo zbog toga na 354. liniji provjeravali da ne dodamo viska novi red koji je ustvari "" (odnosno nije broj)
			_bodovi.push_back(broj);
			/*
				ako ne zelimo dodati if uslov na 354. liniji onda bi ovdje samo dodali uslov:
					if(brojStr=="")
					{
						broj=stoi(brojStr);
						_bodovi.push_back(broj);
					}
				bez ovog uslova i uslova na 354. liniji desio bi se runtime error
			*/
		}
	}

	Rjecnik<Karakteristike, const char*> GetKupovineByKomentar(const char* kljucnaRijec) {
		Rjecnik<Karakteristike, const char*> rjecnik;
		string trazeni = kljucnaRijec;
		for (int i = 0; i < _kupovine->getTrenutno(); i++)
		{
			for (int j = 0; j < _kupovine->getElement2(i).GetKomentareKarakteristika().getTrenutno(); j++)
			{
				string tekst = _kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j);
				if (tekst.find(trazeni) != -1) //metoda find vraca lokaciju na kojoj pocinje trazeni substring u stringu, vraca -1 ako nije pronadjen substring, moguce koristiti string::npos umjesto -1 : if (tekst.find(trazeni) != string::npos)
					rjecnik.AddElement(_kupovine->getElement2(i).GetKomentareKarakteristika().getElement1(j),
						_kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j));
			}
		}
		return rjecnik;
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Nabrojite i ukratko pojasnite osnovne razlike izmedju list i vector klase?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasnite opcije vezane za preslikavanje (vrijednosti, referenci, dostupnost) parametara prilikom koristenja lambda funkcija?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

	/*cout << PORUKA;
	cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();*/

	const int rjecnikTestSize = 9;
	Rjecnik<int, int> rjecnik1(false);
	for (int i = 0; i < rjecnikTestSize - 1; i++)
		rjecnik1.AddElement(i, i);//dodaje vrijednosti u rjecnik

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 ili T2), metoda AddElement treba baciti objekat tipa exception
		rjecnik1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	rjecnik1.AddElement(9, 9);

	cout << rjecnik1 << crt;

	/*
	* metoda prihvata indekse lokacije, te vraća elemente koji se u rjecniku nalaze na proslijedjenim lokacijama (ukljucujuci i te lokacije)
	*/
	Rjecnik<int, int> rjecnik2 = rjecnik1(2, 5);
	/*clanovi objekta rjecnik2 bi trebali biti:
	2 2
	3 3
	4 4
	5 5
	*/
	cout << rjecnik2 << crt;


	const int maxKupaca = 3;
	Osoba* kupci[maxKupaca];
	kupci[0] = new Kupac("Denis Music", Datum(12, 1, 1980), "denis@fit.ba");
	kupci[1] = new Kupac("Jasmin Azemovic", Datum(12, 2, 1980), "jasmin@fit.ba");
	kupci[2] = new Kupac("Adel Handzic", Datum(12, 3, 1980), "adel@edu.fit.ba");

	ZadovoljstvoKupca zadovoljstvoKupca(7);
	zadovoljstvoKupca.DodajKomentarKarakteristike(NARUDZBA, "Nismo mogli odabrati sve potrebne opcije");
	zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");

	try {
		//karakteristika kvalitet je vec komentarisana
		zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	zadovoljstvoKupca.DodajKomentarKarakteristike(PAKOVANJE, "Pakovanje je bio osteceno");
	zadovoljstvoKupca.DodajKomentarKarakteristike(ISPORUKA, "Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana");

	Kupac* denis = dynamic_cast<Kupac*>(kupci[0]);
	/*za svakih 10KM kupcu se dodaje 1 bod, pa ce tako kupovina od 128KM kupcu donijeti 12 bodova*/
	denis->DodajKupovinu(128, zadovoljstvoKupca);
	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();//12 bodova


	ZadovoljstvoKupca zadovoljstvoKupca2(4);
	zadovoljstvoKupca2.DodajKomentarKarakteristike(KVALITET, "Jako lose, proizvod ostecen");
	denis->DodajKupovinu(81, zadovoljstvoKupca2);

	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();//20 bodova

	/*prilikom svake kupovine, ukoliko je kupac ostvario vise od 5 bodova, potrebno je, u zasebnom thread-u (nakon 3 sekunde), poslati email sa sljedecim sadrzajem:

	  To: denis@fit.ba;
	  Subject: Osvareni bodovi

	  Postovani,

	  Prilikom posljednje kupovine ste ostvarili 8 bodova, tako da trenutno vas ukupan broj bodova iznosi 20.

	  Zahvaljujemo vam na kupovini.
	  Puno pozdrava
  */


  //ispisuje sve podatke o kupcu i njegovim kupovinama.
	denis->Info();
	/* Primjer ispisa:
		-------------------------------------------
		Denis Music 12.1.1980 denis@fit.ba
		KUPOVINE ->
		-------------------------------------------
		Iznos racuna: 128KM, zadovoljstvo kupca: 7
				NARUDZBA - Nismo mogli odabrati sve potrebne opcije
				KVALITET - Kvalitet je ocekivan
				PAKOVANJE - Pakovanje je bio osteceno
				ISPORUKA - Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana
		-------------------------------------------
		Iznos racuna: 81KM, zadovoljstvo kupca: 4
				KVALITET - Jako lose, proizvod ostecen
		-------------------------------------------
		BODOVI -> 12, 8,
		-------------------------------------------
   */

     /*metoda SacuvajBodove treba sve clanove vector-a _bodovi upisati u fajl(ignorisuci ranije dodate/postojece vrijednosti u fajlu)
	 pod nazivom emailKorisnika.txt npr. denis@fit.ba.txt.
     na osnovu trenutnog stanja objekta, sadrzaj fajla denis@fit.ba.txt bi trebao biti sljedeci:
     12
     8

     nakon spasavanja u fajl, sadrzaj vector-a ostaje nepromijenjen.
     */
	  denis->SacuvajBodove();

	  //metoda UcitajBodove ucitava sadrzaj fajla pod nazivom emailKorisnika.txt i njegove vrijednosti 
	  // pohranjuje/dodaje u vector _bodovi (zadrzavajuci postojece bodove).  
	  denis->UcitajBodove();

	  //metoda GetKupovineByKomentar treba da pronadje i vrati sve karakteristike proizvoda i komentare kupaca koji sadrze vrijednost proslijedjenog parametra
	  Rjecnik<Karakteristike, const char*> osteceniProizvodi = denis->GetKupovineByKomentar("ostecen");
	  cout << crt << "Rezultat pretrage -> " << crt << osteceniProizvodi << crt;

	  /*Ocekivani ispis:
	  	-------------------------------------------
	  	Rezultat pretrage ->
	  	-------------------------------------------
	  	PAKOVANJE Pakovanje je bio osteceno
	  	KVALITET Jako lose, proizvod ostecen
	  	-------------------------------------------
	  */

	  for (size_t i = 0; i < maxKupaca; i++)
	  	delete kupci[i], kupci[i] = nullptr;

	  cin.get();
	  system("pause>0");
}
