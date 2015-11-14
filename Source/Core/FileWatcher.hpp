#pragma once

#include <list>
#include <string>
#include <vector>

class FileWatcher
{
public:
	struct ChangeSource
	{
		std::string Path;

		virtual ~ChangeSource() { }

		virtual void update(std::list<std::string>&) = 0;
	};

	FileWatcher();
	FileWatcher(const FileWatcher& copy) = delete;
	FileWatcher(FileWatcher&& move);
	~FileWatcher();

	FileWatcher& operator=(const FileWatcher& copy) = delete;

	void addSource(const std::string& path, bool recursive = true);
	void removeSource(const std::string& path);

	bool hasChanges() const;
	bool pollChange(std::string& out);
	void update();

private:
	std::list<std::string> mChangeQueue;
	std::vector<ChangeSource*> mChangeSources;
};
