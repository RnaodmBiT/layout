#include <ui_factory.hpp>

using namespace tk::core;
using namespace tk::graphics;

namespace tk {
    namespace layout {

        class UIFactory::Impl {
        public:
            std::map<std::string, Builder> builders;

            void addDefaultTypes() {
                addType("text", [] (const std::string& name, const json& data, const Style& style, const ResourceCollection& resources) {
                    DrawableNode* node = new TextNode(name,
                                                      resources.get<Font>(style.getFont()),
                                                      data["text"],
                                                      style.getFontSize());
                    node->setTint(style.getColor());
                    return node;
                });
            }

            void addType(const std::string& name, const Builder& constructor) {
                tk_assert(builders.count(name) == 0, "Type constructor already exists");
                builders[name] = constructor;
            }

            DrawableNode* build(const std::string& name, const json& data, const StyleSheet& styles, const ResourceCollection& resources) {
                std::string type = data["type"];
                tk_assert(builders.count(type), format("No builder for '%%' type", type));
                return builders[type](name, data, styles.get(data["style"]), resources);
            }
        };

        UIFactory::UIFactory() {
            impl = new Impl();
        }

        UIFactory::~UIFactory() {
            delete impl;
        }

        UIFactory::UIFactory(UIFactory&& move) {
            *this = std::move(move);
        }

        void UIFactory::operator=(UIFactory&& move) {
            std::swap(impl, move.impl);
        }

        void UIFactory::addDefaultTypes() {
            impl->addDefaultTypes();
        }

        void UIFactory::addType(const std::string& name, const Builder& constructor) {
            impl->addType(name, constructor);
        }

        DrawableNode* UIFactory::build(const std::string& name, const json& data, const StyleSheet& styles, const ResourceCollection& resources) {
            return impl->build(name, data, styles, resources);
        }
    }
}