import { ComponentFixture, TestBed } from '@angular/core/testing';

import { VendegRendelesekComponent } from './vendeg-rendelesek.component';

describe('VendegRendelesekComponent', () => {
  let component: VendegRendelesekComponent;
  let fixture: ComponentFixture<VendegRendelesekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ VendegRendelesekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(VendegRendelesekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
