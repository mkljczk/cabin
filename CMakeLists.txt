cmake_minimum_required(VERSION 3.10)
project(cabin C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

add_subdirectory(.third-party/json)
add_subdirectory(.third-party/sqlitecpp)
add_subdirectory(.third-party/spdlog)
add_subdirectory(.third-party/dbg-macro)

include_directories(include)
include_directories(.third-party/uWebSockets/src)

set(SOURCES 
  .third-party/httplib.cc

  src/main.cc
  src/http.cc
  src/server.cc
  src/database.cc
  src/util.cc

  src/entities/Emoji.cc
  src/entities/EmojiReact.cc
  src/entities/Follow.cc
  src/entities/Like.cc
  src/entities/Note.cc
  src/entities/User.cc
  src/entities/Notification.cc

  src/services/UserService.cc
  src/services/NoteService.cc
  src/services/IngestService.cc
  src/services/DeliveryService.cc
  src/services/FollowService.cc
  src/services/InstanceService.cc
  src/services/NotificationService.cc
  src/services/FetchService.cc
  src/services/BiteService.cc

  src/routes/federation/webfinger.cc
  src/routes/federation/meta.cc
  src/routes/federation/notes.cc
  src/routes/federation/instance.cc

  src/routes/web/main.cc

  src/routes/mastodon/apps.cc
  src/routes/mastodon/accounts.cc
  src/routes/mastodon/statuses.cc

  src/routes/pleroma/frontend.cc
  src/routes/pleroma/emojis.cc

  src/migrations/1-init.cc
)

add_compile_options(-fsanitize=address -rdynamic)
add_link_options(-fsanitize=address -rdynamic)
add_executable(cabin ${SOURCES})

target_link_libraries(cabin nlohmann_json::nlohmann_json OpenSSL::SSL SQLiteCpp dbg_macro usockets cpptrace dwarf z fmt)
