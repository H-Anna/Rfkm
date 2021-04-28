import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class FutarService {

  private url = 'http://localhost:3000';

  constructor(private http: HttpClient) { }

  reszesedesModositasa(reszesedes){
    return this.http.post<{Message: string}>(this.url+'/etterem/futarreszesedes/modositas',JSON.stringify(reszesedes));
  }
}
