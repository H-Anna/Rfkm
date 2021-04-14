import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Etterem } from '../ettermek/model/etterem';

@Injectable({
  providedIn: 'root'
})
export class EttermekListazasaService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000';

  ettermekListazasa(parameterek){
    return this.http.post<{Ettermek: Etterem[]}>(this.url+'/etterem',JSON.stringify(parameterek));
  }
}
