module;

#include <variant>
#include <unordered_map>
#include <string>
#include <memory>
#include <expected>

export module databook.notebook.memory;

import databook.notebook.interface;

namespace databook {

   export class MemoryNode {
   public:
      using NodeData = std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double, bool, char, std::string, std::shared_ptr<MemoryNode>>;

      std::unordered_map<NotebookKey, NodeData> children;
   };

   export class WritingNotebookMemory : public WritingNotebookInterface {
   public:
      WritingNotebookMemory()
         : _node(std::make_shared<MemoryNode>()) {

      }

      ~WritingNotebookMemory() {
         // Populate parent once out of scope
         if (_parent) {
            _parent->children.emplace(_name, _node);
         }
      }

      const std::shared_ptr<MemoryNode> data() const {
         return _node;
      }

      void write(const NotebookKey& key, int8_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, int16_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, int32_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, int64_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }

      void write(const NotebookKey& key, uint8_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, uint16_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, uint32_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, uint64_t val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }

      void write(const NotebookKey& key, float val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, double val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }

      void write(const NotebookKey& key, bool val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, char val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }
      void write(const NotebookKey& key, const std::string& val) override {
         _node->children.emplace(key, MemoryNode::NodeData(val));
      }

      std::unique_ptr<WritingNotebookInterface> writeComposite(const NotebookKey& key) override {
         return std::unique_ptr<WritingNotebookMemory>(new WritingNotebookMemory(std::shared_ptr<MemoryNode>(_node), NotebookKey(key)));
      }


      void finish() && override {
         _parent->children.emplace(_name, _node);
      }
   private:
      WritingNotebookMemory(std::shared_ptr<MemoryNode>&& parent, std::string&& key_in_parent)
         : _node(std::make_shared<MemoryNode>())
         , _parent(std::move(parent))
         , _name(std::move(key_in_parent)) {

      }

      std::shared_ptr<MemoryNode> _node;
      std::shared_ptr<MemoryNode> _parent;
      std::string _name;
   };

   export class LoadingNotebookMemory : public LoadingNotebookInterface {
   public:
      LoadingNotebookMemory(std::shared_ptr<MemoryNode> &&src)
         : _node(std::move(src)) {

      }

      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, int8_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, int16_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, int32_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, int64_t& val) override {
         return load_primitive(key, val);
      }

      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, uint8_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, uint16_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, uint32_t& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, uint64_t& val) override {
         return load_primitive(key, val);
      }

      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, float& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, double& val) override {
         return load_primitive(key, val);
      }

      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, bool& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, char& val) override {
         return load_primitive(key, val);
      }
      std::expected<LoadOk, DeserializationFailure> load(const NotebookKey& key, std::string& val) override {
         return load_primitive(key, val);
      }

      std::expected<std::unique_ptr<LoadingNotebookInterface>, DeserializationFailure> loadComposite(const NotebookKey& key) override {
         if (auto found = _node->children.find(key); found != _node->children.end())
         {
            if (std::holds_alternative<std::shared_ptr<MemoryNode>>(found->second)) {
               return std::make_unique<LoadingNotebookMemory>(std::shared_ptr<MemoryNode>(std::get<std::shared_ptr<MemoryNode>>(found->second)));
            }
         }
         return std::unexpected(DeserializationFailure());
      }


      void finish() && override {
      }
   private:
      template<typename T>
      std::expected<LoadOk, DeserializationFailure> load_primitive(const NotebookKey& key, T& val) {
         if (auto found = _node->children.find(key); found != _node->children.end())
         {
            if (std::holds_alternative<T>(found->second)) {
               val = std::get<T>(found->second);
               return LoadOk();
            }
         }
         return LoadOk();
         //return std::unexpected(DeserializationFailure());
      }

      std::shared_ptr<MemoryNode> _node;
   };

}