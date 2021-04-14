import { ComponentFixture, TestBed } from '@angular/core/testing';

import { KategoriaHozzaadasComponent } from './kategoria-hozzaadas.component';

describe('KategoriaHozzaadasComponent', () => {
  let component: KategoriaHozzaadasComponent;
  let fixture: ComponentFixture<KategoriaHozzaadasComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ KategoriaHozzaadasComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(KategoriaHozzaadasComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
