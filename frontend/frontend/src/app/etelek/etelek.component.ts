import { ETELEK } from './etelek.storage';
import { Etel } from './model/etel';
import { Component, OnInit } from '@angular/core';
import { FormGroup, FormControl, FormArray, AbstractControl, Validators } from '@angular/forms';

@Component({
  selector: 'app-etelek',
  templateUrl: './etelek.component.html',
  styleUrls: ['./etelek.component.css']
})
export class EtelekComponent implements OnInit {
  service: any;

  constructor() { }

  etelek: Etel[];

  ngOnInit(): void {
    this.etelek = ETELEK;
  }
  form = new FormGroup({
    
    nev: new FormControl('',Validators.required),
    
    ar: new FormControl('',Validators.required),
    leiras: new FormControl('',Validators.required),
  
  

});


get nev(){
  return this.form.get('nev');
}
get leiras(){
  return this.form.get('leiras');
}
get ar(){
  return this.form.get('ar');
}

submit(form: FormGroup){


  let hozzaadas = {
    
    Nev: form.get('nev').value,
    Leiras: form.get('leiras').value,
    Ar: form.get('ar').value,
 
  }
  
  console.log(hozzaadas);

  
  this.service.etelHozzadas(hozzaadas).subscribe( response => {
    console.log(response);
   });
}



}
