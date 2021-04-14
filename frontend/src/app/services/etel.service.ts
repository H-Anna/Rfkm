import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
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

  getEtelek(etteremId: number){
    return this.http.get<{Etelek: Etel[]}>(this.url+'/etterem/'+etteremId);
  }


  getEtelCimkek(){    
    return this.http.get<{EtelCimke: Kategoria[]}>(this.url+'/cimke/etel');
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
}