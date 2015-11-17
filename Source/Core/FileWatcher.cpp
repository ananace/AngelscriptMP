#include "FileWatcher.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <SFML/System/String.hpp>

#include <windows.h>
#include <list>
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
	void recurseDirectories(const std::string& folder, std::list<std::string>& subdirs)
	{
		WIN32_FIND_DATAA find = { };
		HANDLE findHandle = FindFirstFileExA((folder + "\\*").c_str(), FindExInfoStandard, &find, FindExSearchNameMatch, NULL, 0);
		if (findHandle == INVALID_HANDLE_VALUE)
			return;

		do
		{
			std::string name = find.cFileName;

			if ((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				(name != "." && name != ".."))
			{
				subdirs.push_back(folder + "\\" + name);
				recurseDirectories(subdirs.back(), subdirs);
			}

			if (!FindNextFileA(findHandle, &find))
				break;
		} while (findHandle != INVALID_HANDLE_VALUE);
	}

	struct ChangeSourceImpl : public FileWatcher::ChangeSource
	{
		ChangeSourceImpl(const std::string& path, bool recurse):
			mRecurse(recurse)
		{
			Path = path;

			Data mainDir = {
				path,
				CreateFileA(path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL),
				{ },
				{ }
			};

			mainDir.Overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

			mWatches.push_back(mainDir);

			if (recurse)
			{
				std::list<std::string> subdirs;
				recurseDirectories(path, subdirs);

				for (auto& dir : subdirs)
				{
					// Only need to add additional watches for links
					if (!(GetFileAttributes(dir.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT))
						continue;

					Data subdir = {
						dir,
						CreateFileA(dir.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL),
						{ },
						{ }
					};
					subdir.Overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

					mWatches.push_back(subdir);
				}
			}

			for (auto& watch : mWatches)
				ReadDirectoryChangesW(watch.Handle,
					&watch.Buffer,
					sizeof(watch.Buffer),
					recurse,
					FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE,
					NULL,
					&watch.Overlapped,
					NULL);
		}

		~ChangeSourceImpl()
		{
			for (auto& watch : mWatches)
				CloseHandle(watch.Handle);
		}

		void update(std::list<std::string>& list)
		{
			DWORD numBytes;
			for (auto& watch : mWatches)
			{
				GetOverlappedResult(watch.Handle, &watch.Overlapped, &numBytes, FALSE);
				ResetEvent(watch.Overlapped.hEvent);

				if (numBytes <= 0)
					continue;

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

						path += sf::String(std::wstring(pEntry->FileName, pEntry->FileNameLength / sizeof(wchar_t))); // FIXME: Dewide

						list.push_back(path);
					}
					else
						break;
				} while (true);

				CancelIo(watch.Handle);
				std::fill_n(watch.Buffer, sizeof(watch.Buffer), 0);
				ReadDirectoryChangesW(watch.Handle,
					&watch.Buffer,
					sizeof(watch.Buffer),
					mRecurse,
					FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE,
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

			std::string path = folder + '/' + ent->d_name;
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
			int flags = IN_ATTRIB | IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO;
			auto fd = inotify_add_watch(mInotify, path.c_str(), flags);
			mFDs[fd] = path;

			if (!recurse)
				return;

			std::list<std::string> subdirs;
			recurseDirectories(path, subdirs);
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

