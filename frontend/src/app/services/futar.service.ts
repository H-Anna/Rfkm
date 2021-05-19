import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class FutarService {

  private url = 'http://localhost:3000';

  constructor(private http: HttpClient) { }

  reszesedesModositasa(reszesedes) {
    return this.http.post<{ Message: string }>(this.url + '/etterem/futarreszesedes/modositas', JSON.stringify(reszesedes));
  }

  futarokListazasa(adatok) {
    return this.http.post<{ "Knev": string, "Vnev": string, "FutarID": number }[]>(this.url + '/futarok', JSON.stringify(adatok));
  }

  futarReszesedese(id) {
    return this.http.get<{
      "Osszesen": number,
      "Rendelesek": {
        "RendelesID": number,
        "FutarDij": number
      }[]
    }>(this.url + '/futar/reszesedes/' + id);
  }

}
