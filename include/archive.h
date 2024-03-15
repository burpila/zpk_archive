#ifndef _ARCHIVE_H
#define _ARCHIVE_H
#include "zpk_config.h"
#include "file_stream.h"
#include "file_handle.h"

namespace zpk
{
	namespace consts
	{
		constexpr static inline uint32_t ZPK_ID = 0x50aa1441;
		constexpr static inline uint32_t ZPK_VERSION = 0x00000001;
	}

	struct ArchiveHeader_t
	{
		uint32_t m_signature;
		uint32_t m_version;
	};

	class Archive
	{
		friend class ArchiveWriter;
		friend class ArchiveReader;

		using Path = std::unordered_map<std::string, std::unordered_map<std::string, FileHandle*>>;
		using Directory = std::unordered_map<std::string, FileHandle*>;

		class FileDirectory : public FileHandle
		{
		public:
			FileDirectory() noexcept = default;
			FileDirectory(std::vector<uint8_t>& data) noexcept
				: FileHandle(data.size(), 0)
				, m_data(std::move(data))
			{}
			~FileDirectory() noexcept = default;

			// Clear m_data
			void clear() noexcept;

			std::vector<uint8_t> getData() const noexcept;
		private:
			std::vector<uint8_t> m_data;
		};
	public:
		constexpr Archive() noexcept = default;
		Archive(const std::string& path) noexcept;
		~Archive() noexcept;

		// You can open a file archive once and load many files
		void open() noexcept;
		void close() noexcept;

		void setPath(const std::string& path) noexcept;

		std::vector<uint8_t> loadFile(const std::string &path) const;
		std::vector<uint8_t> loadFile(FileHandle* file) const;

		// Find file
		FileHandle* findFile(const std::string& path) const;
		template <class _T> 
		_T* findFile(const std::string& path) const;

		// Trying to find files in '(main)' directory by default
		Directory findFiles(const std::string& path = "", const std::string& extension = "") const;

		bool isPathExists(const std::string& path) const;
		// More sense use findFile instead isFileExists
		bool isFileExists(const std::string& path) const;
		
		Path getPaths() const noexcept;
		uint32_t getFilesCount() const noexcept;
		uint32_t getSize() const noexcept;
		uint32_t getDirectorySize(Directory directory) const noexcept;
	private:
		void writeDirectory(const std::string& archive_name, std::string& path);
		void writeDirectory(const std::string& path, const std::string& name, FileHandle* file);
	private:
		ArchiveHeader_t m_header {};
		Path m_paths {};
		std::string m_fullPath {};

		static FileStream m_fileStream;
	};

	template <class _T>
	inline _T* Archive::findFile(const std::string& path) const
	{
		return static_cast<_T*>(findFile(path));
	}
}

#endif // !_ARCHIVE_H
