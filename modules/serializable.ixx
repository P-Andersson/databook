module;

#include <concepts>

export module databook.serializable;

namespace databook {

   export template<typename T, typename NotebookT>
      concept FreeFunctionStorable = false;

   export template<typename T, typename NotebookT>
      concept FreeFunctionLoadable = false;

   export template<typename T, typename NotebookT>
      concept FreeFunctionSerializable = false;

   export template<typename T, typename NotebookT>
      concept MemberFunctionStorable = requires(T & a, NotebookT & notebook)
   {
      { a.store(notebook) };
   };

   export template<typename T, typename NotebookT>
      concept MemberFunctionLoadable = requires(T & a, NotebookT & notebook)
   {
      { T::load(notebook) } -> std::same_as<T>;
   };

   export template<typename T, typename NotebookT>
      concept MemberFunctionSerializable = requires(T & a, NotebookT & notebook)
   {
      { a.serialize(notebook) };
   };


   export template<typename T, typename NotebookT>
      concept Storable = FreeFunctionStorable<T, NotebookT> || MemberFunctionStorable<T, NotebookT> || FreeFunctionSerializable<T, NotebookT> || MemberFunctionSerializable<T, NotebookT>;

   export template<typename T, typename NotebookT>
      concept Loadable = MemberFunctionLoadable<T, NotebookT> || MemberFunctionStorable<T, NotebookT> || FreeFunctionLoadable<T, NotebookT> || MemberFunctionSerializable<T, NotebookT>;

   export template<typename T, typename NotebookT>
      concept Serializable = Storable<T, NotebookT>&& Loadable<T, NotebookT>;

}