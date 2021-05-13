import { ModositasService } from './../services/modositas.service';
import { Component, OnInit } from '@angular/core';
import { RegisztracioService } from './../services/regisztracio.service';
import { FormGroup, FormControl, FormArray, AbstractControl, Validators } from '@angular/forms';
import { MatSnackBar } from '@angular/material/snack-bar';
import { Router, ActivatedRoute } from '@angular/router';


@Component({
  selector: 'app-futar-elerhetoseg',
  templateUrl: './futar-elerhetoseg.component.html',
  styleUrls: ['./futar-elerhetoseg.component.css']
})
export class FutarElerhetosegComponent implements OnInit {

  futarId: number;

  constructor(private service: ModositasService, private activatedRoute: ActivatedRoute, private _snackBar: MatSnackBar) { }

  ngOnInit(): void {
    this.activatedRoute.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    })
  }

  form = new FormGroup({
    
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

    
  });


  napok: string[] = ['Hétfő', 'Kedd', 'Szerda', 'Csütörtök', 'Péntek', 'Szombat', 'Vasárnap'];

  vallaltNapok: any = [];

  orak: number[] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];

  openSnackBar(message: string, action: string) {
    this._snackBar.open(message, action, {
      duration: 4000
    });
  }

  getMikorVallal() {
    if (+this.form.get('hetfoKezd').value == 0 || +this.form.get('hetfoBefejez').value == 0 ) {
      this.vallaltNapok[0] = null;
    }
    else {
      this.vallaltNapok[0] = { "Nap": 0, "Kezdes": +this.form.get('hetfoKezd').value, "Befejezes": +this.form.get('hetfoBefejez').value};
    }
    if (+this.form.get('keddKezd').value == 0 || +this.form.get('keddBefejez').value == 0) {
      this.vallaltNapok[1] = null;
    }
    else {
      this.vallaltNapok[1] = { "Nap": 1, "Kezdes": +this.form.get('keddKezd').value, "Befejezes": +this.form.get('keddBefejez').value};
    }
    if (+this.form.get('szerdaKezd').value == 0 || +this.form.get('szerdaBefejez').value == 0) {
      this.vallaltNapok[2] = null;
    }
    else {
      this.vallaltNapok[2] = { "Nap": 2, "Kezdes": +this.form.get('szerdaKezd').value, "Befejezes": +this.form.get('szerdaBefejez').value};
    }
    if (+this.form.get('csutKezd').value == 0 || +this.form.get('csutBefejez').value == 0) {
      this.vallaltNapok[3] = null;
    }
    else {
      this.vallaltNapok[3] = { "Nap": 3, "Kezdes": +this.form.get('csutKezd').value, "Befejezes": +this.form.get('csutBefejez').value};
    }
    if (+this.form.get('pentKezd').value == 0 || +this.form.get('pentBefejez').value == 0) {
      this.vallaltNapok[4] = null;
    }
    else {
      this.vallaltNapok[4] = { "Nap": 4, "Kezdes": +this.form.get('pentKezd').value, "Befejezes": +this.form.get('pentBefejez').value};
    }
    if (+this.form.get('szoKezd').value == 0 || +this.form.get('szoBefejez').value == 0) {
      this.vallaltNapok[5] = null;
    }
    else {
      this.vallaltNapok[5] = { "Nap": 5, "Kezdes": +this.form.get('szoKezd').value, "Befejezes": +this.form.get('szoBefejez').value};
    }
    if (+this.form.get('vasKezd').value == 0 || +this.form.get('vasBefejez').value == 0) {
      this.vallaltNapok[6] = null;
    }
    else {
      this.vallaltNapok[6] = { "Nap": 6, "Kezdes": +this.form.get('vasKezd').value, "Befejezes": +this.form.get('vasBefejez').value};
    }

  }

  tablazatEmpty(){
    //táblázat tisztítása nem tudom hogy kell lol
    this.form.reset();///???
  }

  submit(form: FormGroup) {

    this.getMikorVallal();
    
    let json = {
      "FutarID": this.futarId,
      "Mikor": this.vallaltNapok
    }
    console.log(json);

    this.service.modosit(json).subscribe( response => {
      console.log(response);
      if(response.Message=="OK"){
        this.openSnackBar("Sikeres módosítás","OK");
        this.tablazatEmpty();
      }
      else{
        this.openSnackBar("Módosítás sikertelen","OK");
      }
    }, error => {
      
    });


  }

}