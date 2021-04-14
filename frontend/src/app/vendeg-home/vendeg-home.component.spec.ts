import { ComponentFixture, TestBed } from '@angular/core/testing';

import { VendegHomeComponent } from './vendeg-home.component';

describe('VendegHomeComponent', () => {
  let component: VendegHomeComponent;
  let fixture: ComponentFixture<VendegHomeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ VendegHomeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(VendegHomeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
