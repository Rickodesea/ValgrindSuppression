# ValgrindSuppression
Valgrind Suppression is a generic-valgrind-suppression file used to help valgrind to ignore Graphics API (OpenGL) underlying built-in errors. Therefore, the programmer can forcus on programmer-made-errors during the development of the application on top of the API.

It is generic,  therefore, feel free to customize it.  
Attempt is made to suppress all of the OpenGL API errors that valgrind detects.
Valgrind detects these errors whenever you run it on any program that uses OpenGL.  However, they are not
any errors you can fix because they are built into the API (the API may be designed that way).
The most that can be done is to suppress the errors, that is, hide them.

Requirements:
- [Linux](https://github.com/torvalds/linux) OS via any of the distros, such as, [Ubuntu](https://ubuntu.com/)
- [OpenGL](https://www.opengl.org//), low-level-graphics API.  
- [Valgrind](https://valgrind.org/), the star of the show.
- [SDL](https://www.libsdl.org/), graphics and window framework for easy use of OpenGL.

Sometimes, the suppression file will have to be updated after the computer (platform) receives an (os) update.
This is because these updates may include new internal leaks.

## Demo
A [demo](SDLDemo.c) is provided that test suppressions of default graphics errors. Just run [make](Makefile) to test it.  
The 'C' file only just open an OpenGL window via SDL and glad.  It allows you to test OpenGL internal memory leaks and 
therefore suppress those errors.

## License
This project is licensed under the Zlib License - see the [eula.txt](eula.txt) file for details.

## Resources
- [Valgrind Suppression File Howto](https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto)
- [Writing suppression files](https://www.valgrind.org/docs/manual/mc-manual.html#mc-manual.suppfiles)

## References
- [Glad](https://glad.dav1d.de/)

