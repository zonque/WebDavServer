# WebDav server for POSIX-compatible systems

This package implements a WebDav server for systems with a POSIX-compatible file system storage.

# Compatibility

## Web server

The code has all parts that are specific to the http request and response factored out through an abstact base class. Examples are provided for the ESPIDF http server as well as libsoup.

## File system

A POSIX compatible file system must be provided. In particular, the code uses the following functions:

        * `fopen()`, `fclose()`
        * `fread()`, `fwrite()`
        * `stat()`
        * `access()`
        * `opendir()`, `closedir()`
        * `unlink()`
        * `rename()`

# Dependencies

* [libtinyxml2](https://github.com/leethomason/tinyxml2/)

# Current limitations

* No support for locking/unlocking
* No support for property writing

# License

LGPL-3, see LICENSE file.

