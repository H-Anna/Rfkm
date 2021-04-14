import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtelekComponent } from './etelek.component';

describe('EtelekComponent', () => {
  let component: EtelekComponent;
  let fixture: ComponentFixture<EtelekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtelekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtelekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
