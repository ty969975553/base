#pragma once

#include <stdio.h>

#include <stdexcept>
#include <string>

#include "base/base_export.h"
#include "base/files/file.h"
#include "base/files/file_path.h"

namespace base
{
    class Time;

    BASE_EXPORT fspath MakeAbsoluteFilePath(const fspath& input);

    BASE_EXPORT int64_t ComputeDirectorySize(const fspath& root_path);

#if defined(OS_WIN)
    // Schedules to delete the given path, whether it's a file or a directory,
    // until the operating system is restarted. Note: 1) The file/directory to
    // be deleted should exist in a temp folder. 2) The directory to be deleted
    // must be empty.
    BASE_EXPORT bool DeleteFileAfterReboot(const fspath& path);
#endif
    BASE_EXPORT bool Move(const fspath& from_path, const fspath& to_path);

    BASE_EXPORT bool ReplaceFile(const fspath& from_path, const fspath& to_path,
                                 std::error_code* error);

    // Copies a single file. Use CopyDirectory to copy directories.
    // This function fails if either path contains traversal components ('..').
    //
    // This function keeps the metadata on Windows. The read only bit on Windows
    // is not kept.
    BASE_EXPORT bool CopyFile(const fspath& from_path, const fspath& to_path);

    // Copies the given path, and optionally all subdirectories and their
    // contents as well.
    //
    // If there are files existing under to_path, always overwrite. Returns true
    // if successful, false otherwise. Wildcards on the names are not supported.
    //
    // This function calls into CopyFile() so the same behavior w.r.t. metadata
    // applies.
    //
    // If you only need to copy a file use CopyFile, it's faster.
    BASE_EXPORT bool CopyDirectory(const fspath& from_path,
                                   const fspath& to_path, bool recursive);

    // Returns true if the given path exists on the local filesystem,
    // false otherwise.
    BASE_EXPORT bool PathExists(const fspath& path);

    // Returns true if the given path is writable by the user, false otherwise.
    BASE_EXPORT bool PathIsWritable(const fspath& path);

    // Returns true if the given path exists and is a directory, false
    // otherwise.
    BASE_EXPORT bool DirectoryExists(const fspath& path);

    // Returns true if the contents of the two files given are equal, false
    // otherwise.  If either file can't be read, returns false.
    BASE_EXPORT bool ContentsEqual(const fspath& filename1,
                                   const fspath& filename2);

    // Returns true if the contents of the two text files given are equal, false
    // otherwise.  This routine treats "\r\n" and "\n" as equivalent.
    BASE_EXPORT bool TextContentsEqual(const fspath& filename1,
                                       const fspath& filename2);

    // Reads the file at |path| into |contents| and returns true on success and
    // false on error.  For security reasons, a |path| containing path traversal
    // components ('..') is treated as a read error and |contents| is set to
    // empty. In case of I/O error, |contents| holds the data that could be read
    // from the file before the error occurred. |contents| may be NULL, in which
    // case this function is useful for its side effect of priming the disk
    // cache (could be used for unit tests).
    BASE_EXPORT bool ReadFileToString(const fspath& path,
                                      std::string* contents);

    // Returns true if the given directory is empty
    BASE_EXPORT bool IsDirectoryEmpty(const fspath& dir_path);

    // Get the temporary directory provided by the system.
    //
    // WARNING: In general, you should use CreateTemporaryFile variants below
    // instead of this function. Those variants will ensure that the proper
    // permissions are set so that other users on the system can't edit them
    // while they're open (which can lead to security issues).
    BASE_EXPORT bool GetTempDir(fspath* path);

    // Get the home directory. This is more complicated than just getenv("HOME")
    // as it knows to fall back on getpwent() etc.
    //
    // You should not generally call this directly. Instead use DIR_HOME with
    // the path service which will use this function but cache the value. Path
    // service may also override DIR_HOME.
    BASE_EXPORT fspath GetHomeDir();

    // Creates a temporary file. The full path is placed in |path|, and the
    // function returns true if was successful in creating the file. The file
    // will be empty and all handles closed after this function returns.
    BASE_EXPORT bool CreateTemporaryFile(fspath* path);

    // Same as CreateTemporaryFile but the file is created in |dir|.
    BASE_EXPORT bool CreateTemporaryFileInDir(const fspath& dir,
                                              fspath* temp_file);

    // Create and open a temporary file.  File is opened for read/write.
    // The full path is placed in |path|.
    // Returns a handle to the opened file or NULL if an error occurred.
    BASE_EXPORT FILE* CreateAndOpenTemporaryFile(fspath* path);

    // Similar to CreateAndOpenTemporaryFile, but the file is created in |dir|.
    BASE_EXPORT FILE* CreateAndOpenTemporaryFileInDir(const fspath& dir,
                                                      fspath* path);

    // Create a new directory. If prefix is provided, the new directory name is
    // in the format of prefixyyyy. NOTE: prefix is ignored in the POSIX
    // implementation. If success, return true and output the full path of the
    // directory created.
    BASE_EXPORT bool CreateNewTempDirectory(const fspath::string_type& prefix,
                                            fspath* new_temp_path);

    // Create a directory within another directory.
    // Extra characters will be appended to |prefix| to ensure that the
    // new directory does not have the same name as an existing directory.
    BASE_EXPORT bool CreateTemporaryDirInDir(const fspath& base_dir,
                                             const std::wstring& prefix,
                                             fspath* new_dir);

    // Creates a directory, as well as creating any parent directories, if they
    // don't exist. Returns 'true' on successful creation, or if the directory
    // already exists.  The directory is only readable by the current user.
    // Returns true on success, leaving *error unchanged.
    // Returns false on failure and sets *error appropriately, if it is
    // non-NULL.
    BASE_EXPORT bool CreateDirectoryAndGetError(const fspath& full_path,
                                                std::error_code* error);

    // Backward-compatible convenience method for the above.
    BASE_EXPORT bool CreateDirectory(const fspath& full_path);

    // Returns the file size. Returns true on success.
    BASE_EXPORT bool GetFileSize(const fspath& file_path, int64_t* file_size);

    // Sets |real_path| to |path| with symbolic links and junctions expanded.
    // On windows, make sure the path starts with a lettered drive.
    // |path| must reference a file.  Function will fail if |path| points to
    // a directory or to a nonexistent path.  On windows, this function will
    // fail if |path| is a junction or symlink that points to an empty file,
    // or if |real_path| would be longer than MAX_PATH characters.
    BASE_EXPORT bool Normalizefspath(const fspath& path, fspath* real_path);

#if defined(OS_WIN)

    // Given a path in NT native form ("\Device\HarddiskVolumeXX\..."),
    // return in |drive_letter_path| the equivalent path that starts with
    // a drive letter ("C:\...").  Return false if no such path exists.
    BASE_EXPORT bool DevicePathToDriveLetterPath(const fspath& device_path,
                                                 fspath* drive_letter_path);

    // Given an existing file in |path|, set |real_path| to the path
    // in native NT format, of the form "\Device\HarddiskVolumeXX\..".
    // Returns false if the path can not be found. Empty files cannot
    // be resolved with this function.
    BASE_EXPORT bool NormalizeToNativefspath(const fspath& path,
                                             fspath* nt_path);
#endif

    BASE_EXPORT bool GetFileInfo(const fspath& file_path, FileInfo* info);

    // Sets the time of the last access and the time of the last modification.
    BASE_EXPORT bool TouchFile(const fspath& path, const Time& last_accessed,
                               const Time& last_modified);

    // Wrapper for fopen-like calls. Returns non-NULL FILE* on success.
    BASE_EXPORT FILE* OpenFile(const fspath& filename, const char* mode);

    // Closes file opened by OpenFile. Returns true on success.
    BASE_EXPORT bool CloseFile(FILE* file);

    // Truncates an open file to end at the location of the current file
    // pointer. This is a cross-platform analog to Windows' SetEndOfFile()
    // function.
    BASE_EXPORT bool TruncateFile(FILE* file);

    // Reads at most the given number of bytes from the file into the buffer.
    // Returns the number of read bytes, or -1 on error.
    BASE_EXPORT int ReadFile(const fspath& filename, char* data, int max_size);

    // Writes the given buffer into the file, overwriting any data that was
    // previously there.  Returns the number of bytes written, or -1 on error.
    BASE_EXPORT int WriteFile(const fspath& filename, const char* data,
                              int size);

    // Reads at most the given number of bytes from the file into the buffer.
    // Returns the number of read bytes, or -1 on error.
    BASE_EXPORT int ReadFile(const fspath& filename, char* data, int max_size);


    // Appends |data| to |filename|.  Returns true iff |size| bytes of |data|
    // were written to |filename|.
    BASE_EXPORT bool AppendToFile(const fspath& filename, const char* data,
                                  int size);

    // Gets the current working directory for the process.
    BASE_EXPORT bool GetCurrentDirectory(fspath* path);

    // Sets the current working directory for the process.
    BASE_EXPORT bool SetCurrentDirectory(const fspath& path);

    // Attempts to find a number that can be appended to the |path| to make it
    // unique. If |path| does not exist, 0 is returned.  If it fails to find
    // such a number, -1 is returned. If |suffix| is not empty, also checks the
    // existence of it with the given suffix.
    BASE_EXPORT int GetUniquePathNumber(const fspath& path,
                                        const fspath::string_type& suffix);

    // Sets the given |fd| to non-blocking mode.
    // Returns true if it was able to set it in the non-blocking mode, otherwise
    // false.
    BASE_EXPORT bool SetNonBlocking(int fd);

    // Returns the maximum length of path component on the volume containing
    // the directory |path|, in the number of fspath::CharType, or -1 on
    // failure.
    BASE_EXPORT int GetMaximumPathComponentLength(const fspath& path);

}  // namespace base