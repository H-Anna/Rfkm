import { Rendeles } from './../vendeg-home/rendeles';
import { CommunicatorService } from './../services/communicator.service';
import { FutarService } from './../services/futar.service';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';
import { Component, OnInit } from '@angular/core';

import { interval, fromEvent, Subscription } from 'rxjs';
import { switchMap } from 'rxjs/operators';

@Component({
  selector: 'app-etterem-rendelesek',
  templateUrl: './etterem-rendelesek.component.html',
  styleUrls: ['./etterem-rendelesek.component.css']
})
export class EtteremRendelesekComponent implements OnInit {

  etteremId: number;
  kiszallitasraVaroRendelesekSzama: number = 0;
  fogadottRendelesekSzama: number = 0;

  fogadottRendelesek: {
    "Allapot": string,
    "Etelek": { "EtelID": number, "Mennyi": number }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];

  keszitesAlattRendelesek: {
    "Allapot": string,
    "Etelek": { "EtelID": number, "Mennyi": number }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];

  kiszallitasraVaroRendelesek: {
    "Allapot": string,
    "Etelek": { "EtelID": number, "Mennyi": number }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];

  kiszallitasAlattRendelesek: {
    "Allapot": string,
    "Etelek": { "EtelID": number, "Mennyi": number }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];

  befejezettRendelesek: {
    "Allapot": string,
    "Etelek": { "EtelID": number, "Mennyi": number }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];



  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute, private futarService: FutarService, private commService: CommunicatorService) {
    console.log(this.futarKivalasztvaTomb);
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');
    })
    this.futarService.futarokListazasa().subscribe(response => {
      this.futarok = response;
      //console.log(response);
      console.log(this.futarok);
    })
  }



  futarok: { "Nev": string, "FutarID": number }[] = [];
  nullFutar = { "Nev": null, "FutarID": null };
  futarKivalasztva: boolean = false;
  futarKivalasztvaTomb: boolean[] = [];
  selectedFutarTomb: { "Nev": string, "FutarID": number }[] = [];
  selectedFutar: { "Nev": string, "FutarID": number };
  onFutarChange(futarObj,RendelesID) {
    this.selectedFutar = futarObj;
    this.selectedFutarTomb[RendelesID] = futarObj;
    
    if (futarObj != "" ) {
      this.futarKivalasztvaTomb[RendelesID] = false;
    }
    else {
      this.futarKivalasztvaTomb[RendelesID] = true;
    }
    console.log(this.selectedFutarTomb);
    console.log(this.futarKivalasztvaTomb);
  }

  selectedVarIdoTomb: number[] = [];
  varIdoKivalasztvaTomb: boolean[] = [];
  onVarIdoChange(event,RendelesID){
    if(this.selectedVarIdoTomb[RendelesID] != null ){
     // varIdoKivalasztvaTomb[RendelesID] = true;
    }
    console.log(this.selectedVarIdoTomb);
    console.log(this.varIdoKivalasztvaTomb);
  }

  ujratoltes(response) {
    this.fogadottRendelesek = [];
    this.keszitesAlattRendelesek = [];
    this.kiszallitasraVaroRendelesek = [];
    this.kiszallitasAlattRendelesek = [];
    this.befejezettRendelesek = [];
    this.kiszallitasraVaroRendelesekSzama=0;
    response.forEach(element => {
      if (element.Allapot == 'Fogadva' && element.SzallitasiMod!="Átvétel") {
        this.fogadottRendelesek.push(element);
      }
      else if (element.Allapot == 'Elkészítés alatt' && element.SzallitasiMod!="Átvétel") {
        this.keszitesAlattRendelesek.push(element);
      }
      else if (element.Allapot == 'Kiszállításra vár' && element.SzallitasiMod!="Átvétel") {
        this.kiszallitasraVaroRendelesek.push(element);
        this.kiszallitasraVaroRendelesekSzama++;
        this.selectedFutarTomb[element.RendelesID] = null;
        this.futarKivalasztvaTomb[element.RendelesID] = false;
      }
      else if (element.Allapot == 'Kiszállítás alatt' && element.SzallitasiMod!="Átvétel") {
        this.kiszallitasAlattRendelesek.push(element);
      }
      else if (element.Allapot == 'Befejezett' && element.SzallitasiMod!="Átvétel") {
        this.befejezettRendelesek.push(element);
      }
    });
    console.log(this.selectedFutarTomb);
    console.log(this.futarKivalasztvaTomb);
  }

  futarokk(futarSzam:number){ //ennek kéne vmi jobb név
    for (let index = 0; index < futarSzam; index++) {
      
      
    }
  }


  ngOnInit(): void {    
    this.rendelesService.etteremRendelesei(this.etteremId).subscribe(response => {
      this.ujratoltes(response);
      this.futarokk(this.kiszallitasraVaroRendelesekSzama);
    });
  }

  

  /*
  példa:
  [
    {
        "Allapot": 0,
        "Etelek": [
            {
                "EtelID": 4,
                "Mennyi": 10
            }
        ],
        "FutarDij": 0,
        "FutarID": 0,
        "Prioritas": 1,
        "RendelesID": 3,
        "RendeloID": 2,
        "SzallitasiMod": 0,
        "VarakozasiIdo": 120
    }
  ]
   
  */

  // EZT még nem tudom hogy lesz!
  rendelesModositas(item) {
    console.log(item);
    /*this.rendelesService.rendelesModositas(item).subscribe( response => {
      console.log(response);
    })*/
  }


  elkeszitesElkezdese(item, id = this.etteremId) {
    let adatok = {
      "RendelesID": item.RendelesID,
      "Allapot": "Elkészítés alatt"
    }
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log("Response 1");
      console.log(response);
      this.ujratoltes(response);
    })

  }


  elkeszitve(item, id = this.etteremId) {  //ez ilyen default paraméter
    let adatok = {
      "RendelesID": item.RendelesID,
      "Allapot": "Kiszállításra vár"
    }
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    })

  }


  futarnakAtadva(item, id = this.etteremId) {
    let adatok = {
      "RendelesID": item.RendelesID,
      "FutarID": this.selectedFutarTomb[item.RendelesID].FutarID,
      "Allapot": "Kiszállítás alatt"
    }
    console.log(adatok);
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    })

  }



}
