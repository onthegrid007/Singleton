/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*   Other Contributers:
*/

#ifndef NONMOVEABLE_H_
#define NONMOVEABLE_H_
  class NonMovable {
    public:
    NonMovable(NonMovable &&) = delete;
    NonMovable &operator=(NonMovable &&) = delete;

    protected:
    NonMovable() = default;
  };
#endif