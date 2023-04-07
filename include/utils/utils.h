#ifndef UTILS_H
#define UTILS_H

namespace utils {
template <typename Iterator, typename Pred, typename Operation>
void for_each_if(Iterator begin, Iterator end, Pred p, Operation op) {
    for(; begin != end; begin++) {
        if(p(*begin)) {
            op(*begin);
        }
    }
}
}  // namespace utils

#endif