# valgrindSup
Valgrind Suppression is a generic-valgrind-suppression file used to help valgrind to ignore Graphics API (OpenGL) underlying built-in errors. Therefore, the programmer can forcus on programmer-made-errors during the development of the application on top of the API.

It is generic,  therefore, feel free to customize it.  
Attempt is made to suppress all of the OpenGL API errors that valgrind detects.
Valgrind detects these errors whenever you run it on any program that uses OpenGL.  However, they are not
any errors you can fix because they are built into the API (the API may be designed that way).
The most that can be done is to suppress the errors, that is, hide them.

- os       : linux
- api      : opengl
- valgrind : [https://valgrind.org/](https://valgrind.org/)

## License
This project is licensed under the Zlib License - see the [eula.txt](eula.txt) file for details.
