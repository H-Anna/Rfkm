import { ComponentFixture, TestBed } from '@angular/core/testing';

import { VarakozasiIdoMegadasaComponent } from './varakozasi-ido-megadasa.component';

describe('VarakozasiIdoMegadasaComponent', () => {
  let component: VarakozasiIdoMegadasaComponent;
  let fixture: ComponentFixture<VarakozasiIdoMegadasaComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ VarakozasiIdoMegadasaComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(VarakozasiIdoMegadasaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
