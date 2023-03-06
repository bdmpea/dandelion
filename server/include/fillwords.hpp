#ifndef FILLWORDS_HPP_
#define FILLWORDS_HPP_

#include <memory>
#include <vector>
#include "abstract_game.hpp"

namespace Dandelion::Logic {
struct fillwords final : Game {

private:
    std::vector <std::vector <int>> board;
};

}  // namespace Dandelion::Logic

#endif  // FILLWORDS_HPP_