# Tutorials - C#

Every so often we get someone who asks how to use GodotSteam with C# so I figured I would write this up. First and foremost, I have never used C# so my understanding of it is prety much null and void.

---

## What's Mono?

To the point, GodotSteam does not have a C# version. I tried once or twice to compile a version with Mono and it failed; I have yet to attempt this again. However, anyone and everyone is welcome to make that work and submit a pull request for it!

---

## Workarounds

Another user from the Discord, **jolexxa**, mentions this advice:

"My current recommendation is to use GodotSteam as a GDExtension and write "glue" code in GDScript that can be called from C# to interface with Steam. Definitely a little overhead to doing that but it shouldn't be much worse that interfacing with the engine normally from C#"

---

## Other Resources

More often than not, I will refer C# users to one of two wonderful libraries for Steamworks and C#:

- [Facepunch.Steamworks](https://wiki.facepunch.com/steamworks) by the makers of Rust and Garry's Mod
- [Steamworks.net](https://steamworks.github.io) by the super-nice Riley Labrecque

I have no idea how you actually integrate these with Godot but some members of our Discord do. I will try reaching out to them at some point for additions to this tutorial.
