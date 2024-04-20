module;

#include <concepts>

export module databook.serializable;

export template<typename T, typename NotebookT>
concept MemberFunctionStorable = requires(const T& a, NotebookT & notebook)
{
   { a.store(notebook) };
};


export template<typename T, typename NotebookT>
concept FreeFunctionStorable = requires(const T& a, NotebookT & notebook)
{
   { store(a, notebook) };
};

export template<typename T, typename NotebookT>
concept MemberFunctionLoadable = requires(const T& a, NotebookT & notebook)
{
   { T::load(notebook) } -> std::same_as<T>;
};


export template<typename T, typename NotebookT>
concept FreeFunctionLoadable = requires(const T& a, NotebookT & notebook)
{
   { load<T>(notebook) } -> std::same_as<T>;
};

export template<typename T, typename NotebookT>
concept MemberFunctionSerializable = requires(const T& a, NotebookT & notebook)
{
   { a.serialize(notebook) };
};


export template<typename T, typename NotebookT>
concept FreeFunctionSerializable = requires(T& a, NotebookT & notebook)
{
   { serialize(a, notebook) };
};


export template<typename T, typename NotebookT>
concept Storable = FreeFunctionStorable<T, NotebookT> || MemberFunctionStorable<T, NotebookT> || FreeFunctionSerializable<T, NotebookT> || MemberFunctionStorable<T, NotebookT>;

export template<typename T, typename NotebookT>
concept Loadable = MemberFunctionLoadable<T, NotebookT> || MemberFunctionStorable<T, NotebookT> || FreeFunctionLoadable<T, NotebookT> || MemberFunctionStorable<T, NotebookT>;

export template<typename T, typename NotebookT>
concept Serializable = Storable<T, NotebookT> && Loadable<T, NotebookT>;
