#include "Text.h"

#include "imgui.h"

using namespace dss;

Text::Text(const std::string &string, glm::vec2 position, std::shared_ptr<Transform> parentTransform)
    :   _textString(string),
        _position(position),
        _parentTransform(parentTransform)
{
}

void Text::Draw() {
    ImGui::SetCursorPos(ImVec2(_parentTransform->translation.x + _position.x, _parentTransform->translation.y + _position.y - ImGui::GetTextLineHeightWithSpacing()));
    ImGui::Text(_textString.c_str());
}