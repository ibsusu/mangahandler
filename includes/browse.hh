#include <stdio.h>
#include <cjson/cJSON.h>
#include "client_buffer_generated.h"
#include "server_buffer_generated.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <uuid/uuid.h>



struct uuid_Resource_Node{
	uuid_t id;
	uuid_t parent;
	std::string id_str;
	std::string parent_str;
	std::string name;
	uuid_Resource_Node* parent_node;
	bool banned = false;
	std::vector<uuid_t> subresource_uuids;
	std::vector<uuid_Resource_Node*> subresources;
};

struct uuid_Resource_Map_Combo{
	std::unordered_map<std::string, uuid_Resource_Node*>* uuid_nodes;
	std::unordered_map<std::string, uuid_Resource_Node*>* name_to_uuid;
};

uuid_Resource_Map_Combo* createDirectoryMapFromList(std::vector<std::string> directory_list);
int check_for_updated_paths();
int load_updated_paths();
int loadTitles();
std::vector<std::string> loadTitlesFromList();
int add_title();
int hide_title();
int remove_title();
int swap_trees();
int swap_titles();
std::string get_sub_directories(uint64_t location);
std::string get_title_info(uint64_t title);