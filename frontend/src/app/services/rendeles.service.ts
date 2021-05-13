import { HttpClient } from '@angular/common/http';
import { Rendeles } from './../vendeg-home/rendeles';
import { Injectable } from '@angular/core';

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
      "Allapot": Number,
      "Etelek": {"EtelID": number, "Mennyi":number}[],
      "FutarDij": number,
      "FutarID": number,
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number,
      "SzallitasiMod": number,
      "VarakozasiIdo": number
    }[]>(this.url+'/etterem/rendelesek/'+id);
  }

  //  /vendeg/rendelesek/vendegId
  vendegRendelesei(id){
    return this.http.get<{
      "Allapot":number,
      "Etelek": {"EtelID": number, "Mennyi":number}[],
      "FutarID": number,
      "RendelesID": number,
      "SzallitasiMod": number,
      "VarakozasiIdo": number
    }[]>(this.url+'/vendeg/rendelesek/'+id);

  }

  //TODO
  futarRendelesei(id){
    return this.http.get<{
      // "Allapot":number,
      // "Etelek": {"EtelID": number, "Mennyi":number}[],
      // "FutarID": number,
      // "RendelesID": number,
      // "SzallitasiMod": number,
      // "VarakozasiIdo": number
    }[]>(this.url+'/futar/rendelesek/'+id);

  }


}
