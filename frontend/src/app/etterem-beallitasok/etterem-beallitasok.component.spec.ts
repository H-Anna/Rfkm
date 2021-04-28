import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremBeallitasokComponent } from './etterem-beallitasok.component';

describe('EtteremBeallitasokComponent', () => {
  let component: EtteremBeallitasokComponent;
  let fixture: ComponentFixture<EtteremBeallitasokComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremBeallitasokComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremBeallitasokComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
