import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class RegisztracioService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000/';// ???

  
  etelHozzadas(hozzadas){
    //return this.http.post<{ID: number,Tipus: string}>(this.url+'/belepes', JSON.stringify(belepesAdatok));
    //return this.http.post(this.url+'/vendeg', JSON.stringify(regisztracio));
  }
  


}
