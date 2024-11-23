#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "Utils.hpp"

namespace Evaluation {
    int evaluate(const Position& pos);
    int count_material(const Position& pos, Colour colour);
}

#endif // #ifndef EVALUATION_HPP