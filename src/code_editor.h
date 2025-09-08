#pragma once
#include <string>
#include <vector>
#include <imgui.h>
extern "C" {
     #include ""
    }    
#include "tree-sitter/a"  // Assume you have a C++ grammar for C++ (tree-sitter-cpp)

class CodeEditor {
public:
    CodeEditor();
    void SetText(const std::string& text);
    const std::string& GetText() const;
    void Render(const char* label, ImVec2 size = ImVec2(0, 0));
    void SetLanguageCpp();

private:
    void Parse();
    void Highlight();

    std::string m_text;
    std::vector<ImU32> m_colors; // Per-character color
    TreeParser m_parser;
    TreeParserLanguage m_language;
    bool m_dirty = true;
};

// Implementation (in .cpp file):

// #include "code_editor.h"
// #include <imgui_internal.h>

// CodeEditor::CodeEditor() {
//     SetLanguageCpp();
// }

// void CodeEditor::SetText(const std::string& text) {
//     m_text = text;
//     m_dirty = true;
// }

// const std::string& CodeEditor::GetText() const {
//     return m_text;
// }

// void CodeEditor::SetLanguageCpp() {
//     m_language = treeparser_cpp_language(); // Provided by treeparser_cpp.h
//     m_parser.SetLanguage(m_language);
//     m_dirty = true;
// }

// void CodeEditor::Parse() {
//     m_parser.Parse(m_text.c_str(), m_text.size());
// }

// void CodeEditor::Highlight() {
//     m_colors.assign(m_text.size(), IM_COL32(220, 220, 220, 255));
//     auto tokens = m_parser.GetTokens();
//     for (const auto& token : tokens) {
//         ImU32 color = IM_COL32(220, 220, 220, 255);
//         if (token.type == "string_literal") color = IM_COL32(200, 120, 80, 255);
//         else if (token.type == "comment") color = IM_COL32(120, 200, 120, 255);
//         else if (token.type == "keyword") color = IM_COL32(120, 120, 220, 255);
//         else if (token.type == "type_identifier") color = IM_COL32(120, 180, 220, 255);
//         for (size_t i = token.start; i < token.end && i < m_colors.size(); ++i)
//             m_colors[i] = color;
//     }
// }

// void CodeEditor::Render(const char* label, ImVec2 size) {
//     if (m_dirty) {
//         Parse();
//         Highlight();
//         m_dirty = false;
//     }
//     ImGui::BeginChild(label, size, true, ImGuiWindowFlags_HorizontalScrollbar);
//     ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
//     size_t i = 0;
//     while (i < m_text.size()) {
//         ImU32 color = m_colors[i];
//         size_t j = i;
//         while (j < m_text.size() && m_colors[j] == color && m_text[j] != '\n') ++j;
//         ImGui::PushStyleColor(ImGuiCol_Text, color);
//         ImGui::TextUnformatted(m_text.data() + i, m_text.data() + j);
//         ImGui::PopStyleColor();
//         i = j;
//         if (i < m_text.size() && m_text[i] == '\n') {
//             ImGui::TextUnformatted("\n");
//             ++i;
//         }
//     }
//     ImGui::PopStyleVar();
//     ImGui::EndChild();
// }