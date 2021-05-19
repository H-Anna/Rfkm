import { HttpClient } from '@angular/common/http';
import { Rendeles } from './../vendeg-home/rendeles';
import { Injectable } from '@angular/core';

import { interval, fromEvent } from 'rxjs';
import { catchError, switchMap } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class RendelesService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000';

  //  /vendeg/rendeles
  rendelesElkuldese(rendeles) {
    return this.http.post<{ Message: string }>(this.url + '/vendeg/rendeles', JSON.stringify(rendeles));
  }



  // /etterem/rendelesek/etteremId
  etteremRendelesei(id) {
    return this.http.get<{
      "Allapot": string,
      "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string }[],
      "FutarDij": number,
      "FutarID": number,
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number,
      "SzallitasiMod": string,
      "VarakozasiIdo": number
    }[]>(this.url + '/etterem/rendelesek/' + id);
  }

  //  /vendeg/rendelesek/vendegId
  vendegRendelesei(id) {
    return this.http.get<{
      "Allapot": string,
      "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string }[],
      "FutarID": number,
      "RendelesID": number,
      "SzallitasiMod": number,
      "VarakozasiIdo": number
    }[]>(this.url + '/vendeg/rendelesek/' + id);

  }

  //  /futar/rendelesek/futarId
  futarRendelesei(id) {
    return this.http.get<{
      "Allapot": string,
      "Cim": {
        "Emelet_ajto": string,
        "Hazszam": number,
        "Irsz": number,
        "Kozterulet": string
      },
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number,
      "TimeStamp": string
    }[]>(this.url + '/futar/rendelesek/' + id);

  }

  //  /etterem/rendeles/modositas
  rendelesModositas(adatok, id) {
    return this.http.post<{ "Message": string }>(this.url + '/etterem/rendeles/modositas', JSON.stringify(adatok)).pipe(
      switchMap(nemtudom => {
        console.log(nemtudom);
        return this.http.get<{
          "Allapot": string,
          "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string }[],
          "FutarID": number,
          "RendelesID": number,
          "SzallitasiMod": number,
          "VarakozasiIdo": number
        }[]>(this.url + '/etterem/rendelesek/' + id);
      })
    );
  }

  futarHozzarendeles(adatok) {
    return this.http.post<{ "Message": string }>(this.url + '/etterem/rendeles/modositas', JSON.stringify(adatok));
  }

  varakozasiIdoMegadas(adatok) {
    return this.http.post<{ "Message": string }>(this.url + '/etterem/rendeles/modositas', JSON.stringify(adatok));
  }

  allapotModositas(adatok, futarId) {
    return this.http.post(this.url + '/etterem/rendeles/modositas', JSON.stringify(adatok)).pipe(
      switchMap(idk => {
        return this.http.get<{
          "Allapot": string,
          "Cim": {
            "Emelet_ajto": string,
            "Hazszam": number,
            "Irsz": number,
            "Kozterulet": string
          },
          "Prioritas": number,
          "RendelesID": number,
          "RendeloID": number,
          "TimeStamp": string
        }[]>(this.url + '/futar/rendelesek/' + futarId);
      })
    );
  }

  rendelesElutasitasaEsUjratoltes(adatok, futarId) {
    return this.http.post(this.url + '/futar/elutasit', JSON.stringify(adatok)).pipe(
      switchMap(idk => {
        return this.http.get<{
          "Allapot": string,
          "Cim": {
            "Emelet_ajto": string,
            "Hazszam": number,
            "Irsz": number,
            "Kozterulet": string
          },
          "Prioritas": number,
          "RendelesID": number,
          "RendeloID": number,
          "TimeStamp": string
        }[]>(this.url + '/futar/rendelesek/' + futarId);
      })
    );
  }

  rendelesKeszEsUjratoltes(adatok, futarId) {
    return this.http.post(this.url + '/futar/kesz', JSON.stringify(adatok)).pipe(
      switchMap(idk => {
        return this.http.get<{
          "Allapot": string,
          "Cim": {
            "Emelet_ajto": string,
            "Hazszam": number,
            "Irsz": number,
            "Kozterulet": string
          },
          "Prioritas": number,
          "RendelesID": number,
          "RendeloID": number,
          "TimeStamp": string
        }[]>(this.url + '/futar/rendelesek/' + futarId);
      })
    );
  }


  //----------------------------------------------------------------------------------------------------------------------

  prioritasModositas(adatok, id) {
    return this.http.post<{ "Message": string }>(this.url + '/futar/prioritas', JSON.stringify(adatok)).pipe(
      switchMap(nemtudom => {
        console.log(nemtudom);
        return this.http.get<{
          "Allapot": string,
          "Cim": {
            "Emelet_ajto": string,
            "Hazszam": number,
            "Irsz": number,
            "Kozterulet": string
          },
          "Prioritas": number,
          "RendelesID": number,
          "RendeloID": number,
          "TimeStamp": string
        }[]>(this.url + '/futar/rendelesek/' + id);
      })
    );
  }
  // Rendelés prioritás módosítása
  // Post: http://localhost:3000/futar/prioritas
  // {
  //     "FutarID": 1,
  //     "RendelesID": 4,
  //     "Prioritas": 2
  // }






}
