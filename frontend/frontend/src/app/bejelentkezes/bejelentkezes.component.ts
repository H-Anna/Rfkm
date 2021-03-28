import { BelepesService } from './../services/belepes.service';
import { RegisztracioService } from './../services/regisztracio.service';
import { FormGroup, FormControl, Validators } from '@angular/forms';
import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { stringify } from '@angular/compiler/src/util';

@Component({
  selector: 'bejelentkezes',
  templateUrl: './bejelentkezes.component.html',
  styleUrls: ['./bejelentkezes.component.css']
})
export class BejelentkezesComponent implements OnInit {

  constructor(private router: Router, private regService: RegisztracioService, private belepesService: BelepesService) { }

  //ezt le kell kérni szerverről hogy milyen típusú user
  belepesMint: string = "vendeg";

  formBelep = new FormGroup({
    belepEmail: new FormControl(),
    belepJelszo: new FormControl()
  });

  formReg = new FormGroup({
    email: new FormControl('',Validators.required),
    jelszo: new FormControl('',Validators.required),
    vnev: new FormControl('',Validators.required),
    knev: new FormControl('',Validators.required),
    cim: new FormGroup({
      irszam: new FormControl('',Validators.required),
      kozterulet: new FormControl('',Validators.required),
      hazszam: new FormControl('',Validators.required),
      emeletajto: new FormControl()      
    }),
    tel: new FormControl('',Validators.required)
    }
  );

  get email(){
    return this.formReg.get('email')
  }
  get jelszo(){
    return this.formReg.get('jelszo')
  }
  get vnev(){
    return this.formReg.get('vnev')
  }
  get knev(){
    return this.formReg.get('knev')
  }
  get irszam(){
    return this.formReg.get('cim').get('irszam');
  }
  get kozterulet(){
    return this.formReg.get('cim').get('kozterulet');
  }
  get hazszam(){
    return this.formReg.get('cim').get('hazszam');
  }  
  get emeletajto(){
    return this.formReg.get('cim').get('emeletajto');
  }
  get tel(){
    return this.formReg.get('tel')
  }

  ngOnInit(): void {
  }

  submitBelep(formBelep){
    
    let belepesAdatok = {
      Email: formBelep.get('belepEmail').value,
      Jelszo: formBelep.get('belepJelszo').value
    }
    console.log(belepesAdatok);
    this.belepesService.bejelentkezes(belepesAdatok).subscribe( response => {
      console.log(response);
      this.belepesMint = response.Tipus;
      console.log(this.belepesMint);
    });

    //megnézi hogy milyen user (étterem, vendég vagy futár)
    if(this.belepesMint == "Etterem"){
      this.router.navigate(['/etterem']);
    }
    else if(this.belepesMint  == "RegisztraltVendeg"){
      this.router.navigate(['/vendeg']);
    }
    
  }


  submitVendegReg(form){
    let regisztracio = {
      Email: form.get('email').value,
      Jelszo: form.get('jelszo').value,
      VNev: form.get('vnev').value,
      KNev: form.get('knev').value,
      Cim:
      {
        Irsz: form.get('cim').get('irszam').value,
        Kozterulet: form.get('cim').get('kozterulet').value,
        Hazszam: form.get('cim').get('hazszam').value,
        Emelet_ajto: form.get('cim').get('emeletajto').value
      },
      Telefonszam: form.get('tel').value
    }
    
    console.log(regisztracio);

    this.regService.vendegRegisztral(regisztracio).subscribe( response => {
      console.log(response);
    })
    
  }

}
