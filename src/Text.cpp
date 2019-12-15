#include "Text.h"

#include "imgui.h"

using namespace dss;

Text::Text(const std::string &string, glm::vec2 position)
    :   _textString(string),
        _position(position)
{
}

void Text::Draw() {
    ImGui::SetCursorPos(ImVec2(_position.x, _position.y - ImGui::GetTextLineHeightWithSpacing()));
    ImGui::Text(_textString.c_str());
}