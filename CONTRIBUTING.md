# Contributing Code To GodotSteam

Thanks for contributing to the GodotSteam source code! We only have a few minor rules for contributing code to make the whole process easier for everyone.

1. Use camel-case for any new Steam function names; ie. newSteamFunction(), you can use snake_case for anything else.
2. Use all lower-case, underscore-separated names for all variables and arguments; ie. new_argument, new_variable
3. Make sure your new function names, variables, and argument clearly state what they are.
4. Make sure your code compiles before submitting a pull-request.  You may want to use `scons platform=[your platform] production=yes tools=yes target=release_debug` to test your editor build.

So far that's it!  Thanks again for helping make the project useful for the community!
