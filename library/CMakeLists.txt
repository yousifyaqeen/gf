file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_executable(gfdev_source_file tools/gfdev_source_file.cc)

# process gamecontrollerdb

set(GCDB_SOURCES
  gamecontrollerdb.txt
  older_releases/gamecontrollerdb_205.txt
  older_releases/gamecontrollerdb_204.txt
)

set(GCDB_HEADERS "")

foreach(GCDB_RELATIVE_SOURCE ${GCDB_SOURCES})
  set(GCDB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gamecontrollerdb/${GCDB_RELATIVE_SOURCE}")
  get_filename_component(GCDB_NAME ${GCDB_RELATIVE_SOURCE} NAME)
  string(REPLACE "." "_" GCDB_VARIABLE ${GCDB_NAME})
  set(GCDB_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${GCDB_NAME}.h")
  add_custom_command(
    OUTPUT "${GCDB_HEADER}"
    COMMAND gfdev_source_file "${GCDB_SOURCE}" "${GCDB_HEADER}" "gamecontrollerdb"
    COMMENT "Compiling ${GCDB_NAME} to header"
    DEPENDS gfdev_source_file "${GCDB_SOURCE}"
  )
  list(APPEND GCDB_HEADERS ${GCDB_HEADER})
endforeach()

add_custom_target(gf0_gamecontrollerdb ALL DEPENDS ${GCDB_HEADERS})

# process shader files

set(SHADER_SOURCES
  data/shaders/color_matrix.frag
  data/shaders/default_alpha.frag
  data/shaders/default.frag
  data/shaders/default.vert
  data/shaders/edge.frag
  data/shaders/fxaa.frag
#   data/shaders/simple_fxaa.frag
)

set(SHADER_HEADERS "")

foreach(SHADER_RELATIVE_SOURCE ${SHADER_SOURCES})
  set(SHADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_RELATIVE_SOURCE}")
  get_filename_component(SHADER_NAME ${SHADER_RELATIVE_SOURCE} NAME)
  string(REPLACE "." "_" SHADER_VARIABLE ${SHADER_NAME})
  set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${SHADER_NAME}.h")
  add_custom_command(
    OUTPUT "${SHADER_HEADER}"
    COMMAND gfdev_source_file "${SHADER_SOURCE}" "${SHADER_HEADER}" "${SHADER_VARIABLE}"
    COMMENT "Compiling ${SHADER_NAME} to header"
    DEPENDS gfdev_source_file "${SHADER_SOURCE}"
  )
  list(APPEND SHADER_HEADERS ${SHADER_HEADER})
endforeach()

add_custom_target(gf0_shader_headers ALL DEPENDS ${SHADER_HEADERS})

# build library

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (NOT APPLE)
  if (GF_BUILD_WITH_OPENGL3)
    set(GF_GLAD_SRC "vendor/glad/gl33/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gl33/include")
  else()
    set(GF_GLAD_SRC "vendor/glad/gles20/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gles20/include")
  endif()
endif()

if (GF_SINGLE_COMPILTATION_UNIT)
  add_library(gf0
    XUnityBuild.cc
  )
else()
  add_library(gf0
    Action.cc
    Activities.cc
    Activity.cc
    AnimatedSprite.cc
    Animation.cc
    AssetManager.cc
    BasicSprite.cc
    BasicText.cc
    Blend.cc
    BufferedGeometry.cc
    Circ.cc
    Clipboard.cc
    Clock.cc
    Collision.cc
    Color.cc
    ColorRamp.cc
    Console.cc
    ConsoleFont.cc
    Control.cc
    Controls.cc
    Coordinates.cc
    Cursor.cc
    Curve.cc
    Curves.cc
    Direction.cc
    Drawable.cc
    Easings.cc
    Effects.cc
    Entity.cc
    EntityContainer.cc
    Flags.cc
    Font.cc
    Gamepad.cc
    Geometry.cc
    GraphicsInfo.cc
    Grid.cc
    Heightmap.cc
    Image.cc
    Keyboard.cc
    Library.cc
    Log.cc
    Logo.cc
    Map.cc
    Math.cc
    Matrix.cc
    MessageManager.cc
    Model.cc
    ModelContainer.cc
    Models.cc
    Monitor.cc
    Move.cc
    NinePatch.cc
    Noise.cc
    Noises.cc
    Orientation.cc
    Particles.cc
    Paths.cc
    Path.cc
    PhysicsBody.cc
    PhysicsGeometry.cc
    PhysicsModel.cc
    Polygon.cc
    Polyline.cc
    PostProcessing.cc
    Random.cc
    RandomBinaryTree.cc
    Range.cc
    Rect.cc
    RenderPipeline.cc
    RenderTarget.cc
    RenderTexture.cc
    RenderWindow.cc
    ResourceManager.cc
    Serialization.cc
    SerializationOps.cc
    Shader.cc
    Shape.cc
    Shapes.cc
    Sleep.cc
    Spatial.cc
    SpriteBatch.cc
    Sprite.cc
    Stream.cc
    Streams.cc
    StringUtils.cc
    SystemInfo.cc
    Text.cc
    TextureAtlas.cc
    Texture.cc
    TileLayer.cc
    Time.cc
    Tmx.cc
    Transformable.cc
    Transform.cc
    Triangulation.cc
    UI.cc
    Vector.cc
    Vertex.cc
    VertexArray.cc
    VertexBuffer.cc
    View.cc
    ViewContainer.cc
    Views.cc
    Widget.cc
    Widgets.cc
    WidgetContainer.cc
    Window.cc
    # generated
    data/generated.cc
    # priv
    priv/Debug.cc
    # vendor
    vendor/pugixml/src/pugixml.cpp
    ${GF_GLAD_SRC}
  )
endif()

add_dependencies(gf0
  gf0_shader_headers
  gf0_gamecontrollerdb
)

if (GF_BUILD_WITH_OPENGL3)
  target_compile_definitions(gf0
    PRIVATE GF_OPENGL3
  )
endif()

if (GF_DEBUG)
  target_compile_definitions(gf0
    PRIVATE GF_DEBUG
  )
endif()

if (GF_SHARED)
  target_compile_definitions(gf0
    PUBLIC GF_SHARED
  )
endif()

target_compile_definitions(gf0
  PRIVATE
    GF_EXPORT_SYMBOLS
    ZLIB_CONST
)

target_include_directories(gf0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
  PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

if (APPLE)
  target_include_directories(gf0
    PRIVATE
      ${OPENGL_INCLUDE_DIR}
  )
else()
  target_include_directories(gf0
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/${GF_GLAD_INCLUDE_DIR}"
  )
endif()

target_link_libraries(gf0
  ${SDL2_LIBRARY}
  ${Boost_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

if (APPLE)
  target_link_libraries(gf0
    ${OPENGL_gl_LIBRARY}
  )
endif()

# workaround for Travis-CI
# see: https://bugs.launchpad.net/ubuntu/+source/gcc-5/+bug/1568899
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(gf0
    gcc_s
    gcc
  )
endif()

set_target_properties(gf0
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS gf0
  EXPORT gfConfig
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32 AND BUILD_SHARED_LIBS)
  install(
    TARGETS gf0
    RUNTIME DESTINATION "${CMAKE_INSTALL_DOCDIR}/examples"
  )
  install(
    TARGETS gf0
    RUNTIME DESTINATION games
  )
endif()

install(
  EXPORT gfConfig
  NAMESPACE gf::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)
