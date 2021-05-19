import { element } from 'protractor';
import { Rendeles } from './../vendeg-home/rendeles';
import { CommunicatorService } from './../services/communicator.service';
import { FutarService } from './../services/futar.service';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';
import { Component, OnInit } from '@angular/core';

import { interval, fromEvent, Subscription } from 'rxjs';
import { switchMap } from 'rxjs/operators';
import { utf8Encode } from '@angular/compiler/src/util';

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
    "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string }[],
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
    "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string  }[],
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
    "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string  }[],
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
    "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string  }[],
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
    "Etelek": { "EtelID": number, "Mennyi": number, "Nev": string  }[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": string,
    "VarakozasiIdo": number
  }[] = [];




  
  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');
    })
   
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
        ;
      }
      else if (element.Allapot == 'Kiszállítás alatt' && element.SzallitasiMod!="Átvétel") {
        this.kiszallitasAlattRendelesek.push(element);
      }
      else if (element.Allapot == 'Befejezett' && element.SzallitasiMod!="Átvétel") {
        this.befejezettRendelesek.push(element);
      }
    });
  }




  ngOnInit(): void {    
    this.rendelesService.etteremRendelesei(this.etteremId).subscribe(response => {
      this.ujratoltes(response);
    });
  }

  futarChanged(event, item){
    //console.log(eventargs);
    //ide lehetne hogy output evemtbe visszaadja a futár id-ját és ha 0 akkor false
    //ha nem akkor true és ehez bind-olni a gomb disabled-jét
    console.log('2');
    console.log(item);
    /*this.rendelesService.etteremRendelesei(this.etteremId).subscribe(response => {
      this.ujratoltes(response);
    });*/
    console.log('2');
    console.log(item);
    
  }

  idoChanged(){
    
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
      "Allapot": "Kiszállítás alatt"
    }
    console.log(adatok);
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    })

  }



}
