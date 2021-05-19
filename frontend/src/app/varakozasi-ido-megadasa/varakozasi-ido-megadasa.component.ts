import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from '../services/rendeles.service';

@Component({
  selector: 'app-varakozasi-ido-megadasa',
  templateUrl: './varakozasi-ido-megadasa.component.html',
  styleUrls: ['./varakozasi-ido-megadasa.component.css']
})
export class VarakozasiIdoMegadasaComponent implements OnInit {

  ngOnInit(): void {
  }

  @Input() rendelesID: number;

  @Output() changeido = new EventEmitter();

  etteremId: number;
  ido: number;

  constructor(private activatedRoute: ActivatedRoute,  private rendelesService: RendelesService) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('etteremId');

    });
  }

  ok(){
    console.log(this.ido);
    let adatok = {
      "RendelesID": this.rendelesID,
      "VarakozasiIdo": this.ido 
    }
    this.rendelesService.varakozasiIdoMegadas(adatok).subscribe(response => {
      console.log(response);
    })
    this.changeido.emit();
  }
}
