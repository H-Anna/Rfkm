import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarRendelesekComponent } from './futar-rendelesek.component';

describe('FutarRendelesekComponent', () => {
  let component: FutarRendelesekComponent;
  let fixture: ComponentFixture<FutarRendelesekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarRendelesekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarRendelesekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
