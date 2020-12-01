#ifndef KLASY_H
#define KLASY_H


#include <vector>
#include <unordered_map>

using namespace std;

class Postac{
protected:
  int szybkosc;
public:
  Postac(int);
  Postac() = default;
  Postac(const Postac&) = delete;
  virtual string inicjal() = 0;
  virtual char ktoryZespol() = 0;
  virtual bool walka(Postac*) = 0;
  int jakaSzybkosc();
  Postac& operator = (const Postac&) = delete;
  virtual ~Postac() = default;
};

class Rebeliant :public Postac{
  char zespol;
public:
  Rebeliant(int);
  char ktoryZespol();
  Rebeliant(const Rebeliant&) = delete;
  Rebeliant& operator = (const Rebeliant&) = delete;
  ~Rebeliant() = default;
};

class ZwyklyRebeliant :public Rebeliant{
  int numer = 0;
public:
  ZwyklyRebeliant(int, int);
  ZwyklyRebeliant() = default;
  bool walka(Postac*);
  string inicjal();
  ZwyklyRebeliant& operator = (const ZwyklyRebeliant&) = default;
  ZwyklyRebeliant(const ZwyklyRebeliant&) = default;
  ~ZwyklyRebeliant() = default;
};

class Programista :public Rebeliant{
  int numer = 0;
public:
  Programista(int, int);
  Programista() = default;
  bool walka(Postac*);
  string inicjal();
  Programista& operator = (const Programista&) = default;
  Programista(const Programista&) = default;
  ~Programista() = default;
};

class KsiezniczkaLeia :public Rebeliant{
public:
  KsiezniczkaLeia(int);
  KsiezniczkaLeia(const KsiezniczkaLeia&) = delete;
  bool walka(Postac*);
  string inicjal();
  KsiezniczkaLeia& operator = (const KsiezniczkaLeia&) = delete;
  ~KsiezniczkaLeia() = default;
};

class LukeSkywalker :public Rebeliant{
public:
  LukeSkywalker(int);
  LukeSkywalker(const LukeSkywalker&) = delete;
  bool walka(Postac*);
  string inicjal();
  LukeSkywalker& operator = (const LukeSkywalker&) = delete;
  ~LukeSkywalker() = default;
};

class Detektor{
  bool detekt;
public:
  Detektor(bool = false);
  bool czyDetektor() const;
  void zmienDetektor(bool);
  Detektor(const Detektor&) = default;
  ~Detektor() = default;
};

class Imperialista :public Postac{
  char zespol;
public:
  Imperialista(int);
  char ktoryZespol();
  Imperialista& operator = (const Imperialista&) = delete;
  Imperialista(const Imperialista&) = delete;
  ~Imperialista() = default;
};

class RobotZolnierz :public Imperialista{
  int numer = 0;
public:
  RobotZolnierz(int, int);
  bool walka(Postac*);
  string inicjal();
  RobotZolnierz& operator = (const RobotZolnierz&) = default;
  RobotZolnierz(const RobotZolnierz&) = default;
  ~RobotZolnierz() = default;
};

class LordVader :public Imperialista{
public:
  LordVader(int);
  LordVader(const LordVader&) = delete;
  bool walka(Postac*);
  string inicjal();
  LordVader& operator = (const LordVader&) = delete;
  ~LordVader() = default;
};

class Kanclerz :public Imperialista{
public:
  Kanclerz();
  Kanclerz(const Kanclerz&) = delete;
  bool walka(Postac*);
  string inicjal();
  Kanclerz& operator = (const Kanclerz&) = delete;
  ~Kanclerz() = default;
};

struct hash_pair{
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2> &p) const{
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

class Teren{
protected:
  char teren;
public:
  Teren();
  Teren(char);
  char jakiTeren() const;
  void zmienTeren(char);
  Teren& operator = (const Teren&) = default;
  Teren(const Teren&) = default;
  ~Teren() = default;
};

class Pole :public Teren{
  int licznik; //ilu rebeliantow widzi to pole
  Postac *p;
  Detektor d;
public:
  Pole() = delete;
  Pole(char, Postac* = nullptr, bool = false, int = 0);
  void zmienPostac(Postac*);
  void zwiekszLicznik();
  void zmniejszLicznik();
  int dajLicznik() const;
  void zmienDetektor();
  bool czyDetektor() const;
  Postac* jakaPostac() const;
  Pole& operator = (const Pole&) = default;
  Pole(const Pole&) = default;
  ~Pole() = default;
};

class Mapa{
  vector<vector<Pole>> plansza;
  int **kolory;
  int promien;
  int d;
  int s;
public:
  Mapa() = default;
  Mapa(int, int, int);
  void wstawPole(int, int, Pole&);
  void wstawPostac(int, int, Postac*);
  void usunPostac(int, int, Pole&);
  void wyswietl();
  void aktualizujDetektor(int, int);
  int dajDlugosc();
  int dajSzerokosc();
  Pole& dajPole(int, int);
  vector<vector<Pole>> dajPlansze();
  Mapa& operator = (const Mapa&) = default;
  Mapa(const Mapa&) = delete;
  ~Mapa();
};

class Gra{
  Mapa m;
  unordered_map <pair<int, int>, Postac*, hash_pair> postacie; //przechowanie postaci ktore sa na mapie z ich pozycjami
  int czas;
  string wygrany;
  bool czyDozwolony(pair<int,int>, Postac*, string);
  void przesun(pair<int,int>, int, int);
  bool czyOstatecznaWalka(pair<int, int>, int, int);
public:
  Gra(const Mapa&, int, string = "");
  void ruchGracza();
  void ruchKomputera();
  void wyswietl();
  string ktoWygral();
  bool koniecGry();
  int jakiCzas();
  Gra& operator = (const Gra&) = delete;
  Gra(const Gra&) = delete;
  ~Gra() = default;
};

#endif
