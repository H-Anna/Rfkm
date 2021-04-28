import { TestBed } from '@angular/core/testing';

import { FutarService } from './futar.service';

describe('FutarService', () => {
  let service: FutarService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(FutarService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
