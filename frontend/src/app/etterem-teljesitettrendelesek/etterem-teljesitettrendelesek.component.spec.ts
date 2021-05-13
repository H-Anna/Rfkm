import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremTeljesitettrendelesekComponent } from './etterem-teljesitettrendelesek.component';

describe('EtteremTeljesitettrendelesekComponent', () => {
  let component: EtteremTeljesitettrendelesekComponent;
  let fixture: ComponentFixture<EtteremTeljesitettrendelesekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremTeljesitettrendelesekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremTeljesitettrendelesekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
