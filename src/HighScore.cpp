#include "HighScore.h"

namespace {
constexpr int k_buffer = 10;
constexpr int k_characterSize = 100;
constexpr int k_bannerHeight = 150;
constexpr int k_maxNumScores = 6;
const std::string k_highScoreText = "HIGH SCORES";
const std::string k_filename = "../memeinvaders/scoredata.meme";
const std::string k_fontPath = "../memeinvaders/assets/slkscre.ttf";
const std::string k_delimiter = " ";
} // namespace

HighScore::HighScore(SDL_Renderer *renderer)
    : AbstractEntity(renderer, "high score") {

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_nameBox = new SDL_Rect{k_buffer, k_bannerHeight, 0, 0};
  m_scoreBox = new SDL_Rect{SCREEN_WIDTH / 2 + k_buffer, k_bannerHeight, 0, 0};
  m_highScoreBox = new SDL_Rect{0, k_buffer, 0, 0};
}

HighScore::~HighScore() {
  TTF_CloseFont(m_silkscreen);

  delete m_nameBox;
  m_nameBox = nullptr;
  delete m_scoreBox;
  m_scoreBox = nullptr;
  delete m_highScoreBox;
  m_highScoreBox = nullptr;

  SDL_FreeSurface(m_nameSurface);
  SDL_FreeSurface(m_scoreSurface);
  SDL_FreeSurface(m_highScoreSurface);
  SDL_DestroyTexture(m_nameTexture);
  SDL_DestroyTexture(m_scoreTexture);
  SDL_DestroyTexture(m_highScoreTexture);
}

bool HighScore::Initialize() {
  m_silkscreen = TTF_OpenFont(k_fontPath.c_str(), k_characterSize);

  std::string line, highScores, highScoreNames;
  std::ifstream highScoreFile(k_filename);
  if (highScoreFile.is_open()) {
    // Read in all scores
    while (getline(highScoreFile, line)) {
      // Catch scores that cannot convert to long
      try {
        m_numScores++;
        // Parse the line for name and score
        std::string name = line.substr(0, line.find(k_delimiter));
        long int score =
            std::stol(line.substr(line.find(k_delimiter) + k_delimiter.length()));
        m_highScores.push_back(std::make_pair(score, name));
      }
      catch (...) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Score read in is not a number!");
        continue;
      }
    }
    // Sort in decreasing order
    // Have to use lambda because std::pair doesn't get along with std::greater<int>
    std::sort(m_highScores.begin(), m_highScores.end(),
              [](const auto &left, const auto &right) {
                return left.first > right.first;
              });
    for (int i = 0; i < m_highScores.size(); ++i) {
      std::string name = m_highScores[i].second;
      std::string score = std::to_string(m_highScores[i].first);
      highScoreNames += name + "\n";
      highScores += score + "\n";
    }
    highScoreFile.close();
  } else {
    // Assume score file included for now
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "File does not exist!");
    return false;
  }

  // Create text surfaces and textures
  m_nameSurface = TTF_RenderText_Blended_Wrapped(
      m_silkscreen, highScoreNames.c_str(), {255, 255, 255}, SCREEN_WIDTH / 2);
  m_scoreSurface = TTF_RenderText_Blended_Wrapped(
      m_silkscreen, highScores.c_str(), {255, 255, 255}, SCREEN_WIDTH / 2);
  m_nameTexture = SDL_CreateTextureFromSurface(m_renderer, m_nameSurface);
  m_scoreTexture = SDL_CreateTextureFromSurface(m_renderer, m_scoreSurface);

  // Same for high score text but later so we can apply outline
  TTF_SetFontOutline(m_silkscreen, 2);
  m_highScoreSurface = TTF_RenderText_Blended_Wrapped(
      m_silkscreen, k_highScoreText.c_str(), {255, 255, 255}, SCREEN_WIDTH);
  m_highScoreTexture =
      SDL_CreateTextureFromSurface(m_renderer, m_highScoreSurface);
  // Reset outline
  TTF_SetFontOutline(m_silkscreen, 0);

  SDL_QueryTexture(m_nameTexture, NULL, NULL, &m_nameBox->w, &m_nameBox->h);
  SDL_QueryTexture(m_scoreTexture, NULL, NULL, &m_scoreBox->w, &m_scoreBox->h);
  SDL_QueryTexture(m_highScoreTexture, NULL, NULL, &m_highScoreBox->w,
                   &m_highScoreBox->h);
  m_highScoreBox->x = SCREEN_WIDTH / 2 - m_highScoreBox->w / 2;

  return true;
}

void HighScore::Render() {
  // bool flag = false;
  // if (!flag) {
  // }
  SDL_RenderCopy(m_renderer, m_nameTexture, NULL, m_nameBox);
  SDL_RenderCopy(m_renderer, m_scoreTexture, NULL, m_scoreBox);
  SDL_RenderCopy(m_renderer, m_highScoreTexture, NULL, m_highScoreBox);
}

bool HighScore::AddEntry(const ScoreEntry &entry) {
  std::ofstream highScoreFile(k_filename);
  int rank = 1;
  if (m_numScores >= k_maxNumScores) {
    for (size_t i = 0; i < m_highScores.size(); ++i) {
      if (entry.first < m_highScores[i].first) {
        rank++;
      }
    }
    if (rank > k_maxNumScores) {
      // Not better than current high scores
      return false;
    }
  } else {
    return true;
  }
  return true;
}
