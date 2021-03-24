import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ReMealsComponent } from './re-meals.component';

describe('ReMealsComponent', () => {
  let component: ReMealsComponent;
  let fixture: ComponentFixture<ReMealsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ReMealsComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ReMealsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
