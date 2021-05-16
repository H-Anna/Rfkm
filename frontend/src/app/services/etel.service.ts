import { switchMap } from 'rxjs/operators';
import { Injectable } from '@angular/core';
import { HttpClient, JsonpClientBackend } from '@angular/common/http';
import { Etel } from '../etelek/model/etel';
import { Kategoria } from '../etelek/model/kategoria';

@Injectable({
  providedIn: 'root'
})
export class EtelService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000';
  
  etelHozzadas(uj_etel){
    console.log('x');
    return this.http.post(this.url+'/etterem/etel',JSON.stringify(uj_etel));
  }

  //1.
  getEtelek(etteremId: number){
    return this.http.get<{Etelek: Etel[]}>(this.url+'/etterem/'+etteremId);
  }


  //2.
  getEtelCimkek(){    
    return this.http.get<{EtelCimke: {CimkeID: number, Nev: string, Tipus: string}[]}>(this.url+'/cimke/etel');
  }

  getEtteremCimkek(){
    return this.http.get<{EtteremCimke: Kategoria[]}>(this.url+'/cimke/etterem');
  }

  deleteEtel(etelID: number){
    return this.http.post(this.url+'/etterem/etel/torles/'+etelID,null);
  }
  
  kategoriaHozzadas(uj_kategoria){
    console.log('x');
    return this.http.post(this.url+'/cimke/etel',JSON.stringify(uj_kategoria));
  }

  kategoriaTorles(id: number){
    return this.http.post(this.url+'/cimke/etel/torles/'+id,null);
  }

  etelModositas(etelAdatok){
    return this.http.post(this.url+'/etterem/etel/modositas',JSON.stringify(etelAdatok));
  } 

  //3.
  akciokLekerese(){
    return this.http.get<{Akciok: {AkcioID: number, Ertek :number, Nev: string}[]}>(this.url+'/etterem/akcio');
  }

  rendelesModositas(adatok,id){
    return this.http.post<{"Message": string}>(this.url+'/etterem/rendeles/modositas',JSON.stringify(adatok)).pipe(
      switchMap(nemtudom => {
        console.log(nemtudom);
        return this.http.get<{
          "Allapot": string,
          "Etelek": {"EtelID": number, "Mennyi":number}[],
          "FutarID": number,
          "RendelesID": number,
          "SzallitasiMod": number,
          "VarakozasiIdo": number
        }[]>(this.url+'/etterem/rendelesek/'+id);
      })
    );
  }
  getEtelek2(etteremId: number){
    return this.http.get<{Etelek: Etel[]}>(this.url+'/etterem/'+etteremId).pipe(
      switchMap( idk => {
        console.log(idk);
        return this.http.get<{EtelCimke: {CimkeID: number, Nev: string, Tipus: string}[]}>(this.url+'/cimke/etel').pipe(
          switchMap( idk2 => {
            console.log(idk2);
            return this.http.get<{Akciok: {AkcioID: number, Ertek :number, Nev: string}[]}>(this.url+'/etterem/akcio');
          })
        );
      })
    )
  }

  getEtelek3(etteremId: number){
    return this.http.get<{Etelek: Etel[]}>(this.url+'/etterem/'+etteremId);
  }
}