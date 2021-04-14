import { Rendeles } from './../vendeg-home/rendeles';
import { Component, Input, OnInit } from '@angular/core';

@Component({
  selector: 'app-kosar',
  templateUrl: './kosar.component.html',
  styleUrls: ['./kosar.component.css']
})
export class KosarComponent implements OnInit {

  @Input() rendeles: Rendeles;
   
  constructor() { 
  }

  ngOnInit(): void {
    console.log(this.rendeles);
  }

}
