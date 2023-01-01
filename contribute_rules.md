# Contributing Code To GodotSteam

Thanks for contributing to the GodotSteam source code! We only have a few minor rules for contributing code to make the whole process easier for everyone.

<hr/>

- [x] Use camel-case for any new function names; ie. newSteamFunction()
- [x] Use all lower-case, underscore-separated names for all variables and arguments; ie. new_argument, new_variable
- [x] Make sure your new function names, variables, and argument clearly state what they are.
- [x] Make sure your code compiles before submitting a pull-request. You may want to use the following to test your changes:

	=== "Godot 2.x, 3.x"
		`scons platform=[your platform] production=yes tools=yes target=release_debug`
	=== "Godot 4.x"
		`scons platform=[your platform] tools=yes target=editor`

So far that's it! Thanks again for helping make the project useful for the community! 