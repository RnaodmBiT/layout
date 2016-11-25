#pragma once
#include <core.hpp>
#include <graphics.hpp>
#include <style_sheet.hpp>
#include <string>
#include <functional>


namespace tk {
    namespace layout {

        class TK_LAYOUT UIFactory {
            class Impl;
            Impl* impl;
        public:
            typedef std::function<graphics::DrawableNode*(const std::string&, const core::json&, const Style&, const core::ResourceCollection&)> Builder;

            UIFactory();
            ~UIFactory();

            UIFactory(UIFactory&& move);
            void operator=(UIFactory&& move);

            void addDefaultTypes();
            void addType(const std::string& name, const Builder& constructor);

            graphics::DrawableNode* build(const std::string& name, const core::json& data, const StyleSheet& styles, const core::ResourceCollection& resources);
        };

    }
}