import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EtteremHomeComponent } from './etterem-home.component';

describe('EtteremHomeComponent', () => {
  let component: EtteremHomeComponent;
  let fixture: ComponentFixture<EtteremHomeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EtteremHomeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EtteremHomeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
