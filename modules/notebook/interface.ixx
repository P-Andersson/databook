module;

#include <concepts>
#include <string>
#include <cstdint>
#include <memory>

export module databook.notebook.interface;

export using NotebookKey = std::string;

export class WritingNotebookInterface {
public:
   virtual ~WritingNotebookInterface() = default;

   virtual void write(const NotebookKey& key, int8_t) = 0;
   virtual void write(const NotebookKey& key, int16_t) = 0;
   virtual void write(const NotebookKey& key, int32_t) = 0;
   virtual void write(const NotebookKey& key, int64_t) = 0;

   virtual void write(const NotebookKey& key, uint8_t) = 0;
   virtual void write(const NotebookKey& key, uint16_t) = 0;
   virtual void write(const NotebookKey& key, uint32_t) = 0;
   virtual void write(const NotebookKey& key, uint64_t) = 0;

   virtual void write(const NotebookKey& key, float) = 0;
   virtual void write(const NotebookKey& key, double) = 0;

   virtual void write(const NotebookKey& key, bool) = 0;
   virtual void write(const NotebookKey& key, char) = 0;
   virtual void write(const NotebookKey& key, const std::string&) = 0;

   virtual std::unique_ptr<WritingNotebookInterface> writeComposite(const NotebookKey& key) = 0;


   virtual void finish() && = 0;
};

export class LoadingNotebookInterface {
public:
   virtual ~LoadingNotebookInterface() = default;

   virtual void load(const NotebookKey& key, int8_t&) = 0;
   virtual void load(const NotebookKey& key, int16_t&) = 0;
   virtual void load(const NotebookKey& key, int32_t&) = 0;
   virtual void load(const NotebookKey& key, int64_t&) = 0;

   virtual void load(const NotebookKey& key, uint8_t&) = 0;
   virtual void load(const NotebookKey& key, uint16_t&) = 0;
   virtual void load(const NotebookKey& key, uint32_t&) = 0;
   virtual void load(const NotebookKey& key, uint64_t&) = 0;

   virtual void load(const NotebookKey& key, float&) = 0;
   virtual void load(const NotebookKey& key, double&) = 0;

   virtual void load(const NotebookKey& key, bool&) = 0;
   virtual void load(const NotebookKey& key, char&) = 0;
   virtual void load(const NotebookKey& key, std::string&) = 0;

   virtual std::unique_ptr<LoadingNotebookInterface> writeComposite(const NotebookKey& key) = 0;
};
