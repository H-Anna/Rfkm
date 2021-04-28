import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremRendelesekComponent } from './etterem-rendelesek.component';

describe('EtteremRendelesekComponent', () => {
  let component: EtteremRendelesekComponent;
  let fixture: ComponentFixture<EtteremRendelesekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremRendelesekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremRendelesekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
