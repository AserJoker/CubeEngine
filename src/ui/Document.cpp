#include "ui/Document.hpp"
#include <cstring>
#include <format>
#include <imgui.h>
#include <stdexcept>
#include <tinyxml2.h>
using namespace cube;
using namespace cube::ui;
void Document::draw(const Context &ctx) {

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin(NULL, NULL,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
  ImGui::PopStyleVar(3);
  Node::draw(ctx);
  ImGui::End();
}
void Document::parse(Context &ctx, const std::string &source,
                     const std::string &filename) {
  ctx.clearFont();
  tinyxml2::XMLDocument doc;
  auto error = doc.Parse(source.c_str());
  if (error != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error(
        std::format("Failed to parse xml '{}': {}", filename, doc.ErrorStr()));
  }
  auto root = doc.RootElement();
  if (std::strcmp(root->Name(), "document") != 0) {
    throw std::runtime_error(std::format(
        "Failed to parse xml '{}': root node is not document", filename));
  }
  
}