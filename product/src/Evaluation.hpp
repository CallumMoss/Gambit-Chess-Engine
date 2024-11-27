#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "Types.hpp"
#include "Utils.hpp"

namespace Evaluation {
    int evaluate(const Position& pos);
    int count_material(const Position& pos, Turn colour);
}

#endif // #ifndef EVALUATION_HPP