#include "objects/label.h"
#include "scene/scene.h"
#include "root.h"

Label::Label(Scene* _owner, std::wstring _name) : Object2D(_owner, _name){
    type = L"Label";
    fontSize = 16;
    size = { 200, 40 };
    glyph_size = { 200, 40 };
    y_separation = 0.1;
}

Label::~Label() {
    for (auto& p : characters) {
        glDeleteTextures(1, &p.textureID);
    }
    for (auto& p : outline_characters) {
        glDeleteTextures(1, &p.textureID);
    }
}

void Label::load(std::wstring _fontPath){
    for (auto& p : characters) glDeleteTextures(1, &p.textureID);
    for (auto& p : outline_characters) glDeleteTextures(1, &p.textureID);
    characters.clear();
    outline_characters.clear();
    fontPath = _fontPath;
    text = replaceAll(text, L"\\n", L"\n");
    FT_Library ft;
    FT_Init_FreeType(&ft);
    std::ifstream test(_fontPath); if (!test)alert(_fontPath + L"cannot load font file. :: " + _fontPath);
    FT_Face face;
    if (FT_New_Face(ft, ws2s(_fontPath).c_str(), 0, &face)) alert(L"cannot make new face :: " + name);
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Stroker stroker;
    FT_Stroker_New(ft, &stroker);
    FT_Stroker_Set(stroker, outline_size * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
    for (int c = 0; c < text.size(); c++) {
        if (FT_Load_Char(face, text[c], FT_LOAD_RENDER)) {
            alert(L"failed to load glyph");
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        Character character = {
            text[c],
            texture,
            vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.push_back(character);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (outline_size == 0) continue;
        
        FT_UInt glyphIndex = FT_Get_Char_Index(face, text[c]);
        FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        FT_Glyph glyph;
        FT_Get_Glyph(face->glyph, &glyph);
        FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
        FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
        unsigned int texture1;
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            bitmapGlyph->bitmap.width,
            bitmapGlyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            bitmapGlyph->bitmap.buffer
        );
        Character character1 = {
            text[c],
            texture1,
            vec2(bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows),
            vec2(bitmapGlyph->left, bitmapGlyph->top),
            static_cast<unsigned int>(glyph->advance.x)
        };
        outline_characters.push_back(character1);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Label::update() {
    FrameEvent event = owner->root->currentFrameEvent;
}

void Label::draw() {
    if (owner == nullptr) return;
    if (!visible) return;
    if (percent_visible < 0.0) percent_visible = 0.0;
    if (percent_visible > 1.0) percent_visible = 1.0;

    float x = position.x, y = owner->root->WINDOW_HEIGHT_ORIGIN - position.y;
    float line = 1;
    float max_width = 0.0f;
    float width = 0;
    if (wrap_text == false) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(position.x - owner->currentCamera->position.x, owner->root->WINDOW_HEIGHT - (position.y + size.y - owner->currentCamera->position.y), size.x, size.y);
    }
    if (clipping_rect.empty() == false) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(clipping_rect.x -owner->currentCamera->position.x, owner->root->WINDOW_HEIGHT_ORIGIN - (clipping_rect.y + clipping_rect.h - owner->currentCamera->position.y), clipping_rect.w, clipping_rect.h);
    }
    for (int index = 0; index < int(text.size() * percent_visible); index++) {
        Character ch = characters[index];
        float xpos = (x + ch.bearing.x * fontScale);
        float ypos = (y - (ch.size.y - ch.bearing.y) * fontScale);
        float w = ch.size.x * fontScale;
        float h = ch.size.y * fontScale;
        Character o_ch{}; float o_xpos = 0, o_ypos = 0, o_w = 0, o_h = 0;
        if (outline_size != 0) {
            o_ch = outline_characters[index];
            o_xpos = (x + o_ch.bearing.x * fontScale);
            o_ypos = (y - (o_ch.size.y - o_ch.bearing.y) * fontScale);
            o_w = o_ch.size.x * fontScale;
            o_h = o_ch.size.y * fontScale;
            if (ch.data == L'\n') {
                width = 0;
                x = position.x;
                line++;
                line += y_separation;
                continue;
            }
            if (wrap_mode == LabelWrapMode::Word) {
                if (ch.data == ' ') {
                    float rest_size = 0.0f;
                    int k = index + 1;
                    while (k < characters.size() && characters[k].data != ' ') {
                        rest_size += characters[k].advance >> 6;
                        k++;
                    }
                    if (rest_size + width > size.x) {
                        width = 0;
                        x = position.x;
                        o_xpos = position.x + o_ch.bearing.x * fontScale;
                        line++;
                        line += y_separation;
                    }
                }
            }
            if (wrap_text == true && index + 1 < characters.size() && width + w + outline_characters[index + 1].size.x * fontScale > size.x) {
                width = 0;
                x = position.x;
                o_xpos = position.x + o_ch.bearing.x * fontScale;
                line++;
                line += y_separation;
            }
            glViewport(0.0f, 0.0f, owner->root->WINDOW_WIDTH, owner->root->WINDOW_HEIGHT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, owner->root->WINDOW_WIDTH_ORIGIN, 0, owner->root->WINDOW_HEIGHT_ORIGIN, 0, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, o_ch.textureID);
            glPushMatrix();
            if (owner->currentCamera != nullptr) {
                glMultMatrixf(glm::value_ptr(owner->currentCamera->getViewMatrix()));
            }

            glTranslatef(position.x + (pivot_offset.x / scale.x), (owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
            glRotatef(rotation, 0, 0, 1);
            glScalef(scale.x, scale.y, 1.0f);
            glm::mat4 italic(1, 0, 0, 0, shearing_amount, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            glMultMatrixf(glm::value_ptr(italic));
            glTranslatef(-(position.x + (pivot_offset.x / scale.x)), -(owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);

            if (shaking != 0.0) glTranslated(getRand(-shaking, shaking), getRand(-shaking, shaking), 0);

            glColor4f(outline_color.x, outline_color.y, outline_color.z, outline_color.w);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(o_xpos, o_ypos - (line * fontSize));
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(o_xpos + o_w, o_ypos - (line * fontSize));
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(o_xpos + o_w, o_ypos + o_h - (line * fontSize));
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(o_xpos, o_ypos + o_h - (line * fontSize));
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
            width += ch.advance >> 6;
            if (width > max_width) max_width = width;
            x += (ch.advance >> 6) * fontScale;
        }
    }
    x = position.x; y = owner->root->WINDOW_HEIGHT_ORIGIN - position.y;
    line = 1;
    max_width = 0.0f;
    width = 0;

    for (int index = 0; index < int(text.size() * percent_visible); index++) {
        Character ch = characters[index];
        float xpos = (x + ch.bearing.x * fontScale);
        float ypos = (y - (ch.size.y - ch.bearing.y) * fontScale);
        float w = ch.size.x * fontScale;
        float h = ch.size.y * fontScale;
        if (ch.data == L'\n') {
            width = 0;
            x = position.x;
            line++;
            line += y_separation;
            continue;
        }
        if (wrap_mode == LabelWrapMode::Word) {
            if (ch.data == ' ') {
                float rest_size = 0.0f;
                int k = index + 1;
                while (k < characters.size() && characters[k].data != ' ') {
                    rest_size += characters[k].advance >> 6;
                    k++;
                }
                if (rest_size + width > size.x) {
                    width = 0;
                    x = position.x;
                    xpos = position.x + ch.bearing.x * fontScale;
                    line++;
                    line += y_separation;
                }
            }
        }
        if (wrap_text == true && index+1 < characters.size() && width + w + characters[index+1].size.x * fontScale > size.x) {
            width = 0;
            x = position.x;
            xpos = position.x + ch.bearing.x * fontScale;
            line++;
            line += y_separation;
        }
        glViewport(0.0f, 0.0f, owner->root->WINDOW_WIDTH, owner->root->WINDOW_HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, owner->root->WINDOW_WIDTH_ORIGIN, 0, owner->root->WINDOW_HEIGHT_ORIGIN, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glPushMatrix();
        if (owner->currentCamera != nullptr) {
            glMultMatrixf(glm::value_ptr(owner->currentCamera->getViewMatrix()));
        }
        glTranslatef(position.x + (pivot_offset.x / scale.x), (owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x, scale.y, 1.0f);
        glm::mat4 italic(1, 0, 0, 0, shearing_amount, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        glMultMatrixf(glm::value_ptr(italic));
        glTranslatef(-(position.x + (pivot_offset.x / scale.x)), -(owner->root->WINDOW_HEIGHT_ORIGIN - position.y - (pivot_offset.y / scale.y)), 0.0f);
        if (shaking != 0.0) glTranslated(getRand(-shaking, shaking), getRand(-shaking, shaking), 0);
        glColor4f(color.x, color.y, color.z, color.w);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(xpos, ypos - (line * fontSize));
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(xpos + w, ypos - (line * fontSize));
        if (gradient) glColor4f(gradient_color.x, gradient_color.y, gradient_color.z, gradient_color.w);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(xpos + w, ypos + h - (line * fontSize));
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(xpos, ypos + h - (line * fontSize));
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        width += ch.advance >> 6;
        if (width > max_width) max_width = width;
        x += (ch.advance >> 6) * fontScale;
    }
    if (wrap_text == false || clipping_rect.empty() == false) {
        glDisable(GL_SCISSOR_TEST);
    }
    glyph_size.x = max_width;
    glyph_size.y = line * fontSize;
}