#pragma once
#include <resource.hpp>
#include <vector.hpp>

namespace tk {
    namespace layout {

        enum Direction { Left, Top, Right, Bottom, Count };

        class TK_LAYOUT Style {
            Style* parent;
            core::Vec4i* padding;
            core::Vec4f* color;
            std::string* font;
            int* fontSize;

            friend class StyleSheet;
        public:
            Style();
            ~Style();

            const core::Vec4i& getPadding() const;
            const core::Vec4f& getColor() const;
            const std::string& getFont() const;
            int getFontSize() const;
        };

        class TK_LAYOUT StyleSheet : public core::IResource {
            class Impl;
            Impl* impl;

            static core::Vec4f* createColorFromHex(const std::string& hex);

        public:
            StyleSheet();
            ~StyleSheet();

            StyleSheet(StyleSheet&& move);
            void operator=(StyleSheet&& move);

            static IResource* loadFromFile(const std::string& filename);

            const Style& get(const std::string& style);
            const Style& operator[](const std::string& style);
            bool has(const std::string& name);
        };

    }
}