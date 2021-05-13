import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';

@Component({
  selector: 'app-etterem-teljesitettrendelesek',
  templateUrl: './etterem-teljesitettrendelesek.component.html',
  styleUrls: ['./etterem-teljesitettrendelesek.component.css']
})
export class EtteremTeljesitettrendelesekComponent implements OnInit {


  etteremId: number;

  //itt olyannak kéne megjelennie azoknak a rendeleseknek ahol az allapot : kész rendelések
  rendelesek: {
    "Allapot": Number,
    "Etelek": {"EtelID": number, "Mennyi":number}[],
    "FutarDij": number,
    "FutarID": number,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[];

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) {
    console.log(this.rendelesek);
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');
    })
   }

  

  ngOnInit(): void {
    this.rendelesService.etteremRendelesei(this.etteremId).subscribe( response => {
      console.log(response);
      this.rendelesek = response;
    })
  }

}
