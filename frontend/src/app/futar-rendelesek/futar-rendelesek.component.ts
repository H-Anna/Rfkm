import { element } from 'protractor';
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


  rendelesek: {
    "Allapot": string,
    "Cim": {
      "Emelet_ajto": string,
      "Hazszam": number,
      "Irsz": number,
      "Kozterulet": string
    },
    "Prioritas": number,
    "RendelesID": number,
    "RendeloID": number,
    "TimeStamp": string
  }[] = [];


  constructor(private rendelesService: RendelesService, private activatedRoute: ActivatedRoute, private commService: CommunicatorService) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    })
  }

  ngOnInit(): void {
    this.rendelesService.futarRendelesei(this.futarId).subscribe(response => {
      console.log("riszponz");
      console.log(response);
      this.ujratoltes(response);
      /*response.forEach(element => {
        if (element.Allapot == "Kiszállításra vár") {
          this.aktivRendelesek.push(element);
        }
        else if (element.Allapot == "Kiszállítás alatt") {
          this.aktivRendelesek.push(element);
        }
      });*/
    })

  }


  ujratoltes(response) {
    this.rendelesek = [];
    response.forEach(element => {
      if(element.Allapot=="Kiszállításra vár" || element.Allapot=="Kiszállítás alatt"){
        this.rendelesek.push(element);
      }      
      //this.rendelesek.sort(this.sortByPrioritas(element.Prioritas));
    })
    this.rendelesek.sort(function (a,b){
      return a.Prioritas - b.Prioritas;
    })
    

  }

  /*sortByPrioritas(prio){  
    return function(a,b){  
       if(a[prio] > b[prio])  
          return 1;  
       else if(a[prio] < b[prio])  
          return -1;  
   
       return 0;  
    }  
 }*/

  rendelesElfogadasa(rendelesId) {
    let adatok = {
      "RendelesID": rendelesId,
      "Allapot": "Kiszállítás alatt"
    };
    this.rendelesService.allapotModositas(adatok, this.futarId).subscribe(response => {
      console.log(response);
      this.ujratoltes(response);
    });
  }

  rendelesElutasitasa(rendelesId) {
    let adatok = {
      "FutarID": this.futarId,
      "RendelesID": rendelesId
    };
    this.rendelesService.rendelesElutasitasaEsUjratoltes(adatok, this.futarId).subscribe(response => {
      this.ujratoltes(response);
    })

  }

  kezbesitve(rendelesId) {
    let adatok = {
      "RendelesID": rendelesId
    };
    this.rendelesService.rendelesKeszEsUjratoltes(adatok, this.futarId).subscribe(response => {
      this.ujratoltes(response);
    })
  }

  //------------------------------------------------------------------------------------------------



  ujprioritas: number;


  //         "Allapot": "Kiszállításra vár",
  //         "Cim": {
  //             "Emelet_ajto": "",
  //             "Hazszam": 1,
  //             "Irsz": 8150,
  //             "Kozterulet": "Fő utca"
  //         },
  //         "Prioritas": 1,
  //         "RendelesID": 2,
  //         "RendeloID": 2,
  //         "TimeStamp": "2021-05-14 20:28:23"








  prioritasModositas(item, id = this.futarId) {
    let adatok = {
      "RendelesID": item.RendelesID,
      "Prioritas": this.ujprioritas,
      "FutarID": this.futarId,
    }
    console.log(adatok);

    this.rendelesService.prioritasModositas(adatok, id).subscribe(response => {
      console.log("Response 1");
      console.log(response);
      this.ujratoltes(response);
    })

  }





}

// Rendelés prioritás módosítása
// Post: http://localhost:3000/futar/prioritas
// {
//     "FutarID": 1,
//     "RendelesID": 4,
//     "Prioritas": 2
// }


// Rendelés elutasítása
// Post: http://localhost:3000/futar/elutasit
// {
//     "FutarID": 1,
//     "RendelesID": 3
// }


// Rendelés befejezése
// Post: http://localhost:3000/futar/kesz
// {
//     "RendelesID": 3
// }

// Rendelések lekérése futáronként
// GET: http://localhost:3000/futar/rendelesek/ID
// Visszakapja: 
// [
//     {
//         "Allapot": "Kiszállításra vár",
//         "Cim": {
//             "Emelet_ajto": "",
//             "Hazszam": 1,
//             "Irsz": 8150,
//             "Kozterulet": "Fő utca"
//         },
//         "Prioritas": 1,
//         "RendelesID": 2,
//         "RendeloID": 2,
//         "TimeStamp": "2021-05-14 20:28:23"
//     },
//     {
//         "Allapot": "Befejezett",
//         "Cim": {
//             "Emelet_ajto": "",
//             "Hazszam": 1,
//             "Irsz": 8150,
//             "Kozterulet": "Fő utca"
//         },
//         "Prioritas": 2,
//         "RendelesID": 3,
//         "RendeloID": 2,
//         "TimeStamp": "2021-04-27 15:50:36"
//     }
// ]



