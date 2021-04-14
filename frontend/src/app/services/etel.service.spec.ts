import { TestBed } from '@angular/core/testing';

import { EtelService } from './etel.service';

describe('EtelService', () => {
  let service: EtelService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(EtelService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
