import { Kategoria } from './../etelek/model/kategoria';
import { EtelService } from './../services/etel.service';
import { FormGroup, FormControl } from '@angular/forms';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';


@Component({
  selector: 'app-kategoria-hozzaadas',
  templateUrl: './kategoria-hozzaadas.component.html',
  styleUrls: ['./kategoria-hozzaadas.component.css']
})
export class KategoriaHozzaadasComponent implements OnInit {

  constructor(private activatedRoute: ActivatedRoute,private etelService: EtelService) {
    this.kategoriakLekerese(); 
  }

  etteremId: number;
  categories: Kategoria[] = [];

  form = new FormGroup({
    kategoria: new FormControl()
  })

  get kategoria() {
    return this.form.get('kategoria');
  }


  ngOnInit(): void {
    this.activatedRoute.paramMap.subscribe(
      params => {
        this.etteremId = +params.get('etteremId');

      }
    );
    console.log(this.categories);  
  }

  kategoriakLekerese() {
    this.etelService.getEtelCimkek().subscribe(
      response => {
        this.categories = [];
        response.EtelCimke.forEach(element => {          
          if(element.Tipus=="Kategória"){
            this.categories.push(element);
          }
          
        });
       
      }
    )
  }

  kategoriaTorles(kategoria: Kategoria){
    this.etelService.kategoriaTorles(kategoria.CimkeID).subscribe( response => {
      console.log(response);
      this.kategoriakLekerese();
    });
  }

  
  submit(form:FormGroup){
    let hozzaadas = {

      Nev: form.get('kategoria').value,
  
      
    }

    this.etelService.kategoriaHozzadas(hozzaadas).subscribe(response => {
      console.log(response);
      this.kategoriakLekerese();
    });
  }

}