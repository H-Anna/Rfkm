import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ReNavbarComponent } from './re-navbar.component';


import { MatToolbarModule } from '@angular/material/toolbar';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatButtonModule } from '@angular/material/button';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatMenuModule } from '@angular/material/menu';


@NgModule({
  declarations: [ReNavbarComponent],
  imports: [
    CommonModule,
    MatToolbarModule,
        MatInputModule,
        MatIconModule,
        MatListModule,
        MatFormFieldModule,
        MatButtonModule,
        MatCheckboxModule,
        MatMenuModule,
  ],
  exports:[ReNavbarComponent]
})
export class ReNavbarModule { }
