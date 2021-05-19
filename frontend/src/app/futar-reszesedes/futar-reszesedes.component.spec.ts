import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarReszesedesComponent } from './futar-reszesedes.component';

describe('FutarReszesedesComponent', () => {
  let component: FutarReszesedesComponent;
  let fixture: ComponentFixture<FutarReszesedesComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarReszesedesComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarReszesedesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
