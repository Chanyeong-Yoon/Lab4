// $ g++ -o queue queue.cc

#include "../atom/queue.h"
#include "../atom/mem_req.h"

#include <cassert>

int main(int argc, char** argv) {

  // example 1: because we do not specify the size, the queue does not reject insertion at all.
  queue_c* q1 = new queue_c();
  queue_c* q2 = new queue_c();

  // insert 10 memory requests
  // the memory request should not be destroyed until the processor gets the data from the memory hierarchy.
  // so, we use "heap" in our simulation.
  for (int ii = 0; ii < 10; ++ii) {
    mem_req_s* req = new mem_req_s(ii*5, 0);
    assert(q1->push(req));
  }

  std::cout << "Before\n";
  std::cout << "---------------------------------------------------------" << "\n";
  std::cout << "Q1: ";
  for (auto& item : q1->m_entry) { std::cout << item->m_addr << " "; }
  std::cout << "\n" << "---------------------------------------------------------" << "\n";

  // example 2: suppose that we search for the memory request with addr 10 and move it from q1 to q2.
  // wrong version!! learn how iterators work.
  /*
  for (auto it = q1->m_entry.begin(); it != q1->m_entry.end(); ++ it) {
    // note that the queue contains the pointers of 'mem_req_s'
    mem_req_s* req = (*it);

    if (req->m_addr == 10) { 
      q2->push(req);  // push req to q2
      q1->pop(req);   // pop req from q1 => this 
    }
  }
  */

  // example 2: a correct version
  for (auto it = q1->m_entry.begin(); it != q1->m_entry.end(); /**/) {
    mem_req_s* req = (*it);

    if (req->m_addr == 10) { 
      // should not advance the iterator because the iterator already points to the next after popping the request.
      q2->push(req);  // first push req to q2
      q1->pop(req);   // thn pop req from q1
    } else {
      ++it; // if we did not pop, we advance the iterator.
    }
  }

  std::cout << "After\n";
  std::cout << "---------------------------------------------------------" << "\n";
  std::cout << "Q1: ";
  for (auto& item : q1->m_entry) { std::cout << item->m_addr << " "; }

  std::cout << "\n" << "Q2: ";
  for (auto& item : q2->m_entry) { std::cout << item->m_addr << " "; }
  std::cout << "\n" << "---------------------------------------------------------" << "\n";

  // do not foget to free the memory request when data is returned to the upper-most meory level. :)
  // I did not do it here...

  delete q1;
  delete q2;
}
