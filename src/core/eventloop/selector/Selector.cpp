/**
  * @file Selector.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "Selector.h"
#include "EpollSelector.h"

namespace iphael {
    SelectorPtr MakeDefaultSelector() {
        return SelectorPtr{new EpollSelector{}};
    }
}