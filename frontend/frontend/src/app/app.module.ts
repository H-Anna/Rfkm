import { HttpClient, HttpClientModule } from '@angular/common/http';
import { RegisztracioService } from './services/regisztracio.service';
import { NotFoundComponent } from './not-found/not-found.component';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { BrowserModule } from '@angular/platform-browser';
import { RouterModule } from '@angular/router';


import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BejelentkezesComponent } from './bejelentkezes/bejelentkezes.component';
import { EtteremRegisztralasaComponent } from './etterem-regisztralasa/etterem-regisztralasa.component';
import { EtteremHomeComponent } from './etterem-home/etterem-home.component';

import { MatToolbarModule } from '@angular/material/toolbar';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatButtonModule } from '@angular/material/button';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatMenuModule } from '@angular/material/menu';
import { MatCardModule } from '@angular/material/card';


import { CommonModule } from '@angular/common';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { EtelekComponent } from './etelek/etelek.component';
import { VendegHomeComponent } from './vendeg-home/vendeg-home.component';

@NgModule({
  declarations: [
    AppComponent,
    BejelentkezesComponent,
    EtteremRegisztralasaComponent,
    EtteremHomeComponent,
    EtelekComponent,
    VendegHomeComponent
  ],
  imports: [
    CommonModule,
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    MatToolbarModule,
    MatInputModule,
    MatIconModule,
    MatListModule,
    MatFormFieldModule,
    MatButtonModule,
    MatCheckboxModule,
    MatMenuModule,
    MatCardModule,

    RouterModule.forRoot([
      // which component should be displayed when the address changes to a certain path

      //empty path represents the home page = default route
      { path: '', component: BejelentkezesComponent },
      { path: 'etteremreg', component: EtteremRegisztralasaComponent },
      { path: 'etterem', component: EtteremHomeComponent },
      { path: 'etterem/etelek', component: EtelekComponent },
      { path: 'vendeg', component: VendegHomeComponent },

      //wildcard, catches any urls, ennek kell lennie az utolsónak
      { path: '**', component: NotFoundComponent }


    ]),
    BrowserAnimationsModule
  ],
  providers: [RegisztracioService],
  bootstrap: [AppComponent]
})
export class AppModule { }
