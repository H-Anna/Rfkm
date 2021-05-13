import { ActivatedRoute } from '@angular/router';
import { FutarService } from './../services/futar.service';
import { Component, OnInit } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';

@Component({
  selector: 'app-etterem-beallitasok',
  templateUrl: './etterem-beallitasok.component.html',
  styleUrls: ['./etterem-beallitasok.component.css']
})
export class EtteremBeallitasokComponent implements OnInit {

  constructor(private futarService: FutarService, private activatedRoute: ActivatedRoute) { }

  ngOnInit(): void {
    this.activatedRoute.paramMap.subscribe(
      params => {
        this.etteremId = +params.get('etteremId');
      }
    );
  }


  etteremId: number;

  form1 = new FormGroup({   
    reszesedes: new FormControl('', Validators.required)
  });

  get reszesedes() {
    return this.form1.get('reszesedes');
  }

  submit1(form1){
    let request = {
      "EtteremID": this.etteremId,
      "FutarReszesedes": form1.get('reszesedes').value
    }
    console.log(request);
    this.futarService.reszesedesModositasa(request).subscribe( response => {
      console.log(response);
    })
  }
}