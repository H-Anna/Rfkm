import { RegisztracioService } from './../services/regisztracio.service';
import { Component, OnInit } from '@angular/core';
import { FormGroup, FormControl, FormArray, AbstractControl, Validators } from '@angular/forms';

@Component({
  selector: 'app-etterem-regisztralasa',
  templateUrl: './etterem-regisztralasa.component.html',
  styleUrls: ['./etterem-regisztralasa.component.css']
})
export class EtteremRegisztralasaComponent implements OnInit {

  constructor(private service: RegisztracioService) { }

  ngOnInit(): void {
  }

  form = new FormGroup({
    
      email: new FormControl('',Validators.required),
      jelszo: new FormControl('',Validators.required),
      nev: new FormControl('',Validators.required),
      leiras: new FormControl('',Validators.required),
    
    cim: new FormGroup({
      irsz: new FormControl('',Validators.required),
      kozterulet: new FormControl('',Validators.required),
      hazszam: new FormControl('',Validators.required),
      emeletajto: new FormControl(),
    }),
    nyitvatartas: new FormGroup({
      hetfoEttNyit: new FormControl(),
      hetfoEttZar: new FormControl(),
      hetfoKonyhaNyit: new FormControl(),
      hetfoKonyhaZar: new FormControl(),

      keddEttNyit: new FormControl(),
      keddEttZar: new FormControl(),
      keddKonyhaNyit: new FormControl(),
      keddKonyhaZar: new FormControl(),

      szerdaEttNyit: new FormControl(),
      szerdaEttZar: new FormControl(),
      szerdaKonyhaNyit: new FormControl(),
      szerdaKonyhaZar: new FormControl(),

      csutEttNyit: new FormControl(),
      csutEttZar: new FormControl(),
      csutKonyhaNyit: new FormControl(),
      csutKonyhaZar: new FormControl(),

      pentEttNyit: new FormControl(),
      pentEttZar: new FormControl(),
      pentKonyhaNyit: new FormControl(),
      pentKonyhaZar: new FormControl(),

      szoEttNyit: new FormControl(),
      szoEttZar: new FormControl(),
      szoKonyhaNyit: new FormControl(),
      szoKonyhaZar: new FormControl(),

      vasEttNyit: new FormControl(),
      vasEttZar: new FormControl(),
      vasKonyhaNyit: new FormControl(),
      vasKonyhaZar: new FormControl(),

     
    }),

      cimke: new FormControl(),
      irszamok: new FormArray([]),
      szallktg: new FormControl() 
    

  });

  get irszamok() {
    return this.form.get('irszamok') as FormArray;
  }
  get email(){
    return this.form.get('email');
  }
  get jelszo(){
    return this.form.get('jelszo');
  }
  get nev(){
    return this.form.get('nev');
  }
  get leiras(){
    return this.form.get('leiras');
  }
  get irsz(){
    return this.form.get('cim').get('irsz');
  }
  get kozterulet(){
    return this.form.get('cim').get('kozterulet');
  }
  get hazszam(){
    return this.form.get('cim').get('hazszam');
  }  
  get emeletajto(){
    return this.form.get('cim').get('emeletajto');
  }
  get cim(){
    return this.form.get('cim');
  }

  napok: string[] = ['Hétfő', 'Kedd', 'Szerda', 'Csütörtök','Péntek','Szombat','Vasárnap'];

  orak: number[] = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24];

  //nyitvatartas = new Nyitvatartas();
  nyitvatartas: any = [];

  getNyitvaNapok(){
    if(+this.form.get('nyitvatartas').get('hetfoKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('hetfoEttNyit').value == 0 || +this.form.get('nyitvatartas').get('hetfoEttZar').value == 0 || +this.form.get('nyitvatartas').get('hetfoKonyhaZar').value == 0 ){
      this.nyitvatartas[0] = null;
    }
    else{
      this.nyitvatartas[0]={ "NapID":0, "KonyhaNyit":+this.form.get('nyitvatartas').get('hetfoKonyhaNyit').value, "EtteremNyit": +this.form.get('nyitvatartas').get('hetfoEttNyit').value, "EtteremZar": +this.form.get('nyitvatartas').get('hetfoEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('hetfoKonyhaZar').value};
    }
    if(+this.form.get('nyitvatartas').get('keddKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('keddEttNyit').value == 0 || +this.form.get('nyitvatartas').get('keddEttZar').value == 0 || +this.form.get('nyitvatartas').get('keddKonyhaZar').value == 0 ){
      this.nyitvatartas[1] = null;
    }
    else{
      this.nyitvatartas[1] = { "NapID":1,"KonyhaNyit": +this.form.get('nyitvatartas').get('keddKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('keddEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('keddEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('keddKonyhaZar').value };
    }
    if(+this.form.get('nyitvatartas').get('szerdaKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('szerdaEttNyit').value == 0 || +this.form.get('nyitvatartas').get('szerdaEttZar').value == 0 || +this.form.get('nyitvatartas').get('szerdaKonyhaZar').value == 0 ){
      this.nyitvatartas[2] = null;
    }
    else{
      this.nyitvatartas[2] = { "NapID":2,"KonyhaNyit": +this.form.get('nyitvatartas').get('szerdaKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('szerdaEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('szerdaEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('szerdaKonyhaZar').value };
    }
    if(+this.form.get('nyitvatartas').get('csutKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('csutEttNyit').value == 0 || +this.form.get('nyitvatartas').get('csutEttZar').value == 0 || +this.form.get('nyitvatartas').get('csutKonyhaZar').value == 0 ){
      this.nyitvatartas[3] = null;
    }
    else{
      this.nyitvatartas[3] = { "NapID":3,"KonyhaNyit": +this.form.get('nyitvatartas').get('csutKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('csutEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('csutEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('csutKonyhaZar').value };
    }
    if(+this.form.get('nyitvatartas').get('pentKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('pentEttNyit').value == 0 || +this.form.get('nyitvatartas').get('pentEttZar').value == 0 || +this.form.get('nyitvatartas').get('pentKonyhaZar').value == 0 ){
      this.nyitvatartas[4] = null;
    }
    else{
      this.nyitvatartas[4] = { "NapID":4,"KonyhaNyit": +this.form.get('nyitvatartas').get('pentKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('pentEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('pentEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('pentKonyhaZar').value };
    }
    if(+this.form.get('nyitvatartas').get('szoKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('szoEttNyit').value == 0 || +this.form.get('nyitvatartas').get('szoEttZar').value == 0 || +this.form.get('nyitvatartas').get('szoKonyhaZar').value == 0 ){
      this.nyitvatartas[5] = null;
    }
    else{
      this.nyitvatartas[5] = { "NapID":5,"KonyhaNyit": +this.form.get('nyitvatartas').get('szoKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('szoEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('szoEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('szoKonyhaZar').value };
    }
    if(+this.form.get('nyitvatartas').get('vasKonyhaNyit').value == 0 ||  +this.form.get('nyitvatartas').get('vasEttNyit').value == 0 || +this.form.get('nyitvatartas').get('vasEttZar').value == 0 || +this.form.get('nyitvatartas').get('vasKonyhaZar').value == 0 ){
      this.nyitvatartas[6] = null;
    }
    else{
      this.nyitvatartas[6] = { "NapID":6,"KonyhaNyit": +this.form.get('nyitvatartas').get('vasKonyhaNyit').value, "EtteremNyit":+this.form.get('nyitvatartas').get('vasEttNyit').value, "EtteremZar":+this.form.get('nyitvatartas').get('vasEttZar').value, "KonyhaZar": +this.form.get('nyitvatartas').get('vasKonyhaZar').value };
    }

  }



 

  irszamHozzaad(irszam: HTMLInputElement){
    (this.irszamok as FormArray).push(new FormControl(irszam.value));
    irszam.value = '';
  }

  irszamEltavolit(irsz: AbstractControl){
    let index = this.irszamok.controls.indexOf(irsz);
    this.irszamok.removeAt(index);
  }

  submit(form: FormGroup){

    this.getNyitvaNapok();

    var iranyitoszamok: number[] = []
    for(let control of (form.get('irszamok') as FormArray).controls){
      iranyitoszamok.push(+control.value);
    }

    let regisztracio = {
      Email: form.get('email').value,
      Jelszo: form.get('jelszo').value,
      Nev: form.get('nev').value,
      Leiras: form.get('leiras').value,
      Cim:
      {
        Irsz: form.get('cim').get('irsz').value,
        Kozterulet: form.get('cim').get('kozterulet').value,
        Hazszam: form.get('cim').get('hazszam').value,
        Emelet_ajto: form.get('cim').get('emeletajto').value
      },

      Nyitvatartas: this.nyitvatartas,
      
      Cimke: form.get('cimke').value,
      Kep: null,
      Szallit: iranyitoszamok,
      Szallitasi_ktsg: form.get('szallktg').value
    }
    
    console.log(regisztracio);

    
    this.service.etteremRegisztral(regisztracio).subscribe( response => {
      console.log(response);
     });
  }

}
