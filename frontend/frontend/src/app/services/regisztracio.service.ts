import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class RegisztracioService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000/regisztracio';

  //  /regisztracio/etterem
  etteremRegisztral(regisztracio){
    return this.http.post(this.url+'/etterem', JSON.stringify(regisztracio));
  }

  //  /helloworld
  hw(){
    return this.http.get(this.url+'/helloworld');
  }

   //  /regisztracio/vendeg
   vendegRegisztral(regisztracio){
    return this.http.post(this.url+'/vendeg', JSON.stringify(regisztracio));
  }


}
