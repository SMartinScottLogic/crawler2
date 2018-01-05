
 	
/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <string.h>
#include <microhttpd.h>
#include <stdio.h>

#include <sstream>
#include <map>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

std::map<std::string, std::set<std::string>> have_seen;

#define PORT 8888
struct connection_data {
  connection_data():is_parsing(false),entity() {}

  bool is_parsing;
  std::stringstream entity;
};

int print_out_key(void * /*cls*/, enum MHD_ValueKind /*kind*/, const char *key, const char *value) {
  fprintf(stderr, "%s: %s\n", key, value);
  return MHD_YES;
}

void print_tree(const ptree &pt, int level) {
  if(pt.empty()) {
    fprintf(stderr, "value(%d): \"%s\"\n", level, pt.data().c_str());
    return;
  }
  for(auto entry : pt) {
    fprintf(stderr, "key(%d): \"%s\"\n", level, entry.first.c_str());
    print_tree(entry.second, level+1);
  }
}

static int
answer_to_connection (void * /*cls*/, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  static int call_count = 0;

  if(strcmp(method, MHD_HTTP_METHOD_POST) == 0) {
    connection_data *data = static_cast<connection_data*>(*con_cls);
    if(nullptr == data) {
      data = new connection_data();
      data->is_parsing = false;
      *con_cls = data;
    }

    if(!data->is_parsing) {
      data->is_parsing = true;
      return MHD_YES;
    }

    if(*upload_data_size != 0) {
      data->entity << std::string(upload_data, *upload_data_size);
      *upload_data_size = 0;
      return MHD_YES;
    }

    fprintf(stderr, "%s %s %s\n", method, url, version);

    MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);
    fprintf(stderr, "> %s\n", data->entity.str().c_str());

    std::string queue;
    std::string url;

    try {
      ptree pt;
      read_json(data->entity, pt);
      print_tree(pt, 1);
      queue = pt.get<std::string>("queue");
      url = pt.get<std::string>("url");
      fprintf(stderr, "queue: %s\n", queue.c_str());
      fprintf(stderr, "url: %s\n", queue.c_str());
      if(have_seen[queue].find(url) == have_seen[queue].end()) {
        fprintf(stderr, "add '%s' to queue '%s'\n", url.c_str(), queue.c_str());
        have_seen[queue].insert(url);
      } else {
        fprintf(stderr, "'%s' already in queue '%s'\n", url.c_str(), queue.c_str());
      }
    } catch(const boost::property_tree::json_parser::json_parser_error& e) {
      fprintf(stderr, "Invalid JSON: %s\n", e.what() );
    } catch(const boost::property_tree::ptree_bad_path& e) {
      fprintf(stderr, "Bad tree: %s\n", e.what());
    }
    delete data;
    *con_cls = nullptr;
  } else {
    fprintf(stderr, "%s %s %s\n", method, url, version);

    MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);
  }
  
  ptree pt;
  pt.put ("count", ++ call_count);
  pt.put ("hello", "world");
  std::ostringstream buf; 
  write_json (buf, pt, false);
  std::string json = buf.str();
 
  struct MHD_Response *response = MHD_create_response_from_buffer (json.length(), (void *) json.c_str(), MHD_RESPMEM_MUST_COPY);
  MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json");
  int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  fprintf(stderr, "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  return ret;
}


int
main ()
{
  struct MHD_Daemon *daemon;

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon)
    return 1;

  (void) getchar ();

  MHD_stop_daemon (daemon);
  return 0;
}
