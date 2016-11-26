#include <style_sheet.hpp>
#include <core.hpp>
#include <map>

using namespace tk::core;

namespace tk {
    namespace layout {

        Style::Style() : parent(nullptr), padding(nullptr), color(nullptr), font(nullptr), fontSize(nullptr) { }

        Style::~Style() {
            delete padding;
            delete color;
            delete font;
            delete fontSize;
        }

        const core::Vec4i& Style::getPadding() const {
            static core::Vec4i default;
            return padding ? *padding : (parent ? parent->getPadding() : default);
        }

        const core::Vec4f& Style::getColor() const {
            static core::Vec4f default;
            return color ? *color : (parent ? parent->getColor() : default);
        }

        const std::string& Style::getFont() const {
            static std::string default;
            return font ? *font: (parent ? parent->getFont() : default);
        }

        int Style::getFontSize() const {
            return fontSize ? *fontSize: (parent ? parent->getFontSize() : 20);
        }


        class StyleSheet::Impl {
        public:
            std::map<std::string, Style> styles;

            Style& get(const std::string& name) {
                return styles[name];
            }

            bool has(const std::string& name) {
                return styles.count(name) > 0;
            }
        };


        StyleSheet::StyleSheet() {
            impl = new Impl();
        }

        StyleSheet::~StyleSheet() {
            delete impl;
        }

        StyleSheet::StyleSheet(StyleSheet&& move) {
            *this = std::move(move);
        }

        void StyleSheet::operator=(StyleSheet&& move) {
            std::swap(impl, move.impl);
        }

        IResource* StyleSheet::loadFromFile(const std::string& filename) {
            json styles = json::parse(readFile(filename).c_str());
            StyleSheet* sheet = new StyleSheet();

            for (auto it = styles.begin(); it != styles.end(); ++it) {
                Style& style = sheet->impl->get(it.key());
                const json& data = it.value();

                if (data.count("parent")) {
                    tk_assert(sheet->has(data["parent"]), format("Error loading style sheet '%%'. Missing reference to style '%%'",
                              filename, data["parent"]));

                    style.parent = &sheet->impl->get(data["parent"]);
                }

                if (data.count("padding")) {
                    const auto& p = data["padding"];
                    style.padding = new Vec4i{ p[0], p[1], p[2], p[3] };
                }

                if (data.count("color")) {
                    style.color = createColorFromHex(data["color"]);
                }

                if (data.count("font")) {
                    const std::string& font = data["font"];
                    style.font = new std::string(font.begin(), font.end());
                }

                if (data.count("fontSize")) {
                    style.fontSize = new int(data["fontSize"]);
                }
            }

            return sheet;
        }

        const Style& StyleSheet::get(const std::string& style) const {
            return impl->get(style);
        }

        const Style& StyleSheet::operator[](const std::string& style) const {
            return impl->get(style);
        }

        bool StyleSheet::has(const std::string& style) {
            return impl->has(style);
        }

        Vec4f* StyleSheet::createColorFromHex(const std::string& hex) {
            Vec4f* color = new Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
            std::string::const_iterator it = hex.begin();
            if (*it == '#') {
                it++;
            }

            color->x = float(std::stoul(std::string(it, it + 2), nullptr, 16)) / 255;
            color->y = float(std::stoul(std::string(it + 2, it + 4), nullptr, 16)) / 255;
            color->z = float(std::stoul(std::string(it + 4, it + 6), nullptr, 16)) / 255;

            if (it + 6 != hex.end()) {
                color->w = float(std::stoul(std::string(it + 6, it + 8), nullptr, 16)) / 255;
            }
            return color;
        }
    }
}