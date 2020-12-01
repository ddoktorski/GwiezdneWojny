#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <typeinfo>
#include <stdlib.h>
#include <list>
#include "klasy.h"

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

static int przejscie[10][2] = {
  {0,0},{1,-1},{1,0},{1,1},{0,-1},{0,0},{0,1},{-1,-1},{-1,0},{-1,1}
};

Postac::Postac(int s): szybkosc(s){}

int Postac::jakaSzybkosc() {
  return szybkosc;
}

char Rebeliant::ktoryZespol() {
  return zespol;
}

char Imperialista::ktoryZespol() {
  return zespol;
}

Rebeliant::Rebeliant(int s): Postac(s), zespol('R'){}
Imperialista::Imperialista(int s): Postac(s), zespol('I'){}
ZwyklyRebeliant::ZwyklyRebeliant(int s, int n): numer(n), Rebeliant(s){}
KsiezniczkaLeia::KsiezniczkaLeia(int s): Rebeliant(s){}
Programista::Programista(int s, int n): numer(n), Rebeliant(s){}
LukeSkywalker::LukeSkywalker(int s): Rebeliant(s){}
RobotZolnierz::RobotZolnierz(int s, int n): numer(n), Imperialista(s){}
LordVader::LordVader(int s): Imperialista(s){}
Kanclerz::Kanclerz(): Imperialista(0){}
Detektor::Detektor(bool b): detekt(b){}

bool ZwyklyRebeliant::walka(Postac *p) {
    return false;
}

bool Programista::walka(Postac *p) {
  string s = p->inicjal();
  if(s[0] == 'Z') return true;
  else return false;
}

bool KsiezniczkaLeia::walka(Postac *p) {
  if(p->inicjal() == "KA") return true;
  return false;
}

bool LukeSkywalker::walka(Postac *p) {
  if(p->inicjal() == "LV") return true;
  else return false;
}

bool RobotZolnierz::walka(Postac *p) {
  return true;
}

bool LordVader::walka(Postac *p) {
  return true;
}

bool Kanclerz::walka(Postac *p) {
  return true;
}

string ZwyklyRebeliant::inicjal() {
  return "R" + to_string(numer);
}

string Programista::inicjal() {
  return "P" + to_string(numer);
}

string KsiezniczkaLeia::inicjal() {
  return "KL";
}

string LukeSkywalker::inicjal() {
  return "LS";
}

string RobotZolnierz::inicjal() {
  return "Z" + to_string(numer);
}

string LordVader::inicjal() {
  return "LV";
}

string Kanclerz::inicjal() {
  return "KA";
}

bool Detektor::czyDetektor() const {
  return detekt;
}

void Detektor::zmienDetektor(bool b) {
  detekt = b;
}

Teren::Teren(): teren('-'){}
Teren::Teren(char c): teren(c){}

char Teren::jakiTeren() const {
  return teren;
}

void Teren::zmienTeren(char c) {
  teren = c;
}

Pole::Pole(char t, Postac *post, bool _d, int licz):
  Teren(t), p(post), licznik(licz) {
    d = Detektor(_d);
  }

void Pole::zmienPostac(Postac *post) {
  p = post;
}

Postac* Pole::jakaPostac() const {
  return p;
}

bool Pole::czyDetektor() const {
  return d.czyDetektor();
}

void Pole::zmienDetektor() {
  if(d.czyDetektor()) d.zmienDetektor(false);
  else d.zmienDetektor(true);
}

void Pole::zwiekszLicznik() {
  licznik++;
}

void Pole::zmniejszLicznik() {
  licznik--;
}

int Pole::dajLicznik() const {
  return licznik;
}

Mapa::Mapa(int _d, int _s, int _p) {
  d = _d;
  s = _s;
  promien =_p;
  vector<Pole> pom;
  Teren t;
  Pole p('-');
  kolory = new int *[d];
  for(int i = 0; i < d; i++){
    kolory[i] = new int[s];
    for(int j = 0; j < s; j++){
        pom.push_back(p);
        kolory[i][j] = 0;
    }
    plansza.push_back(pom);
    pom.erase(pom.begin(), pom.end());
  }
}

//wstawianie postaci na pole jesli to rebeliant to aktualizujemy liczniki
void Mapa::wstawPostac(int a, int b, Postac *p) {
  if(p != nullptr && p->ktoryZespol() == 'R'){
      for(int i = max(0,a - promien); i <= a + promien; i++){
        for(int j = max(0,b - promien); j <= b + promien; j++){
          if (i < d && j < s) {
            plansza[i][j].zwiekszLicznik();
            kolory[i][j] = 1;
          }
        }
      }
  }
  plansza[a][b].zmienPostac(p);
}

void Mapa::wstawPole(int a, int b, Pole &p) {
  plansza[a][b] = p;
}

//analogicznie do wstawiania postaci tutaj zmniejszamy liczniki
void Mapa::usunPostac(int a, int b, Pole &pol) {
  if(pol.jakaPostac()->ktoryZespol() == 'R'){
    for(int i = max(0, a - promien); i <= a + promien; i++)
      for(int j = max(0, b - promien); j <= b + promien; j++){
        if (i < d && j < s) {
          plansza[i][j].zmniejszLicznik();
          if (plansza[i][j].dajLicznik() == 0){ //zaden rebeliant nie widzi pola wiec zmieniamy kolor
            kolory[i][j] = 2;
          }
        }
      }
  }
  pol.zmienPostac(nullptr);
}

//jezeli ktos dostawi detektor to zwiekszamy liczniki
//jezeli ktos usunie detektor to zmniejszamy
void Mapa::aktualizujDetektor(int a, int b) {
  for(int i = max(0,a - promien); i <= a + promien; i++)
    for(int j = max(0,b - promien); j <= b + promien; j++)
      if (i < d && j < s) {
        if(plansza[a][b].czyDetektor()){
          plansza[i][j].zwiekszLicznik();
          kolory[i][j] = 1;
        }
        else {
          plansza[i][j].zmniejszLicznik();
          if(plansza[i][j].dajLicznik() == 0){
            kolory[i][j] = 2;
          }
        }
      }
}

void Mapa::wyswietl() {
  int i = 0, j = 0;
  for(auto const &w: plansza) {
    cout << " | ";
    for(auto const &v: w) {
      switch(kolory[i][j]) {
        case 0: //teren nieznany
          cout << "xxx" << " | ";
          break;
        case 1: //teren widoczny
          if(v.czyDetektor()) {
            cout << "\e[1m" << GREEN << v.jakiTeren() << "\e[0m" << RESET ;
            if(v.jakaPostac()) cout << "\e[1m" << GREEN << v.jakaPostac()->inicjal() << RESET << "\e[0m" << " | ";
            else cout << "\e[1m" << GREEN << v.jakiTeren() << v.jakiTeren() << RESET << "\e[0m" << " | ";
            break;
          }
          else {
          cout << GREEN << v.jakiTeren() << RESET ;
          if(v.jakaPostac()) cout << GREEN << v.jakaPostac()->inicjal() << RESET << " | ";
          else cout << GREEN << v.jakiTeren() << v.jakiTeren() << RESET << " | ";
          break;
          }
        case 2: //teren odkryty ale aktualnie niewidoczny
          cout << RED << v.jakiTeren() << v.jakiTeren() << v.jakiTeren() << RESET << " | ";
          break;
        default:
          break;
      }
      j++;
    }
    cout << endl;
    j = 0;
    i++;
  }
}

Gra::Gra(const Mapa &_m, int _czas, string _wygrany) {
  m = _m;
  czas = _czas;
  wygrany = _wygrany;
  int i = 0, j = 0;
  for(auto const &w: m.dajPlansze()) {
    for(auto const &v: w){
      if(v.jakaPostac()){
        pair<int, int> pom(i,j);
        postacie[pom] = v.jakaPostac();
      }
      j++;
    }
    j = 0;
    i++;
  }
}

//sprawdzenie czy ktorys z warunkow zakonczenia gry zostal spelniony
//czy kanclerz zostal przekonany lub czy ksiezniczka leia zginela
bool Gra::czyOstatecznaWalka(pair<int, int> p, int a, int b) {
  Postac * kto = m.dajPole(p.first, p.second).jakaPostac();
  Postac * kogo = m.dajPole(a, b).jakaPostac();
  if(!(kto->inicjal() == "KL" || kogo->inicjal() == "KL")) return false;
  if(kto->inicjal() == "KL") {
    bool found = false;
    if(kogo->inicjal() == "KA")
      for(int i = max(0,a - 1); i <= a + 1; i++)
        for(int j = max(0,b - 1); j <= b + 1; j++)
          if(i < m.dajDlugosc() && j < m.dajSzerokosc() && m.dajPole(i, j).jakaPostac() && (m.dajPole(i, j).jakaPostac()->inicjal() == "LS"
            || m.dajPole(i, j).jakaPostac()->inicjal()[0] == 'P')) found = true;

    if(found) {
      m.usunPostac(a,b,m.dajPole(a, b));
      postacie.erase({a,b});
      przesun(p, a, b);
      wygrany = "R";
    }
    else {
      m.usunPostac(p.first,p.second,m.dajPole(p.first, p.second));
      postacie.erase(p);
      wygrany = "I";
    }
  }
  else {
      m.usunPostac(a,b,m.dajPole(a, b));
      postacie.erase({a,b});
      przesun(p, a, b);
      wygrany = "I";
  }
  return true;
}

//przesuniecie postaci, wstawienie/usuniecie detektora
void Gra::przesun(pair<int, int> p, int a, int b) {
  Postac *post = m.dajPole(p.first, p.second).jakaPostac();
  if(((post->inicjal()[0] == 'R' || post->inicjal() == "KL") && !(m.dajPole(a, b).czyDetektor()))
      || (post->ktoryZespol() == 'I' && m.dajPole(a, b).czyDetektor())) {
      m.dajPole(a, b).zmienDetektor();
      m.aktualizujDetektor(a, b);
  }
  if(post->inicjal() == "KL" || post->inicjal()[0] == 'R') {
    m.dajPole(a, b).zmienTeren('-');
  }
  postacie[{a, b}] = m.dajPole(p.first, p.second).jakaPostac();
  m.wstawPostac(a, b, postacie[p]);
  m.usunPostac(p.first, p.second, m.dajPole(p.first, p.second));
  postacie.erase(p);
}

//sprawdzamy czy ruch jest mozliwy
//jezeli jest to go wykonujemy
bool Gra::czyDozwolony(pair<int, int> p, Postac *post, string s) {
  int szyb = post->jakaSzybkosc();
  int x = p.first + przejscie[stoi(s.substr(2,3))][0] * szyb;
  int y = p.second + przejscie[stoi(s.substr(2,3))][1] * szyb;
  if(!(x >= 0 && x < m.dajDlugosc() && y >= 0 && y < m.dajSzerokosc()) ||
  ((s[0] == 'P' || s[0] == 'Z') && m.dajPole(x, y).jakiTeren() != '-')) {
    return false;
  }

  if(m.dajPole(x, y).jakaPostac() == nullptr) {
    przesun(p, x, y);
    return true;
  }
  else if(m.dajPole(x, y).jakaPostac()->ktoryZespol() != post->ktoryZespol()) {
    if(czyOstatecznaWalka(p, x, y)) return true;
    if(post->walka(m.dajPole(x, y).jakaPostac())) {
      m.usunPostac(x,y,m.dajPole(x, y));
      postacie.erase({x,y});
      przesun(p, x, y);
    }
    else {
      postacie.erase(p);
      m.usunPostac(p.first, p.second, m.dajPole(p.first, p.second));
    }
    return true;
  }
  return false;
}

void Gra::ruchGracza() {
  string s;
  bool found;
  unordered_map <pair<int, int>, Postac*, hash_pair> postacie_rebelianci;
  for(auto const &iter : postacie) {
    if(iter.second->ktoryZespol() == 'R')
      postacie_rebelianci[iter.first] = iter.second;
  }
  while(postacie_rebelianci.size()){
    if(koniecGry()) return;
    found = false;
    cout << "Wybierz postac i wykonaj ruch: ";
    if(!(cin >> s)){ //wczytanie znaku EOF powoduje zakonczenie gry
      cout << "EOF" << endl;
      czas = 0;
      return;
    }
    if(s.length() != 3 || !isdigit(s[2]) || s[2] == '0') {
      cout << "NIEPOPRAWNE WEJSCIE\n";
      continue;
    }
    for(auto const &it: postacie_rebelianci) {
      if(s.substr(0,2) == it.second->inicjal()) {
        found = true;
        if(czyDozwolony(it.first, it.second, s)) cout << "OK\n";
        else cout << "NIEDOZWOLONY RUCH\n";
        postacie_rebelianci.erase(it.first);
        break;
      }
    }
    if(!found) cout << "NIEPOPRAWNA LUB WYKORZYSTANA POSTAC\n";
    this->wyswietl();
  }
}

void Gra::ruchKomputera() {
  list<pair<int, int>> kolejnosc_postaci;
  for (auto const &iter : postacie){
    if (iter.second->ktoryZespol() == 'I')
      kolejnosc_postaci.push_back(iter.first);
  }
  for(auto const &skad : kolejnosc_postaci) {
    if (koniecGry()) return;
    int kierunek = rand() % 9 + 1;
    int liczba_prob = 3;
    string kto = postacie[skad]->inicjal();
    while (!czyDozwolony(skad, postacie[skad], postacie[skad]->inicjal() + to_string(kierunek)) && --liczba_prob) {
      kierunek = rand() % 9 + 1;
    }
    cout << "Ruch komputera: kto " << kto << " kierunek " << kierunek << endl;
    if (liczba_prob) cout << "OK\n";
    else cout << "NIEDOZWOLONY RUCH\n";
  }
  czas--;
  this->wyswietl();
}

void Gra::wyswietl() {
  m.wyswietl();
}

bool Gra::koniecGry() {
  return czas == 0 || wygrany != "";
}

string Gra::ktoWygral() {
  if (czas == 0 && wygrany == "") {
    wygrany = 'I';
  }
  return wygrany;
}

int Gra::jakiCzas() {
  return czas;
}

int Mapa::dajDlugosc() {
  return d;
}

int Mapa::dajSzerokosc() {
  return s;
}

Pole& Mapa::dajPole(int a, int b) {
  return plansza[a][b];
}

vector<vector<Pole>> Mapa::dajPlansze() {
  return plansza;
}

Mapa::~Mapa() {
  for(int i = 0; i < d; ++i) delete[] kolory[i];
  delete[] kolory;
}
