#ifndef SCORE_H
#define SCORE_H

#include "AbstractEntity.h"

// Entity to track the player's total score.
class Score : public AbstractEntity {
public:
  Score(SDL_Renderer *renderer);
  ~Score();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;

  inline long GetScore() { return m_rawScore; }

  inline void UpdateScore(int amount) {
    m_rawScore += amount;
    m_1AndDone = false;
    UpdateScoreDisplay();
  }

  inline void ResetScore() {
    m_rawScore = 0;
    m_1AndDone = false;
    UpdateScoreDisplay();
  }

  // Determines if player has hit checkpoint score for 1-Up
  bool Give1Up(int checkpoint);

private:
  void UpdateScoreDisplay();

  TTF_Font *m_silkscreen = NULL;
  SDL_Rect *m_textBox = NULL;
  SDL_Surface *m_textSurface = NULL;
  SDL_Texture *m_textTexture = NULL;

  long m_rawScore = 0;
  bool m_1AndDone = false;
};

#endif // SCORE_H