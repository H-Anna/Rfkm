import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtelekRendelesComponent } from './etelek-rendeles.component';

describe('EtelekRendelesComponent', () => {
  let component: EtelekRendelesComponent;
  let fixture: ComponentFixture<EtelekRendelesComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtelekRendelesComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtelekRendelesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
