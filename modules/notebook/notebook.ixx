module;

#include <concepts>
#include <string>
#include <cstdint>
#include <memory>
#include <stack>
#include <variant>

export module databook.notebook;

import databook.notebook.interface;
import databook.serializable;

namespace databook {

   template <typename T, typename... U>
   concept IsAnyOf = (std::same_as<T, U> || ...);

   export template<typename T>
      concept IsArray = std::is_array<T>::value;

   export template<typename T>
      concept PrimitiveSerializable = std::is_integral_v<T> || std::is_floating_point_v<T>;

   export template<typename T, typename NotebookT>
      concept CompositeSerializable = Serializable<T, NotebookT>;

   export template<typename T, typename NotebookT>
      concept AnySerializable = !IsArray<T> && (PrimitiveSerializable<T> || CompositeSerializable<T, NotebookT>);

   export class Notebook {
   public:
      Notebook(std::unique_ptr<WritingNotebookInterface>&& writing_databook) : _state(Writing(std::move(writing_databook))) {

      }

      Notebook(std::unique_ptr<LoadingNotebookInterface>&& loading_databook) : _state(Loading(std::move(loading_databook))) {

      }

      void serialize(const NotebookKey& key, std::string& value) {
         if (auto writable = std::get_if<Writing>(&_state)) {
            writable->write(key, value, *this);
         } else if (auto loadable = std::get_if<Loading>(&_state)) {
            loadable->load(key, value, *this);
         }
      }

      void write(const NotebookKey& key, std::string& value) {
         if (auto writable = std::get_if<Writing>(&_state)) {
            writable->write(key, value, *this);
         }
      }

      void load(const NotebookKey& key, std::string& value) {
         if (auto loadable = std::get_if<Loading>(&_state)) {
            loadable->load(key, value, *this);
         }
      }


      template<AnySerializable<Notebook> SerializableT>
      void serialize(const NotebookKey& key, SerializableT& value) {
         if (auto writable = std::get_if<Writing>(&_state)) {
            writable->write(key, value, *this);
         }
      }

      template<AnySerializable<Notebook> SerializableT>
      void write(const NotebookKey& key, SerializableT& value) {
         if (auto writable = std::get_if<Writing>(&_state)) {
            writable->write(key, value, *this);
         } else if (auto loadable = std::get_if<Loading>(&_state)) {
            loadable->load(key, value, *this);
         }
      }


      template<AnySerializable<Notebook> SerializableT>
      void load(const NotebookKey& key, SerializableT& value) {
         if (auto loadable = std::get_if<Loading>(&_state)) {
            loadable->load(key, value, *this);
         }
      }

      void finish()&& {
         if (auto writable = std::get_if<Writing>(&_state)) {
            std::move(*writable).finish();
         }
      }

   private:
      class Writing {
      public:
         Writing(std::unique_ptr<WritingNotebookInterface>&& root) {
            _notebook_stack.push(std::move(root));
         }

         void write(const NotebookKey& key, const std::string& value, Notebook& notebook) {
            _notebook_stack.top()->write(key, value);
         }

         template<PrimitiveSerializable PrimitiveT>
         void write(const NotebookKey& key, const PrimitiveT& value, Notebook& notebook) {
            _notebook_stack.top()->write(key, value);
         }

         template<CompositeSerializable<Notebook> CompositeT>
         void write(const NotebookKey& key, CompositeT& value, Notebook& notebook) {
            _notebook_stack.push(_notebook_stack.top()->writeComposite(key));
            writeInner(value, notebook);
            finish();
         }

         void finish() {
            std::move(*_notebook_stack.top()).finish();
            _notebook_stack.pop();
         }

      private:
         template<FreeFunctionStorable<Notebook> CompositeT>
         void writeInner(CompositeT& value, Notebook& notebook) {
            store<CompositeT>(value, notebook);
         }

         template<MemberFunctionStorable<Notebook> CompositeT>
         void writeInner(CompositeT& value, Notebook& notebook) {
            value.store(notebook);
         }

         template<FreeFunctionSerializable<Notebook> CompositeT>
         void writeInner(CompositeT& value, Notebook& notebook) {
            serialize(value, notebook);
         }

         template<MemberFunctionSerializable<Notebook> CompositeT>
         void writeInner(CompositeT& value, Notebook& notebook) {
            value.serialize(notebook);
         }

         std::stack<std::unique_ptr<WritingNotebookInterface>> _notebook_stack;
      };

      class Loading {
      public:
         Loading(std::unique_ptr<LoadingNotebookInterface>&& root) {
            _notebook_stack.push(std::move(root));
         }

         void load(const NotebookKey& key, std::string& value, Notebook& notebook) {
            _notebook_stack.top()->load(key, value);
         }

         template<PrimitiveSerializable PrimitiveT>
         void load(const NotebookKey& key, PrimitiveT& value, Notebook& notebook) {
            _notebook_stack.top()->load(key, value);
         }

         template<CompositeSerializable<Notebook> CompositeT>
         void load(const NotebookKey& key, CompositeT& value, Notebook& notebook) {
            auto res = _notebook_stack.top()->loadComposite(key);
            if (res.has_value()) {
               _notebook_stack.push(std::move(res.value()));
               loadInner(value, notebook);
               finish();
            }
         }

         void finish() {
            std::move(*_notebook_stack.top()).finish();
            _notebook_stack.pop();
         }

      private:
         template<FreeFunctionLoadable<Notebook> CompositeT>
         void loadInner(CompositeT& value, Notebook& notebook) {
            load<CompositeT>(value, notebook);
         }

         template<MemberFunctionLoadable<Notebook> CompositeT>
         void loadInner(CompositeT& value, Notebook& notebook) {
            value = CompositeT::load(notebook);
         }

         template<FreeFunctionSerializable<Notebook> CompositeT>
         void loadInner(CompositeT& value, Notebook& notebook) {
            serialize(value, notebook);
         }

         template<MemberFunctionSerializable<Notebook> CompositeT>
         void loadInner(CompositeT& value, Notebook& notebook) {
            value.serialize(notebook);
         }

         std::stack<std::unique_ptr<LoadingNotebookInterface>> _notebook_stack;
      };

      using States = std::variant<Writing, Loading>;

      States _state;
   };

}