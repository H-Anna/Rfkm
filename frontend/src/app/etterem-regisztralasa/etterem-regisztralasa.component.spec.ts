import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremRegisztralasaComponent } from './etterem-regisztralasa.component';

describe('EtteremRegisztralasaComponent', () => {
  let component: EtteremRegisztralasaComponent;
  let fixture: ComponentFixture<EtteremRegisztralasaComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremRegisztralasaComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremRegisztralasaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
