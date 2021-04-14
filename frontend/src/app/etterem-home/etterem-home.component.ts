import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';


 
@Component({
  selector: 'app-etterem-home',
  templateUrl: './etterem-home.component.html',
  styleUrls: ['./etterem-home.component.css']
})
export class EtteremHomeComponent implements OnInit {

  constructor(private route: ActivatedRoute) { }

  etteremId: number;

  ngOnInit(): void {
    this.route.paramMap.subscribe(params => {
      console.log(params);
      this.etteremId = +params.get('id');
    })
  }

}
