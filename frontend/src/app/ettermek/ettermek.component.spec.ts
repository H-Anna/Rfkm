import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EttermekComponent } from './ettermek.component';

describe('EttermekComponent', () => {
  let component: EttermekComponent;
  let fixture: ComponentFixture<EttermekComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EttermekComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EttermekComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
