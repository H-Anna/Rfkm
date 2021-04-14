import { Rendeles } from './../vendeg-home/rendeles';
import { ActivatedRoute, Router } from '@angular/router';
import { EtelService } from './../services/etel.service';
import { Component, OnInit } from '@angular/core';
import { Etel } from '../etelek/model/etel';

@Component({
  selector: 'app-etelek-rendeles',
  templateUrl: './etelek-rendeles.component.html',
  styleUrls: ['./etelek-rendeles.component.css']
})
export class EtelekRendelesComponent implements OnInit {

  constructor(private service: EtelService, private activatedRoute : ActivatedRoute, private router: Router) {
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
  tovabb: boolean = false;

  rendeles;
  


  ngOnInit(): void {
    this.rendeles = new Rendeles(this.vendegId);

    this.rendeles
    this.service.getEtelek(this.etteremId).subscribe( response => {
      this.etelek = response.Etelek;
    })
  }

  rendel(){
    console.log(this.rendeles);
    //this.router.navigate(['/vendeg',this.vendegId,'kosar']);
    //még nem tudom hogy lesz a kosár, meg a rendelés véglegesítése
    this.tovabb = true;
  }

}
