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
  teljesitettRendelesek: {
    "Allapot": string,
      "Etelek": {"EtelID": number, "Mennyi":number}[],
      "FutarDij": number,
      "FutarID": number,
      "Prioritas": number,
      "RendelesID": number,
      "RendeloID": number,
      "SzallitasiMod": string,
      "VarakozasiIdo": number
  }[] = [];

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) {
    console.log(this.teljesitettRendelesek);
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');
    })
   }

  

  ngOnInit(): void {
    this.rendelesService.etteremRendelesei(this.etteremId).subscribe( response => {
      console.log(response);
      response.forEach(element => {
        if(element.Allapot=="Befejezett")
          this.teljesitettRendelesek.push(element);
      });
    })
  }

}
