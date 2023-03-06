#ifndef BALDA_HPP_
#define BALDA_HPP_

#include <memory>
#include <vector>
#include "abstract_game.hpp"

namespace Dandelion::Logic {
struct balda final : Game {

private:
    std::vector <std::vector <int>> board;
};

}  // namespace Dandelion::Logic

#endif  // BALDA_HPP_