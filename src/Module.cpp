#include "Module.h"

namespace Halide {

void Module::append(Internal::Stmt stmt) {
    if (body_.defined()) {
        body_ = Internal::Block::make(body_, stmt);
    } else {
        body_ = stmt;
    }
}

Module link_modules(const std::string &name, const std::vector<Module> &modules) {
    Module output(name, modules.front().target());

    for (size_t i = 0; i < modules.size(); i++) {
        const Module &input = modules[i];

        if (output.target() != input.target()) {
            user_error << "Mismatched targets in modules to link ("
                       << output.name() << ", " << output.target().to_string()
                       << "), ("
                       << input.name() << ", " << input.target().to_string() << ")\n";
        }

        // TODO(dsharlet): Check for naming collisions, maybe rename
        // internal linkage declarations in the case of collision.
        output.append(input.body());
    }

    return output;
}

}
