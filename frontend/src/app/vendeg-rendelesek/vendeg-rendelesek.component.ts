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
    "Etelek": {"EtelID": number, "Mennyi":number}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[] = [];
  aktivRendelesek: {
    "Allapot": string,
    "Etelek": {"EtelID": number, "Mennyi":number}[],
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
    this.aktivRendelesek=[];
    this.rendelesService.vendegRendelesei(this.vendegId).subscribe( response => {
      console.log(response);
      response.forEach(element => {
        if(element.Allapot=="Fogadva"){
          this.fogadottRendelesek.push(element);
        }
        else {
          this.aktivRendelesek.push(element);
        }
      });
    })
  }

}
