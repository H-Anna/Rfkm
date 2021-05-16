import { CommunicatorService } from './../services/communicator.service';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from './../services/rendeles.service';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-futar-rendelesek',
  templateUrl: './futar-rendelesek.component.html',
  styleUrls: ['./futar-rendelesek.component.css']
})
export class FutarRendelesekComponent implements OnInit {

  futarId: number;

  beerkezettRendelesek: {
    "Cim": string,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number
    //"Allapot": string    //ez még nincs backenden = "Kiszállításra vár"
  }[] = []/* [   //tesztelés miatt feltöltöm
    {
      "Cim": "cim1",
      "Prioritas": 1,
      "RendelesID": 1,
      "RendeloID": 2,
      "Allapot": "Kiszállításra vár"
    },
    {
      "Cim": "cim1",
      "Prioritas": 1,
      "RendelesID": 2,
      "RendeloID": 2,
      "Allapot": "Kiszállításra vár"
    }
  ];*/
  aktivRendelesek: {
    "Cim": string,
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number
    //"Allapot": string   //ez még nincs de lesz = "Kiszállítás alatt"
  }[] = [];


  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute, private  commService: CommunicatorService) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    })
   }

  ngOnInit(): void {
    this.rendelesService.futarRendelesei(this.futarId).subscribe( response => {
      console.log(response);
      this.beerkezettRendelesek = response;
     /* response.forEach(element => {
        if(element.Allapot=="Kiszállításra vár"){
          this.beerkezettRendelesek.push(element);
        }
        else if(element.Allapot=="Kiszállítás alatt"){
          this.aktivRendelesek.push(element);
        }
      });*/
    })
    
  }

  rendelesElfogadasa(){
  }

  rendelesElutasitasa(){
  }
  

}
