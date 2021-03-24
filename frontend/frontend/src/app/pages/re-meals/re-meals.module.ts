import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ReMealsComponent } from './re-meals.component';

  
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatCardModule } from '@angular/material/card';

@NgModule({
  declarations: [ReMealsComponent],
  imports: [
    CommonModule,MatCardModule, MatIconModule, MatButtonModule
  ],
  exports:[ReMealsComponent],
})
export class ReMealsModule { }
