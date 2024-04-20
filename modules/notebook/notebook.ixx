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
         writable->serializeWrite(key, value, *this);
      }
   }

   void write(const NotebookKey& key, const std::string& value) {
      if (auto writable = std::get_if<Writing>(&_state)) {
         writable->write(key, value, *this);
      }
   }

   template<AnySerializable<Notebook> SerializableT>
   void serialize(const NotebookKey& key, SerializableT& value) {
      if (auto writable = std::get_if<Writing>(&_state)) {
         writable->serializeWrite(key, value, *this);
      }
   }

   template<AnySerializable<Notebook> SerializableT>
   void write(const NotebookKey& key, const SerializableT& value) {
      if (auto writable = std::get_if<Writing>(&_state)) {
         writable->write(key, value, *this);
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
      void write(const NotebookKey& key, const CompositeT& value, Notebook& notebook) {
         _notebook_stack.push(_notebook_stack.top()->writeComposite(key));
         writeInner(value, notebook);
         finish();
      }

      void serializeWrite(const NotebookKey& key, std::string& value, Notebook& notebook) {
         _notebook_stack.top()->write(key, value);
      }

      template<PrimitiveSerializable PrimitiveT>
      void serializeWrite(const NotebookKey& key, PrimitiveT& value, Notebook& notebook) {
         _notebook_stack.top()->write(key, value);
      }

      template<CompositeSerializable<Notebook> CompositeT>
      void serializeWrite(const NotebookKey& key, CompositeT& value, Notebook& notebook) {
         _notebook_stack.push(_notebook_stack.top()->writeComposite(key));
         serializeWriteInner(value, notebook);
         finish();
      }

      void finish() {
         std::move(*_notebook_stack.top()).finish();
         _notebook_stack.pop();
      }

   private:
      template<FreeFunctionStorable<Notebook> CompositeT>
      void writeInner(const CompositeT& value, Notebook& notebook) {
         store(value, notebook);
      }

      template<MemberFunctionStorable<Notebook> CompositeT>
      void writeInner(const CompositeT& value, Notebook& notebook) {
         value.store(notebook);
      }

      template<FreeFunctionSerializable<Notebook> CompositeT>
      void serializeWriteInner(CompositeT& value, Notebook& notebook) {
         serialize(value, notebook);
      }

      template<MemberFunctionSerializable<Notebook> CompositeT>
      void serializeWriteInner(CompositeT& value, Notebook& notebook) {
         value.serialize(notebook);
      }

      std::stack<std::unique_ptr<WritingNotebookInterface>> _notebook_stack;
   };

   class Loading {
   public:
      Loading(std::unique_ptr<LoadingNotebookInterface>&& root) {
         _notebook_stack.push(std::move(root));
      }

   private:
      std::stack<std::unique_ptr<LoadingNotebookInterface>> _notebook_stack;
   };

   using States = std::variant<Writing, Loading>;

   States _state;
};
