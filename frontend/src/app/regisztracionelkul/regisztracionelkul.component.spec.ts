import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RegisztracionelkulComponent } from './regisztracionelkul.component';

describe('RegisztracionelkulComponent', () => {
  let component: RegisztracionelkulComponent;
  let fixture: ComponentFixture<RegisztracionelkulComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ RegisztracionelkulComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(RegisztracionelkulComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
