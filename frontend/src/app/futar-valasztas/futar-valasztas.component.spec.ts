import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarValasztasComponent } from './futar-valasztas.component';

describe('FutarValasztasComponent', () => {
  let component: FutarValasztasComponent;
  let fixture: ComponentFixture<FutarValasztasComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarValasztasComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarValasztasComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
