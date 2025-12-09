#include "utils/root_path.hpp"
#include "utils/exception.hpp"

namespace utils {

namespace fs = std::filesystem;

static fs::path find_project_root(fs::path start) {
    for (fs::path p = fs::canonical(start); !p.empty(); p = p.parent_path()) {
        if (fs::exists(p / ".git")) {
            return p;
        }
    }
    throw Exception("Project root not found!");
}

const std::filesystem::path& utils::get_root_path() {
    static auto path = find_project_root(std::filesystem::current_path());
    return path;
}

} // namespace utils
