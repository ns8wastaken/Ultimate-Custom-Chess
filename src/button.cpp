#include "button.hpp"


Button::Button(Vector2 pos, const char* imagePath)
{
    Image image = LoadImage(imagePath);
    m_texture = LoadTextureFromImage(image);
    UnloadImage(image);

    m_pos = Vector2{ pos.x - (m_texture.width / 2) * Constants::ButtonSize, pos.y - (m_texture.height / 2) * Constants::ButtonSize };
}


// Returns true if clicked
bool Button::update(const Vector2& mousePos)
{
    return CheckCollisionPointRec(
               mousePos,
               Rectangle{
                   m_pos.x,
                   m_pos.y,
                   (float)(m_texture.width * Constants::ButtonSize),
                   (float)(m_texture.height * Constants::ButtonSize) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


void Button::render()
{
    DrawTextureEx(m_texture, m_pos, 0, Constants::ButtonSize, Color{ 255, 255, 255, 255 });
}
