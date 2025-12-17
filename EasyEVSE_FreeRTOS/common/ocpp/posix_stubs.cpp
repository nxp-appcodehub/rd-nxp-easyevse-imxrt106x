

extern "C" {

#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>

// Stub for truncate
int truncate(const char* path, off_t length) {
    // Not supported on this platform
    return -1;
}

// Stub for pathconf
long pathconf(const char* path, int name) {
    // Return a default or error value
    errno = ENOSYS;
    return -1;
}

// Stub for mkdir
int mkdir(const char* pathname, mode_t mode) {
    // Not supported
    return -1;
}

// Stub for getcwd
char* getcwd(char* buf, size_t size) {
    // Return a dummy path or nullptr
    if (buf && size > 0) {
        buf[0] = '/';
        buf[1] = '\0';
        return buf;
    }
    return nullptr;
}

// Stub for chmod
int chmod(const char* path, mode_t mode) {
    return 0;  // Pretend success
}

// Stub for chdir
int chdir(const char* path) {
    return 0;  // Pretend success
}

} // extern "C"
