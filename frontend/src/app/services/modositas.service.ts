import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class ModositasService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000/futar/modositas/idoszak';  

  modosit(valami:any){
    return this.http.post<{Message: string}>(this.url, JSON.stringify(valami));
  }
}