#include "Score.h"
#include <iostream>

namespace {
constexpr int k_width = 250;
constexpr int k_height = 45;
constexpr int k_widthOffset = 20;
constexpr int k_characterSize = 50;
const std::string k_fontPath = "../memeinvaders/assets/slkscre.ttf";
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

  SDL_FreeSurface(m_textSurface);
  SDL_DestroyTexture(m_textTexture);
}

bool Score::Initialize() {
  m_silkscreen = TTF_OpenFont(k_fontPath.c_str(), k_characterSize);
  UpdateScoreDisplay();
  return true;
}

void Score::Render() {
  SDL_RenderCopy(m_renderer, m_textTexture, NULL, m_textBox);
}

void Score::UpdateScoreDisplay() {
  // Create text surface and texture
  std::string rawScoreText = std::to_string(m_rawScore);

  m_textSurface =
      TTF_RenderText_Solid(m_silkscreen, rawScoreText.c_str(), {255, 255, 255});
  m_textTexture =
      SDL_CreateTextureFromSurface(m_renderer, m_textSurface);

  // Keep moving text box left as score increases
  TTF_SizeUTF8(m_silkscreen, rawScoreText.c_str(), &m_textBox->w,
               &m_textBox->h);
  m_textBox->x = SCREEN_WIDTH - m_textBox->w;
}

bool Score::Give1Up(int checkpoint) {
  // For some reason (0 % x == 1) I guess
  if (m_rawScore <= 0) {
    return false;
  }

  // It's time!
  if (m_rawScore % checkpoint == 0 && !m_1AndDone) {
    m_1AndDone = true;
    return true;
  }

  return false;
}
