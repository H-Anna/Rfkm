import { element } from 'protractor';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from '../services/rendeles.service';

@Component({
  selector: 'app-vendeg-rendelesek',
  templateUrl: './vendeg-rendelesek.component.html',
  styleUrls: ['./vendeg-rendelesek.component.css']
})
export class VendegRendelesekComponent implements OnInit {

  vendegId: number;

  fogadottRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  keszitesAlattRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  szallitasraVaroRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  szallitasAlattRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  elvihetoRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  befejezettRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number, "Nev": string}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) { 
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.vendegId = +params.get('vendegId');
    })
  }

  ngOnInit(): void {
    this.fogadottRendelesek=[];
    this.keszitesAlattRendelesek=[];
    this.szallitasraVaroRendelesek=[];
    this.szallitasAlattRendelesek=[];
    this.elvihetoRendelesek=[];
    this.befejezettRendelesek=[];
    this.rendelesService.vendegRendelesei(this.vendegId).subscribe( response => {
      console.log(response);
      response.forEach(element => {
        if(element.Allapot=="Fogadva"){
          this.fogadottRendelesek.push(element);
        }
        else if (element.Allapot=="Elkészítés alatt") {
          this.keszitesAlattRendelesek.push(element);
        }
        else if (element.Allapot=="Kiszállításra vár") {
          this.szallitasraVaroRendelesek.push(element);
        }
        else if (element.Allapot=="Kiszállítás alatt") {
          this.szallitasAlattRendelesek.push(element);
        }
        else if (element.Allapot=="Elvitelre vár") {
          this.elvihetoRendelesek.push(element);
        }
        else if (element.Allapot=="Befejezett") {
          this.befejezettRendelesek.push(element);
        }
      });
    })
  }

}
