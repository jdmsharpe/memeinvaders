#include "Score.h"
#include <iostream>

namespace {
constexpr int k_width = 250;
constexpr int k_height = 45;
constexpr int k_widthOffset = 20;
constexpr size_t k_rawScoreDefaultLength = 8;
constexpr int k_characterSize = 50;
} // namespace

Score::Score(SDL_Renderer *renderer) : AbstractEntity(renderer, "score") {

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textBox = new SDL_Rect{SCREEN_WIDTH - k_width - k_widthOffset, 0, k_width,
                           k_height};
}

Score::~Score() {
  TTF_CloseFont(m_silkscreen);
  delete m_textBox;
  m_textBox = nullptr;
}

bool Score::Initialize() {
  m_silkscreen = TTF_OpenFont("../memeinvaders/assets/slkscre.ttf", k_characterSize);
  return true;
}

void Score::Render() {
  // Create text surface and texture
  // Doing this every rendering loop is most likely bad for efficiency
  // TODO: Refactor later
  std::string rawScoreText = std::to_string(m_rawScore);
  int offset =
      (rawScoreText.length() - k_rawScoreDefaultLength) * k_characterSize;

  SDL_Surface *textSurface =
      TTF_RenderText_Solid(m_silkscreen, rawScoreText.c_str(), {255, 255, 255});
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(m_renderer, textSurface);

  // Keep moving text box left as score increases
  TTF_SizeUTF8(m_silkscreen, rawScoreText.c_str(), &m_textBox->w,
               &m_textBox->h);
  m_textBox->x = SCREEN_WIDTH - m_textBox->w;
  SDL_RenderCopy(m_renderer, textTexture, NULL, m_textBox);

  // TODO: Refactor later
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

bool Score::Give1Up() {
  // For some reason (0 % 5000 == 1) I guess
  if (m_rawScore <= 0) {
    return false;
  }

  // It's time!
  if (m_rawScore % 5000 == 0 && !m_1AndDone) {
    m_1AndDone = true;
    return true;
  }

  return false;
}
