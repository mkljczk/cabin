#define USE_DB
#include "ingest.h"

#include "user.h"
#include "note.h"
#include "../utils.h"
#include <cpptrace/from_current.hpp>

namespace IngestService {


  void Ingest(json body) {
    std::string type = body["type"];
    info("starting ingest of {}", type);

    body["@context"] = "";
    std::cout << body.dump() << "\n";


    if (type == "Create") {
      auto object = body["object"];
      if (object["type"] != "Note") return;

      if (NoteService::lookup(object["id"]).has_value()) {
        return;
      }

      URL noteuri(object["id"]);
      UserService::fetchRemote(object["attributedTo"]);
      Note n = {
        .uri = object["id"],
        .id = utils::genid(),
        .local = false,
        .host = noteuri.host,

        .replyToUri = object["inReplyTo"].is_null() ? nullopt : std::make_optional(object["inReplyTo"]),
        .content = object["content"],
        // .cw = object["content"],
        .sensitive = object["sensitive"],
        .owner = object["attributedTo"],
        .published = utils::isoToMillis(object["published"]),

        .lastUpdatedAt = utils::millis(),

        // .remoteLikeCount
      };

      if (!n.insert()) {
        throw std::runtime_error("");
      }
    } else if (type == "Announce") {
      string object = body["object"];
      Note note = NoteService::fetchRemote(object);
    }
  }

  void QueueIngest(json activity) {
    std::thread t([activity](){
      CPPTRACE_TRY {
        Ingest(activity);    
      } CPPTRACE_CATCH(const std::exception &e) {
          error("ingest failed: {}", e.what());
          auto t = cpptrace::from_current_exception();

          t.frames.erase(std::remove_if(t.frames.begin(), t.frames.end(), [](auto f) {
            return f.filename.find(".third-party") != std::string::npos || f.filename.find("/usr") != std::string::npos;
          }), t.frames.end());

          t.print_with_snippets();
      }
    });
    t.detach();
  }
}