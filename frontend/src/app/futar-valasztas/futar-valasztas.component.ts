import { RendelesService } from './../services/rendeles.service';
import { FutarService } from './../services/futar.service';
import { Input, Output } from '@angular/core';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { EventEmitter } from '@angular/core';

@Component({
  selector: 'app-futar-valasztas',
  templateUrl: './futar-valasztas.component.html',
  styleUrls: ['./futar-valasztas.component.css']
})
export class FutarValasztasComponent implements OnInit {

  @Input() rendelesID: number;

  @Output() changefutar = new EventEmitter();

  etteremId: number;
  futarok: {
    "FutarID": number
    "Knev": string,
    "Vnev": string
  }[];
  kivalasztottFutarId: number;
  nullfutar: number = null;

  constructor(private activatedRoute: ActivatedRoute, private futarService: FutarService, private rendelesService: RendelesService) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');

    });
    setTimeout(() => {
      console.log("Futar list. kedz");
      let adatok = { "RendelesID": this.rendelesID };
      futarService.futarokListazasa(adatok).subscribe(response => {
        console.log("futarok");
        console.log(response);
        this.futarok = response;
      });
      console.log("Futar list. vege");
    }, 3000);
    /*     let adatok = { "RendelesID": this.rendelesID };
          futarService.futarokListazasa(adatok).subscribe(response => {
            console.log("futarok");
            console.log(response);
            this.futarok = response;
          }) */

  }

  ngOnInit(): void {
  }

  kivalasztas() {
    let adatok = {
      "FutarID": this.kivalasztottFutarId,
      "RendelesID": this.rendelesID,
      "Allapot": "Kiszállításra vár"
    }
    console.log("kiválasztott futár:");
    console.log(adatok);
    this.rendelesService.futarHozzarendeles(adatok).subscribe(response => {
      console.log("válasz a futarHozzarendel()-tol:");
      console.log(response);
    })
    this.changefutar.emit("helo");
    //this.kivalasztottFutarId = null;
  }
}
