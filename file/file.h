/**
 * file helper
 * @author Tobias Weber <tobias.weber@tum.de>
 * @date 2013-2016
 * @license GPLv2 or GPLv3
 */

#ifndef __TLIB_FILE_HELPER__
#define __TLIB_FILE_HELPER__

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <type_traits>


namespace tl {

namespace fs = boost::filesystem;

template<typename t_char=char>
std::streampos get_file_size(std::basic_istream<t_char>& istr)
{
	std::streampos iPos = istr.tellg();

	istr.seekg(0, std::ios_base::end);
	std::streampos iSize = istr.tellg();
	istr.seekg(iPos, std::ios_base::beg);

	return iSize;
}


template<typename t_char=char>
std::streampos get_file_pos(std::basic_istream<t_char>& istr)
{
	std::streampos iPos = istr.tellg();

	if(iPos < 0) return 0;
	return iPos;
}


template<typename t_char=char> static inline
std::size_t get_file_size(const std::basic_string<t_char>& _strFile)
{
	using t_char_fs = fs::path::value_type;
	std::basic_string<t_char_fs> strFile(_strFile.begin(), _strFile.end());

	return fs::file_size(fs::path(strFile));
}
template<>
inline std::size_t get_file_size(const std::basic_string<typename fs::path::value_type>& strFile)
{
	return fs::file_size(fs::path(strFile));
}

// ----------------------------------------------------------------------------


template<typename t_char=char>
bool dir_exists(const t_char* pcDir)
{
	fs::path path(pcDir);
	bool bExists = fs::exists(path);
	bool bIsDir = fs::is_directory(path);

	return bExists && bIsDir;
}

template<typename t_char=char>
bool file_exists(const t_char* pcDir)
{
	fs::path path(pcDir);
	bool bExists = fs::exists(path);
	bool bIsDir = fs::is_directory(path);
	bool bIsFile = fs::is_regular_file(path);
	bool bIsLink = fs::is_symlink(path);

	return bExists && (bIsFile || bIsLink) && !bIsDir;
}


// ----------------------------------------------------------------------------


// iterates over all files in a directory
template<bool bRecursive=0, class t_char = char,
	template<class...> class t_cont = std::vector>
t_cont<std::basic_string<t_char>> get_all_files(const t_char* pcPath)
{
	t_cont<std::basic_string<t_char>> vecFiles;
	if(!dir_exists(pcPath))
		return vecFiles;

	using t_iter = typename std::conditional<bRecursive,
		fs::recursive_directory_iterator,
		fs::directory_iterator>::type;

	fs::path path(pcPath);
	t_iter iter(path), iterEnd;
	for(; iter!=iterEnd; ++iter)
	{
		const fs::directory_entry& entry = *iter;
		if(!fs::is_directory(entry))
			vecFiles.push_back(entry.path().string());
	}

	return vecFiles;
}

// ----------------------------------------------------------------------------


/**
 * attempts to find a file in the given choice of paths
 */
template<class t_str = std::string, template<class...> class t_cont = std::vector>
std::tuple<bool, t_str> find_file(const t_cont<t_str>& vecPaths, const t_str& strFile)
{
	fs::path filepath(strFile);

	// if path is already absolute, use it directly
	if(filepath.is_absolute())
	{
		bool bExists = fs::exists(filepath);
		return std::make_tuple(bExists, t_str(filepath.string()));
	}

	for(const t_str& strPath : vecPaths)
	{
		fs::path path(strPath);
		path /= filepath;

		if(fs::exists(path))
			return std::make_tuple(true, t_str(path.string()));
	}

	// nothing found
	return std::make_tuple(false, t_str(""));
}

}

#endif
