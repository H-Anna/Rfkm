import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from '../services/rendeles.service';

@Component({
  selector: 'app-etterem-rendelesek-elvitelre',
  templateUrl: './etterem-rendelesek-elvitelre.component.html',
  styleUrls: ['./etterem-rendelesek-elvitelre.component.css']
})
export class EtteremRendelesekElvitelreComponent implements OnInit {

  etteremId: number;

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

  elvitelreVaroRendelesek: {
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

  

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) { 
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');
    })
  }

  ngOnInit(): void {
    this.rendelesService.etteremRendelesei(this.etteremId).subscribe(response => {
      this.ujratoltes(response);
    });
  }

  ujratoltes(response) {
    this.fogadottRendelesek = [];
    this.keszitesAlattRendelesek = [];
    this.elvitelreVaroRendelesek = [];
    this.befejezettRendelesek = [];
    response.forEach(element => {
      if (element.Allapot == 'Fogadva' && element.SzallitasiMod=="Átvétel") {
        this.fogadottRendelesek.push(element);
      }
      else if (element.Allapot == 'Elkészítés alatt' && element.SzallitasiMod=="Átvétel") {
        this.keszitesAlattRendelesek.push(element);
      }
      else if (element.Allapot == 'Elvitelre vár' && element.SzallitasiMod=="Átvétel") {
        this.elvitelreVaroRendelesek.push(element);
      }
      else if (element.Allapot == 'Befejezett' && element.SzallitasiMod=="Átvétel") {
        this.befejezettRendelesek.push(element);
      }
    })
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
      "Allapot": "Elvitelre vár"
    }
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    })

  }

  felveve(item, id=this.etteremId){
    let adatok = {
      "RendelesID": item.RendelesID,
      "Allapot": "Befejezett"
    }
    this.rendelesService.rendelesModositas(adatok, id).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    })
  }



  

}
