#include "arbiter.h"

Request *Arbiter::Arbitrate(Request requests[]) {
  // The request with highest priority(0) is the default request.
  Request *selected_request = requests;

  // Arbitrate according to fix priority scheme.
  for (unsigned i = 0; i < kPriorityNum; i++) {
    if (requests[i].status == TRANSFER_REQUEST) {
      selected_request = &requests[i];
      break;
    }
  }
  return selected_request;
}
