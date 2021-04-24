import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ReMenuComponent } from './re-menu.component';

describe('ReMenuComponent', () => {
  let component: ReMenuComponent;
  let fixture: ComponentFixture<ReMenuComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ReMenuComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ReMenuComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
