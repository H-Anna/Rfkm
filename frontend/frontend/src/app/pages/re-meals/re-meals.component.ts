import { Component, Input, OnInit} from '@angular/core';
import { Meal } from './re-meals.model';
import{MEALS} from './re-meals.storage';


@Component({
  selector: 'app-re-meals',
  templateUrl: './re-meals.component.html',
  styleUrls: ['./re-meals.component.css']
})
export class ReMealsComponent implements OnInit {

  model : Array<Meal>;


  constructor() { }

  ngOnInit(): void {
    this.model = MEALS;
  }

}
