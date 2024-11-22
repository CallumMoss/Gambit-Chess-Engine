#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "Utils.hpp"

namespace Evaluation {
    int evaluate(Position& pos);
    int count_material(Position& pos, Colour colour);
}

#endif // #ifndef EVALUATION_HPP