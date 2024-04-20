module;

#include <functional>
#include <iostream>

#include <yaml-cpp/yaml.h>


export module databook.notebook.yaml;

import databook.notebook.interface;

export class WritingNotebookYaml : public WritingNotebookInterface {
public:
   WritingNotebookYaml(std::shared_ptr<std::ostream>&& stream) 
      : _node(std::make_shared<YAML::Node>())
      , _output(std::move(stream)) {

   }

   ~WritingNotebookYaml() {
      // Populate parent once out of scope
      if (_parent) {
         (*_parent)[_name] = *_node;
      }
   }

   void write(const NotebookKey& key, int8_t val) override {
      (*_node)[key] = static_cast<int64_t>(val);
   }
   void write(const NotebookKey& key, int16_t val) override {
      (*_node)[key] = static_cast<int64_t>(val);
   }
   void write(const NotebookKey& key, int32_t val) override {
      (*_node)[key] = static_cast<int64_t>(val);
   }
   void write(const NotebookKey& key, int64_t val) override {
      (*_node)[key] = static_cast<int64_t>(val);
   }

   void write(const NotebookKey& key, uint8_t val) override {
      (*_node)[key] = static_cast<uint64_t>(val);
   }
   void write(const NotebookKey& key, uint16_t val) override {
      (*_node)[key] = static_cast<uint64_t>(val);
   }
   void write(const NotebookKey& key, uint32_t val) override {
      (*_node)[key] = static_cast<uint64_t>(val);
   }
   void write(const NotebookKey& key, uint64_t val) override {
      (*_node)[key] = static_cast<uint64_t>(val);
   }

   void write(const NotebookKey& key, float val) override {
      (*_node)[key] = val;
   }
   void write(const NotebookKey& key, double val) override {
      (*_node)[key] = val;
   }

   void write(const NotebookKey& key, bool val) override {
      (*_node)[key] = val;
   }
   void write(const NotebookKey& key, char val) override {
      (*_node)[key] = val;
   }
   void write(const NotebookKey& key, const std::string& val) override {
      (*_node)[key] = val;
   }

   std::unique_ptr<WritingNotebookInterface> writeComposite(const NotebookKey& key) override {
      return std::unique_ptr<WritingNotebookYaml>(new WritingNotebookYaml(std::shared_ptr<YAML::Node>(_node), NotebookKey(key)));
   }


   void finish() && override {
      if (_output) {
         (*_output) << *_node;
      }
   }
private:
   WritingNotebookYaml(std::shared_ptr<YAML::Node>&& parent, std::string&& key_in_parent) 
      : _node(std::make_shared<YAML::Node>())
      , _parent(std::move(parent))
      , _name(std::move(key_in_parent)) {

   }

   std::shared_ptr<YAML::Node> _node;
   std::shared_ptr<YAML::Node> _parent;
   std::string _name;

   std::shared_ptr<std::ostream> _output;
};