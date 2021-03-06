import { element } from 'protractor';
import { Kategoria } from './model/kategoria';
import { ActivatedRoute } from '@angular/router';
import { EtelService } from './../services/etel.service';
import { Etel } from './model/etel';
import { Component, OnInit, resolveForwardRef } from '@angular/core';
import { FormGroup, FormControl, FormArray, AbstractControl, Validators } from '@angular/forms';
import { switchMap } from 'rxjs/operators';
import { HttpClient, JsonpClientBackend } from '@angular/common/http';

@Component({
  selector: 'app-etelek',
  templateUrl: './etelek.component.html',
  styleUrls: ['./etelek.component.css']
})
export class EtelekComponent implements OnInit {

  constructor(private etelService: EtelService, private activatedRoute: ActivatedRoute, private http: HttpClient) {
    this.activatedRoute.paramMap.subscribe(
      params => {
        this.etteremId = +params.get('etteremId');
        console.log('c');
      }
    );

  }

  etteremId: number;
  etelek: Etel[];

  categories: Kategoria[] = [];

  imgUrl: string;

  allergenek_tomb: Kategoria[] = [];

  selectedAkcioID: number = -1;
  nullvalue: number = null;
  selectedAkcio: {
    "Nev": string,
    "AkcioID": number,
    "Ertek": number
  };

  //ezt backendről kéne lekérni h milyen akciók vannak csak még nincs meg
  akciok: {
    "Nev": string,
    "AkcioID": number,
    "Ertek": number
  }[];


  form = new FormGroup({
    kategoria: new FormControl('', Validators.required),
    nev: new FormControl('', Validators.required),
    ar: new FormControl('', Validators.required),
    etelKep: new FormControl(),
    leiras: new FormControl('', Validators.required),
    allergenek: new FormControl(),
    akcio: new FormControl(),
    ettol: new FormControl(),
    eddig: new FormControl()
  });



  ngOnInit(): void {
    /*this.etelService.akciokLekerese().subscribe( response => {
      //console.log(response);
      this.akciok=response.Akciok;
      console.log('Akciók:');
      console.log(this.akciok);
    });*/
    //this.kategoriakLekerese();
    //this.etelekLekerese();

    /*this.etelService.getEtelek2(this.etteremId).subscribe( idk3 => {
      console.log(idk3);
    })*/


    //ennek ugye a service-ben kéne lennie de kell a pipe meg a switchmap, különben meghülyül a db mert ugye aszinkron
    //és csak így működik, de így meg nem tudom hogy lehet megoldani hogy mindhárom kérést visszaadja mert ha a service-ben
    //hagyom ezt, akkor csak az utolsót adja vissza a response
    this.etelService.getEtelek3(this.etteremId).pipe(
      switchMap(idk => {
        console.log('etelek');
        console.log(idk);
        this.etelek = idk.Etelek;
        return this.http.get<{ EtelCimke: { CimkeID: number, Nev: string, Tipus: string }[] }>('http://localhost:3000/cimke/etel').pipe(
          switchMap(idk2 => {
            console.log('cimkek');
            console.log(idk2);
            idk2.EtelCimke.forEach(element => {

              if (element.Tipus == "Kategória") {
                this.categories.push(element);
              }
              else if (element.Tipus == "Allergén") {
                this.allergenek_tomb.push(element);
              }
            });
            return this.http.get<{ Akciok: { AkcioID: number, Ertek: number, Nev: string }[] }>('http://localhost:3000/etterem/akcio');
          })
        );
      })
    ).subscribe(response => {
      console.log('akciok');
      console.log(response);
      this.akciok = response.Akciok;
    })


  }

  onSelectImage() {
    this.imgUrl = this.form.get('etelKep').value;
  }

  etelekLekerese() {
    this.etelService.getEtelek(this.etteremId).subscribe(response => {
      console.log(response);
      this.etelek = response.Etelek;
      console.log('d');
    })
  }

  kategoriakLekerese() {
    this.etelService.getEtelCimkek().subscribe(
      response => {
        response.EtelCimke.forEach(element => {

          if (element.Tipus == "Kategória") {
            this.categories.push(element);
          }
          else if (element.Tipus == "Allergén") {
            this.allergenek_tomb.push(element);
          }
        });
        console.log('b');
        console.log(this.allergenek_tomb);
      }
    )
  }




  get nev() {
    return this.form.get('nev');
  }
  get leiras() {
    return this.form.get('leiras');
  }
  get ar() {
    return this.form.get('ar');
  }
  get kategoria() {
    return this.form.get('kategoria');
  }
  get allergen() {
    return this.form.get('alergen');
  }
  get etelKep() {
    return this.form.get('etelKep');
  }


  etelModositasaAkcioval(item) {
    console.log(item);
    let cimkek: number[] = [];
    let modositandoEtel: {
      Akcio: {
        Ertek: number,
        Nev: string
      },
      Ar: number,
      Cimke: string[],
      EtelID: number,
      Idoszak: {
        Befejezes: number,
        Kezdes: number
      },
      Kep: any,
      Leiras: string,
      Nev: string
    } = null; 
   /* modositandoEtel = this.etelek.find(etel => {    //ez nem működik
      etel.EtelID == item.EtelID;
    });*/
    this.etelek.forEach(element => {
      if(element.EtelID === item.EtelID){
        modositandoEtel = element;
      }
    });
    console.log(modositandoEtel);
    modositandoEtel.Cimke.forEach(cimke => {
      this.categories.forEach(category => {
        console.log(category.Nev);
        console.log(cimke);
        if(category.Nev == cimke){
          console.log("kat. egyezés");
          cimkek.push(category.CimkeID);        
        }
      });
    });
    modositandoEtel.Cimke.forEach(cimke => {
      this.allergenek_tomb.forEach(allergen => {
        console.log(allergen.Nev);
        console.log(cimke);
        if(allergen.Nev == cimke){
          console.log("all. egyezés");
          cimkek.push(allergen.CimkeID);    
        }
      });
    });
    let akcioid;
    if(this.selectedAkcioID==-1){
      akcioid = null;
    }
    else{
      akcioid = +this.selectedAkcioID;
    }
    let idoszak: { 
      "Kezdes": number,
      "Befejezes": number
    } = {
      "Kezdes": null,
      "Befejezes": null
    };
    if(item.Idoszak.Kezdes == null || item.Idoszak.Befejezes == null){
      idoszak.Kezdes = 0;
      idoszak.Befejezes = 24;
    }
    else{
      idoszak.Kezdes = item.Idoszak.Kezdes;
      idoszak.Befejezes = item.Idoszak.Befejezes;
    }

    let etelAdatok = {
      Nev: item.Nev,
      Ar: item.Ar,
      Kep: item.Kep,
      Leiras: item.Leiras,
      EtelID: item.EtelID,
      Idoszak: idoszak,
      AkcioID: akcioid,
      Cimke: cimkek
    }
    //Címkénél ki kell nézni előbb az id-ját, item.Cimke csak az értéket tárolja

    console.log(etelAdatok);
    this.etelService.etelModositasEsEtelekLekerese(etelAdatok, this.etteremId).subscribe(respone => {
      console.log(respone);
      this.etelek = respone.Etelek;
    })
    this.selectedAkcioID = -1;
  }

  selectedAkcioChanged() {
    console.log("Kiválasztot akció:")
    console.log(this.selectedAkcio);
    console.log("Kiválasztot akció id:")
    console.log(this.selectedAkcioID);
  }

  etelTorlese(etel: Etel) {
    this.etelService.deleteEtel(etel.EtelID).subscribe(response => {
      console.log(response);
      this.etelekLekerese();
    });
  }

  submit(form: FormGroup) {

    let hozzaadas = {

      Nev: form.get('nev').value,
      Ar: form.get('ar').value,
      Kep: form.get('etelKep').value,
      Leiras: form.get('leiras').value,
      EtteremID: this.etteremId,
      Idoszak: {
        Kezdes: form.get('ettol').value,
        Befejezes: form.get('eddig').value
      },
      AkcioID: null,
      Cimke: [+form.get('kategoria').value]
    }
    if (form.get('allergenek').value != null) {
      form.get('allergenek').value.forEach(element => {
        hozzaadas.Cimke.push(element);
      });
    }

    console.log(hozzaadas);


    this.etelService.etelHozzadas(hozzaadas).subscribe( response => {
      console.log(response);
    });


  }

}