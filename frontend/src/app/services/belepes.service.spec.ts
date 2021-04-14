import { TestBed } from '@angular/core/testing';

import { BelepesService } from './belepes.service';

describe('BelepesService', () => {
  let service: BelepesService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(BelepesService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
