import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarRegisztralasComponent } from './futar-regisztralas.component';

describe('FutarRegisztralasComponent', () => {
  let component: FutarRegisztralasComponent;
  let fixture: ComponentFixture<FutarRegisztralasComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarRegisztralasComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarRegisztralasComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
