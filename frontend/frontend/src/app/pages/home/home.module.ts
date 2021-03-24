import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HomeComponent } from './home.component';
import { LoginModule } from '../login/login.module';
import { RegistrationModule } from '../registration/registration.module';
import { ReMealsModule } from '../re-meals/re-meals.module';
import { ReMenuModule } from '../re-menu/re-menu.module';
import { ReNavbarModule } from '../re-navbar/re-navbar.module';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatButtonModule } from '@angular/material/button';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatMenuModule } from '@angular/material/menu';



@NgModule({
  declarations: [HomeComponent],
  imports: [
    CommonModule, LoginModule, RegistrationModule, ReMealsModule,ReMenuModule,ReNavbarModule,
    MatToolbarModule,
        MatInputModule,
        MatIconModule,
        MatListModule,
        MatFormFieldModule,
        MatButtonModule,
        MatCheckboxModule,
        MatMenuModule,
  ],
  exports: [HomeComponent],
})
export class HomeModule { }
