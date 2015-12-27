#include "algorithms.h"

#include "workingsolution.h"

#include <algorithm>
#include <cassert>


void dummy (WorkingSolution & sol)
{
  sol.clear();
  for (auto & node: sol.nodes())
  {
    if (node.customer->id() != sol.data().depot())
    {
      sol.open_specific_route(node);
    }
  }
}
