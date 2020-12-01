#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "klasy.h"

using namespace std;

unordered_map<string,int> read_params(string file_name) {
  ifstream fin(file_name);
  if(!fin.is_open()){
    cout << "Cannot open file\n";
    cout << "Terminate execution\n";
    exit(1);
  }
  unordered_map<string,int> m;
  string par_name;
  int par_value;
  while (fin >> par_name >> par_value)
    m[par_name] = par_value;

  return m;
}

void read_plansza(Mapa &m) {
  int a, b;
  char teren;
  ifstream fin("plansza.txt");
  if(!fin.is_open()){
    cout << "Cannot open file\n";
    cout << "Terminate execution\n";
    exit(1);
  }

  while (fin >> a >> b >> teren) {
    Pole * pol = new Pole(teren, nullptr);
    m.wstawPole(a, b, *pol);
  }
}

void wstaw_rebeliantow(Mapa &m, unordered_map<string, int> dict) {
  int d = dict["szerokosc"];
  int s = dict["dlugosc"];
  int liczba_zwykli = dict["ZwykliRebelianci_ilosc"];
  int liczba_programisci = dict["Programisci_ilosc"];
  int prev_reb_a, prev_reb_b, akt_reb_a, akt_reb_b;

  KsiezniczkaLeia *kl = new KsiezniczkaLeia(dict["Leia_szybkosc"]);
  LukeSkywalker *ls = new LukeSkywalker(dict["LukeSkywalker_szybkosc"]);
  m.wstawPostac(0, 0, kl);
  if (d > 1) {
    m.wstawPostac(0, 1, ls);
    prev_reb_a = 0, prev_reb_b = 1;
  }
  else {
    m.wstawPostac(1, 0, ls);
    prev_reb_a = 1; prev_reb_b = 0;
  }
  akt_reb_a = prev_reb_a; akt_reb_b  = prev_reb_b;
  for (int i = 0; i < liczba_programisci + liczba_zwykli; ++i) {
    akt_reb_a++, akt_reb_b--;
    if (akt_reb_a > d - 1 || akt_reb_b < 0) {
      akt_reb_b = ++prev_reb_b; akt_reb_a = prev_reb_a;
    }
    if (i < liczba_programisci) {
      Programista *pro = new Programista(dict["Programista_szybkosc"], i + 1);
      m.wstawPostac(akt_reb_a, akt_reb_b, pro);
    }
    else {
      ZwyklyRebeliant * zwykly = new ZwyklyRebeliant(dict["ZwyklyRebeliant_szybkosc"], i + 1 - liczba_programisci);
      m.wstawPostac(akt_reb_a, akt_reb_b, zwykly);
    }
  }
}

void wstaw_imperialistow(Mapa &m, unordered_map<string,int> dict) {
  int a, b, numer = 0;
  string kto;
  ifstream fin("imperialisci.txt");
  if(!fin.is_open()){
    cout << "Cannot open file\n";
    cout << "Terminate execution\n";
    exit(1);
  }
  Postac *p;
  while (fin >> a >> b >> kto) {
    if (kto == "kanclerz") {
      p = new Kanclerz();
    }
    else if (kto == "vader") {
      p = new LordVader(dict["LordVader_szybkosc"]);
    }
    else {
      p = new RobotZolnierz(dict["RobotZolnierz_szybkosc"], ++numer);
    }
    m.wstawPostac(a, b, p);
  }
}

int main() {
  unordered_map<string,int> dict = read_params("param.txt");
  Mapa m(dict["dlugosc"], dict["szerokosc"], dict["promien"]);
  read_plansza(m);
  wstaw_rebeliantow(m, dict);
  wstaw_imperialistow(m, dict);
  Gra g(m, dict["czas"]);
  g.wyswietl();
  while (!g.koniecGry()) {
    g.ruchGracza();
    g.ruchKomputera();
    cout << "Pozostaly czas to: " << g.jakiCzas() << endl;
  }
  cout << "Wygrana druzyna to: " << g.ktoWygral() << endl;
  return 0;
}
