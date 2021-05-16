import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremRendelesekElvitelreComponent } from './etterem-rendelesek-elvitelre.component';

describe('EtteremRendelesekElvitelreComponent', () => {
  let component: EtteremRendelesekElvitelreComponent;
  let fixture: ComponentFixture<EtteremRendelesekElvitelreComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremRendelesekElvitelreComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremRendelesekElvitelreComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
