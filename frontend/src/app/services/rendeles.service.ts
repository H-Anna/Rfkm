import { HttpClient } from '@angular/common/http';
import { Rendeles } from './../vendeg-home/rendeles';
import { Injectable } from '@angular/core';

import { interval, fromEvent } from 'rxjs';
import { catchError, switchMap } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class RendelesService {

  constructor(private http:HttpClient) { }

  private url = 'http://localhost:3000';

  //  /vendeg/rendeles
  rendelesElkuldese(rendeles){
    return this.http.post<{Message:string}>(this.url +'/vendeg/rendeles', JSON.stringify(rendeles));
  }



  // /etterem/rendelesek/etteremId
  etteremRendelesei(id){
    return this.http.get<{
      "Allapot": string,
      "Etelek": {"EtelID": number, "Mennyi":number}[],
      "FutarDij": number,
      "FutarID": number,
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number,
      "SzallitasiMod": string,
      "VarakozasiIdo": number
    }[]>(this.url+'/etterem/rendelesek/'+id);
  }

  //  /vendeg/rendelesek/vendegId
  vendegRendelesei(id){
    return this.http.get<{
      "Allapot": string,
      "Etelek": {"EtelID": number, "Mennyi":number}[],
      "FutarID": number,
      "RendelesID": number,
      "SzallitasiMod": number,
      "VarakozasiIdo": number
    }[]>(this.url+'/vendeg/rendelesek/'+id);

  }

  //  /futar/rendelesek/futarId
  futarRendelesei(id){
    return this.http.get<{
      "Cim": string,
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number
      //"Allapot": string   //ez még nincs de lesz
    }[]>(this.url+'/futar/rendelesek/'+id);

  }

  //  /etterem/rendeles/modositas
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


}
