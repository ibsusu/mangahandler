#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include "includes/browse.hh"

/*
int linkToParent(uuid_Resource_Node* node){
        node->parent_node->subresources.push_back(node);
        node->parent_node->subresource_uuids.push_back(node->id);
        return 0;
}

uuid_Resource_Node* createNode(std::string id, uuid_Resource_Node* parent,
std::string name){
        uuid_Resource_Node* node = new uuid_Resource_Node();

        uuid_parse(id.c_str(), node->id);
        node->name = name;
        if (node->name != std::string("/")){
                node->parent_str = parent->name;
                node->parent_node = parent;
                uuid_parse(parent->id_str.c_str(), node->parent);
        }

        return node;
}*/
/*
uuid_Resource_Map_Combo* createDirectoryMapFromList(std::vector<std::string>
directory_list){
        using namespace std;

        unordered_map<string,uuid_Resource_Node*>* uuid_directory_map = new
unordered_map<string,uuid_Resource_Node*>();
        unordered_map<string, uuid_t>* name_to_uuid_link = new
unordered_map<string, uuid_t>();
        string segment;
        string uid;
        char charu[37];
        // fill them
        uuid_t root;
        uuid_generate(root);
        uuid_unparse(root, charu);
        uuid_Resource_Node* root_node = createNode(charu, NULL, string("/"));
        uuid_directory_map->insert({string(charu), root_node});

        for (int i=0;i<directory_list.size();i++){
                stringstream segmenter(directory_list[i]);

                uuid_t testu;
                int count = 0;
                uuid_Resource_Node* last;
                // setting last to root uuid
                while(getline(segmenter, segment, '/')){
                        if (segment.compare(".") == 0){
                                continue;
                        }

                        if(name_to_uuid_link->find(segment) ==
name_to_uuid_link->end()){
                                uuid_generate(testu);

                                uuid_unparse(testu, charu);
                                // create resource node
                                uuid_Resource_Node* parent_select = count > 0 ?
last : root_node;
                                uuid_Resource_Node* temp_node =
createNode(charu, parent_select, segment);

                                // save to map
                                uuid_directory_map->insert({temp_node->id_str,
temp_node});
                                name_to_uuid_link->insert({temp_node->name,
temp_node});

                                // link to parent for that Tree delightfulness
                                linkToParent(temp_node);
                        }

                        count++;
                        last =
uuid_directory_map(name_to_uuid_link->find(segment)->second);
                }
        }
        uuid_Resource_Map_Combo* output = new uuid_Resource_Map_Combo();
        output->uuid_nodes = uuid_directory_map;
        output->name_to_uuid = name_to_uuid_link;
        return output;
}

std::vector<std::string> loadTitlesFromList(){
        using namespace std;
        ifstream directory_file("listings.txt");
        string line;
        vector<string> directory_list;
        cout << "entering loadTitlesFromList" << endl;
        if (directory_file.is_open()){
                while(std::getline(directory_file, line)){
                        directory_list.push_back(line);
                }
                for (auto i : directory_list){
                        cout << i << endl;
                }
                directory_file.close();
        }
        return directory_list;
}

int loadTitles(){
        return 0;
}

int check_for_updated_paths(){
        return 0;
}
int load_updated_paths(){return 0;}

int add_title(){return 0;}
int hide_title(){return 0;}
int remove_title(){return 0;}
int swap_trees(){return 0;}
int swap_titles(){return 0;}
std::string get_sub_directories(uint64_t location){
        return std::string("hoho");
}
std::string get_title_info(uint64_t location){
        return std::string("hoho");
}*/
