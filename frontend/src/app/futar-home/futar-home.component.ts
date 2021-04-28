import { ActivatedRoute } from '@angular/router';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-futar-home',
  templateUrl: './futar-home.component.html',
  styleUrls: ['./futar-home.component.css']
})
export class FutarHomeComponent implements OnInit {

  constructor(private route: ActivatedRoute) { }

  futarId: number;

  ngOnInit(): void {
    this.route.paramMap.subscribe(params => {
      console.log(params);
      this.futarId = +params.get('futarId');
    })
  }

}
