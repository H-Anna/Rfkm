import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';

@Component({
  selector: 'app-futar-rendelesek',
  templateUrl: './futar-rendelesek.component.html',
  styleUrls: ['./futar-rendelesek.component.css']
})
export class FutarRendelesekComponent implements OnInit {

  futarId: number;

  rendelesek: {
    "Prioritas": number,
    "Leadasideje": string, 
    "SzallitasiCim": string,
    "RendelesID": number,
  }[];

  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute) {
    console.log(this.rendelesek);
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    })
   }

  ngOnInit(): void {
    this.rendelesService.futarRendelesei(this.futarId).subscribe( response => {
      console.log(response);
      //this.rendelesek = response;
    })
  }
  

}
