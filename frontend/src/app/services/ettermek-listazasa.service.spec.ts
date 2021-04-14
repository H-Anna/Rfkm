import { TestBed } from '@angular/core/testing';

import { EttermekListazasaService } from './ettermek-listazasa.service';

describe('EttermekListazasaService', () => {
  let service: EttermekListazasaService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(EttermekListazasaService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
