import { Etterem } from './../ettermek/model/etterem';
import { Kategoria } from './../etelek/model/kategoria';
import { EtelService } from './../services/etel.service';
import { FormControl, FormControlName, FormGroup, Validators } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { EttermekListazasaService } from './../services/ettermek-listazasa.service';
import { Component, OnInit } from '@angular/core';


@Component({
  selector: 'app-regisztracionelkul',
  templateUrl: './regisztracionelkul.component.html',
  styleUrls: ['./regisztracionelkul.component.css']
})
export class RegisztracionelkulComponent implements OnInit {

  // constructor(){}

  // ngOnInit(){}

  constructor(private service: EttermekListazasaService, private route: ActivatedRoute, private etelService: EtelService, private router: Router) { 
    etelService.getEtteremCimkek().subscribe(response => {
      this.etteremCimkek =  response.EtteremCimke;
    })
  }

  form = new FormGroup({
    irszam: new FormControl('',Validators.required),
    nev: new FormControl(),
    etteremCimke: new FormControl()
  });

  ettermek: Etterem[] = [];
  etteremCimkek: Kategoria[] = [];
  irszam: number;
  vendegId: number;
  etteremId: number;

  ngOnInit(): void {

    this.route.paramMap.subscribe(
      params => {
        //this.irszam = +params.get('irszam');
        console.log(params);
        /////////////////???????????????
        //this.vendegId = +params.get('vendegId');
      }
    );
  }

  submit(form) {
    let parameterek = {
      Irsz: this.form.get('irszam').value,
      Nev: this.form.get('nev').value,
      EtelCimke: [
      ],
      EtteremCimke: form.get('etteremCimke').value
    };
    // if(form.get('etteremCimke').value != null){
    //   parameterek.EtteremCimke.push(form.get('etteremCimke').value);
    // }

    console.log(parameterek);

    this.service.ettermekListazasa(parameterek).subscribe(response => {
      console.log(response);
      this.ettermek = response.Ettermek;
    })
  }


  // rendelInnen(id){
  //   ///vendeg',vendegId,'ettermek',etteremId,'etelek'
  //   this.router.navigate(['/vendeg', this.vendegId,'ettermek',id,'rendeles']);
  // }

}
