import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-etterem-rendelesek',
  templateUrl: './etterem-rendelesek.component.html',
  styleUrls: ['./etterem-rendelesek.component.css']
})
export class EtteremRendelesekComponent implements OnInit {

  etteremId: number;

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

}
