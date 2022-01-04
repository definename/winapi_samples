#pragma once

using namespace std;

#include "resource.h"
#include "data.h"

std::vector<data_t>directory;
std::mutex mutex_;

data_t collect_file_info(const std::filesystem::path& fpath) {
#ifdef WIN32
	HANDLE hFile;
	try {
		hFile = CreateFile(fpath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		FILETIME ftCreationTime = { 0 };
		GetFileTime(hFile, &ftCreationTime, NULL, NULL);


		SYSTEMTIME sCreate = { 0 };
		FileTimeToSystemTime(&ftCreationTime, &sCreate);
		SystemTimeToTzSpecificLocalTime(NULL, &sCreate, &sCreate);

		wchar_t createDate[21];
		swprintf(createDate, 21, TEXT("%02d.%02d.%04d %02d:%02d"), sCreate.wDay, sCreate.wMonth, sCreate.wYear, sCreate.wHour, sCreate.wMinute);

		wstring fsize = to_wstring(file_size(fpath));

		data_t direct(createDate, fpath.filename().wstring(), fpath.wstring(), fsize, file_size(fpath), sCreate.wDay, sCreate.wMonth, sCreate.wYear, sCreate.wHour, sCreate.wMinute);
		CloseHandle(hFile);
		return direct;
	}
	catch (...) {
		CloseHandle(hFile);
	}
#endif // WIN32
	return data_t();
}

void directory_traversal(std::filesystem::path& p, HWND hwnd) {
	try {
		std::filesystem::recursive_directory_iterator it(p, std::filesystem::directory_options::skip_permission_denied);
		for (it; it!= std::filesystem::recursive_directory_iterator(); ++it) {
			try {
				if (std::filesystem::is_regular_file(*it)) {
					std::lock_guard<std::mutex> l(mutex_);
					data_t push = collect_file_info(it->path());
					directory.push_back(push);

					if (directory.size() % 100 == 0) {
						SendMessage(hwnd,LVM_SETITEMCOUNT,0,0);
						SendMessage(hwnd,LVM_REDRAWITEMS,0,0);
					}
				}
			} catch (const std::filesystem::filesystem_error& e) {
				MessageBox(NULL, TEXT("Denied access"), TEXT("push error"), MB_OK | MB_ICONINFORMATION);
			}
		}
	} catch (const std::filesystem::filesystem_error& e) {
		MessageBox(NULL, TEXT("Denied access"), TEXT("scan error"), MB_OK);
	}
	SendMessage(hwnd,LVM_SETITEMCOUNT,0,0);
	SendMessage(hwnd,LVM_REDRAWITEMS,0,0);

	MessageBox(NULL, TEXT("SCAN COMPLETE"), TEXT("scan complete"), MB_OK | MB_ICONINFORMATION);
}