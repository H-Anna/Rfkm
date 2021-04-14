import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-vendeg-home',
  templateUrl: './vendeg-home.component.html',
  styleUrls: ['./vendeg-home.component.css']
})
export class VendegHomeComponent implements OnInit {

  constructor(private route: ActivatedRoute) { }

  vendegId: number;

  ngOnInit(): void {
    this.route.paramMap.subscribe(params => {
      console.log(params);
      this.vendegId = +params.get('id');
    })
  }

}
