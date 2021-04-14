import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarHomeComponent } from './futar-home.component';

describe('FutarHomeComponent', () => {
  let component: FutarHomeComponent;
  let fixture: ComponentFixture<FutarHomeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarHomeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarHomeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
