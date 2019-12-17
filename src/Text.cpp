#include "Text.h"

#include "imgui.h"

using namespace dss;

Text::Text(const std::string &string, glm::vec2 position, std::shared_ptr<Transform> parentTransform, 
    Transform *attachedTransform, TextPositioning textPositioning, ImFont *font)
    :   _textString(string),
        _position(position),
        _parentTransform(parentTransform),
        _attachedTransform(attachedTransform),
        _textPositioning(textPositioning),
        _font(font)
{
}

void Text::Draw() {
    ImGui::PushFont(_font);

    if (_textPositioning == TextPositioning::ABOVE) {
        // Lack of good content-layout capabilities strikes again. Stick us 2 line heights above
        // the tracked transform which allows 2 lines of wrapped text. If it needs to be 3, its gonna
        // look funky
        ImGui::SetCursorPos(ImVec2(_parentTransform->translation.x + _position.x, 
            _parentTransform->translation.y + _position.y - ImGui::GetTextLineHeightWithSpacing() * 2));
        
        ImGui::BeginChild("##aboveText", ImVec2(_attachedTransform->scale.x, 0));
        ImGui::TextWrapped(_textString.c_str());
        ImGui::EndChild();
    } else {
        // Position us such that text begins from the bottom of the attachedTransform and grows down
        ImGui::SetCursorPos(ImVec2(_parentTransform->translation.x + _position.x,
            _parentTransform->translation.y));

        ImGui::BeginChild("##belowText", ImVec2(_attachedTransform->scale.x, 0));
        ImGui::TextWrapped(_textString.c_str());
        ImGui::EndChild();
    }

    ImGui::PopFont();
}