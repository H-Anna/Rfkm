import { Component, OnInit } from '@angular/core';
import { RegisztracioService } from './../services/regisztracio.service';
import { FormGroup, FormControl, FormArray, AbstractControl, Validators } from '@angular/forms';
import { MatSnackBar } from '@angular/material/snack-bar';
import { Router } from '@angular/router';
import { of } from 'rxjs';

@Component({
  selector: 'app-futar-regisztralas',
  templateUrl: './futar-regisztralas.component.html',
  styleUrls: ['./futar-regisztralas.component.css']
})
export class FutarRegisztralasComponent implements OnInit {

  constructor(private service: RegisztracioService, private _snackBar: MatSnackBar, private router: Router) { }

  ngOnInit(): void {
  }

  form = new FormGroup({

    email: new FormControl('', Validators.required),
    jelszo: new FormControl('', Validators.required),
    vnev: new FormControl('', Validators.required),
    knev: new FormControl('', Validators.required),
    irszamok: new FormArray([]),

    mikorVallal: new FormGroup({
      hetfoKezd: new FormControl(),
      hetfoBefejez: new FormControl(),

      keddKezd: new FormControl(),
      keddBefejez: new FormControl(),

      szerdaKezd: new FormControl(),
      szerdaBefejez: new FormControl(),

      csutKezd: new FormControl(),
      csutBefejez: new FormControl(),

      pentKezd: new FormControl(),
      pentBefejez: new FormControl(),

      szoKezd: new FormControl(),
      szoBefejez: new FormControl(),

      vasKezd: new FormControl(),
      vasBefejez: new FormControl(),

    })
  });

  
  get email() {
    return this.form.get('email');
  }
  get jelszo() {
    return this.form.get('jelszo');
  }
  get vnev() {
    return this.form.get('vnev');
  }
  get knev() {
    return this.form.get('knev');
  }
  get irszamok() {
    return this.form.get('irszamok') as FormArray;
  }

  napok: string[] = ['Hétfő', 'Kedd', 'Szerda', 'Csütörtök', 'Péntek', 'Szombat', 'Vasárnap'];

  vallaltNapok: any = [];

  orak: number[] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];

  getMikorVallal() {
    if (+this.form.get('mikorVallal').get('hetfoKezd').value == 0 || +this.form.get('mikorVallal').get('hetfoBefejez').value == 0 ) {
      this.vallaltNapok[0] = null;
    }
    else {
      this.vallaltNapok[0] = { "Nap": 0, "Kezdes": +this.form.get('mikorVallal').get('hetfoKezd').value, "Befejezes": +this.form.get('mikorVallal').get('hetfoBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('keddKezd').value == 0 || +this.form.get('mikorVallal').get('keddBefejez').value == 0) {
      this.vallaltNapok[1] = null;
    }
    else {
      this.vallaltNapok[1] = { "Nap": 1, "Kezdes": +this.form.get('mikorVallal').get('keddKezd').value, "Befejezes": +this.form.get('mikorVallal').get('keddBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('szerdaKezd').value == 0 || +this.form.get('mikorVallal').get('szerdaBefejez').value == 0) {
      this.vallaltNapok[2] = null;
    }
    else {
      this.vallaltNapok[2] = { "Nap": 2, "Kezdes": +this.form.get('mikorVallal').get('szerdaKezd').value, "Befejezes": +this.form.get('mikorVallal').get('szerdaBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('csutKezd').value == 0 || +this.form.get('mikorVallal').get('csutBefejez').value == 0) {
      this.vallaltNapok[3] = null;
    }
    else {
      this.vallaltNapok[3] = { "Nap": 3, "Kezdes": +this.form.get('mikorVallal').get('csutKezd').value, "Befejezes": +this.form.get('mikorVallal').get('csutBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('pentKezd').value == 0 || +this.form.get('mikorVallal').get('pentBefejez').value == 0) {
      this.vallaltNapok[4] = null;
    }
    else {
      this.vallaltNapok[4] = { "Nap": 4, "Kezdes": +this.form.get('mikorVallal').get('pentKezd').value, "Befejezes": +this.form.get('mikorVallal').get('pentBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('szoKezd').value == 0 || +this.form.get('mikorVallal').get('szoBefejez').value == 0) {
      this.vallaltNapok[5] = null;
    }
    else {
      this.vallaltNapok[5] = { "Nap": 5, "Kezdes": +this.form.get('mikorVallal').get('szoKezd').value, "Befejezes": +this.form.get('mikorVallal').get('szoBefejez').value};
    }
    if (+this.form.get('mikorVallal').get('vasKezd').value == 0 || +this.form.get('mikorVallal').get('vasBefejez').value == 0) {
      this.vallaltNapok[6] = null;
    }
    else {
      this.vallaltNapok[6] = { "Nap": 6, "Kezdes": +this.form.get('mikorVallal').get('vasKezd').value, "Befejezes": +this.form.get('mikorVallal').get('vasBefejez').value};
    }

  }

  visszairanyitas() {

    this.openSnackBar('Sikeresen regisztráltál! Most vissza leszel irányítva a belépési oldalra.', 'OK');

    setTimeout(() => {
      this.router.navigate(['/']);
    },
      5000);
  }

  openSnackBar(message: string, action: string) {
    this._snackBar.open(message, action, {
      duration: 4000
    });
  }


  irszamHozzaad(irszam: HTMLInputElement) {
    if(irszam.value != '' && irszam.value.length == 4){
      (this.irszamok as FormArray).push(new FormControl(irszam.value));
    irszam.value = '';
    }    
  }

  irszamEltavolit(irsz: AbstractControl) {
    let index = this.irszamok.controls.indexOf(irsz);
    this.irszamok.removeAt(index);
  }

  submit(form: FormGroup) {

    this.getMikorVallal();

    var iranyitoszamok: number[] = []
    for (let control of (form.get('irszamok') as FormArray).controls) {
      iranyitoszamok.push(+control.value);
    }

    let regisztracio = {
      Email: this.email.value,
      Jelszo: this.jelszo.value,
      Vnev: this.vnev.value,
      Knev: this.knev.value,
      Szallit: iranyitoszamok,
      Mikor: this.vallaltNapok,
    }

    console.log(regisztracio);

    console.log();

    this.service.futarRegisztral(regisztracio).subscribe( response => {
      console.log(response);
      if(response.Message == "OK"){
        this.visszairanyitas();
      }
    }, error => {
      this.openSnackBar("Regisztráció sikertelen", "OK");
    });


  }



}
