#pragma once

#include <string>
#include <vector>

namespace ctui {

    // WIP: intended to be a selectable text-menu widget — a `title` heading
    // over a list of `options` the user can navigate/select. Not yet wired
    // up to font.hpp rendering or any input handling; currently just holds
    // state with no public API.
    class mainMenu {
        private:
            std::string title;
            std::vector<std::string> options;
            // std::vector
    };

} // namespace ctui
