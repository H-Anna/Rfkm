import { MatSnackBar } from '@angular/material/snack-bar';
import { RendelesService } from './../services/rendeles.service';
import { Rendeles } from './../vendeg-home/rendeles';
import { ActivatedRoute, Router } from '@angular/router';
import { EtelService } from './../services/etel.service';
import { Component, OnInit } from '@angular/core';
import { Etel } from '../etelek/model/etel';
import { FormControl, FormGroup, Validators } from '@angular/forms';

@Component({
  selector: 'app-etelek-rendeles',
  templateUrl: './etelek-rendeles.component.html',
  styleUrls: ['./etelek-rendeles.component.css']
})
export class EtelekRendelesComponent implements OnInit {

  constructor(private service: EtelService, private activatedRoute : ActivatedRoute, private router: Router, private rendelesService: RendelesService, private _snackBar: MatSnackBar) {
    this.activatedRoute.paramMap.subscribe(
      params => {
        console.log(params);
        this.etteremId = +params.get('etteremId');
        this.vendegId = +params.get('vendegId');
      }
    );
   }

  etelek: Etel[];
  vendegId: number;
  etteremId: number;

  rendeles;

  form = new FormGroup({
    szallmod: new FormControl('',Validators.required),
    fizmod: new FormControl()
  });


  ngOnInit(): void {
    this.rendeles = new Rendeles(this.vendegId);

    //this.rendeles   //ez mi? ez itt csak így volt
    this.service.getEtelek(this.etteremId).subscribe( response => {
      this.etelek = response.Etelek;
    })
  }

  visszairanyitas() {

    this.openSnackBar('Sikeres rendelés!', 'OK');

    setTimeout(() => {
      this.router.navigate(['/vendeg/',this.vendegId,'rendelesek']);
    },
      5000);
  }

  openSnackBar(message: string, action: string) {
    this._snackBar.open(message, action, {
      duration: 4000
    });
  }

  rendelesElkuldese(){
    let etelek: {"EtelID": Number, "Mennyi": Number}[] = [];
    this.rendeles.Etelek.forEach(element => {
      etelek.push({"EtelID": element.EtelID,"Mennyi": element.Mennyi });
    });
    let rendeles = {
      "VendegID": this.vendegId,
      "FizetesiMod": this.form.get("fizmod").value,
      "SzallitasiMod": this.form.get("szallmod").value,
      "Etelek": etelek  
    }
    console.log(rendeles);
    this.rendelesService.rendelesElkuldese(rendeles).subscribe(response => {
      if(response.Message=="OK"){
        this.visszairanyitas();
      }
      else{
        this.openSnackBar("Rendelés sikertelen","OK");
      }
    });
  }


}
