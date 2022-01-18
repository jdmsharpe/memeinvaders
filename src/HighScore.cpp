#include "HighScore.h"

namespace {
constexpr int k_width = int(0.9 * SCREEN_WIDTH);
constexpr int k_height = int(0.5 * SCREEN_HEIGHT);
constexpr int k_characterSize = 100;
constexpr int k_bannerHeight = 200;
constexpr int k_maxScoreHeight = 4;
constexpr int k_maxLineLength = 16;
const std::string k_filename = "../memeinvaders/scoredata.meme";
const std::string k_fontPath = "../memeinvaders/assets/slkscre.ttf";
const std::string k_delimiter = " ";
} // namespace

HighScore::HighScore(SDL_Renderer *renderer)
    : AbstractEntity(renderer, "high score") {

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textBox = new SDL_Rect{SCREEN_WIDTH / 2 - k_width / 2, k_bannerHeight,
                           k_width, k_height};
}

HighScore::~HighScore() {
  TTF_CloseFont(m_silkscreen);
  delete m_textBox;
  m_textBox = nullptr;

  SDL_FreeSurface(m_textSurface);
  SDL_DestroyTexture(m_textTexture);
}

bool HighScore::Initialize() {
  m_silkscreen = TTF_OpenFont(k_fontPath.c_str(), k_characterSize);

  std::string line, highScoreText;
  std::ifstream highScoreFile(k_filename);
  if (highScoreFile.is_open()) {
    while (getline(highScoreFile, line)) {
      m_scoreHeight++;
      // Parse the line for name and score
      std::string name = line.substr(0, line.find(k_delimiter));
      long int score =
          std::stol(line.substr(line.find(k_delimiter) + k_delimiter.length()));
      m_highScores.push_back(std::make_pair(score, name));
    }
    // Have to use lambda because std::pair doesn't work well with std::greater
    std::sort(m_highScores.begin(), m_highScores.end(),
              [](const auto &left, const auto &right) {
                return left.first > right.first;
              });
    for (int i = 0; i < m_highScores.size(); ++i) {
      std::string name = m_highScores[i].second;
      std::string score = std::to_string(m_highScores[i].first);
      std::string filler(k_maxLineLength - name.length() - score.length(), ' ');
      highScoreText += name + filler + score + "\n";
    }
    highScoreFile.close();
  } else {
    // Assume score file included for now
    std::cout << "File does not exist!" << std::endl;
    return false;
  }

  //   Create text surface and texture
  m_textSurface = TTF_RenderText_Blended_Wrapped(
      m_silkscreen, highScoreText.c_str(), {255, 255, 255}, SCREEN_WIDTH);
  m_textTexture = SDL_CreateTextureFromSurface(m_renderer, m_textSurface);

  SDL_QueryTexture(m_textTexture, NULL, NULL, &m_textBox->w, &m_textBox->h);

  //   std::cout << m_textBox->x << " " << m_textBox->y << " " << m_textBox->w
  //   << " " << std::endl;
  return true;
}

void HighScore::Render() {
  // Keep moving text box left as score increases
  //   TTF_SizeUTF8(m_silkscreen, m_highScores.c_str(), &m_textBox->w,
  //                &m_textBox->h);
  SDL_RenderCopy(m_renderer, m_textTexture, NULL, m_textBox);
}

bool HighScore::AddEntry(const ScoreEntry &entry) {
  std::ofstream highScoreFile(k_filename);
  int rank = 1;
  if (m_scoreHeight >= k_maxScoreHeight) {
    for (size_t i = 0; i < m_highScores.size(); ++i) {
      if (entry.first < m_highScores[i].first) {
        rank++;
      }
    }
    if (rank > k_maxScoreHeight) {
      // Not better than current high scores
      return false;
    }
  } else {
    return true;
  }
  return true;
}
