/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <gf/Random.h>

#include <algorithm>
#include <functional>
#include <iterator>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    // see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
    // and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
    std::mt19937 getCorrectlyInitializedEngine() {
      std::mt19937::result_type data[std::mt19937::state_size];
      std::random_device source;
      std::generate(std::begin(data), std::end(data), std::ref(source));
      std::seed_seq seeds(std::begin(data), std::end(data));
      return std::mt19937(seeds);
    }

  } // anonymous namespace

  Random::Random()
  : m_engine(getCorrectlyInitializedEngine())
  {

  }

  Vector2f Random::computePosition(const RectF& area) {
    std::uniform_real_distribution<float> distX(0.0f, area.width);
    std::uniform_real_distribution<float> distY(0.0f, area.height);
    float x = area.left + distX(m_engine);
    float y = area.top + distY(m_engine);
    return { x, y };
  }

  Vector2i Random::computePosition(const RectI& area) {
    std::uniform_int_distribution<int> distX(0, area.width);
    std::uniform_int_distribution<int> distY(0, area.height);
    int x = area.left + distX(m_engine);
    int y = area.top + distY(m_engine);
    return { x, y };
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
