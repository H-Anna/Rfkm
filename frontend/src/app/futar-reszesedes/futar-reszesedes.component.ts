import { FutarService } from './../services/futar.service';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { RendelesService } from '../services/rendeles.service';

@Component({
  selector: 'app-futar-reszesedes',
  templateUrl: './futar-reszesedes.component.html',
  styleUrls: ['./futar-reszesedes.component.css']
})
export class FutarReszesedesComponent implements OnInit {

  futarId: number;

  reszesedesek: {
    "Osszesen": number,
    "Rendelesek": {
      "RendelesID": number,
      "FutarDij": number
    }[]
  } = {
    "Osszesen": null,
    "Rendelesek": []
  };
  
  constructor(private futarService: FutarService, private activatedRoute: ActivatedRoute) {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    });
    this.futarService.futarReszesedese(this.futarId).subscribe( response => {
      console.log(response);
      this.reszesedesek = response;
    })
  }

  ngOnInit(): void {
  }

}
