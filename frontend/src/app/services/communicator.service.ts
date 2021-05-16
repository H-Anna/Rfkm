import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { BehaviorSubject } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class CommunicatorService {
  //ezzel próbálkoztam hogy ha a futár elfogad egy rendelést akkor az étteremnél frissüljön az oldal
  //de nem jött össze

  //innen van: https://stackoverflow.com/questions/63888794/how-to-refresh-a-component-from-another-in-angular
  //magamtól ilyet nem tudok lol

  /*constructor() { }

  private subjectName = new Subject<any>(); //need to create a subject

  sendUpdate(message: string) { //the component that wants to update something, calls this fn
    this.subjectName.next({ text: message }); //next() will feed the value in Subject
    console.log("in service");
    console.log(this.subjectName);
  }

  getUpdate(): Observable<any> { //the receiver component calls this function 
    console.log("in service2");
    return this.subjectName.asObservable(); //it returns as an observable to which the receiver funtion will subscribe
  }
  */

  //ez sajnos nem működik
}
