import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ReNavbarComponent } from './re-navbar.component';

describe('ReNavbarComponent', () => {
  let component: ReNavbarComponent;
  let fixture: ComponentFixture<ReNavbarComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ReNavbarComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ReNavbarComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
