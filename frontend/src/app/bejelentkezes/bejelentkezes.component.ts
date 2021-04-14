import { BelepesService } from './../services/belepes.service';
import { RegisztracioService } from './../services/regisztracio.service';
import { FormGroup, FormControl, Validators } from '@angular/forms';
import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { MatSnackBar } from '@angular/material/snack-bar';

@Component({
  selector: 'bejelentkezes',
  templateUrl: './bejelentkezes.component.html',
  styleUrls: ['./bejelentkezes.component.css']
})
export class BejelentkezesComponent implements OnInit {

  constructor(private router: Router, private regService: RegisztracioService, private belepesService: BelepesService,private _snackBar: MatSnackBar) { }

  //ezt le kell kérni szerverről hogy milyen típusú user
  belepesMint: string = "";
  id: number;
  sikeresBelepes: boolean = true;

  formBelep = new FormGroup({
    belepEmail: new FormControl('', Validators.required),
    belepJelszo: new FormControl('', Validators.required)
  });

  formReg = new FormGroup({
    email: new FormControl('', Validators.required),
    jelszo: new FormControl('', Validators.required),
    vnev: new FormControl('', Validators.required),
    knev: new FormControl('', Validators.required),
    cim: new FormGroup({
      irszam: new FormControl('', Validators.required),
      kozterulet: new FormControl('', Validators.required),
      hazszam: new FormControl('', Validators.required),
      emeletajto: new FormControl()
    }),
    tel: new FormControl('', Validators.required)
  }
  );

  get email() {
    return this.formReg.get('email')
  }
  get jelszo() {
    return this.formReg.get('jelszo')
  }
  get vnev() {
    return this.formReg.get('vnev')
  }
  get knev() {
    return this.formReg.get('knev')
  }
  get irszam() {
    return this.formReg.get('cim').get('irszam');
  }
  get kozterulet() {
    return this.formReg.get('cim').get('kozterulet');
  }
  get hazszam() {
    return this.formReg.get('cim').get('hazszam');
  }
  get emeletajto() {
    return this.formReg.get('cim').get('emeletajto');
  }
  get tel() {
    return this.formReg.get('tel')
  }

  get belepEmail() {
    return this.formBelep.get('belepEmail');
  }
  get belepJelszo() {
    return this.formBelep.get('belepJelszo');
  }

  ngOnInit(): void {
  }

  visszairanyitas() {

    this.openSnackBar('Sikeresen regisztráltál! Most már be tudsz jelentkezni.', 'OK');


    setTimeout(() => {
      //this.router.navigate(['/']);
      window.scrollTo(0,0);
    },
      3000);
  }

  openSnackBar(message: string, action: string) {
    this._snackBar.open(message, action, {
      duration: 4000
    });
  }

  submitBelep(formBelep) {

    let belepesAdatok = {
      Email: formBelep.get('belepEmail').value,
      Jelszo: formBelep.get('belepJelszo').value
    }
    console.log(belepesAdatok);


    this.belepesService.bejelentkezes(belepesAdatok).subscribe(response => {
      console.log(response);
      this.belepesMint = response.Tipus;
      this.id = response.ID;
      console.log(this.belepesMint);
      this.sikeresBelepes = true;

      //megnézi hogy milyen user (étterem, vendég vagy futár)
      if (this.belepesMint == "Etterem") {
        this.router.navigate(['/etterem', this.id]);
      }
      else if (this.belepesMint == "RegisztraltVendeg") {
        this.router.navigate(['/vendeg', this.id]);
      }
      else if(this.belepesMint == "Futar"){
        this.router.navigate(['/futar', this.id]);
      }


    }, error => {
      console.log(error);
      this.sikeresBelepes = false;
    });



  }


  submitVendegReg(form) {
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

    this.regService.vendegRegisztral(regisztracio).subscribe(response => {
      console.log(response);
      if (response.Message == "OK") {
        this.visszairanyitas();
      }
    }, error => {
      this.openSnackBar("Regisztráció sikertelen", "OK");
    })

  }

}
