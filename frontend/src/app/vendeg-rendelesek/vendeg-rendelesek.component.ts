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

  rendelesek: {
    "Allapot": Number,
    "Etelek": {"EtelID": number, "Mennyi":number}[],
    "FutarID": number,
    "RendelesID": number,
    "SzallitasiMod": number,
    "VarakozasiIdo": number
  }[];

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) { 
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.vendegId = +params.get('vendegId');
    })
  }

  ngOnInit(): void {
    this.rendelesService.vendegRendelesei(this.vendegId).subscribe( response => {
      console.log(response);
      this.rendelesek = response;
    })
  }

}
