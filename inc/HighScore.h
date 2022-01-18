#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>

#include "AbstractEntity.h"

// Entity rendered when the current game state is HIGHSCORE.
class HighScore : public AbstractEntity {
public:
  HighScore(SDL_Renderer *renderer);
  ~HighScore();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
  // Add new high score
  using ScoreEntry = std::pair<long int, std::string>;
  bool AddEntry(const ScoreEntry& entry);

private:
  int m_scoreHeight = -1;
  SDL_Rect *m_textBox = NULL;
  SDL_Surface *m_textSurface = NULL;
  SDL_Texture *m_textTexture = NULL;

  std::vector<ScoreEntry> m_highScores;

  TTF_Font *m_silkscreen = NULL;
};

#endif // HIGHSCORE_H
