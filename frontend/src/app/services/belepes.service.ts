import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class BelepesService {

  constructor(private http: HttpClient) { }

  private url = 'http://localhost:3000';

  //  /belepes
  bejelentkezes(belepesAdatok){
    return this.http.post<{ID: number,Tipus: string}>(this.url+'/belepes', JSON.stringify(belepesAdatok));
  }
}
