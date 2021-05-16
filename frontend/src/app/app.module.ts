import { EtteremRendelesekElvitelreComponent } from './etterem-rendelesek-elvitelre/etterem-rendelesek-elvitelre.component';

import { ModositasService } from './services/modositas.service';

import { EtelService } from './services/etel.service';
import { BelepesService } from './services/belepes.service';
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
import { EtteremRendelesekComponent } from './etterem-rendelesek/etterem-rendelesek.component';

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
import { KategoriaHozzaadasComponent } from './kategoria-hozzaadas/kategoria-hozzaadas.component';
import { MatSnackBarModule } from '@angular/material/snack-bar';
import { EttermekComponent } from './ettermek/ettermek.component';
import { SafePipe } from './safe.pipe';
import { FutarRegisztralasComponent } from './futar-regisztralas/futar-regisztralas.component';
import { FutarHomeComponent } from './futar-home/futar-home.component';
import { EtelekRendelesComponent } from './etelek-rendeles/etelek-rendeles.component';
import { EtteremBeallitasokComponent } from './etterem-beallitasok/etterem-beallitasok.component';
import { FutarElerhetosegComponent } from './futar-elerhetoseg/futar-elerhetoseg.component';
import { VendegRendelesekComponent } from './vendeg-rendelesek/vendeg-rendelesek.component';
import { FutarRendelesekComponent } from './futar-rendelesek/futar-rendelesek.component';
import { EtteremTeljesitettrendelesekComponent } from './etterem-teljesitettrendelesek/etterem-teljesitettrendelesek.component';
import { RegisztracionelkulComponent } from './regisztracionelkul/regisztracionelkul.component';


@NgModule({
  declarations: [
    AppComponent,
    BejelentkezesComponent,
    EtteremRegisztralasaComponent,
    EtteremHomeComponent,
    EtelekComponent,
    VendegHomeComponent,
    KategoriaHozzaadasComponent,
    EttermekComponent,
    SafePipe,
    FutarRegisztralasComponent,
    FutarHomeComponent,
    EtelekRendelesComponent,
    EtteremBeallitasokComponent,
    EtteremRendelesekComponent,
    FutarElerhetosegComponent,
    VendegRendelesekComponent,
    FutarRendelesekComponent,
    EtteremTeljesitettrendelesekComponent,
    RegisztracionelkulComponent,
    EtteremRendelesekElvitelreComponent
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
        MatSnackBarModule,
    RouterModule.forRoot([ 
      // which component should be displayed when the address changes to a certain path

      //empty path represents the home page = default route
      { path: '', component: BejelentkezesComponent },
      { path: 'etteremreg', component: EtteremRegisztralasaComponent },
      { path: 'regisztracionelkul', component: RegisztracionelkulComponent},      
      { path: 'etterem/:etteremId', component: EtteremHomeComponent },
      { path: 'etterem/:etteremId/etelek', component: EtelekComponent, pathMatch: 'full' },
      { path: 'vendeg/:vendegId', component: VendegHomeComponent },
      { path: 'vendeg/:vendegId/ettermek', component: EttermekComponent, pathMatch: 'full' },
      { path: 'etterem/:etteremId/kategoriak', component: KategoriaHozzaadasComponent, pathMatch: 'full' },
      { path: 'futarreg', component: FutarRegisztralasComponent },
      { path: 'futar/:futarId', component: FutarHomeComponent },
      { path: 'vendeg/:vendegId/ettermek/:etteremId/rendeles', component: EtelekRendelesComponent, pathMatch: 'full'},
      { path: 'etterem/:etteremId/beallitasok', component: EtteremBeallitasokComponent, pathMatch: 'full'},
      { path: 'etterem/:etteremId/rendelesek', component: EtteremRendelesekComponent, pathMatch: 'full'},
      { path: 'futar/:futarId/modositas', component: FutarElerhetosegComponent, pathMatch: 'full' },
      { path: 'vendeg/:vendegId/rendelesek', component: VendegRendelesekComponent, pathMatch: 'full'},
      { path: 'futar/:futarId/rendelesek', component: FutarRendelesekComponent, pathMatch: 'full'},
      { path: 'etterem/:etteremId/teljesitettrendelesek', component: EtteremTeljesitettrendelesekComponent , pathMatch: 'full'},      
      { path: 'etterem/:etteremId/rendelesekelvitelre', component: EtteremRendelesekElvitelreComponent , pathMatch: 'full'},
      

     

      
      //wildcard, catches any urls, ennek kell lennie az utolsónak
      { path: '**', component: NotFoundComponent }


    ],{scrollPositionRestoration: 'enabled'}
    
    ),
    BrowserAnimationsModule
  ],
  providers: [RegisztracioService, BelepesService ,EtelService, ModositasService],
  bootstrap: [AppComponent]
})
export class AppModule { }
