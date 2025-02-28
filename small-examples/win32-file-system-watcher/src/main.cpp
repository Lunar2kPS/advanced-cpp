#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>

using std::wcout;
using std::wcerr;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::wstring;
using std::stringstream;
using std::thread;

struct WatcherContext {
    HANDLE directoryHandle;
    OVERLAPPED overlapped;
    char buffer[1024];
    DWORD bytesReturned;
};

//NOTE: This variable is only accessed from the file watching thread:
WatcherContext* lastContext;
void CALLBACK fileIOCompletionRoutine(
    DWORD errorCode,
    DWORD numberOfBytesTransferred,
    LPOVERLAPPED lpOverlapped
) {
    cout << "CALLBACK" << endl;
    if (errorCode != ERROR_SUCCESS) {
        wcerr << L"Error in completion routine: " << errorCode << endl;
        return;
    }

    WatcherContext* context = lastContext;
    FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(context->buffer);
    do {
        wstring fileName(fni->FileName, fni->FileNameLength / sizeof(WCHAR));

        switch (fni->Action) {
        case FILE_ACTION_ADDED:
            wcout << L"File added: " << fileName << endl;
            break;
        case FILE_ACTION_REMOVED:
            wcout << L"File removed: " << fileName << endl;
            break;
        case FILE_ACTION_MODIFIED:
            wcout << L"File modified: " << fileName << endl;
            break;
        case FILE_ACTION_RENAMED_OLD_NAME:
            wcout << L"File renamed from: " << fileName << endl;
            break;
        case FILE_ACTION_RENAMED_NEW_NAME:
            wcout << L"File renamed to: " << fileName << endl;
            break;
        default:
            wcout << L"Unknown action: " << fni->Action << endl;
            break;
        }

        if (fni->NextEntryOffset == 0)
            break;

        fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
            reinterpret_cast<BYTE*>(fni) + fni->NextEntryOffset
        );
    } while (true);

    // Reissue the ReadDirectoryChangesW call to continue monitoring
    if (!ReadDirectoryChangesW(
        context->directoryHandle,
        context->buffer,
        sizeof(context->buffer),
        TRUE,
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | 
        FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | 
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION,
        &context->bytesReturned,
        &context->overlapped,
        fileIOCompletionRoutine
    )) {
        wcerr << L"Failed to reissue directory watch. Error: " << GetLastError() << endl;
    }
}

void watchDirectoryThread(const wstring& directoryPath) {
    WatcherContext context = {};
    context.directoryHandle = CreateFileW(
        directoryPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,  // Overlapped for async
        NULL
    );

    if (context.directoryHandle == INVALID_HANDLE_VALUE) {
        DWORD errorCode = GetLastError();
        stringstream errorMessage = { };
        switch (errorCode) {
            case ERROR_FILE_NOT_FOUND:
                errorMessage << "File not found";
                break;
        }
        cerr << "Failed to open directory handle.\n    Error Code " << errorCode;
        string errorMessageStr = errorMessage.str();
        if (!errorMessageStr.empty())
            cerr << "\n    (" << errorMessageStr << ")" << endl;
        return;
    }

    context.overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    lastContext = &context;

    if (!ReadDirectoryChangesW(
        context.directoryHandle,
        context.buffer,
        sizeof(context.buffer),
        TRUE,
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | 
        FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | 
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION,
        &context.bytesReturned,
        &context.overlapped,
        fileIOCompletionRoutine
    )) {
        wcerr << L"Failed to start async directory watch. Error Code " << GetLastError() << endl;
        CloseHandle(context.directoryHandle);
        return;
    }

    wcout << L"Watching directory asynchronously: " << directoryPath << endl;

    while (true) {
        SleepEx(INFINITE, TRUE);
        
        //WARNING: For some reason, WaitForSingleObject(HANDLE, DWORD) was never getting the event signal,
        //  and the callback was never getting called.
        //  Is it because we were blocking the thread without sleeping in an async I/O-compatible way or something?
        //  Not sure why, but SleepEx(DWORD, BOOL) works instead. (above)
        // DWORD waitStatus = WaitForSingleObject(context.overlapped.hEvent, INFINITE);
        // if (waitStatus == WAIT_OBJECT_0) {
        //     // Let the completion routine handle the event
        // } else {
        //     wcerr << L"Wait failed. Error Code " << GetLastError() << endl;
        //     break;
        // }
    }

    CloseHandle(context.overlapped.hEvent);
    CloseHandle(context.directoryHandle);
}

int main() {
    wstring directory = L"C:/dev";
    thread watcherThread(watchDirectoryThread, directory);
    watcherThread.join();
    return 0;
}
