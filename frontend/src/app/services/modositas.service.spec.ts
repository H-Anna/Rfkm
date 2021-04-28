import { TestBed } from '@angular/core/testing';

import { ModositasService } from './modositas.service';

describe('ModositasService', () => {
  let service: ModositasService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ModositasService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
