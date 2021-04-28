import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FutarElerhetosegComponent } from './futar-elerhetoseg.component';

describe('FutarElerhetosegComponent', () => {
  let component: FutarElerhetosegComponent;
  let fixture: ComponentFixture<FutarElerhetosegComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FutarElerhetosegComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FutarElerhetosegComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
