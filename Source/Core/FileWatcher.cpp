#include "FileWatcher.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <SFML/System/String.hpp>

#include <windows.h>

#include <codecvt>
#include <iostream>
#include <list>
#include <locale>
#elif __linux__
#include <dirent.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/inotify.h>

#include <unordered_map>
#endif

namespace
{
#ifdef _WIN32
	void recurseDirectories(const std::wstring& folder, std::list<std::wstring>& subdirs)
	{
		WIN32_FIND_DATAW find = { };
		HANDLE findHandle = FindFirstFileExW((folder + L"\\*").c_str(), FindExInfoStandard, &find, FindExSearchNameMatch, NULL, 0);
		if (findHandle == INVALID_HANDLE_VALUE)
			return;

		do
		{
			std::wstring name = find.cFileName;

			if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				(name != L"." && name != L".."))
			{
				subdirs.push_back(folder + L"\\" + name);
				recurseDirectories(subdirs.back(), subdirs);
			}

			if (!FindNextFileW(findHandle, &find))
				break;
		} while (findHandle != INVALID_HANDLE_VALUE);
	}

	std::string getError()
	{
		LPVOID lpMsgBuf = NULL;

		// Search for the message description in the std windows
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);

		std::string msg = (LPCTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);

		return msg;
	}

	struct ChangeSourceImpl : public FileWatcher::ChangeSource
	{
		typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;

		struct PriviledgeEnabler
		{
		public:
			PriviledgeEnabler()
			{
				LPCTSTR arPrivelegeNames[] = {
					SE_BACKUP_NAME,
					SE_RESTORE_NAME,
					SE_CHANGE_NOTIFY_NAME
				};

				for (int i = 0; i < sizeof(arPrivelegeNames) / sizeof(arPrivelegeNames[0]); ++i)
				{
					if (!EnablePrivilege(arPrivelegeNames[i], TRUE))
					{
#ifndef NDEBUG
						std::cerr << "[WATCH] Unable to enable privilege: " << arPrivelegeNames[i] << " - " << getError();
#endif
					}
				}
			}

			bool EnablePrivilege(const char* name, BOOL enable)
			{
				bool success = false;
				HANDLE hToken;

				if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
				{    
					TOKEN_PRIVILEGES tp = { 1 };
					
					if (LookupPrivilegeValue(NULL, name, &tp.Privileges[0].Luid))
					{
						tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;
						
						AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
						
						success = (GetLastError() == ERROR_SUCCESS);
					}

					CloseHandle(hToken);
				}
				return success;
			}
		};

		ChangeSourceImpl(const std::string& path, bool recurse):
			mRecurse(recurse)
		{
			static PriviledgeEnabler enable;

			Path = path;

			int flags = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
			std::wstring str = convert().from_bytes(path);

			Data mainDir = {
				path,
				CreateFileW(str.c_str(), FILE_LIST_DIRECTORY, flags, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL),
				{ },
				{ }
			};

			if (mainDir.Handle == INVALID_HANDLE_VALUE)
			{
				std::cerr << getError() << std::endl;
				return;
			}

			mainDir.Overlapped.hEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

			mWatches.push_back(mainDir);

			if (recurse)
			{
				std::list<std::wstring> subdirs;
				recurseDirectories(str, subdirs);

				for (auto& dir : subdirs)
				{
					// Only need to add additional watches for links
					if (!(GetFileAttributesW(dir.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT))
						continue;

					Data subdir = {
						convert().to_bytes(dir),
						CreateFileW(dir.c_str(), FILE_LIST_DIRECTORY, flags, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL),
						{ },
						{ }
					};
					subdir.Overlapped.hEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

					mWatches.push_back(subdir);
				}
			}

			for (auto& watch : mWatches)
			{
				auto ret = ReadDirectoryChangesW(watch.Handle,
					&watch.Buffer,
					sizeof(watch.Buffer),
					recurse,
					FILE_NOTIFY_CHANGE_LAST_WRITE,
					NULL,
					&watch.Overlapped,
					NULL);
			}
		}

		~ChangeSourceImpl()
		{
			for (auto& watch : mWatches)
			{
				CloseHandle(watch.Overlapped.hEvent);
				CancelIo(watch.Handle);
				CloseHandle(watch.Handle);
			}
		}

		void update(std::list<std::string>& list)
		{
			DWORD numBytes = 0;
			for (auto& watch : mWatches)
			{
				GetOverlappedResult(watch.Handle, &watch.Overlapped, &numBytes, FALSE);

				if (numBytes <= 0)
					continue;

				ResetEvent(watch.Overlapped.hEvent);

				int i = 0;
				do
				{
					FILE_NOTIFY_INFORMATION* pEntry = (FILE_NOTIFY_INFORMATION*)&watch.Buffer[i];
					i += pEntry->NextEntryOffset;

					if (pEntry->FileNameLength > 0)
					{
						std::string path;
						if (watch.Directory == Path)
							path = "";
						else
							path = watch.Directory.substr(Path.size() + 1) + '\\';

						std::string file = convert().to_bytes(pEntry->FileName, pEntry->FileName + pEntry->FileNameLength / sizeof(wchar_t));

						list.push_back(path + file);
					}
					

					if (pEntry->NextEntryOffset == 0)
					{
						break;
					}
				} while (true);

				CancelIo(watch.Handle);

				std::fill_n(watch.Buffer, sizeof(watch.Buffer), 0);
				ReadDirectoryChangesW(watch.Handle,
					&watch.Buffer,
					sizeof(watch.Buffer),
					mRecurse,
					FILE_NOTIFY_CHANGE_LAST_WRITE,
					NULL,
					&watch.Overlapped,
					NULL);
			}
		}

	private:
		struct Data
		{
			std::string Directory;
			HANDLE Handle;
			OVERLAPPED Overlapped;
			char Buffer[1024];
		};

		bool mRecurse;
		std::list<Data> mWatches;
	};
#else
	void recurseDirectories(const std::string& folder, std::list<std::string>& subdirs)
	{
		DIR* dp;
		if ((dp = opendir(folder.c_str())) == nullptr)
			return;

		dirent* ent;
		while ((ent = readdir(dp)))
		{
			if (!(ent->d_type & (DT_DIR | DT_LNK)))
				continue;

			std::string name = ent->d_name;
			if (name == "." || name == "..")
				continue;

			std::string path = folder + '/' + name;
			subdirs.push_back(path);

			recurseDirectories(path, subdirs);
		}

		closedir(dp);
	}

	struct ChangeSourceImpl : public FileWatcher::ChangeSource
	{
		ChangeSourceImpl(const std::string& path, bool recurse) :
			mInotify(inotify_init())
		{
			Path = path;
			int flags = IN_MODIFY;
			int fd = 0;

			fd = inotify_add_watch(mInotify, path.c_str(), flags);
			mFDs[fd] = path;

			if (!recurse)
				return;

			std::list<std::string> subdirs;
			recurseDirectories(path, subdirs);

			for (auto& subdir : subdirs)
			{
				fd = inotify_add_watch(mInotify, subdir.c_str(), flags);
				mFDs[fd] = subdir;
			}
		}

		~ChangeSourceImpl()
		{
			for (auto& fd : mFDs)
				inotify_rm_watch(mInotify, fd.first);
			close(mInotify);
		}

		void update(std::list<std::string>& list)
		{
			timeval tv;
			tv.tv_sec = tv.tv_usec = 0;

			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(mInotify, &fds);

			select(mInotify + 1, &fds, NULL, NULL, &tv);

			if (FD_ISSET(mInotify, &fds))
			{
				char buffer[1024];

				int i = 0, ret = read(mInotify, buffer, 1024);

				if (ret < 0)
					return;

				while (i < ret)
				{
					inotify_event* event = (inotify_event*)&buffer[i];
					if (event->len > 0 && mFDs.count(event->wd) > 0)
					{
						auto path = mFDs.at(event->wd);

						if (path == Path)
							path = "";
						else
							path = path.substr(Path.size() + 1) + '/';

						path += event->name;

						list.push_back(path);
					}

					i += sizeof(inotify_event) + event->len;
				}
			}
		}

	private:
		int mInotify;
		std::unordered_map<int, std::string> mFDs;
	};
#endif

	bool wildcmp(const char* wild, const char* string)
	{
		const char* cp = NULL, *mp = NULL;

		while ((*string) && (*wild != '*'))
		{
			if ((*wild != *string) && (*wild != '?'))
				return false;

			wild++;
			string++;
		}

		while (*string)
		{
			if (*wild == '*')
			{
				if (!*++wild)
					return true;

				mp = wild;
				cp = string + 1;
			}
			else if ((*wild == *string) || (*wild == '?'))
			{
				wild++;
				string++;
			}
			else
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == '*')
			wild++;

		return !*wild;
	}
}

FileWatcher::FileWatcher()
{
}
FileWatcher::FileWatcher(FileWatcher&& move) :
	mChangeQueue(std::move(mChangeQueue)), mChangeSources(std::move(move.mChangeSources))
{
}
FileWatcher::~FileWatcher()
{
	for (auto& source : mChangeSources)
		delete source;
	mChangeSources.clear();
}

void FileWatcher::addSource(const std::string& path, bool recurse)
{
	auto source = new ChangeSourceImpl(path, recurse);

	mChangeSources.push_back(source);
}
void FileWatcher::removeSource(const std::string& path)
{
	for (auto it = mChangeSources.begin(); it != mChangeSources.end(); ++it)
		if ((*it)->Path == path)
		{
			delete (*it);
			mChangeSources.erase(it);

			break;
		}
}

bool FileWatcher::hasChanges() const
{
	return !mChangeQueue.empty();
}
bool FileWatcher::pollChange(std::string& out)
{
	update();

	if (mChangeQueue.empty())
		return false;

	out = mChangeQueue.front();
	mChangeQueue.pop_front();

	return true;
}
void FileWatcher::update()
{
	for (auto& source : mChangeSources)
		source->update(mChangeQueue);
}

void FileWatcher::recurseDirectory(const std::string& dir, std::list<std::string>& output, const std::string& wildcard)
{
#ifdef WIN32
	WIN32_FIND_DATAA find = {};
	HANDLE findHandle = FindFirstFileExA((dir + "\\*").c_str(), FindExInfoStandard, &find, FindExSearchNameMatch, NULL, 0);
	if (findHandle == INVALID_HANDLE_VALUE)
		return;

	do
	{
		std::string name = find.cFileName;

		if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (name != "." && name != "..")
				recurseDirectory(dir + '\\' + name, output, wildcard);
		}
		else
		{
			if (wildcmp(wildcard.c_str(), name.c_str()))
				output.push_back(dir + '\\' + name);
		}

		if (!FindNextFileA(findHandle, &find))
			break;
	} while (findHandle != INVALID_HANDLE_VALUE);
#else
	DIR* dp;
	if ((dp = opendir(dir.c_str())) == nullptr)
		return;

	dirent* ent;
	while ((ent = readdir(dp)))
	{
		std::string name = ent->d_name;
		if (!(ent->d_type & (DT_DIR | DT_LNK)))
		{
			if (wildcmp(wildcard.c_str(), name.c_str()))
				output.push_back(dir + '/' + name);
			continue;
		}

		if (name == "." || name == "..")
			continue;

		std::string path = dir + '/' + name;

		recurseDirectory(path, subdirs, wildcard);
	}

	closedir(dp);
#endif
}
