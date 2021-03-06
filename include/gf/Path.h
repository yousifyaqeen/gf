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
#ifndef GF_PATH_H
#define GF_PATH_H

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>

#include "Portability.h"
#include "SerializationFwd.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A path in the filesystem
   *
   * gf::Path is a simple typedef for `boost::filesystem::path`.
   * [Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/)
   * will eventually be part of the C++17 standard. So, in the future, gf::Path
   * will be a typedef for `std::path`.
   */
  using Path = boost::filesystem::path;

  /**
   * @relates Serializer
   * @brief Serialize a path
   */
  GF_API Serializer& operator|(Serializer& ar, const Path& path);

  /**
   * @relates Deserializer
   * @brief Deserialize a path
   */
  GF_API Deserializer& operator|(Deserializer& ar, Path& path);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PATH_H
